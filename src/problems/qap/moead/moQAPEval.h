//
// Created by jbuisine on 30/11/17.
//

#ifndef QAP_EVAL_H
#define QAP_EVAL_H

#include <algorithms/moead/moEval.h>
#include <problems/qap/moead/QAPUniParser.hpp>

class QAPEval : public moEval {
public:

    // constructor
    QAPEval(QAPUniParser _parser) : moEval() {

        // getting data
        A = _parser.getA();
        BOne = _parser.getBOne();
        BTwo = _parser.getBTwo();
        pbSize = _parser.getN();
    }

    // evaluation function
    virtual void operator()(moSolution & _solution) {

        _solution.objvec.resize(2);

        _solution.objvec[0] = flowObj1(_solution);
        _solution.objvec[1] = flowObj2(_solution);
    }

private:

    // distance matrix A
    double ** A;

    // first flow matrix
    double ** BOne;

    // second flow matrix
    double ** BTwo;

    /**
     * computation of first objective
     * @param _sol the genotype to evaluate
     */
    double flowObj1(moSolution & _sol)
    {
        double score = 0.;

        for (int i = 0; i < pbSize; i++) {
            for (int j = 0; j < pbSize; j++) {
                score += BOne[_sol[i]][_sol[j]] * A[i][j];
            }
        }

        return score;
    };

    /**
     * computation of second objective
     * @param _sol the genotype to evaluate
     */
    double flowObj2(moSolution & _sol)
    {
        double score = 0.;

        for (int i = 0; i < pbSize; i++) {
            for (int j = 0; j < pbSize; j++) {
                score += BTwo[_sol[i]][_sol[j]] * A[i][j];
            }
        }

        return score;
    };
};


#endif // QAP_EVAL_H
