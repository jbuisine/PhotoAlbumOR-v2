//
// Created by jbuisine on 30/11/17.
//

#ifndef PHOTOALBUM_MOPHOTOALBUMEVAL_H_H
#define PHOTOALBUM_MOPHOTOALBUMEVAL_H_H

#include "algorithms/moead/moEval.h"
#include "PhotoAlbumParser.hpp"

class PhotoAlbumEval : public moEval {
public:

    // constructor
    PhotoAlbumEval(PhotoAlbumParser _parser) : moEval() {

        // getting data
        A = _parser.getA();
        BOne = _parser.getBOne();
        BTwo = _parser.getBTwo();
        pbSize = _parser.getN();
    }

    // evaluation function
    virtual void operator()(moSolution & _solution) {

        _solution.objvec.resize(2);

        _solution.objvec[0] = similitudeObj1(_solution);
        _solution.objvec[1] = similitudeObj2(_solution);
    }

private:

    // distance matrix A
    double ** A;

    // first similitude matrix
    double ** BOne;

    // second similitude matrix
    double ** BTwo;

    /**
     * computation of first objective
     * @param _photoAlbum the genotype to evaluate
     */
    double similitudeObj1(moSolution & _photoAlbum)
    {
        double score = 0.;

        for (int i = 0; i < pbSize; i++) {
            for (int j = 0; j < pbSize; j++) {
                score += BOne[_photoAlbum[i]][_photoAlbum[j]] * A[i][j];
            }
        }

        return score;
    };

    /**
     * computation of second objective
     * @param _photoAlbum the genotype to evaluate
     */
    double similitudeObj2(moSolution & _photoAlbum)
    {
        double score = 0.;

        for (int i = 0; i < pbSize; i++) {
            for (int j = 0; j < pbSize; j++) {
                score += BTwo[_photoAlbum[i]][_photoAlbum[j]] * A[i][j];
            }
        }

        return score;
    };
};


#endif //PHOTOALBUM_MOPHOTOALBUMEVAL_H_H
