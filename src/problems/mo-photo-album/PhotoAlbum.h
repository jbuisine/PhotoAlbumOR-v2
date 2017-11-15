//
// Created by jbuisine on 15/11/17.
//

#ifndef PHOTOALBUM_PHOTOALBUM_H
#define PHOTOALBUM_PHOTOALBUM_H

#include <core/moeoVector.h>
#include "PhotoAlbumObjectiveVector.h"

/**
 *  Structure of the genotype for the photo album QAP problem: a vector of unsigned int int.
 */
class PhotoAlbum: public moeoVector < PhotoAlbumObjectiveVector , unsigned int >
{
public:

    /**
     * class name
     */
    std::string className() const;

};


#endif //PHOTOALBUM_PHOTOALBUM_H
