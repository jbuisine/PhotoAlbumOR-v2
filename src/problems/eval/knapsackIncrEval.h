//
// Created by jbuisine on 08/11/17.
//

#ifndef PROJECTNAME_KNAPSACKINCREVAL_H
#define PROJECTNAME_KNAPSACKINCREVAL_H

#include <eval/moEval.h>
#include "knapsackEval.h"

/**
 * Incremental evaluation Function for the knapsack problem
 * when several bits are flipped (moBitsNeighbor)
 */
template< class Neighbor >
class knapsackIncrEval : public moEval<Neighbor>
{
public:
    typedef typename Neighbor::EOT EOT;

    /*
     * default constructor
     * @param knapsack full evaluation of the knapsack problem
     */
    knapsackIncrEval(knapsackEval<EOT> & _knapsackEval) {

        // Retrieving all necessary information
        nbVar = _knapsackEval.getNbVar();
        P = _knapsackEval.getP();
        W = _knapsackEval.getW();
        capacity = _knapsackEval.getCapacity();
        profitsSum = _knapsackEval.getProfitsSum();
        weightsSum = _knapsackEval.getWeightsSum();
        beta = _knapsackEval.getBeta();
    }

    /*
     * Incremental evaluation of the neighbor for the knapsack problem (complexity O(n * k) when k bits are flipped)
     *
     * @param _solution the solution to move (bit string)
     * @param _neighbor the neighbor to consider of type moBitsNeighbor (several bits are flipped)
     */
    virtual void operator()(EOT & _solution, Neighbor & _neighbor) {

        double delta = 0.;

        unsigned int k = _neighbor.key;

        if (_solution[k] == 1){
            delta += P->at(k);
            weightsSum += W->at(k);
        }else{
            delta -= P->at(k);
            weightsSum -= W->at(k);
        }

        // TODO how to know if solution already has this behavior
        if(weightsSum > capacity){
            delta -= beta * (weightsSum - capacity);
        }

        // move back the solution
        _solution[k] = !_solution[k];

        _neighbor.fitness(_solution.fitness() + delta);
    }

    /*
     * to get the number of variable (bit string length)
     *
     * @return bit string length
     */
    int getNbVar() {
        return nbVar;
    }

private:

    // size of solution
    unsigned int nbVar;

    // knapsack capacity
    unsigned int capacity;

    // vector of profits
    std::vector<unsigned int>* P;

    // vector of weights
    std::vector<unsigned int>* W;

    // store last profits obtained
    double profitsSum;

    // store last weightSum
    double weightsSum;

    // store beta ratio
    double beta;

};

#endif //PROJECTNAME_KNAPSACKINCREVAL_H
