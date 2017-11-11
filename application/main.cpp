//
// Created by jbuisine on 11/11/17.
//

// rapidjson/example/simpledom/simpledom.cpp`
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <iostream>
#include <fstream>

using namespace rapidjson;
using namespace std;

// Distance between photos
int photoSize;
double ** photoDist;

// Inverse of the distance between positions in the album
int albumSize;
double ** albumInvDist;


/**
 *
 * Example of json file parsing
 *
 * see: https://code.google.com/p/json-simple/
 * for more example to decode json under java
 *
 */
void readPhotoExample(const char * fileName) {
    Document document;

    // read the file
    std::ifstream f(fileName);
    std::string lines((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    f.close();

    // parser the json file
    document.Parse(lines.c_str());

    // to print all the document
    /*
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);
    std::cout << buffer.GetString() << std::endl;
    */

    // id of the first element
    std::cout << document[0]["id"].GetInt() << std::endl;

    const Value& w = document[0]["tags"]["classes"];

    for (SizeType i = 0; i < w.Size(); i++) // rapidjson uses SizeType instead of size_t.
        printf("w[%d] = %s\n", i, w[i].GetString());
}

double inverseDistance(int size, int i, int j) {
    // number of pages
    int pagei = i / size;
    int pagej = j / size;

    if (pagei != pagej)
        // not on the same page: distance is infinite. Another choice is possible of course!
        return 0;
    else {
        // positions in the page
        int posi = i % size;
        int posj = j % size;

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
}

void computeAlbumDistances(const char * fileName) {
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
    albumSize = 0;
    for(int i = 0; i < pageSize.Size(); i++)
        albumSize += pageSize[i].GetInt();

    albumInvDist = new double*[albumSize];
    for(int i = 0; i < albumSize; i++)
        albumInvDist[i] = new double[albumSize];

    // compute the distance
    for(int i = 0; i < albumSize; i++)
        for(int j = 0; j < albumSize; j++)
            albumInvDist[i][j] = inverseDistance(size, i, j);

    /*
    for(int i = 0; i < albumSize; i++) {
      for(int j = 0; j < albumSize; j++) {
	cout << " " <<  albumInvDist[i][j];
      }
      cout << endl;
    }
    */
}

void computePhotoDistances(const char * fileName) {
    Document document;

    // read the file
    std::ifstream f(fileName);
    std::string lines((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    f.close();

    // parser the json file
    document.Parse(lines.c_str());

    const Value& array = document;

    photoSize = array.Size();
    photoDist = new double*[photoSize];
    for(int i = 0; i < photoSize; i++)
        photoDist[i] = new double[photoSize];

    // distance based on the distance between average hash
    for(int i = 0; i < photoSize; i++) {
        const Value& d = array[i]["ahashdist"];
        for(int j = 0; j < photoSize; j++) {
            photoDist[i][j] = d[j].GetDouble();
        }
    }

    /*
    for(int i = 0; i < photoSize; i++) {
      for(int j = 0; j < photoSize; j++) {
	cout << " " <<  photoDist[i][j];
      }
      cout << endl;
    }
    */
}

/**
 *  Compute the matrice of distance between solutions
 *                  and of inverse distance between positions
 */
void computeDistances(const char * photoFileName, const char * albumFileName) {
    computePhotoDistances(photoFileName);
    computeAlbumDistances(albumFileName);
}

/**
 * Un exemple de fonction objectif (à minimiser):
 *   distance entre les photos pondérées par l'inverse des distances spatiales sur l'album
 *   Modélisaiton comme un problème d'assignement quadratique (QAP)
 *
 *   Dans cette fonction objectif,
 *      pas de prise en compte d'un effet de page (harmonie/cohérence de la page)
 *      par le choix de distance, pas d'intéraction entre les photos sur des différentes pages
 */
double eval(int * solution) {
    double sum = 0;

    for(int i = 0; i < albumSize; i++) {
        for(int j = i + 1; j < albumSize; j++) {
            sum += photoDist[ solution[i] ][ solution[j] ] * albumInvDist[i][j] ;
        }
    }

    return sum;
}

int main() {
    // Path to the photo information file in json format
    const char * photoFileName = "/Users/verel/enseignement/15-16/RO/projet/prj1-ro/data/info-photo.json";
    // Path to the album information file in json format
    const char * albumFileName = "/Users/verel/enseignement/15-16/RO/projet/prj1-ro/data/info-album.json";

    // uncomment to test it
    //readPhotoExample(photoFileName);

    computeDistances(photoFileName, albumFileName);

    // one basic solution : order of the index

    int numberOfPhoto = 55;
    int solution[numberOfPhoto];

    for(int i = 0; i < 55; i++)
        solution[i] = i;

    // compute the fitness
    cout << eval(solution) << endl;

    // free memory
    for(int i = 0; i < photoSize; i++)
        delete [] photoDist[i];
    delete [] photoDist;

    for(int i = 0; i < albumSize; i++)
        delete [] albumInvDist[i];
    delete [] albumInvDist;

    return 0;
}
