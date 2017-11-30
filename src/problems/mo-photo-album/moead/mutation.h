#ifndef __mutation__h
#define __mutation__h

#include <random>

/*
 * Virtual Mutation operator
 */
class Mutation {
public:
  virtual void operator()(Solution & _solution) = 0;
};


/*
 * Standard Mutation operator with fixed mutation rates
 */
class StandardMutation : public Mutation {
public:
  /**
   * Contructor
   **/
  StandardMutation(const char * fileName, std::mt19937 & _gen) : generator(_gen) {
    rand_real.param(std::uniform_real_distribution<double>::param_type(0.0, 1.0));

    int i ;
    FILE *file;
    char name[255];

    file = fopen(fileName, "r");

    if (file == NULL) {
      printf("Error while opening the file %s.\n", fileName);
      exit(EXIT_FAILURE);
    }
    
    fscanf(file, "%d", &size);

    lowerBound.resize(size);
    upperBound.resize(size);
    stepSize.resize(size);
    mutationRate.resize(size);

    for(i = 0; i < size; i++) {
      fscanf(file, "%s", name);
      fscanf(file, "%d", &lowerBound[i]);
      fscanf(file, "%d", &upperBound[i]);
      fscanf(file, "%d", &stepSize[i]);
      fscanf(file, "%lf", &mutationRate[i]);
    }
    
    fclose(file);
  }

  // mutation operator
  void operator()(Solution & _solution) {
    int m, M, nMut ;
    int value = 0;

    nMut = 0;
    while (nMut < 1) { // to be sure that there is at least one mutation
      for(unsigned i = 0; i < mutationRate.size(); i++) {
	if (rand_real(generator) < mutationRate[i]) {
	  nMut++;
	  m = _solution[i] - stepSize[i] < lowerBound[i]? lowerBound[i] : _solution[i] - stepSize[i];
	  M = upperBound[i] < _solution[i] + stepSize[i] ? upperBound[i] + 1 : _solution[i] + stepSize[i] + 1;
	  //std::cout << "i = " << i << ", m = " << m << ", M = " << M << std::endl;
	  
	  rand_int.param(std::uniform_int_distribution<int>::param_type(m, M));
	  value = rand_int(generator);
	  while (value == _solution[i]) // could be better without 0 some where
	    value = rand_int(generator);

	  _solution[i] = value;
	} 
      }
    }

    _solution.from(_solution.ID());
    _solution.best(0);
  }

  int getSize() const {
    return size;
  }

protected:
  std::mt19937 & generator;
  std::uniform_int_distribution<int> rand_int;
  std::uniform_real_distribution<double> rand_real;

  int size;
  vector<double> mutationRate;
  vector<int> lowerBound;
  vector<int> upperBound;
  vector<int> stepSize;

};

#endif
