#ifndef __subProblems__h
#define __subProblems__h

#include "moSolution.h"
#include "scalarFunc.h"
#include <math.h>

#define PI 3.14159265358979323846

class SubProblems {
public:
  virtual double scalarfunc(unsigned id, moSolution& _solution) = 0;

  virtual vector<unsigned>& neighborProblems(unsigned id) = 0;
};


class BiObjectiveSubProblems : public SubProblems {
public:
  BiObjectiveSubProblems(unsigned _mu, BiScalarFunc & _scalarfunc, double _ref1, double _ref2, unsigned _T, unsigned _W) : mu(_mu), scalarfunction(_scalarfunc), T(_T) {
    referencePoint = std::make_pair(_ref1, _ref2);
    setNeighbors();

    // set sliding box size
    slidingWindow = new std::vector<std::pair<int, double>>(_W);
  }

  virtual double scalarfunc(unsigned id, moSolution& _solution) {
    scalarfunction.referencePoint(referencePoint.first, referencePoint.second);
    scalarfunction.weights(weights[id].first, weights[id].second);
    return scalarfunction(_solution);
  }

  virtual vector<unsigned>& neighborProblems(unsigned id) {
    return neighbors[id];
  }

  virtual void setNeighbors() {
    int dmin, dmax;
    if (T % 2 == 1) {
      dmin = - (int)T/2;
      dmax = T/2;
    } else {
      dmin = - (int)T/2 + 1;
      dmax = + T/2;
    }

    neighbors.resize(mu);
    for(int dir = 0; dir < -dmin; dir++) {
      for(unsigned i = 0; i < T; i++)
	neighbors[dir].push_back(i);
    }
    for(unsigned dir = -dmin; dir < mu - dmax; dir++) {
      for(unsigned i = dir + dmin; i <= dir + dmax; i++)
	neighbors[dir].push_back(i);
    }
    for(unsigned dir = mu - dmax; dir < mu; dir++) {
      for(unsigned i = mu - T; i < mu; i++)
	neighbors[dir].push_back(i);
    }
  }

  virtual void setWeights() = 0;

  void print() {
    std::cout << "Reference point : (" << referencePoint.first << ", " << referencePoint.second << ")" << std::endl;
    std::cout << "Direction number : " << mu << std::endl;
//    std::cout << "Weights : " << std::endl;
//    for(unsigned i = 0; i < mu; i++) 
//      std::cout << weights[i].first << " " << weights[i].second << std::endl;
    std::cout << "Neighbors number : " << T << std::endl;
//    for(unsigned i = 0; i < mu; i++) {
//      std::cout << "Direction " << i << " neighbors' :";
//      for(unsigned j = 0; j < neighbors[i].size(); j++)
//	std::cout << " " << neighbors[i][j] ;
//      std::cout << std::endl;
//    }
  }

  // number of sub-problems
  unsigned mu;
  // scalarization function
  BiScalarFunc & scalarfunction;
  // number of neighbors
  unsigned T;
  // id of neighboring sub-problems
  vector< vector<unsigned> > neighbors;
  // reference point for the scalar function
  std::pair<double, double> referencePoint;
  // weights used by the scalar function
  vector< std::pair<double,double> > weights;

  // TODO set sliding window for each sub problem (OP ID, FIR value)
  std::vector<std::pair<int, double>>* slidingWindow;
};

class WeightedSumSubProblems : public BiObjectiveSubProblems {
public:
  WeightedSumSubProblems(unsigned _mu, double _ref1, double _ref2, unsigned _T, unsigned _W) : BiObjectiveSubProblems(_mu, wsf, _ref1, _ref2, _T, _W)
  {
    setWeights();
  }

  virtual void setWeights() {
    double angle;
    weights.resize(mu);
    for(unsigned i = 0; i < mu; i++) {
      angle = PI/2 * i / (mu - 1);
      weights[i] = std::make_pair(std::cos(angle), std::sin(angle));
    }
  }

protected:
  WeightedSum wsf;

};

class TchebychevSubProblems : public BiObjectiveSubProblems {
public:
  TchebychevSubProblems(unsigned _mu, double _ref1, double _ref2, unsigned _T, unsigned _W) : BiObjectiveSubProblems(_mu, tf, _ref1, _ref2, _T, _W)
  {
    setWeights();
  }

  virtual void setWeights() {
    double angle;
    weights.resize(mu);
    for(unsigned i = 0; i < mu; i++) {
      angle = PI/2 * i / (mu - 1);
      if (std::cos(angle) < 0.000000001)
	weights[i] = std::make_pair(1000, 1);
      else if (std::sin(angle) < 0.000000001)
	weights[i] = std::make_pair(1, 1000);
      else
	weights[i] = std::make_pair(1 / std::cos(angle), 1 / std::sin(angle));
    }
  }

protected:
  Tchebychev tf;

};

#endif
