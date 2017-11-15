//
// Created by jbuisine on 15/11/17.
//

#ifndef PHOTOALBUM_PHOTOALBUMOBJECTIVEVECTOR_H
#define PHOTOALBUM_PHOTOALBUMOBJECTIVEVECTOR_H

#include <core/moeoRealObjectiveVector.h>
#include "PhotoAlbumObjectiveVectorTraits.h"

/**
 * Definition of the objective vector for multi-objective photo album QAP problems: a vector of doubles
 */
typedef moeoRealObjectiveVector < PhotoAlbumObjectiveVectorTraits > PhotoAlbumObjectiveVector;


#endif //PHOTOALBUM_PHOTOALBUMOBJECTIVEVECTOR_H
