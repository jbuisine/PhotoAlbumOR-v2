#ifndef __moAlgo__h
#define __moAlgo__h

//#include "mpicxx.h"

#include <map>
#include <vector>
#include <fstream>
#include <algorithms/moead/moSolution.h>
#include <algorithms/moead/subProblems.h>
#include <algorithms/moead/init.h>
#include <algorithms/moead/repair.h>
#include <algorithms/moead/mutation.h>
#include <algorithms/moead/moAlgo.h>
//#include "checkSol.h"

/**
   FRRMAB which uses sliding windows for each directions and share Reward to compute FRR information of current direction
   - alpha : [0, 1], value which declare affinity of a direction and its neighbors
 **/
class FRRMAB_NR : public MultiObjectiveAlgo {
public:
    FRRMAB_NR(moEval &_eval, SubProblems &_subproblems, InitQAP &_init, std::vector<Mutation*> &_mutations, Repair &_repair,
           unsigned _mu, double _C, double _D, double _affinity, unsigned _maxEval)
            : evaluation(_eval), subProblems(_subproblems), initialization(_init), mutations(_mutations), repair(_repair), mu(_mu),
              C(_C), D(_D), affinity(_affinity), maxEval(_maxEval)  {

        // Initialization
        // FRR values for each sub problem operators
        FFRs.resize(mu);
        // nop : number of op present into each slidingWindows (sub problem)
        nop.resize(mu);
        // non selected op sub problem
        unusedOp.resize(mu);
        // value which will specify if necessary to check unused op
        checkUnusedOp.resize(mu);
        // resize pop to mu
        pop.resize(mu);

        int nbOperators = mutations.size();

        for (int i = 0; i < mu; i++) {

            pop.at(i).resize(evaluation.size());

            checkUnusedOp.at(i) = true;

            // init each sub vectors size
            unusedOp.at(i).resize(nbOperators);
            nop.at(i).resize(nbOperators);

            for (int j = 0; j < nbOperators; j++) {
                unusedOp.at(i).at(j) = true;

                // init each pair(OpID, FFR value) of sub problem
                FFRs.at(i).insert(std::make_pair(j, 0.));
            }
        }
    }

    /**
     * FRRMAB with sliding window for each N sub problem implementation
     *
     * @param fileout
     */
    virtual void run(char *fileout) {
        //simpleHashMap sHM;
        unsigned nbEval = 0;
        time_t begintime = time(NULL);

        // output file header
        FILE *file = fopen(fileout, "w");

        // initialization of the population
        pop.resize(mu);
        for (unsigned i = 0; i < mu; i++) {
            initialization(pop[i]);
            //repair(pop[i]);
            evaluation(pop[i]);
            pop[i].fitness(subProblems.scalarfunc(i, pop[i]));
            //sHM.insertSol(pop[i]);
            nbEval++;
            pop[i].ID(nbEval);
            pop[i].dir(i);
            pop[i].from(-1);
            pop[i].best(1);
            pop[i].save(file);
        }

        moSolution mutant;

        // stopping criterium is the number of seconds
        unsigned i = 0;
        while (nbEval < maxEval) {
            // mutation of the direction i

            // get best next op for sub problem
            int selectedOpIndex = getBestOp(i);
            Mutation& mutation = *mutations.at(selectedOpIndex);

            mutant = pop[i];
            mutant.best(0);

            //while (!sHM.isNewSol(mutant)) {
                mutation(mutant);
                //repair(mutant, i, true);
            //}

            //sHM.insertSol(mutant);
            evaluation(mutant);
            nbEval++;
            mutant.ID(nbEval);

            double fitness;
            std::vector<unsigned> neighbors = subProblems.neighborProblems(i);

            double FIRop = 0.;

            for (unsigned n : neighbors) {

                // getting fir value for this neighbor
                double fitness = subProblems.scalarfunc(n, mutant);
                double fir = computeFIR(pop[n], fitness, n);

                // if fir is better than 0 (we have amelioration)
                if (fir > 0) {
                    // replace solution
                    pop[n] = mutant;

                    // set new fitness value
                    pop[n].fitness(fitness);

                    mutant.best(1);

                    // set new FIR op used
                    FIRop += fir;
                }
            }

            // set new FIR value into sliding window of sub problem i
            updateSlidingWindow(i, selectedOpIndex, FIRop);

            // update credit assignment value of sub problem i
            updateCreditAssignmentSubProblem(i);

            mutant.save(file);

            // next direction
            i++;
            if (i >= mu) i = 0;
        }

        // set duration time
        duration = time(NULL) - begintime;
    }

protected:
    moEval &evaluation;
    SubProblems &subProblems;
    InitQAP &initialization;
    std::vector<Mutation*> &mutations;
    Repair &repair;
    unsigned mu;


    /**
    * Adaptive Operator Selection variables
    */
    // FRR values for each sub problem operators
    std::vector<std::map<int, double>> FFRs;
    // nop : number of op present into each slidingWindows
    std::vector<std::vector<int>> nop;
    // non selected op sub problem
    std::vector<std::vector<bool>> unusedOp;
    // value which will specify if necessary to check unused op
    std::vector<bool> checkUnusedOp;
    // C : UCB scaling factor which control the trade-off EvE
    double C;
    // D decaying factor D in [0,1]
    double D;
    // affinity criteria to compute Reward values
    double affinity;
    // max evaluation of algo
    unsigned maxEval;

private:

