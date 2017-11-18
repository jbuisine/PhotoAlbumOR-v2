//
// Created by jbuisine on 16/11/17.
//

#ifndef PHOTOALBUM_ALBUMPHOTOOPCROSSOVERQUAD_H
#define PHOTOALBUM_ALBUMPHOTOOPCROSSOVERQUAD_H

#include <eoOp.h>
#include "PhotoAlbum.hpp"

/**
 * Quadratic crossover operator for photo album QAP (modify the both genotypes)
 */
class PhotoAlbumOpCrossoverQuad : public eoQuadOp < PhotoAlbum >
{
public:

    /**
     * the class name (used to display statistics)
     */
    std::string className() const
    {
        return "PhotoAlbumOpCrossoverQuad";
    };


    /**
     * eoQuad crossover - _photoalbum1 and _photoalbum2 are the (future) offspring, i.e. _copies_ of the parents
     * @param _photoalbum1 the first parent
     * @param _photoalbum2 the second parent
     */
    bool operator()(PhotoAlbum & _photoalbum1, PhotoAlbum & _photoalbum2)
    {
        bool oneAtLeastIsModified;
        // computation of the 2 random points
        unsigned int point1, point2;
        do
        {
            point1 =  rng.random(std::min(_photoalbum1.size(), _photoalbum2.size()));
            point2 =  rng.random(std::min(_photoalbum1.size(), _photoalbum2.size()));
        }
        while (fabs((double) point1-point2) <= 2);

        // computation of the offspring
        PhotoAlbum offspring1 = generateOffspring(_photoalbum1, _photoalbum2, point1, point2);
        PhotoAlbum offspring2 = generateOffspring(_photoalbum1, _photoalbum2, point1, point2);

        // does at least one genotype has been modified ?
        if ((_photoalbum1 != offspring1) || (_photoalbum2 != offspring2))
        {
            // update
            _photoalbum1.value(offspring1);
            _photoalbum2.value(offspring2);

            // at least one genotype has been modified
            oneAtLeastIsModified = true;
        }
        else
        {
            // no genotype has been modified
            oneAtLeastIsModified = false;
        }
        // return 'true' if at least one genotype has been modified
        return oneAtLeastIsModified;
    };

private:

    /**
     * generation of an offspring by a 2 points crossover
     * @param _parent1 the first parent
     * @param _parent2 the second parent
     * @param _point1 the first point
     * @param _point2 the second point
     */
    PhotoAlbum generateOffspring(const PhotoAlbum & _parent1, const PhotoAlbum & _parent2, unsigned int _point1, unsigned int _point2)
    {
        PhotoAlbum result = _parent1;
        std::vector<bool> taken_values(result.size(), false);
        if (_point1 > _point2)
            std::swap(_point1, _point2);
        /* first parent */
        for (unsigned int i=0 ; i<=_point1 ; i++)
        {
            // result[i] == _parent1[i]
            taken_values[_parent1[i]] = true;
        }
        for (unsigned int i=_point2 ; i<result.size() ; i++)
        {
            // result[i] == _parent1[i]
            taken_values[_parent1[i]] = true;
        }
        /* second parent */
        unsigned int i = _point1+1;
        unsigned int j = 0;
        while (i<_point2 && j<_parent2.size())
        {
            if (! taken_values[_parent2[j]])
            {
                result[i] = _parent2[j];
                i++;
            }
            j++;
        }
        return result;
    };

};

#endif //PHOTOALBUM_ALBUMPHOTOOPCROSSOVERQUAD_H
