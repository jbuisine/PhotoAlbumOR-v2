#ifndef __scalarFunc__h
#define __scalarFunc__h

#include <vector>
#include <stdlib.h>
#include <algorithm>
#include "moSolution.h"


class ScalarFunc {
public:
  virtual double operator()(moSolution& _solution) = 0;

};

class BiScalarFunc : public ScalarFunc {
public:
  BiScalarFunc() : w1(1.0), w2(0.0), ref1(0.0), ref2(0.0), indf1(0), indf2(1) {
  }

  BiScalarFunc(double _w1, double _w2, double _ref1, double _ref2, unsigned _indf1 = 0, unsigned _indf2 = 1) : w1(_w1), w2(_w2), ref1(_ref1), ref2(_ref2), indf1(_indf1), indf2(_indf2) {
    
  }

  virtual void weights(double _w1, double _w2) {
    w1 = _w1;
    w2 = _w2;
  }

  virtual std::pair<double, double> weights() const {
    return std::make_pair(w1, w2);
  }

  virtual void referencePoint(double _r1, double _r2) {
    ref1 = _r1;
    ref2 = _r2;
  }

  virtual std::pair<double, double> referencePoint() const {
    return std::make_pair(ref1, ref2);
  }

protected:
  // weights of the agregation
  double w1, w2;

  // reference point
  double ref1, ref2;

  // indices in objvec to compute the scalar value
  unsigned indf1, indf2;

};

class WeightedSum : public BiScalarFunc {
public:
  double operator()(moSolution& _solution) {
    return w1 * abs(_solution.objvec[indf1] - ref1) + w2 * abs(_solution.objvec[indf2] - ref2);
  }
};

class Tchebychev : public BiScalarFunc {
public:
  double operator()(moSolution& _solution) {
    return std::max(w1 * abs(_solution.objvec[indf1] - ref1), w2 * abs(_solution.objvec[indf2] - ref2));
  }
};

#endif
