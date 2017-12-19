#ifndef __moAlgo__h
#define __moAlgo__h

//#include "mpicxx.h"

#include <vector>
#include <fstream>
#include "moSolution.h"
#include "moPhotoAlbumEval.h"
#include "subProblems.h"
#include "init.h"
//#include "repair.h"
#include "mutation.h"
//#include "checkSol.h"

using namespace std;

class MultiObjectiveAlgo {
public:
    vector<moSolution> pop ;
    unsigned nbEval ;

    /**
    * FFRMAB implementation
    * @param fileout
    */
    virtual void runFFRMAB(char * fileout) = 0;


    /**
    * FFRMABN implementation : version which take consideration of neighborhood
    * @param fileout
    */
    virtual void runFFRMAB_N(char * fileout) = 0;
};

/**
   Biobjective MOEA/D
 **/
class FFRMAB : public MultiObjectiveAlgo {
public:
    FFRMAB(moEval &_eval, SubProblems &_subproblems, InitPhotoAlbum &_init, std::vector<Mutation> &_mutations,
           unsigned _mu, double _C, double _D, unsigned _neighborTaken, double _pFindNeighbor, unsigned _duration)
            : evaluation(_eval), subProblems(_subproblems), initialization(_init), mutations(_mutations), mu(_mu),
              C(_C), D(_D), neighborTaken(_neighborTaken), pFindNeighbor(_pFindNeighbor), duration(_duration)  {
        for (int i = 0; i < mu; i++) {
            checkUnusedOp.at(i) = true;
            for (int j = 0; j < mutations.size(); j++) {
                unusedOp.at(i).at(j) = true;
            }
        }
    }

