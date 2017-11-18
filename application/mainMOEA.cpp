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


using namespace std;


int main(int argc, char* argv[])
{
    try
    {

        eoParser parser(argc, argv);  // for user-parameter reading
        eoState state;                // to keep all things allocated


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

        /*** the representation-dependent things ***/

        // The fitness evaluation
        eoEvalFuncCounter<PhotoAlbum>& eval = do_make_eval(parser, state, dataFileName, dispositionFileName);
        // the genotype (through a genotype initializer)
        eoInit<PhotoAlbum>& init = do_make_genotype(parser, state);
        // the variation operators
        eoGenOp<PhotoAlbum>& op = do_make_op(parser, state);


        /*** the representation-independent things ***/

        // initialization of the population
        eoPop<PhotoAlbum>& pop = do_make_pop(parser, state, init);
        // definition of the archive
        moeoUnboundedArchive<PhotoAlbum> arch;
        // stopping criteria
        unsigned int maxGen = parser.createParam((unsigned int)(100), "maxGen", "Maximum number of gen.",'G',"Stopping criterion").value();
        eoGenContinue<PhotoAlbum> term(maxGen);

        // checkpointing
        eoCheckPoint<PhotoAlbum> checkpoint(term);
        moeoArchiveUpdater < PhotoAlbum > updater(arch, pop);
        checkpoint.add(updater);

        // fitness assignment
        moeoDominanceDepthFitnessAssignment <PhotoAlbum> fitnessAssignment;

        // diversity preservation
        moeoFrontByFrontCrowdingDiversityAssignment <PhotoAlbum> diversityAssignment;

        // comparator
        moeoFitnessThenDiversityComparator <PhotoAlbum> comparator;

        // selection scheme
        moeoDetTournamentSelect <PhotoAlbum> select(comparator, 2);

        // replacement scheme
        moeoElitistReplacement <PhotoAlbum> replace(fitnessAssignment, diversityAssignment, comparator);

        // breeder
        eoGeneralBreeder <PhotoAlbum> breed(select, op);

        // algorithm
        moeoEasyEA <PhotoAlbum> algo (checkpoint, eval, breed, replace, fitnessAssignment, diversityAssignment);


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
        algo(pop);

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
