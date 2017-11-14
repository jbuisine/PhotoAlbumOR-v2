//
// Created by jbuisine on 13/11/17.
//

#ifndef PHOTOALBUM_PHOTOALBUMQAPEVAL_H
#define PHOTOALBUM_PHOTOALBUMQAPEVAL_H

#include <eoEvalFunc.h>

// rapidjson include
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

/**
 * Full evaluation Function for QAP problem
 */
template< class EOT >
class photoAlbumQAPEval : public eoEvalFunc<EOT>
{
public:

    /*
     * Constructor from instance file
     *
     * @param _fileData the file name which contains the instance of QAP from QAPlib
     */
    photoAlbumQAPEval(const char * _fileData, const char * _fileAlbum) {

        computeDistances(_fileData, _fileAlbum);
    }

    /*
     *  default destructor
     */
    ~photoAlbumQAPEval() {
        unsigned i;

        if (A != NULL) {
            for(i = 0; i < n; i++)
                delete[] A[i];
            delete[] A;
        }

        if (B != NULL) {
            for(i = 0; i < n; i++)
                delete[] B[i];
            delete[] B;
        }
    }

    /*
     * full evaluation for QAP
     *
     * @param _solution the solution to evaluate
     */
    void operator()(EOT & _solution) {
        int cost = 0;

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                cost += A[i][j] * B[_solution[i]][_solution[j]];

        _solution.fitness(cost);
    }

    /*
     * to get the matrix A
     *
     * @return matrix A
     */
    double** getA() {
        return A;
    }

    /*
     * to get the matrix B
     *
     * @return matrix B
     */
    double** getB() {
        return B;
    }

    /*
     * to get the number of objects, of variables
     *
     * @return number of objects
     */
    int getNbVar() {
        return n;
    }

private:
    // number of variables
    int n;

    // distance matrix A
    double ** A;

    // similitude matrix B
    double ** B;

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

    void computePhotoDistances(const char * fileName) {
        Document doc;

        // read the file
        std::ifstream f(fileName);
        std::string lines((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        f.close();

        // parser the json file
        doc.Parse(lines.c_str());

        const Value& array = doc;

        int photoSize = array.Size();
        B = new double*[photoSize];
        for(int i = 0; i < photoSize; i++)
            B[i] = new double[photoSize];

        // distance based on the distance between average hash
        for(int i = 0; i < photoSize; i++) {
            const Value& d = array[i]["ahashdist"];
            for(int j = 0; j < photoSize; j++) {
                B[i][j] = d[j].GetDouble();
            }
        }
    }

    /**
     *  Compute the matrice of distance between solutions
     *                  and of inverse distance between positions
     */
    void computeDistances(const char * photoFileName, const char * albumFileName) {
        computePhotoDistances(photoFileName);
        computeAlbumDistances(albumFileName);
    }
};

#endif //PHOTOALBUM_PHOTOALBUMQAPEVAL_H
