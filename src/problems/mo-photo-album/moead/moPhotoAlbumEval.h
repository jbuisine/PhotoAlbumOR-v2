//
// Created by jbuisine on 30/11/17.
//

#ifndef PHOTOALBUM_MOPHOTOALBUMEVAL_H_H
#define PHOTOALBUM_MOPHOTOALBUMEVAL_H_H

#include "moEval.h"
#include "PhotoAlbumParser.hpp"

class PhotoAlbumEval : public moEval {
public:

    // constructor
    PhotoAlbumEval(PhotoAlbumParser _parser){

        // getting data
        A = _parser.getA();
        BOne = _parser.getBOne();
        BTwo = _parser.getBTwo();
        n = _parser.getN();
        moEval(n);
    }

    // evaluation function
    void operator()(moSolution & _solution) {

        _solution.objvec.resize(2);

        _solution.objvec[0] = similitudeObj1(_solution);
        _solution.objvec[1] = similitudeObj2(_solution);
    }

private:

    // problem size
    int n;

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
    double similitudeObj1(const moSolution & _photoAlbum)
    {
        double score = 0.;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                score += BOne[_photoAlbum[i]][_photoAlbum[j]] * A[i][j];
            }
        }

        return score;
    };

    /**
     * computation of second objective
     * @param _photoAlbum the genotype to evaluate
     */
    double similitudeObj2(const moSolution & _photoAlbum)
    {
        double score = 0.;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                score += BTwo[_photoAlbum[i]][_photoAlbum[j]] * A[i][j];
            }
        }

        return score;
    };
};


#endif //PHOTOALBUM_MOPHOTOALBUMEVAL_H_H
