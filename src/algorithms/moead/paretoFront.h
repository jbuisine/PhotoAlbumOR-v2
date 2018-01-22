//
// Created by jbuisine on 16/01/18.
//

#ifndef PHOTOALBUM_PARETOFRONT_H
#define PHOTOALBUM_PARETOFRONT_H

#include <algorithms/moead/moSolution.h>
#include <algorithm>
/**
 * Class which returns pareto front from population
 */
class ParetoFront{
public:
    std::vector<moSolution> operator()(std::vector<moSolution> _pop, bool _minimize){

        unsigned popSize = _pop.size();

        if(popSize <= 0){
            throw "Error, population is empty";
        }

        unsigned nbObjective = _pop[0].objvec.size();

        std::vector<moSolution> paFront;
        std::vector<unsigned> indexes;

        // check if solution dominate all others solutions
        for (int i = 0; i < _pop.size(); ++i) {

            for (int j = 0; j < _pop.size(); ++j) {

                unsigned nbDominatedObjective = 0;

                for (int k = 0; k < nbObjective; ++k) {

                    // check problem type (minimize or maximize)
                    if(_minimize){

                        if(_pop[i].objvec.at(k) > _pop[j].objvec.at(k)){
                            nbDominatedObjective++;
                        }
                    }else{

                        if(_pop[i].objvec.at(k) < _pop[j].objvec.at(k)){
                            nbDominatedObjective++;
                        }
                    }
                }

                // check if solution is non dominated or not
                if(nbDominatedObjective == nbObjective) {
                    indexes.push_back(i);
                    goto nextSol;
                }
            }
            nextSol: std::cout;
        }

        // add non dominated solution found
        for (int i = 0; i < popSize; ++i) {

            if(!(std::find(indexes.begin(), indexes.end(), i) != indexes.end())) {
                paFront.push_back(_pop.at(i));
            }
        }

        return paFront;
    }
};

#endif //PHOTOALBUM_PARETOFRONT_H
