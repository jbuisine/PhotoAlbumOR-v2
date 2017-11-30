//
// Created by jbuisine on 15/11/17.
//

#ifndef PHOTOALBUMPARSER_H
#define PHOTOALBUMPARSER_H

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
     * Constructor
     *
     * @param _dataFile the name of the photo data file
     * @param _dispositionFile photo album disposition information
     */
    PhotoAlbumParser(const std::string _dataFile, const std::string _dispositionFile)
    {
        init(_dataFile, _dispositionFile);
    };

    /**
     * the number of photos
     */
    unsigned int getN()
    {
        return n;
    };


    /**
     * distance matrix
     */
    double** getA()
    {
        return A;
    };


    /**
     * first similitude matrix
     */
    double** getBOne()
    {
        return BOne;
    };


    /**
     * second similitude matrix
     */
    double** getBTwo()
    {
        return BTwo;
    };


    /**
     * printing...
     */
    void printOn(std::ostream & _os) const
    {
        _os << "print data form Parser" << std::endl;
    };


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
    void init(const std::string _dataFile, const std::string _dispositionFile)
    {
        computePhotoDistances(_dataFile);
        computeAlbumDistances(_dispositionFile);
    };

    /**
     * Inverse euclidean distances to create A matrix
     *
     * @param size
     * @param i
     * @param j
     * @return
     */
    double inverseDistance(int _size, int _i, int _j)
    {
        // number of pages
        int pagei = _i / _size;
        int pagej = _j / _size;

        if (pagei != pagej)
            // not on the same page: distance is infinite. Another choice is possible of course!
            return 0;
        else {
            // positions in the page
            int posi = _i % _size;
            int posj = _j % _size;

            // coordinate on the page
            int xi = posi % 2;
            int yi = posi / 2;
            int xj = posj % 2;
            int yj = posj / 2;

            // Manhatthan distance
            double d = (double) (abs(xi - xj) + abs(yi - yj));

            if (d == 0)
                return -1;
            else
                return ((double) 1) / d;
        }
    };

    /**
     * Util function used for determine distances from album disposition
     *
     * @param _fileName
     */
    void computeAlbumDistances(const std::string _fileName)
    {
        Document album;

        // read the file
        std::ifstream f(_fileName);
        std::string lines((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        f.close();

        // parser the json file
        album.Parse(lines.c_str());

        // number of pages
        int nPage = album["page"].GetInt();

        // number of photo in each page
        const Value& pageSize = album["pagesize"];

        // number on the first page
        int size = pageSize[0].GetInt();

        // total number of photo in the album
        int albumSize = 0;
        for(int i = 0; i < pageSize.Size(); i++)
            albumSize += pageSize[i].GetInt();

        n = albumSize;
        A = new double*[albumSize];
        for(int i = 0; i < albumSize; i++)
            A[i] = new double[albumSize];

        // compute the distance
        for(int i = 0; i < albumSize; i++)
            for(int j = 0; j < albumSize; j++)
                A[i][j] = inverseDistance(size, i, j);
    };

    /**
     * Util function used for determine distances form each photo
     *
     * @param _fileName
     */
    void computePhotoDistances(const std::string _fileName)
    {
        Document doc;

        // read the file
        std::ifstream f(_fileName);
        std::string lines((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        f.close();

        // parser the json file
        doc.Parse(lines.c_str());

        const Value& array = doc;

        int photoSize = array.Size();

        BOne = new double*[photoSize];
        BTwo = new double*[photoSize];

        for(int i = 0; i < photoSize; i++){
            BOne[i] = new double[photoSize];
            BTwo[i] = new double[photoSize];
        }

        // distance based on the distance between average hash
        for(int i = 0; i < photoSize; i++) {
            const Value& d1 = array[i]["ahashdist"];
            const Value& d2 = array[i]["dhashdist"];
            for(int j = 0; j < photoSize; j++) {
                BOne[i][j] = d1[j].GetDouble();
                BTwo[i][j] = d2[j].GetDouble();
            }
        }
    };
};

#endif //PHOTOALBUMPARSER_H
