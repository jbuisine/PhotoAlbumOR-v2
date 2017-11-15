//
// Created by jbuisine on 15/11/17.
//

#include "PhotoAlbumEval.h"

PhotoAlbumEval::PhotoAlbumEval(int _n, double **_A, double _BOne, double _BTwo) : n(_n), A(_A), BOne(_BOne), BTwo(_BTwo){}

void PhotoAlbumEval::operator()(PhotoAlbum &_photoAlbum) {

    PhotoAlbumObjectiveVector objVector;
    objVector[0] = similitudeObj1(_photoAlbum);
    objVector[1] = similitudeObj2(_photoAlbum);
    _photoAlbum.objectiveVector(objVector);
}

double PhotoAlbumEval::similitudeObj1(const PhotoAlbum &_photoAlbum) {
    double score = 0.;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            score += BOne[solution[i]][solution[j]] * A[i][j];
        }
    }

    return score;
}

double PhotoAlbumEval::similitudeObj2(const PhotoAlbum &_photoAlbum) {
    double score = 0.;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            score += Btwo[solution[i]][solution[j]] * A[i][j];
        }
    }

    return score;
}