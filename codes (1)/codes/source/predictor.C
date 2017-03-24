#include <stdio.h>
#include <stdlib.h>
#include "Predictor_base.h"

/*
    Current predictor.
*/
extern Predictor *current_predictor;

/*
    Select predictor for initialization.
*/
void init_predictor ()
{
    current_predictor->init();
}

/*
    Select predictor to make prediction.
*/
bool make_prediction (unsigned int pc)
{
    return current_predictor->make(pc);
}

/*
    Select predictor to train.
*/
void train_predictor (unsigned int pc, bool outcome)
{
    current_predictor->train(pc, outcome);
}