    /**
     * FRRMAB implementation
     *
     * @param fileout
     */
    virtual void runFRRMAB(char *fileout) {
        // time duration of the run (in second)
        finishtime = time(NULL) + duration;

        simpleHashMap sHM;

        nbEval = 0;

        // output file header
        FILE *file = fopen(fileout, "w");

        // initialization of the population
        pop.resize(mu);
        for (unsigned i = 0; i < mu; i++) {
            initialization(pop[i]);
            //repair(pop[i]);
            evaluation(pop[i]);
            pop[i].fitness(subProblems.scalarfunc(i, pop[i]));
            sHM.insertSol(pop[i]);
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
        while (time(NULL) < finishtime) {
            // mutation of the direction i

            // get best next op for sub problem
            int selectedOpIndex = getBestOp(i);
            Mutation mutation = mutations.at(selectedOpIndex);

            mutant = pop[i];
            mutant.best(0);
            while (!sHM.isNewSol(mutant)) {
                mutation(mutant);
                //repair(mutant);
            }

            sHM.insertSol(mutant);
            evaluation(mutant);
            nbEval++;
            mutant.ID(nbEval);

            double fitness;
            vector<unsigned> neighbors = subProblems.neighborProblems(i);

            FIRop = 0.;
            for (unsigned n : neighbors) {

                // getting fir value for this neighbor
                fitness = subProblems.scalarfunc(n, mutant);
                fir = computeFIR(pop[n], fitness, n);

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
    InitPhotoAlbum &initialization;
    std::vector<Mutation> &mutations;
    //OverlapRepair repair;
    unsigned mu;


    /**
    * Adaptive Operator Selection variables
    */
    // FRR values for each sub problem operators
    std::vector<std::vector<std::pair<int, double>>> FFRs;
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

    // number of seconds of the run
    time_t finishtime;
    unsigned duration;

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
        subProblems.slidingWindows->at(_subProblem).push_back(std::make_pair(_selectedOpIndex, _FIRop));

        // remove old element if necessary
        if (subProblems.slidingWindows->at(_subProblem).size() > subProblems.getW()) {
            subProblems.slidingWindows->at(_subProblem).erase(subProblems.slidingWindows->at(_subProblem).begin());
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
        return (subProblems.scalarfunc(_n, _solution) - _mutantFitness) / subProblems.scalarfunc(_n, _solution);
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
        for (int i = 0; i < subProblems.slidingWindows->at(_subProblem).size(); i++) {

            unsigned op = subProblems.slidingWindows->at(_subProblem).at(i).first;
            double FIR = subProblems.slidingWindows->at(_subProblem).at(i).second;

            // update reward
            rewards.at(op) += FIR;
            nop.at(_subProblem).at(op)++;
        }

        // getting rank of each reward of operator
        vector<int> opRanks(mutations.size());
        vector<double> sortedRewards = rewards;

        std::sort(sortedRewards.begin(), sortedRewards.end());

        map<double, int> mapValues(mutations.size());

        for (int i = 0; i < sortedRewards.size(); i++) {
            // descending order
            mapValues.insert(make_pair(sortedRewards[sortedRewards.size() - i - 1], i));
        }

        for (int i = 0; i < sortedRewards.size(); i++) {
            // retrieve rank of op
            opRanks.at(i) = mapValues.find(sortedRewards[i]);;
        }

        // Compute decay values of each op and decaySum
        std::vector<double> decays(mutations.size());
        double decaySum = 0.;

        for (int op = 0; op < mutations.size(); op++) {
            decays.at(op) = pow(D, opRanks.at(op)) * rewards.at(op);
            decaySum += decays.at(op);
        }

        // Compute new FFRs value
        for (int op = 0; op < mutations.size(); op++) {
            FFRs.at(_subProblem).at(op).second = decays.at(op) / decaySum;
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
        if (checkUnusedOp(_subProblem)) {

            std::vector<int> unusedOpIndexes;

            for (int i = 0; i < unusedOp.at(_subProblem).size(); i++) {
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
            double maxValue = 0.;

            // sum of number operator (sliding window size)
            int nopSum = 0;

            // check probability acceptance
            if (p < pFindNeighbor) {

                // getting all neighbors
                vector<unsigned> neighbors = subProblems.neighborProblems(i);

                // create new variables which will be used for compute new FFR values based on neighbor hood
                vector<unsigned> nopNeighbor(neighbors.size());
                vector<pair<int, double>> FFRNeighbor(neighbors.size());

                // shuffle neighbors indexes to randomly choose them
                std::random_shuffle(neighbors.begin(), neighbors.end());

                // init values
                for (int i = 0; i < mutations.size(); i++) {
                    nopNeighbor.at(i) = 0;
                    FFRNeighbor.at(i) = 0.;
                }

                // set FFRs neighborhood values
                for (int i = 0; i < neighborTaken; i++) {

                    // getting FFRs values from neighbor sub problem i
                    auto fitnessRateRanks = FFRs.at(neighbors.at(i));

                    for (int j = 0; j < fitnessRateRanks.size(); j++) {
                        int index = fitnessRateRanks.at(j).first;

                        nopNeighbor.at(index)++;
                        FFRNeighbor.at(index).second += fitnessRateRanks.at(j).second;
                    }
                }

                // include current sub problem values
                auto fitnessRateRanks = FFRs.at(_subProblem);

                for (int j = 0; j < fitnessRateRanks.size(); j++) {
                    int index = fitnessRateRanks.at(j).first;

                    nopNeighbor.at(index)++;
                    FFRNeighbor.at(index).second += fitnessRateRanks.at(j).second;
                }

                // nopSum based on neighborhood and local sub problem information
                for (int i = 0; i < nopNeighbor.size(); i++) {
                    nopSum += nopNeighbor.at(i);
                }

                for (int i = 0; i < nop.at(_subProblem).size(); ++i) {
                    nopSum += nop.at(_subProblem).at(i);
                }

                // compute UCB neighbor hood and local information
                // search best op at time t from neighborhood sub problem
                for (int i = 0; i < nopNeighbor.size(); i++) {

                    double explorationValue = sqrt((2 * log(nopSum)) / nopNeighbor.at(i));
                    double currentValue = (FFRNeighbor.at(i).second + (C * explorationValue));

                    if (currentValue > maxValue) {
                        maxValue = currentValue;
                        selectedOp = i;
                    }
                }

            } else {

                // selected operator will based only on local information
                auto fitnessRateRanks = FFRs.at(_subProblem);

                for (int i = 0; i < nop.at(_subProblem).size(); ++i) {
                    nopSum += nop.at(_subProblem).at(i);
                }

                // search best op at time t for subProblem
                for (int i = 0; i < fitnessRateRanks.size(); i++) {

                    double explorationValue = sqrt((2 * log(nopSum)) / nop.at(_subProblem).at(fitnessRateRanks.at(i).first));
                    double currentValue = fitnessRateRanks.at(i).second + (C * explorationValue);

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
