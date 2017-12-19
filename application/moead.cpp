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
#include <problems/mo-photo-album/moead/moPhotoAlbumEval.h>
#include <problems/mo-photo-album/moead/mutation.h>
#include <problems/mo-photo-album/moead/subProblems.h>
#include <problems/mo-photo-album/moead/moAlgo.h>
#include <problems/mo-photo-album/moead/init.h>

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
    //std::string dataFileName = "../../../application/resources/photo-album/templates/FirstTemplate/info-photo.json";

    //std::string dispositionFileName = "../../../application/resources/photo-album/templates/FirstTemplate/album-6-2per3.json";

    // Get all params data
    std::string _dataFileName = argv[1];
    std::string _dispositionFileName = argv[1];
    int seed = atoi(argv[3]);
    unsigned mu = atoi(argv[4]);
    unsigned T = atoi(argv[5]);
    unsigned W = atoi(argv[6]);
    double C = atof(argv[7]);
    double D = atof(argv[8]);
    unsigned neighborTaken = atoi(argv[9]);
    double pFindNeighbor = atof(argv[10]);
    unsigned duration = atoi(argv[11]);


    // init all context info
    PhotoAlbumParser fparser(_dataFileName, _dispositionFileName);

    int problem_size = fparser.getN();

    // Evaluation function
    PhotoAlbumEval eval(fparser);

    // random seed
    srand(seed);

    // Operators
    std::vector<Mutation> mutations;

    StandardMutation mutation1(problem_size);
    DoubleStandardMutation mutation2(problem_size);

    mutations.push_back(mutation1);
    mutations.push_back(mutation2);

    InitPhotoAlbum init;

    //WeightedSumSubProblems sp(mu, 0.2, 0.2, T);
    TchebychevSubProblems sp(mu, 0.0, 0.0, T, W);
    //WeightedSumSubProblems sp(mu, 0.0, 0.0, T);

    sp.print();

    cout << "----Starting FFRMAB----" << endl;
    FFRMAB algo(eval, sp, init, mutations, mu, C, D, neighborTaken, pFindNeighbor, duration);
    algo.runFRRMAB(argv[12]);
    fstream file("front_sq.dat", ios::out);
    for(unsigned i = 0; i < algo.pop.size(); i++)
    file << algo.pop[i].toString() << endl;
    file.close();

    std::cout << "End of FFRMAB (n. eval = " << algo.nbEval << ")" << std::endl;
  
    return 0;
}
