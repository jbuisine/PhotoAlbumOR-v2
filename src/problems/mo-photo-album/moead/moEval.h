#ifndef _moEval_h
#define _moEval_h

#include "moSolution.h"
#include <fstream>
#include <math.h>

/******************************************************
 * Abstract class for the computation of string evaluation
 */
class moEval {
public:
  /** constructor
   * 
   * @param _n problem size, length of the string
   */
  moEval(unsigned int _n) : _pbSize(_n) { }

  /** Empty constructor
   * 
   * default problem size is 0
   */
  moEval() : _pbSize(0) { }

  // return the size of the problem
  unsigned int size() {
    return _pbSize;
  }

  // Set the size of the problem
  void setSize(unsigned int _n) {
    _pbSize = _n;
  }

  // evaluation function to be defined
  virtual void operator()(moSolution & _solution) = 0;

protected:
  // string length
  unsigned int _pbSize;
};

#endif

