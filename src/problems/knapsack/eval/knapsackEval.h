//
// Created by jbuisine on 08/11/17.
//

#ifndef PROJECTNAME_KNAPSACKEVAL_H
#define PROJECTNAME_KNAPSACKEVAL_H

#include <vector>
#include <eoEvalFunc.h>

/**
 * Full evaluation Function
 * for constrainted binary quadratic programming problem
 */
template< class EOT >
class knapsackEval : public eoEvalFunc<EOT>
{
public:

    /**
     * Constructor
     * instance is given by file :
     *  - size
     *  - profits
     *  - costs
     *  - capacity
     *
     * @param _fileName file name where size, profits, costs and capacity are saved
     */
    knapsackEval(std::string & _fileName) {
        std::fstream file(_fileName.c_str(), std::ios::in);

        if (!file) {
            std::string str = "knapsackEval: Could not open file [" + _fileName + "]." ;
            throw std::runtime_error(str);
        }

        // init vector
        W = new std::vector<unsigned int>();
        P = new std::vector<unsigned int>();

        // Size of solution
        string strNbElements;
        getline(file, strNbElements);
        nbVar = stoi(strNbElements);

        // Setting P profits & W weights vectors
        string strProfits;
        getline(file, strProfits);

        string strWeights;
        getline(file, strWeights);

        // Adding values to vectors
        gettingValues(P, strProfits, ' ');
        gettingValues(W, strWeights, ' ');

        // get capacity
        string strCapacity;
        getline(file, strCapacity);
        capacity = stoi(strCapacity);

        file.close();

        // Compute beta ratio value once
        beta = 0.;

        for(unsigned i = 0; i < nbVar; i++){

            double ratio = P->at(i) / W->at(i);

            if(ratio > beta){
                beta = ratio;
            }
        }
    }

    /**
     * Destructor
     */
    ~knapsackEval() {
        if (P != NULL) {
            delete P;
        }
        if (W != NULL) {
            delete W;
        }
    }

    /**
     * fitness evaluation of the solution
     *
     * @param _solution the solution to evaluation
     */
    virtual void operator()(EOT & _solution) {

        profitsSum = 0.0;
        weightsSum = 0.0;

        for(unsigned i = 0; i < nbVar; i++){
            if (_solution[i] == 1){
                profitsSum += P->at(i);
                weightsSum += W->at(i);
            }
        }

        if(weightsSum > capacity){
            profitsSum -= beta * abs(weightsSum - capacity);
        }

        _solution.fitness(profitsSum);
        _solution.weight(weightsSum);
    }


    /*
     * to get W weights vector
     *
     * @return weights vector
     */
    vector<unsigned int>* getW() {
        return W;
    }

    /*
     * to get P profits vector
     *
     * @return profits vector
     */
    vector<unsigned int>* getP() {
        return P;
    }

    /*
    * to get knapsack capacity
    *
    * @return capacity
    */
    double getCapacity(){
        return capacity;
    }

    /*
     * to get beta ratio
     *
     * @return beta ratio
     */
    double getBeta(){
        return beta;
    }

    /*
     * to get the number of variable (bit string length)
     *
     * @return bit string length
     */
    int getNbVar() {
        return nbVar;
    }

    void print() {
        std::cout << nbVar << std::endl;
        std::cout << "Profits vector" << std::endl;
        for(unsigned int i = 0; i < nbVar; i++) {
            std::cout << P->at(i) << " ";
        }
        std::cout << std::endl;

        std::cout << "Weights vector" << std::endl;
        for(unsigned int i = 0; i < nbVar; i++) {
            std::cout << W->at(i) << " ";
        }
        std::cout << std::endl;

    }

private:
    /**
     * variables (used in incremental evaluation)
     */

    // size of solution
    unsigned int nbVar;

    // knapsack capacity
    unsigned int capacity;

    // vector of profits
    std::vector<unsigned int>* P;

    // vector of weights
    std::vector<unsigned int>* W;

    // store last profits obtained
    double profitsSum;

    // store last weightSum
    double weightsSum;

    // store beta ratio
    double beta;

    /**
     * Method which split string
     *
     * @param v : vector which stores values
     * @param val : string value to split
     * @param sep : char separator
     */
    void gettingValues(vector<unsigned int>* v, string val, char sep){

        istringstream f(val);
        string s;
        while(getline(f, s, sep)){
            v->push_back(stod(s));
        }
    }
};


#endif //PROJECTNAME_KNAPSACKEVAL_H
