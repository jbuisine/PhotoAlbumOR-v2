//
// Created by jbuisine on 06/01/18.
//

#ifndef PHOTOALBUM_SOLUTION_H
#define PHOTOALBUM_SOLUTION_H

/**
 * Abstract solution class for combinatory problem
 */
class Solution {

public:
    Solution() {};

    // operator overload
    int &operator[](int i) {
        // check size
        if( i > n ) {
            throw("Index out of bounds");
        }

        return arr[i];
    }

    /**
     * Getters
     */
    void ID(unsigned _id){
        this->id = _id;
    };

    void resize(const unsigned _size){
        arr = new int[_size];
        this->n = _size;
    };

    void fitness(double _fitness){
        this->score = _fitness;
    }

    void best(unsigned _best){
        this->bestInfo = _best;
    }

    void from(unsigned _from){
        this->fromInfo = _from;
    }

    void dir(unsigned _dir){
        this->dirInfo = _dir;
    }

    /**
     * Setters
     */
    unsigned ID(){
        return id;
    };

    unsigned size(){
        return n;
    };

    double fitness(){
        return score;
    }

    int best(){
        return bestInfo;
    }

    unsigned from(){
        return fromInfo;
    }

    unsigned dir(){
        return dirInfo;
    }

    virtual void print() = 0;

protected:
    /**
     * Utils information about solution
     */
    unsigned id;
    int bestInfo;
    unsigned fromInfo;
    unsigned dirInfo;

    // size and fitness value
    unsigned n;
    double score;

    // problem info
    int* arr;
};

#endif //PHOTOALBUM_SOLUTION_H

