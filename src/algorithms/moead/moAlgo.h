//
// Created by jbuisine on 12/01/18.
//

#ifndef PHOTOALBUM_MOALGO_H
#define PHOTOALBUM_MOALGO_H

class MultiObjectiveAlgo {
public:
    std::vector<moSolution> pop ;
    unsigned maxEval;
    unsigned duration;
    /**
    * FFRMAB implementation
    * @param fileout
    */
    virtual void run(char *fileout) = 0;
};


#endif //PHOTOALBUM_MOALGO_H
