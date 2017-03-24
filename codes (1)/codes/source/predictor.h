#ifndef PREDICTOR_H
#define PREDICTOR_H

#include "PAgA2.h"
#include "Perceptron.h"
#include "GShare.h"
#include "Alpha.h"

/*
  FIXME: my program *does not* match this requirement.
  Define all your tables and their sizes here.
  All tables must be statically-sized.
  Please don't call malloc () or any of its relatives within your
  program.  The predictor will have a budget, namely (32K + 256) bits
  (not bytes).  That encompasses all storage (all tables, plus GHR, if
  necessary -- but not PC) used for the predictor.  That may not be
  the amount of storage your predictor uses, however -- for example,
  you may implement 2-bit predictors with a table of ints, in which
  case the simulator will use more memory -- that's okay, we're only
  concerned about the memory used by the simulated branch predictor.
*/

/*
    Current predictor.
*/
Predictor *current_predictor;

/*
    PAg with A2: direct mapping using low address_length bits of PC,
                2^address_length entries in PBHT, each one is LHR_length bits
                and the low LHR_length bits of PBHT is used. 2^LHR_length
                entries in GPHT, each one is 2 bits using A2 2-bit
                saturating predictor and the low 2 bits of GPHT is used.
*/
/*
    LHR_length is local history register length. A local history register is an
    entry in PBHT (Per-address Branch History Table).
*/
//PAgA2 PAgA2_predictor(8, 11);
PAgA2 *PAgA2_predictor;

/*
    Perceptron: a threshold of starting training is defined in theta.
                A global history register in GHR_length bits (x0 is always 1).
                Low address_length bits of PC is used to locate perceptron
                table PT. 2^address_length entries in PT, each one is
                w_count * w_length bits, that is, w_count-dimension vector and
                each dimension is an w_length bits signed number.
*/
/*
    Here in GHR we use bit value 0 and 1 to indicate false or true.
      The paper use -1 and 1.
*/
//Perceptron Perceptron_predictor(275, 9, 8, 10, 8);
Perceptron *Perceptron_predictor;

/*
    G-Share: a global history register in GHR_length bits. Low address_length
             bits of PC as branch address. address_length-bit value
             (GHR << (address_length - GHR_length)) XOR PC[address_length - 1:0]
             is used to locate in pattern history table PHT.
             Each saturating predictor in pattern history table is 2-bit.
*/
//GShare GShare_predictor(5, 10);
GShare *GShare_predictor;

/*
    Alpha 21264: a choice register table CRT contains 2 ^ GHR_length 2-bit counter.
                A global prediction table GPT contains 2 ^ GHR_length 2-bit saturating
                predictors. A global history register GHR contains GHR_length-bit
                global history. A local history table LHT uses low
                address_length bits of PC to index, and each entry contains a
                LHR_length-bit local history register. A local prediction table LPT
                contains 2 ^ LHR_length 3-bit saturating predictors.
*/
/*
    If entry in CRT <= 0b1 then choose local. Otherwise choose global.
*/
//Alpha Alpha_predictor(12, 10, 10);
Alpha *Alpha_predictor;

/*
  Initialize the predictor.
*/
void init_predictor();

/*
  Make a prediction for conditional branch instruction at PC 'pc'.
  Returning true indicates a prediction of taken; returning false
  indicates a prediction of not taken.
*/
bool make_prediction(unsigned int pc);

/*
  Train the predictor the last executed branch at PC 'pc' and with
  outcome 'outcome' (true indicates that the branch was taken, false
  indicates that the branch was not taken).
*/
void train_predictor(unsigned int pc, bool outcome);

#endif
