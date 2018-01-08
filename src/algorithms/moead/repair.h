//
// Created by jbuisine on 18/12/17.
//

#ifndef PHOTOALBUM_REPAIR_H
#define PHOTOALBUM_REPAIR_H

#endif //PHOTOALBUM_REPAIR_H


class HillClimber{

public:

    /**
     * Constructor
     **/
    HillClimber(unsigned _nbIteration, unsigned _problemSize) {
        this->nbIteration = _nbIteration;
        this->problemSize = _problemSize;
    }


    moSolution& operator()(moSolution& _solution){

        unsigned iteration = 0;
        double bestScore = _solution.fitness();

        while(iteration < nbIteration){

            for(int i = 0; i < problemSize; i++){

                int index = rand() % problemSize;

                int tempValue = _solution[index];

                _solution[index] = _solution[i];
                _solution[i] = _solution[tempValue];

                // TODO How to compute this fitness ?
                double currentScore = _solution.fitness();

                iteration++;
            }
        }
    }


private:

    // number of iteration of local search
    unsigned nbIteration;

    // problem size indication
    unsigned problemSize;
};