//
// Created by jbuisine on 13/11/17.
//

#ifndef PHOTOALBUM_MOPHOTOALBUMQAPINCREVAL_H
#define PHOTOALBUM_MOPHOTOALBUMQAPINCREVAL_H

#include <eval/moEval.h>
#include <eval/qapEval.h>

/**
 * Incremental evaluation Function for the QAP problem
 */
template< class Neighbor >
class moPhotoAlbumQAPIncrEval : public moEval<Neighbor>
{
public:
    typedef typename Neighbor::EOT EOT;

    /*
     * default constructor
     * @param _qapEval full evaluation of the QAP problem
     */
    moQAPIncrEval(QAPeval<EOT> & _qapEval) {
        n = _qapEval.getNbVar();
        A = _qapEval.getA();
        B = _qapEval.getB();
    }

    /*
     * incremental evaluation of the neighbor for the QAP problem
     * @param _solution the solution to move (permutation)
     * @param _neighbor the neighbor to consider (of type moSwapNeigbor)
     */
    virtual void operator()(EOT & _solution, Neighbor & _neighbor) {
        int d;
        int k;

        unsigned i, j;

        _neighbor.getIndices(n, i, j);

        d = (A[i][i]-A[j][j])*(B[_solution[j]][_solution[j]]-B[_solution[i]][_solution[i]]) +
            (A[i][j]-A[j][i])*(B[_solution[j]][_solution[i]]-B[_solution[i]][_solution[j]]);

        for (k = 0; k < n; k++)
            if (k != i && k != j)
                d = d + (A[k][i]-A[k][j])*(B[_solution[k]][_solution[j]]-B[_solution[k]][_solution[i]]) +
                    (A[i][k]-A[j][k])*(B[_solution[j]][_solution[k]]-B[_solution[i]][_solution[k]]);

        _neighbor.fitness(_solution.fitness() + d);
    }

private:
    // number of variables
    int n;

    // matrix A
    int ** A;

    // matrix B
    int ** B;

};

#endif //PHOTOALBUM_MOPHOTOALBUMQAPINCREVAL_H
