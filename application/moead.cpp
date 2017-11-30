/**
   Cleaner version of MOEA/D

   Date: 2017/03/23

 **/
//#define TRACE

#include <iostream>
#include <fstream>
//#include "mpicxx.h"
#include "mpi.h"

using namespace std;

#include <problems/mo-photo-album/moead/moSolution.h>
#include <problems/mo-photo-album/moead/utils/moEval.h>
#include <problems/mo-photo-album/moead/mutation.h>
#include <problems/mo-photo-album/moead/utils/subProblems.h>
#include <problems/mo-photo-album/moead/moAlgo.h>
#include "init.h"
#include "repair.h"
#include "worker.h"

/***
 *     Main 
 *
 * argv[1] : parameters file name
 * argv[2] : Sobol sequence file name
 * argv[3] : first sobol number
 * argv[4] : random seed
 * argv[5] : mu 
 * argv[6] : T 
 * argv[7] : time in second of the run
 * argv[8] : output file name
 *
 *
 * /usr/local/mpich2/bin/mpirun -np 4 build/moead ../input/params.txt ../input/sobolseq_d23.dat 1 1 10 3 3 outmpi.dat
 ***/
int main(int argc, char ** argv) {

  int numtasks, len;
  char hostname[MPI::MAX_PROCESSOR_NAME];
  int rank ;

  MPI::Init(argc, argv);
  numtasks = MPI::COMM_WORLD.Get_size();
  rank = MPI::COMM_WORLD.Get_rank();
  MPI::Get_processor_name(hostname, len);

    // Evaluation function
  OneMaxOneMin eval(23);
  //const char* rpath = "/ccc/cont002/home/den/muniglim/Optimisation/rho-MNK/instance.dat";
  //RMNKEval rmnk(rpath);
  //rhoMNK eval(23, rmnk);

  if (rank == 0) {
    // random seed
    int seed = atoi(argv[4]);
    mt19937 mt_rand(seed);
    
    // Sobol
    ReferenceInit refInit;
    SobolInit sobolInit(argv[1], argv[2], atoi(argv[3]));
    
    // Mutation
    StandardMutation mutation(argv[1], mt_rand);
    int size = mutation.getSize();
    OverlapRepair repair;

    unsigned mu = atoi(argv[5]);
    unsigned T = atoi(argv[6]);
    unsigned duration = atoi(argv[7]);

    //WeightedSumSubProblems sp(mu, 0.2, 0.2, T);
    TchebychevSubProblems sp(mu, 0.0, 0.0, T);
    //WeightedSumSubProblems sp(mu, 0.0, 0.0, T);
    sp.print();
    
    if(numtasks == 1) { // one processing unit, sequential algorithm
      cout << "----Starting moead----" << endl;
      MOEAD2 algo(eval, sp, sobolInit, mutation, repair, mu, duration);
      algo.run(argv[8]);
      fstream file("front_sq.dat", ios::out);
      for(unsigned i = 0; i < algo.pop.size(); i++)
        file << algo.pop[i].toString() << endl;
      file.close();
    
      std::cout << "End of MOEAD (n. eval = " << algo.nbEval << ")" << std::endl;
    }else{ // more than one processing unit, parallel algorithm
      cout << "----Master says hello----" << endl;
      MPI::COMM_WORLD.Bcast(&size, 1, MPI::INT, 0);
      int cores = numtasks - 1;
      ms_MOEAD2 master(sp, sobolInit, mutation, repair, mu, cores, size, duration);
      master.run(argv[8]);
      fstream file("front_ms.dat", ios::out);
      for(unsigned i = 0; i < master.pop.size(); i++)
        file << master.pop[i].toString() << endl;
      file.close();
    
      std::cout << "End of MOEAD (n. eval = " << master.nbEval << ")" << std::endl;
    }  
  }else{
    moWorker worker(eval);
    worker.run();
  }

  //  echo 'plot "out.dat" us 3:4 w p, exp(-5*x)' | gnuplot -persist
  std::cout << "Finalization of "<< rank << ", end." << std::endl;
  MPI::Finalize();
  
  return 0;
}
