//
// Created by jbuisine on 09/11/17.
//

#ifndef PROJECTNAME_EOBITKNAPSACK_H
#define PROJECTNAME_EOBITKNAPSACK_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>

#include "eoVector.h"
#include <ga/eoBit.h>

template <class FitT> class eoBitKnapsack: public eoBit<FitT>
{
public:

    using eoVector< FitT, bool >::begin;
    using eoVector< FitT, bool >::end;
    using eoVector< FitT, bool >::resize;
    using eoVector< FitT, bool >::size;

    /**
     * (Default) Constructor.
     * @param size Size of the binary std::string.
     * @param value Default value.
     */
    eoBitKnapsack(unsigned size = 0, bool value = false):
            eoBit<FitT>(size, value) {}

    /// My class name.
    virtual std::string className() const
    {
        return "eoBitKnapsack";
    }

    /**
     * To print me on a stream.
     * @param os The std::ostream.
     */
    virtual void printOn(std::ostream& os) const
    {
        EO<FitT>::printOn(os);
        os << ' ';
        os << size() << ' ';
        std::copy(begin(), end(), std::ostream_iterator<bool>(os));
    }

    /**
     * To read me from a stream.
     * @param is The std::istream.
     */
    virtual void readFrom(std::istream& is)
    {
        EO<FitT>::readFrom(is);
        unsigned s;
        is >> s;
        std::string bits;
        is >> bits;
        if (is)
        {
            resize(bits.size());
            std::transform(bits.begin(), bits.end(), begin(),
                           std::bind2nd(std::equal_to<char>(), '1'));
        }
    }

    /**
     * to set knapsack weight
     *
     * @param w
     */
    void weight(double w){ this->weightSum = w; }

    /**
     * to get knapsack weight
     *
     * @return weightSum
     */
    double weight(){ return weightSum; }

private:

    // Used for store weight of knapsack
    double weightSum;
};

#endif //PROJECTNAME_EOBITKNAPSACK_H
