//
// Created by jbuisine on 15/11/17.
//

#ifndef PHOTOALBUM_PHOTOALBUMEVAL_H
#define PHOTOALBUM_PHOTOALBUMEVAL_H


#include <vector>
#include <core/moeoEvalFunc.h>
#include "PhotoAlbum.hpp"

/**
 * Evaluation of the objective vector a (multi-objective) PhotoAlbum object
 */
class PhotoAlbumEval : public moeoEvalFunc<PhotoAlbum>
{
public:

    /**
     * Constructor
     *
     * @param _n : problem size
     * @param _A : distance matrix
     * @param _BOne : first similitude matrix
     * @param _BTwo : second similitude matrix
     */
    PhotoAlbumEval(int _n, double** _A, double ** _BOne, double ** _BTwo)  : n(_n), A(_A), BOne(_BOne), BTwo(_BTwo) {};

    /**
     * computation of the multi-objective evaluation of a PhotoAlbum object
     * @param _photoAlbum the PhotoAlbum object to evaluate
     */
    void operator()(PhotoAlbum & _photoAlbum)
    {
        if (_photoAlbum.invalidObjectiveVector())
        {
            PhotoAlbumObjectiveVector objVector;
            objVector[0] = similitudeObj1(_photoAlbum);
            objVector[1] = similitudeObj2(_photoAlbum);
            _photoAlbum.objectiveVector(objVector);
        }
    };


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
    double similitudeObj1(const PhotoAlbum & _photoAlbum)
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
    double similitudeObj2(const PhotoAlbum & _photoAlbum)
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


#endif //PHOTOALBUM_PHOTOALBUMEVAL_H