    /**
     * Update sliding window content for the _subProblem
     *
     * @param _subProblem
     * @param _selectedOpIndex
     * @param _FIROp
     */
    void updateSlidingWindow(int _subProblem, int _selectedOpIndex, double _FIROp) {

        // add new element
        subProblems.slidingWindows.at(_subProblem).push_back(std::make_pair(_selectedOpIndex, _FIROp));

        // remove old element if necessary
        if (subProblems.slidingWindows.at(_subProblem).size() > subProblems.getW()) {
            subProblems.slidingWindows.at(_subProblem).erase(subProblems.slidingWindows.at(_subProblem).begin());
        }
    }

    /**
     * FIR compute value
     *
     * @param _solution
     * @param _mutantFitness : fitness value of mutant
     * @return
     */
    double computeFIR(moSolution &_solution, double _mutantFitness, int _n) {
        double solFitness = subProblems.scalarfunc(_n, _solution);
        return (solFitness - _mutantFitness) / solFitness;
    }

    /**
     * Credit assignment updates of a subProblem
     *
     * @param _subProblem
     */
    void updateCreditAssignmentSubProblem(unsigned _subProblem) {

        unsigned nbOperators = mutations.size();

        // Init vector rewards and op values
        std::vector<double> rewards(nbOperators);

        for (int i = 0; i < nbOperators; i++) {
            rewards.at(i) = 0.;
            nop.at(_subProblem).at(i) = 0;
        };

        // adding rewards of neighbors with affinity criteria (include itself)
        std::vector<unsigned > neighbors = subProblems.neighborProblems(_subProblem);

        for(unsigned n : neighbors){

            std::vector<double> rewardsNeighbor(nbOperators);
            std::vector<unsigned> nopNeighbors(nbOperators);

            for (unsigned i = 0; i < nbOperators; i++) {
                rewardsNeighbor.at(i) = 0.;
                nopNeighbors.at(i) = 0;
            }

            for (unsigned i = 0; i < subProblems.slidingWindows.at(n).size(); i++) {

                unsigned op = subProblems.slidingWindows.at(n).at(i).first;
                double FIR = subProblems.slidingWindows.at(n).at(i).second;

                // update reward
                rewardsNeighbor.at(op) += FIR;
                nopNeighbors.at(op)++;
            }

            // add new values based on affinity criteria
            unsigned neighborDist = abs((int)_subProblem - (int)n);

            for(unsigned i = 0; i < nbOperators; i++){
                rewards.at(i) += pow(affinity, neighborDist) * rewardsNeighbor.at(i);

                // only take in consideration current sub problem for number of op
                if(n == _subProblem)
                    nop.at(_subProblem).at(i) += nopNeighbors.at(i);
            }
        }



        // getting rank of each reward of operator
        std::vector<unsigned > opRanks(mutations.size());

        // set default indexes
        std::size_t n(0);
        std::generate(std::begin(opRanks), std::end(opRanks), [&]{ return n++; });

        // getting rank indexes sorted
        std::sort(opRanks.begin(), opRanks.end(),
                  [&](int i1, int i2) { return rewards[i1] < rewards[i2]; } );

        std::reverse(opRanks.begin(), opRanks.end());

        // Compute decay values of each op and decaySum
        std::vector<double> decays(mutations.size());
        double decaySum = 0.;

        for (int op = 0; op < mutations.size(); op++) {
            decays.at(op) = pow(D, (opRanks.at(op)+1)) * rewards.at(op);
            decaySum += decays.at(op);
        }

        // Compute new FFRs value
        for (unsigned op = 0; op < mutations.size(); op++) {
            if(decaySum != 0)
                FFRs.at(_subProblem).at(op) = decays.at(op) / decaySum;
            else
                FFRs.at(_subProblem).at(op) = 0.;
        }
    }

    /**
     * Return best next op for subProblem FFRMAB_NR version
     *
     * take consideration of neighborhood information
     *
     * @param _subProblem
     * @return
     */
    unsigned getBestOp(unsigned _subProblem) {

        int selectedOp;

        // check if all operator are used at least once
        if (checkUnusedOp.at(_subProblem)) {

            std::vector<int> unusedOpIndexes;

            for (unsigned i = 0; i < unusedOp.at(_subProblem).size(); i++) {
                if (unusedOp.at(_subProblem).at(i)) {
                    unusedOpIndexes.push_back(i);
                }
            }

            // all op will be used at least once after this iteration
            if (unusedOpIndexes.size() <= 1)
                checkUnusedOp.at(_subProblem) = false;

            int randIndex = rand() % unusedOpIndexes.size();

            // getting next op choose randomly
            selectedOp = unusedOpIndexes.at(randIndex);

            // set op as viewed
            unusedOp.at(_subProblem).at(selectedOp) = false;

        } else {


            // Use UCB to best next op
            double maxValue = -1.;

            // sum of number operator (sliding window size)
            int nopSum = 0;

            // selected operator will based only on local information
            auto fitnessRateRanks = FFRs.at(_subProblem);

            for (unsigned i = 0; i < nop.at(_subProblem).size(); ++i) {
                nopSum += nop.at(_subProblem).at(i);
            }

            // search best op at time t for subProblem
            for (unsigned i = 0; i < fitnessRateRanks.size(); i++) {

                double explorationValue = sqrt((2 * log(nopSum)) / nop.at(_subProblem).at(i));
                double currentValue = fitnessRateRanks.at(i) + (C * explorationValue);

                if (currentValue > maxValue) {
                    maxValue = currentValue;
                    selectedOp = i;
                }
            }

        }
        return selectedOp;
    }
};// end FFRMAB
#endif
