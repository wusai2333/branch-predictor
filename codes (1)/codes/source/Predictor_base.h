#ifndef PREDICTOR_BASE_H
#define PREDICTOR_BASE_H

/*
    Predictor: a base class.
*/

class Predictor {
public:
    virtual void init() = 0;
    virtual bool make(unsigned int pc) = 0;
    virtual void train(unsigned int pc, bool outcome) = 0;
};

#endif
