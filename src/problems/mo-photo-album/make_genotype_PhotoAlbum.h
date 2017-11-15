//
// Created by jbuisine on 15/11/17.
//

#ifndef PHOTOALBUM_MAKE_GENOTYPE_PHOTOALBUM_H
#define PHOTOALBUM_MAKE_GENOTYPE_PHOTOALBUM_H


#include <utils/eoParser.h>
#include <utils/eoState.h>
#include "PhotoAlbum.h"
#include "PhotoAlbumInit.h"
#include "utils/PhotoAlbumParser.h"

/*
 * This function creates an eoInit<eoPhotoAlbum> that can later be used to initialize the population (see make_pop.h).
 * @param eoParser& _parser  to get user parameters
 * @param eoState& _state  to store the memory
 */
eoInit<PhotoAlbum> & do_make_genotype(eoParser& _parser, eoState& _state)
{
    // data file name
    std::string dataFileName = _parser.getORcreateParam(std::string("/home/jbuisine/Documents/Research/MetaheuristicsFramework/MH-ParadisEO/application/resources/photo-album/templates/FirstTemplate/info-photo.json"),
                                                        "Data photos",
                                                        "Data photos file name",
                                                        'B',
                                                        "Representation",
                                                        false).value();

    // disposition file
    std::string dispositionFileName = _parser.getORcreateParam(std::string("/home/jbuisine/Documents/Research/MetaheuristicsFramework/MH-ParadisEO/application/resources/photo-album/templates/FirstTemplate/album-6-2per3.json"),
                                                               "DispositionAlbum",
                                                               "Disposition album file name",
                                                               'B',
                                                               "Representation",
                                                               false).value();


    // reading of the parameters contained in the data and disposition files
    PhotoAlbumParser fParser(dataFileName, dispositionFileName);

    unsigned int n = fParser.getN();

    // build of the initializer (a pointer, stored in the eoState)
    eoInit<PhotoAlbum>* init = new PhotoAlbumInit(N);

    // store in state
    _state.storeFunctor(init);

    // and return a reference
    return *init;
}

#endif //PHOTOALBUM_MAKE_GENOTYPE_PHOTOALBUM_H
