//
// Created by jbuisine on 04/12/17.
//

#ifndef PHOTOALBUM_INIT_H
#define PHOTOALBUM_INIT_H


/*
 * Standard Mutation operator with fixed mutation rates
 */
class InitPhotoAlbum {
public:
    /**
     * Constructor
     **/
    InitPhotoAlbum() {}

    // mutation operator
    void operator()(Solution & _solution) {

        for(int i = 0; i < _solution.size(); i++){
            _solution[i] = i;
        }

        std::random_shuffle(_solution.begin(), _solution.end());
    }
};

#endif //PHOTOALBUM_INIT_H
