//
// Created by jbuisine on 09/11/17.
//

#ifndef PROJECTNAME_MOBITKNAPSACKNEIGHBOR_H
#define PROJECTNAME_MOBITKNAPSACKNEIGHBOR_H

#include <ga/eoBit.h>
#include <neighborhood/moBackableNeighbor.h>
#include <neighborhood/moIndexNeighbor.h>
#include "eoBitKnapsack.h"

/**
 * Neighbor related to a vector of Bit
 */
template< class Fitness >
class moBitKnapsackNeighbor : public moBackableNeighbor<eoBitKnapsack<Fitness> >, public moIndexNeighbor<eoBitKnapsack<Fitness> >
{
public:
    typedef eoBitKnapsack<Fitness> EOT;

    using moBackableNeighbor<EOT>::fitness;
    using moIndexNeighbor<EOT>::key;

    /**
     * move the solution
     * @param _solution the solution to move
     */
    virtual void move(EOT & _solution) {
        _solution[key] = !_solution[key];
        _solution.invalidate();   // only there for eoEvalFuncCounter !
    }

    /**
     * move back the solution (useful for the evaluation by modif)
     * @param _solution the solution to move back
     */
    virtual void moveBack(EOT & _solution) {
        move(_solution);
    }

    /**
     * return the class name
     * @return the class name as a std::string
     */
    virtual std::string className() const {
        return "moBitNeighbor";
    }

    /**
     * Read object.\
     * Calls base class, just in case that one had something to do.
     * The read and print methods should be compatible and have the same format.
     * In principle, format is "plain": they just print a number
     * @param _is a std::istream.
     * @throw runtime_std::exception If a valid object can't be read.
     */
    virtual void readFrom(std::istream& _is) {
        std::string fitness_str;
        int pos = _is.tellg();
        _is >> fitness_str;
        if (fitness_str == "INVALID") {
            throw std::runtime_error("invalid fitness");
        } else {
            Fitness repFit;
            _is.seekg(pos);
            _is >> repFit;
            _is >> key;
            fitness(repFit);
        }
    }

    /**
     * Write object. Called printOn since it prints the object _on_ a stream.
     * @param _os A std::ostream.
     */
    virtual void printOn(std::ostream& _os) const {
        _os << fitness() << ' ' << key << std::endl;
    }

    /**
     * to set knapsack weight
     *
     * @param w
     */
    void weight(double w){ this->weightSum = w; }

    /**
     * to get knapsack weight
     *
     * @return weightSum
     */
    double weight(){ return weightSum; }

private:

    double weightSum;
};

#endif //PROJECTNAME_MOBITKNAPSACKNEIGHBOR_H
