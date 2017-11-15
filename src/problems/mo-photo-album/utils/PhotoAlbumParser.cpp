//
// Created by jbuisine on 15/11/17.
//

#include <iostream>
#include <stdexcept>

#include "PhotoAlbumParser.h"

PhotoAlbumParser::PhotoAlbumParser(const std::string _dataFile, const std::string _dispositionFile )
{
    init(_dataFile, _dispositionFile);
}

PhotoAlbumParser::~PhotoAlbumParser() {

    if (A != NULL) {
        for(i = 0; i < n; i++)
            delete[] A[i];
        delete[] A;
    }

    if (BOne != NULL) {
        for(i = 0; i < n; i++)
            delete[] BOne[i];
        delete[] BOne;
    }

    if (BTwo != NULL) {
        for(i = 0; i < n; i++)
            delete[] BTwo[i];
        delete[] BTwo;
    }
}


const unsigned int PhotoAlbumParser::getN()
{
    return n;
}

const double** PhotoAlbumParser::getA() {
    return A;
}

const double** PhotoAlbumParser::getBTwo() {
    return BTwo;
}

const double** PhotoAlbumParser::getBOne() {
    return BOne;
}

void PhotoAlbumParser::printOn(std::ostream & _os) const
{
    _os << "print data form Parser" << std::endl;
}


void PhotoAlbumParser::init(const std::string _dataFile, const std::string _dispositionFile)
{
    computePhotoDistances(_dataFile);
    computeAlbumDistances(_dispositionFile);
}

void PhotoAlbumParser::computeAlbumDistances(const char *_fileName) {

    Document album;

    // read the file
    std::ifstream f(fileName);
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
}

void PhotoAlbumParser::computePhotoDistances(const char *_fileName) {

    Document doc;

    // read the file
    std::ifstream f(fileName);
    std::string lines((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    f.close();

    // parser the json file
    doc.Parse(lines.c_str());

    const Value& array = doc;

    int photoSize = array.Size();

    BOne = new double*[photoSize];
    Btwo = new double*[photoSize];

    for(int i = 0; i < photoSize; i++){
        BOne[i] = new double[photoSize];
        BTwo[i] = new double[photoSize];
    }

    // distance based on the distance between average hash
    for(int i = 0; i < photoSize; i++) {
        const Value& d1 = array[i]["ahashdist"];
        const Value& d1 = array[i]["dhashdist"];
        for(int j = 0; j < photoSize; j++) {
            BOne[i][j] = d1[j].GetDouble();
            BTwo[i][j] = d1[j].GetDouble();
        }
    }
}
