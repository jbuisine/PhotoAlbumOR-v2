//
// Created by jbuisine on 15/11/17.
//

#ifndef PHOTOALBUMPARSER_H
#define PHOTOALBUMPARSER_H

#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>

/**
 * Class to handle data of a QAP instance
 */

class QAPUniParser
{
public:

    /**
     * Constructor
     *
     * @param _dataFile the name of the QAP data file
     */
    QAPUniParser(const std::string _dataFile)
    {
        init(_dataFile);
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

    // number of factory
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
     * @param _dataFile the name of QAP instance data file
     */
    void init(const std::string _dataFile)
    {
        // read data file
        std::ifstream file(_dataFile, ios::in);

        std::string line;

        // default get first line information
        std::getline(file, line);
        std::cout << "Problem instance : " << line << std::endl;

        // getting size problem
        std::getline(file, line);
        n = stoi(line);

        writeMatrixData(file, A);
        writeMatrixData(file, BOne);
        writeMatrixData(file, BTwo);

        file.close();
    };

    /**
     * Method which write into matrix problem data
     * @param _file
     * @param m
     */
    void writeMatrixData(std::ifstream& _file, double**& m){

        // init matrix first dimension size
        m = new double*[n];

        std::string line;

        for(int i = 0; i < n; i++){

            // get next line data
            std::getline(_file, line);

            // to avoid empty line
            if(line.empty()){
                i -= 1;
                continue;
            }

            stringstream ss(line);

            // set vectors data
            std::vector<string> strs;
            std::vector<double> rowDist;

            std::string entry;

            while (ss >> entry)
                strs.push_back(entry);

            for (int j = 0; j < n; ++j) {
                rowDist.push_back(stod(strs.at(j)));
            }
            m[i] = new double[n];

            for (int j = 0; j < n; ++j) {
                m[i][j] = rowDist.at(j);
            }
        }
    }
};

#endif //PHOTOALBUMPARSER_H
