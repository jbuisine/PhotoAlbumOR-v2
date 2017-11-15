//
// Created by jbuisine on 15/11/17.
//

#ifndef PHOTOALBUM_PHOTOALBUMPARSER_H
#define PHOTOALBUM_PHOTOALBUMPARSER_H

#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>


// rapidjson include
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

/**
 * Class to handle data of a PhotoAlbum QAP instance
 */

class PhotoAlbumParser
{
public:

    /**
     * Ctor
     *
     * @param _dataFile the name of the photo data file
     * @param _dispositionFile photo album disposition information
     */
    PhotoAlbumParser(const std::string _dataFile, const std::string _dispositionFile);

    ~PhotoAlbumParser();

    /**
     * the number of photos
     */
    const unsigned int getN();


    /**
     * distance matrix
     */
    const double** getA();


    /**
     * first similitude matrix
     */
    const double** getBOne();


    /**
     * second similitude matrix
     */
    const double** getBtwo();


    /**
     * printing...
     */
    void printOn(std::ostream & _os) const;


private:

    // number of variables
    int n;

    // distance matrix A
    double ** A;

    // first similitude matrix
    double ** BOne;

    // second similitude matrix
    double ** BTwo;

    /**
     * Initialisation of the parameters with the data photo and disposition files
     *
     * @param _dataFile the name of the photo data file
     * @param _dispositionFile the name of the album disposition file
     */
    void init(const std::string _dataFile, const std::string _dispositionFile);

    /**
     * Inverse euclidean distances to create A matrix
     *
     * @param size
     * @param i
     * @param j
     * @return
     */
    double inverseDistance(int _size, int _i, int _j);

    /**
     * Util function used for determine distances from album disposition
     *
     * @param _fileName
     */
    void computeAlbumDistances(const char * _fileName);

    /**
     * Util function used for determine distances form each photo
     *
     * @param _fileName
     */
    void computePhotoDistances(const char * _fileName);
};

#endif //PHOTOALBUM_PHOTOALBUMPARSER_H
