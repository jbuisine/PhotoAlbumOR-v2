#ifndef __moAlgo__h
#define __moAlgo__h

//#include "mpicxx.h"

#include <vector>
#include <fstream>
#include "moSolution.h"
#include "problems/mo-photo-album/moead/utils/moEval.h"
#include "problems/mo-photo-album/moead/utils/subProblems.h"
#include "init.h"
#include "repair.h"
#include "mutation.h"
#include "checkSol.h"

using namespace std;

class MultiObjectiveAlgo {
public:
  vector<moSolution> pop ;
  unsigned nbEval ;

  virtual void run(char * fileout) = 0;
};

/**
   Biobjective MOEA/D
 **/
class MOEAD2 : public MultiObjectiveAlgo {
public:
  MOEAD2(moEval & _eval, SubProblems & _subproblems, Init & _init, Mutation & _mutation, OverlapRepair _repair, unsigned _mu, unsigned _duration)
  : evaluation(_eval), subProblems(_subproblems), initialization(_init), mutation(_mutation), repair(_repair), mu(_mu), duration(_duration) {
  }

  virtual void run(char * fileout) {
    // time duration of the run (in second)
    finishtime = time(NULL) + duration;        

    simpleHashMap sHM;
    
    nbEval = 0;
    
    // output file header
    FILE * file = fopen(fileout, "w");
    fprintf(file, "ID fitness obj1 obj2 ");
    fprintf(file, "par1 par2 par3 par4 par5 par6 par7 par8 par9 par10 par11 par12 par13 par14 par15 par16 par17 par18 par19 par20 par21 par22 par23 ");
    fprintf(file, "dir from best\n" );
    
    // initialization of the population
    pop.resize(mu);
    for(unsigned i = 0; i < mu; i++) {
      initialization(pop[i]);
      repair(pop[i]);
      evaluation(pop[i]);
      pop[i].fitness(subProblems.scalarfunc(i, pop[i]));
      sHM.insertSol(pop[i]);
      nbEval++;
      pop[i].ID(nbEval);
      pop[i].dir(i);     
      pop[i].from(-1);      
      pop[i].best(1);   
      pop[i].save(file);  
    }

    moSolution mutant;

    // stopping criterium is the number of seconds 
    unsigned i = 0;
    while (time(NULL) < finishtime) {
      // mutation of the direction i
      mutant = pop[i];        
      mutant.best(0); 
      while(!sHM.isNewSol(mutant)) {
        mutation(mutant);
        repair(mutant);
      }
      sHM.insertSol(mutant);
      evaluation(mutant);
      nbEval++;
      mutant.ID(nbEval);

      double fit;
      vector<unsigned> neighbors = subProblems.neighborProblems(i);
      for(unsigned n : neighbors) {
	    fit = subProblems.scalarfunc(n, mutant);
	      if (fit <= pop[ n ].fitness()) { // minimization problem
	        pop[ n ] = mutant;
	        pop[ n ].fitness(fit);	      
            mutant.best(1);
	      }
      }

      mutant.save(file);
      
      // next direction
      i++;
      if (i >= mu) i = 0;
    }

  }

protected:
  moEval & evaluation;
  SubProblems & subProblems;
  Init & initialization;
  Mutation & mutation;
  OverlapRepair repair;
  unsigned mu;

  // number of seconds of the run
  time_t finishtime; 
  unsigned duration;

}; // end MOEAD2

#endif
