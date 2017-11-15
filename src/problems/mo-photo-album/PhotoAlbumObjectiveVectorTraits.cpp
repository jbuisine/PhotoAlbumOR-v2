//
// Created by jbuisine on 15/11/17.
//

#include "PhotoAlbumObjectiveVectorTraits.h"

bool FlowShopObjectiveVectorTraits::minimizing (int _i)
{
    // minimizing both
    return true;
}

bool FlowShopObjectiveVectorTraits::maximizing (int _i)
{
    // minimizing both
    return false;
}

unsigned int FlowShopObjectiveVectorTraits::nObjectives ()
{
    // 2 objectives
    return 2;
}