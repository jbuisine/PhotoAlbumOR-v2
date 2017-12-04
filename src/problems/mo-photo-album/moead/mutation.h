#ifndef __mutation__h
#define __mutation__h

#include <random>

/*
 * Virtual Mutation operator
 */
class Mutation {
public:

    Mutation(int _size) : size(_size) {};

    virtual void operator()(Solution & _solution) = 0;

    int getSize() const {
        return size;
    }

protected:

    int size;
};


/*
 * Standard Mutation operator with fixed mutation rates
 */
class StandardMutation : public Mutation {
public:
  /**
   * Contructor
   **/
  StandardMutation(int _size) : Mutation(_size) {}

  // mutation operator
  void operator()(Solution & _solution) {

      int fstIndex = rand() % size;
      int sndIndex = rand() % size;

      int tempValue = _solution[fstIndex];

      _solution[fstIndex] = _solution[sndIndex];
      _solution[sndIndex] = _solution[fstIndex];

      _solution.from(_solution.ID());
      _solution.best(0);
  }
};

/*
 * Double Standard Mutation operator with fixed mutation rates
 */
class DoubleStandardMutation : public Mutation {
public:

    /**
     * Contructor
     **/
    DoubleStandardMutation(int _size) : Mutation(_size) {}

    // mutation operator
    void operator()(Solution & _solution) {

        int fstIndex = rand() % size;
        int sndIndex = rand() % size;

        for (int i = 0; i < 2; i++){
            int tempValue = _solution[fstIndex];

            _solution[fstIndex] = _solution[sndIndex];
            _solution[sndIndex] = _solution[fstIndex];
        }

        _solution.from(_solution.ID());
        _solution.best(0);
    }
};

#endif
