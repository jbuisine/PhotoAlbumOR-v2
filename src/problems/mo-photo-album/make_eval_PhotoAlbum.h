//
// Created by jbuisine on 15/11/17.
//

#ifndef PHOTOALBUM_MAKE_EVAL_PHOTOALBUM_H
#define PHOTOALBUM_MAKE_EVAL_PHOTOALBUM_H

#include <utils/eoParser.h>
#include <utils/eoState.h>
#include <eoEvalFuncCounter.h>
#include "PhotoAlbum.h"
#include "utils/PhotoAlbumParser.h"
#include "PhotoAlbumEval.h"

/*
 * This function creates an eoEvalFuncCounter<eoPhotoAlbum> that can later be used to evaluate an individual.
 * @param eoParser& _parser  to get user parameters
 * @param eoState& _state  to store the memory
 */
eoEvalFuncCounter<PhotoAlbum> & do_make_eval(eoParser& _parser, eoState& _state)
{
    // data file name
    std::string dataFileName = _parser.getORcreateParam(std::string("/home/jbuisine/Documents/Research/MetaheuristicsFramework/MH-ParadisEO/application/resources/photo-album/templates/FirstTemplate/info-photo.json"),
                                                        "Data photos",
                                                        "Data photos file name",
                                                        'B',
                                                        "Representation",
                                                        false).value();

    std::string dispositionFileName = _parser.getORcreateParam(std::string("/home/jbuisine/Documents/Research/MetaheuristicsFramework/MH-ParadisEO/application/resources/photo-album/templates/FirstTemplate/album-6-2per3.json"),
                                                        "DispositionAlbum",
                                                        "Disposition album file name",
                                                        'B',
                                                        "Representation",
                                                        false).value();


    // reading of the parameters contained in the data and disposition files
    PhotoAlbumParser fParser(dataFileName, dispositionFileName);

    unsigned int n = fParser.getN();
    double** A = fParser.getA();
    double** BOne = fParser.getBOne();
    double** BTwo = fParser.getBTwo();

    // build of the initializer (a pointer, stored in the eoState)
    PhotoAlbumEval* plainEval = new PhotoAlbumEval(n, A, BOne, BTwo);

    // turn that object into an evaluation counter
    eoEvalFuncCounter<PhotoAlbum>* eval = new eoEvalFuncCounter<PhotoAlbum> (* plainEval);

    // store in state
    _state.storeFunctor(eval);

    // and return a reference
    return *eval;
}

#endif //PHOTOALBUM_MAKE_EVAL_PHOTOALBUM_H
