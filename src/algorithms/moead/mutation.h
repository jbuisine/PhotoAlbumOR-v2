#ifndef __mutation__h
#define __mutation__h

#include <random>

/*
 * Virtual Mutation operator
 */
class Mutation {
public:

    Mutation(int _size) : size(_size) {};

    virtual void operator()(Solution & _solution){};

    int getSize() const {
        return size;
    }

protected:

    int size;
};


/*
 * Standard random Mutation operator with fixed mutation rates
 */
class StandardRndMutation : public Mutation {
public:
  /**
   * Constructor
   **/
  StandardRndMutation(int _size) : Mutation(_size) {}

  // mutation operator
  virtual void operator()(Solution & _solution) {

      unsigned fstIndex;
      unsigned sndIndex;

      do{
          fstIndex = rand() % size;
          sndIndex = rand() % size;
      }while (fstIndex == sndIndex);

      int tempValue = _solution[fstIndex];

      _solution[fstIndex] = _solution[sndIndex];
      _solution[sndIndex] = tempValue;

      _solution.from(_solution.ID());
      _solution.best(0);
  }
};

/*
 * Double Standard Mutation operator with fixed mutation rates
 */
class DoubleStandardRndMutation : public Mutation {
public:

    /**
     * Constructor
     **/
    DoubleStandardRndMutation(int _size) : Mutation(_size) {}

    // mutation operator
    virtual void operator()(Solution & _solution) {

        for (int i = 0; i < 2; i++){

            unsigned fstIndex;
            unsigned sndIndex;

            do{
                fstIndex = rand() % size;
                sndIndex = rand() % size;
            }while (fstIndex == sndIndex);

            int tempValue = _solution[fstIndex];

            _solution[fstIndex] = _solution[sndIndex];
            _solution[sndIndex] = tempValue;
        }

        _solution.from(_solution.ID());
        _solution.best(0);
    }
};

/*
 * Triple Standard Mutation operator with fixed mutation rates
 */
class TripleStandardRndMutation : public Mutation {
public:

    /**
     * Constructor
     **/
    TripleStandardRndMutation(int _size) : Mutation(_size) {}

    // mutation operator
    virtual void operator()(Solution & _solution) {

        for (int i = 0; i < 3; i++){

            unsigned fstIndex;
            unsigned sndIndex;

            do{
                fstIndex = rand() % size;
                sndIndex = rand() % size;
            }while (fstIndex == sndIndex);

            int tempValue = _solution[fstIndex];

            _solution[fstIndex] = _solution[sndIndex];
            _solution[sndIndex] = tempValue;
        }

        _solution.from(_solution.ID());
        _solution.best(0);
    }
};

/*
 * Triple Standard Mutation operator with fixed mutation rates
 */
class StandardMutation : public Mutation {
public:

    /**
     * Constructor
     **/
    StandardMutation(int _size) : Mutation(_size) {}

    // mutation operator
    virtual void operator()(Solution & _solution) {

        unsigned fstIndex = rand() % size;

        unsigned sndIndex;

        if(fstIndex + 1 >= _solution.size()){
            sndIndex = fstIndex - 1;
        }else{
            sndIndex = fstIndex + 1;
        }

        int tempValue = _solution[fstIndex];

        _solution[fstIndex] = _solution[sndIndex];
        _solution[sndIndex] = tempValue;

        _solution.from(_solution.ID());
        _solution.best(0);
    }
};

#endif
