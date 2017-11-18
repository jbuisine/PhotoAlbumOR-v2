//
// Created by jbuisine on 06/11/17.
//

// moeo general include
#include <moeo>

// for the creation of an evaluator
#include <problems/mo-photo-album/make_eval_PhotoAlbum.h>

// for the creation of an initializer
#include <problems/mo-photo-album/make_genotype_PhotoAlbum.h>

// for the creation of the variation operators
#include <problems/mo-photo-album/make_op_PhotoAlbum.h>


// how to initialize the population
#include <do/make_pop.h>

// checks for help demand, and writes the status file and make_help; in libutils
void make_help(eoParser & _parser);

// definition of the representation
#include <problems/mo-photo-album/PhotoAlbum.hpp>
#include <es.h>

using namespace std;

int main(int argc, char* argv[])
{
    try
    {
        eoParser parser(argc, argv);  // for user-parameter reading
        eoState state;                // to keep all things allocated

        // parameters
        unsigned int POP_SIZE = parser.createParam((unsigned int)(100), "popSize", "Population size",'P',"Param").value();
        unsigned int MAX_GEN = parser.createParam((unsigned int)(100), "maxGen", "Maximum number of generations",'G',"Param").value();
        double M_EPSILON = parser.createParam(0.01, "mutEpsilon", "epsilon for mutation",'e',"Param").value();
        double P_CROSS = parser.createParam(0.25, "pCross", "Crossover probability",'C',"Param").value();
        double P_MUT = parser.createParam(0.35, "pMut", "Mutation probability",'M',"Param").value();

        /*** the representation-dependent things ***/

        // getting context files
        std::string dataFileName = parser.getORcreateParam(std::string("/home/jbuisine/Documents/Research/MetaheuristicsFramework/MH-ParadisEO/application/resources/photo-album/templates/FirstTemplate/info-photo.json"),
                                                           "Data photos",
                                                           "Data photos file name",
                                                           'P',
                                                           "Representation",
                                                           false).value();

        std::string dispositionFileName = parser.getORcreateParam(std::string("/home/jbuisine/Documents/Research/MetaheuristicsFramework/MH-ParadisEO/application/resources/photo-album/templates/FirstTemplate/album-6-2per3.json"),
                                                                  "DispositionAlbum",
                                                                  "Disposition album file name",
                                                                  'D',
                                                                  "Representation",
                                                                  false).value();

        // The fitness evaluation
        eoEvalFuncCounter<PhotoAlbum>& eval = do_make_eval(parser, state, dataFileName, dispositionFileName);

        /*** the representation-independent things ***/

        eoInitPermutation < PhotoAlbum > init(54);
        eoPop < PhotoAlbum > pop (POP_SIZE, init);

        // definition of the archive
        moeoUnboundedArchive<PhotoAlbum> arch;
        arch(pop);

        // Crossover and mutation
        eoQuadCloneOp < PhotoAlbum > xover;
        eoUniformMutation < PhotoAlbum > mutation (M_EPSILON);

        // algorithm
        moeoNSGAII <PhotoAlbum> nsgaII(MAX_GEN, eval, xover, P_CROSS, mutation, P_MUT);

        /*** Go ! ***/

        // help ?
        make_help(parser);

        // first evalution (for printing)
        apply<PhotoAlbum>(eval, pop);

        // printing of the initial population
        cout << "Initial Population\n";
        pop.sortedPrintOn(cout);
        cout << endl;

        // run the algo
        nsgaII(pop);

        // printing of the final population
        cout << "Final Population\n";
        pop.sortedPrintOn(cout);
        cout << endl;

        // printing of the final archive
        cout << "Final Archive\n";
        arch.sortedPrintOn(cout);
        cout << endl;


    }
    catch (exception& e)
    {
        cout << e.what() << endl;
    }
    return EXIT_SUCCESS;
}
