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
        beta = _knapsackEval.getBeta();
    }

    /*
     * Incremental evaluation of the neighbor for the knapsack problem (complexity O(n * k) when k bits are flipped)
     *
     * @param _solution the solution to move (bit string)
     * @param _neighbor the neighbor to consider of type moBitsNeighbor (several bits are flipped)
     */
    virtual void operator()(EOT & _solution, Neighbor & _neighbor) {

        unsigned int k = _neighbor.key;

        double deltaProfit = 0.;
        double deltaWeight = 0.;

        double weightSum = 0.;

        for(int i = 0; i < nbVar; i++)
            if(_solution[i] == 1)
                weightSum += W->at(i);

        // if equal 1 we will store the reverse
        if (_solution[k] == 0){
            deltaProfit += P->at(k);
            deltaWeight += W->at(k);
        }else{
            deltaProfit -= P->at(k);
            deltaWeight -= W->at(k);
        }

        if((weightSum + deltaWeight) > capacity){

            deltaProfit -= beta * abs((weightSum + deltaWeight) - capacity);

        }else if(weightSum > capacity && ((weightSum + deltaWeight) <= capacity)){

            deltaProfit += beta * abs(weightSum - capacity);
        }

        // move back the solution
        //_solution[k] = !_solution[k];

        double result = _solution.fitness() + deltaProfit;

        if(result < 0){
            result = 0;
        }

        _neighbor.fitness(result);
        _neighbor.weight(weightSum);
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

    // store beta ratio
    double beta;

};

#endif //PROJECTNAME_KNAPSACKINCREVAL_H
