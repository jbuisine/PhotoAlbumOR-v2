//
// Created by jbuisine on 16/11/17.
//

#ifndef PHOTOALBUM_MAKE_OP_PHOTOALBUM_H
#define PHOTOALBUM_MAKE_OP_PHOTOALBUM_H


#include <utils/eoParser.h>
#include <utils/eoState.h>
#include <eoOp.h>
#include <eoGenOp.h>
#include <eoCloneOps.h>
#include <eoOpContainer.h>
#include <eoProportionalCombinedOp.h>
#include "PhotoAlbumOpCrossoverQuad.hpp"
#include "PhotoAlbumOpMutationShift.h"
#include "PhotoAlbumOpMutationExchange.h"

/*
 * This function builds the operators that will be applied to the eoPhotoAlbum
 * @param eoParameterLoader& _parser to get user parameters
 * @param eoState& _state to store the memory
 */
eoGenOp<PhotoAlbum> & do_make_op(eoParameterLoader& _parser, eoState& _state)
{

    /////////////////////////////
    // Variation operators
    ////////////////////////////

    // the crossover
    ////////////////

    // a first crossover
    eoQuadOp<PhotoAlbum> *cross = new PhotoAlbumOpCrossoverQuad;
    // store in the state
    _state.storeFunctor(cross);

    // relative rate in the combination
    double cross1Rate = _parser.createParam(1.0, "crossRate", "Relative rate for the only crossover", 0, "Variation Operators").value();
    // creation of the combined operator with this one
    eoPropCombinedQuadOp<PhotoAlbum> *propXover = new eoPropCombinedQuadOp<PhotoAlbum>(*cross, cross1Rate);
    // store in the state
    _state.storeFunctor(propXover);

    // the mutation
    ///////////////

    // a first mutation : the shift mutation
    eoMonOp<PhotoAlbum> *mut = new PhotoAlbumOpMutationShift;
    _state.storeFunctor(mut);

    // its relative rate in the combination
    double mut1Rate = _parser.createParam(0.5, "shiftMutRate", "Relative rate for shift mutation", 0, "Variation Operators").value();

    // creation of the combined operator with this one
    eoPropCombinedMonOp<PhotoAlbum> *propMutation = new eoPropCombinedMonOp<PhotoAlbum>(*mut, mut1Rate);
    _state.storeFunctor(propMutation);

    // a second mutation : the exchange mutation
    mut = new PhotoAlbumOpMutationExchange;
    _state.storeFunctor(mut);

    // its relative rate in the combination
    double mut2Rate = _parser.createParam(0.5, "exchangeMutRate", "Relative rate for exchange mutation", 0, "Variation Operators").value();

    // addition of this one to the combined operator
    propMutation -> add(*mut, mut2Rate);

    // end of crossover and mutation definitions
    ////////////////////////////////////////////

    // First read the individual level parameters
    eoValueParam<double>& pCrossParam = _parser.createParam(0.25, "pCross", "Probability of Crossover", 'c', "Variation Operators" );
    // minimum check
    if ( (pCrossParam.value() < 0) || (pCrossParam.value() > 1) )
        throw std::runtime_error("Invalid pCross");

    eoValueParam<double>& pMutParam = _parser.createParam(0.35, "pMut", "Probability of Mutation", 'm', "Variation Operators" );
    // minimum check
    if ( (pMutParam.value() < 0) || (pMutParam.value() > 1) )
        throw std::runtime_error("Invalid pMut");

    // the crossover - with probability pCross
    eoProportionalOp<PhotoAlbum> * propOp = new eoProportionalOp<PhotoAlbum> ;
    _state.storeFunctor(propOp);
    eoQuadOp<PhotoAlbum> *ptQuad = new eoQuadCloneOp<PhotoAlbum>;
    _state.storeFunctor(ptQuad);
    propOp -> add(*propXover, pCrossParam.value()); // crossover, with proba pcross
    propOp -> add(*ptQuad, 1-pCrossParam.value()); // nothing, with proba 1-pcross

    // now the sequential
    eoSequentialOp<PhotoAlbum> *op = new eoSequentialOp<PhotoAlbum>;
    _state.storeFunctor(op);
    op -> add(*propOp, 1.0);	 // always do combined crossover
    op -> add(*propMutation, pMutParam.value()); // then mutation, with proba pmut

    // return a reference
    return *op;
}

#endif //PHOTOALBUM_MAKE_OP_PHOTOALBUM_H
