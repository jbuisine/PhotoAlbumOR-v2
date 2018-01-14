/**
   Cleaner version of MOEA/D

   Date: 2017/03/23

 **/

#include <iostream>
#include <fstream>
//#include "mpicxx.h"
//#include "mpi.h"

using namespace std;

#include <algorithms/moead/moSolution.h>
#include <problems/qap/moead/moQAPEval.h>
#include <algorithms/moead/mutation.h>
#include <algorithms/moead/subProblems.h>
#include <algorithms/moead/moFRRMAB_NR.h>
#include <algorithms/moead/hyperVolume.h>
#include <algorithms/moead/init.h>

/***
 * Main
 *
 * argv[1] : Data file path
 * argv[2] : Disposition file path
 * argv[3] : random seed
 * argv[4] : mu
 * argv[5] : T
 * argv[6] : W (sliding window size)
 * argv[7] : C (UCB scaling trade-off factor EvE)
 * argv[8] : D (decaying factor in [0,1])
 * argv[9] : time in second of the run
 * argv[10] : output file name
 *
 ***/
int main(int argc, char ** argv) {

    // getting context files
    std::string _dataFileName = "./../../application/resources/qap/instanceUni_Rl_100.txt";

    // Get all params data
    //std::string _dataFileName = argv[1];
    //std::string _dispositionFileName = argv[1];
    //int seed = atoi(argv[3]);
    //unsigned mu = atoi(argv[4]);
    //unsigned T = atoi(argv[5]);
    //unsigned W = atoi(argv[6]);
    //double C = atof(argv[7]);
    //double D = atof(argv[8]);
    //unsigned neighborTaken = atoi(argv[9]);
    //double pFindNeighbor = atof(argv[10]);
    //unsigned duration = atoi(argv[11]);

    // default settings
    int seed = 10;
    unsigned mu = 100;
    unsigned T = 20;
    unsigned W = 15;
    double C = sqrt(2.);
    double D = 0.5;
    double affinity = 0.8;
    unsigned nbEval = 100000;

    // init all context info
    QAPUniParser fparser(_dataFileName);

    int problem_size = fparser.getN();

    // Evaluation function
    QAPEval eval(fparser);

    // random seed
    srand(seed);

    // Set Operators
    std::vector<Mutation*> mutations;

    StandardMutation mutation1(problem_size);
    StandardRndMutation mutation2(problem_size);
    DoubleStandardRndMutation mutation3(problem_size);
    TripleStandardRndMutation mutation4(problem_size);

    //mutations.push_back(&mutation1);
    mutations.push_back(&mutation2);
    mutations.push_back(&mutation3);
    mutations.push_back(&mutation4);

    // End set Operators

    InitQAP init;

    // init decomposition with WeightedSum mono objective function
    WeightedSumSubProblems sp(mu, 0.0, 0.0, T, W);

    HillClimber repair(1000, eval, sp, mutation1);

    sp.print();

    cout << "----Starting FRRMAB_NR----" << endl;
    FRRMAB_NR algo(eval, sp, init, mutations, repair, mu, C, D, affinity, nbEval);

    char* fileout = "./../../application/resources/qap/stats/output.txt"; //argv[12]

    algo.run(fileout);
    ofstream file;
    file.open ("./../../application/resources/qap/stats/front_pa.txt", ios::out);
    for(unsigned i = 0; i < algo.pop.size(); i++){
        file << algo.pop[i].toString() << endl;
        //std::cout << algo.pop[i].toString() << std::endl;
    }
    file.close();

    // Stats hyper volume
    HyperVolume hv;

    std::cout << "End of FRRMAB_NR(n. eval = " << nbEval << ", duration = " << algo.duration << ")" << std::endl;
    std::cout << "HV : " << hv(algo.pop) << std::endl;
  
    return 0;
}
