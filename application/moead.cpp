/**
   Cleaner version of MOEA/D

   Date: 2017/03/23

 **/

#include <iostream>
#include <fstream>
//#include "mpicxx.h"
//#include "mpi.h"

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
    std::string _dataFileName = "/home/jbuisine/Documents/Research/MetaheuristicsFramework/MH-ParadisEO/application/resources/photo-album/templates/FirstTemplate/info-photo.json";

    std::string _dispositionFileName = "/home/jbuisine/Documents/Research/MetaheuristicsFramework/MH-ParadisEO/application/resources/photo-album/templates/FirstTemplate/album-6-2per3.json";

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

    int seed = 10;
    unsigned mu = 100;
    unsigned T = 20;
    unsigned W = 10;
    double C = 2.;
    double D = 0.6;
    unsigned neighborTaken = 3;
    double pFindNeighbor = 0.5;
    unsigned duration = 50;


    // init all context info
    PhotoAlbumParser fparser(_dataFileName, _dispositionFileName);

    int problem_size = fparser.getN();

    // Evaluation function
    PhotoAlbumEval eval(fparser);

    // random seed
    srand(seed);

    // Set Operators
    std::vector<Mutation> mutations;

    StandardMutation mutation1(problem_size);
    DoubleStandardMutation mutation2(problem_size);

    mutations.push_back(mutation1);
    mutations.push_back(mutation2);
    // End set Operators

    InitPhotoAlbum init;

    // init decomposition with Tchebychev mono objective function
    TchebychevSubProblems sp(mu, 0.0, 0.0, T, W);

    sp.print();

    cout << "----Starting FFRMAB----" << endl;
    FFRMAB algo(eval, sp, init, mutations, mu, C, D, neighborTaken, pFindNeighbor, duration);

    char* fileout = "output.txt"; //argv[12]

    algo.runFRRMAB(fileout);
    ofstream file;
    file.open ("front_pa.txt", ios::out);
    for(unsigned i = 0; i < algo.pop.size(); i++){
        file << algo.pop[i].toString() << endl;
        std::cout << algo.pop[i].toString() << std::endl;
    }
    file.close();

    std::cout << "End of FFRMAB (n. eval = " << algo.nbEval << ")" << std::endl;
  
    return 0;
}
