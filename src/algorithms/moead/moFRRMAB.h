#ifndef __moAlgo__h
#define __moAlgo__h

//#include "mpicxx.h"

#include <map>
#include <vector>
#include <fstream>
#include "moSolution.h"
#include "subProblems.h"
#include "init.h"
#include "repair.h"
#include "mutation.h"
//#include "checkSol.h"


class MultiObjectiveAlgo {
public:
    std::vector<moSolution> pop ;
    unsigned maxEval;
    /**
    * FFRMAB implementation
    * @param fileout
    */
    virtual void runFRRMAB(char *fileout) = 0;
};

/**
   Bi objective MOEA/D
 **/
class FFRMAB : public MultiObjectiveAlgo {
public:
    FFRMAB(moEval &_eval, SubProblems &_subproblems, InitQAP &_init, std::vector<Mutation*> &_mutations, Repair &_repair,
           unsigned _mu, double _C, double _D, unsigned _neighborTaken, double _pFindNeighbor, unsigned _maxEval)
            : evaluation(_eval), subProblems(_subproblems), initialization(_init), mutations(_mutations), repair(_repair), mu(_mu),
              C(_C), D(_D), neighborTaken(_neighborTaken), pFindNeighbor(_pFindNeighbor), maxEval(_maxEval)  {

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
    virtual void runFRRMAB(char *fileout) {
        //simpleHashMap sHM;
        unsigned nbEval = 0;

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
    // number of neighbor taken in consideration to choose best operator
    unsigned neighborTaken;
    // probability to search into neighbor hood information to choose best operator
    double pFindNeighbor;

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

        // Init vector rewards and op values
        std::vector<double> rewards(mutations.size());

        for (int i = 0; i < rewards.size(); i++) {
            rewards.at(i) = 0.;
            nop.at(_subProblem).at(i) = 0;
        };

        // For each elements into slidingWindow of subProblem
        for (int i = 0; i < subProblems.slidingWindows.at(_subProblem).size(); i++) {

            unsigned op = subProblems.slidingWindows.at(_subProblem).at(i).first;
            double FIR = subProblems.slidingWindows.at(_subProblem).at(i).second;

            // update reward
            rewards.at(op) += FIR;
            nop.at(_subProblem).at(op)++;
        }

        // getting rank of each reward of operator
        std::vector<unsigned > opRanks(mutations.size());
        std::vector<double> sortedRewards = rewards;

        std::sort(sortedRewards.begin(), sortedRewards.end());

        std::map<double, unsigned> mapValues;

        for (int i = 0; i < sortedRewards.size(); i++) {
            // descending order
            mapValues.insert(make_pair(sortedRewards[sortedRewards.size() - i - 1], i));
        }

        for (int i = 0; i < sortedRewards.size(); i++) {
            // retrieve rank of op
            // TODO to improve... use std::sort
            opRanks.at(i) = mapValues.at(sortedRewards.at(i));
        }

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
     * Return best next op for subProblem FFRMABN version
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

            // probability to check neighborhood information
            double p = ((double) rand() / (RAND_MAX));

            // Use UCB to best next op
            double maxValue = -1.;

            // sum of number operator (sliding window size)
            int nopSum = 0;

            // check neighborhood acceptance
            if (p < pFindNeighbor) {

                // getting all neighbors
                std::vector<unsigned> neighbors = subProblems.neighborProblems(_subProblem);

                // create new variables which will be used for compute new FFR values based on neighbor hood
                std::vector<unsigned> nopNeighbor(mutations.size());
                std::map<unsigned, double> FFRNeighbor;

                // shuffle neighbors indexes to randomly choose them
                std::random_shuffle(neighbors.begin(), neighbors.end());

                // init values
                for (unsigned i = 0; i < mutations.size(); i++) {
                    nopNeighbor.at(i) = 0;
                    FFRNeighbor.insert(std::make_pair(i, 0.));
                }

                // set FFRs neighborhood values
                for (unsigned i = 0; i < neighborTaken; i++) {

                    // getting FFRs values from neighbor sub problem i
                    auto fitnessRateRanks = FFRs.at(neighbors.at(i));

                    for (unsigned j = 0; j < fitnessRateRanks.size(); j++) {
                        unsigned index = j;

                        nopNeighbor.at(j)++;
                        FFRNeighbor.at(j) += fitnessRateRanks.at(j);
                    }
                }

                // include current sub problem values too
                auto fitnessRateRanks = FFRs.at(_subProblem);

                for (unsigned j = 0; j < fitnessRateRanks.size(); j++) {
                    unsigned index = j;

                    nopNeighbor.at(index)++;
                    FFRNeighbor.at(index) += fitnessRateRanks.at(index);
                }

                // nopSum based on neighborhood and local sub problem information
                for (unsigned i = 0; i < nopNeighbor.size(); i++) {
                    nopSum += nopNeighbor.at(i);
                }

                for (int i = 0; i < nop.at(_subProblem).size(); ++i) {
                    nopSum += nop.at(_subProblem).at(i);
                }

                // compute UCB neighbor hood and local information
                // search best op at time t from neighborhood sub problem
                for (unsigned i = 0; i < FFRNeighbor.size(); i++) {

                    double explorationValue = sqrt((2 * log(nopSum)) / nopNeighbor.at(i));
                    double currentValue = (FFRNeighbor.at(i) + (C * explorationValue));

                    if (currentValue > maxValue) {
                        maxValue = currentValue;
                        selectedOp = i;
                    }
                }

            } else {

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
        }
        return selectedOp;
    }
};// end FFRMAB
#endif
