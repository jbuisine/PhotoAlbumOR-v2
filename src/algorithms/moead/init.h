//
// Created by jbuisine on 04/12/17.
//

#ifndef QAP_INIT_H
#define QAP_INIT_H


/*
 * Standard Mutation operator with fixed mutation rates
 */
class InitQAP {
public:
    /**
     * Constructor
     **/
    InitQAP() {}

    void operator()(Solution & _solution) {

        _solution.resize(_solution.size());

        for(int i = 0; i < _solution.size(); i++){
            _solution[i] = i;
        }

        std::random_shuffle(&_solution[0], &_solution[_solution.size()-1]);
    }
};

#endif //QAP_INIT_H
