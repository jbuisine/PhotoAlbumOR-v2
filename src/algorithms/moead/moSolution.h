#ifndef _moSolution_h
#define _moSolution_h

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Solution.h"

using namespace std;

/**
   Classe pour représenter une solution
   
   Chaine d'entier auquel est ajoutée la valeur de la qualité (fitness)
   + objective vector
*/
class moSolution : public Solution {
public:
  moSolution() : Solution() {}

  moSolution(const moSolution & _s) : Solution(_s) {
    objvec = _s.objvec;
  }

  moSolution& operator=(moSolution & _s) {
    objvec = _s.objvec;

    this->resize(_s.size());
    for(unsigned int i = 0; i < _s.size(); i++)
      this->operator[](i) = _s[i];

    this->fitness(_s.fitness());
    this->ID(_s.ID());
    this->dir(_s.dir());
    this->from(_s.from());
    this->best(_s.best());

    return *this;
  }

  /**
   * print the solution
   */
  virtual void print() {
    std::cout << toString() << std::endl;
  }

  /**
   * save the solution
   */
  virtual void save(FILE * fileout) {
    unsigned i;
    
    fprintf(fileout, "%d %lf ", this->ID(), this->fitness());
    for(i = 0; i < objvec.size(); i++) fprintf(fileout, "%lf ", objvec[i]);
    //for(i = 0; i < this->size(); i++) fprintf(fileout, "%d ", this->operator[](i));
    fprintf(fileout, "%d %d %d %d\n", this->dir(), this->from(), this->best(), this->op());
    fflush(fileout);
  }

  /**
   * pconvert the solution into string
   */
  virtual string toString() {
    std::stringstream ss;

    ss << this->fitness() ;

//    ss << " " << objvec.size() ;
    for(unsigned int i = 0; i < objvec.size(); i++)
      ss << " " << objvec[i] ;
      
//    ss << " " << this->size() ;

    for(unsigned int i = 0; i < this->size(); i++)
      ss << " " << this->operator[](i) ;

    return ss.str();
  }

  /**
   * Operator which uses to sort object by specific objective
   */
  bool operator< (moSolution &other) {
      return objvec[0] < other.objvec[0];
  }

  //protected:
  // objective vector
  vector<double> objvec;

};

#endif
