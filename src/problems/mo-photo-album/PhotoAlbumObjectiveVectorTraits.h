//
// Created by jbuisine on 15/11/17.
//

#ifndef PHOTOALBUM_PHOTOALBUMOBJECTIVEVECTORTRAITS_H
#define PHOTOALBUM_PHOTOALBUMOBJECTIVEVECTORTRAITS_H

#include <core/moeoObjectiveVectorTraits.h>

/**
 * Definition of the objective vector traits for multi-objective flow-shop problems
 */
class PhotoAlbumObjectiveVectorTraits : public moeoObjectiveVectorTraits
{

public:

    /**
     * Returns true if the _ith objective have to be minimzed
     * @param _i index of the objective
     */
    static bool minimizing (int _i);


    /**
     * Returns true if the _ith objective have to be maximzed
     * @param _i index of the objective
     */
    static bool maximizing (int _i);


    /**
     * Returns the number of objectives
     */
    static unsigned int nObjectives ();

};



#endif //PHOTOALBUM_PHOTOALBUMOBJECTIVEVECTORTRAITS_H
