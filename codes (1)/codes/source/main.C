#include <stdio.h>
#include <netinet/in.h>
#include "traceread.h"
#include "predictor.h"
#include <string>

FILE * stream;

void setup_trace (const char * filename)
{
  if (filename == NULL)
    stream = stdin;
  else {
    stream = fopen (filename, "r");
  }
}

void close_trace ()
{
  fclose (stream);
}


int main (int argc, char * argv[])
{
  int mis_preds = 0;
  int num_branches = 0;
  uint32_t pc = 0;
  bool outcome = false;

  if (argc >= 2) {
      setup_trace (argv[1]);
      char *type = NULL;
      unsigned int address_length = 0, LHR_length = 0, theta = 0,
            GHR_length = 0, w_count = 0, w_length = 0;
      for (int i = 2; i < argc; i++) {
          char *parameter = strtok(argv[i], "=");
          if (strcmp(parameter, "type") == 0) {
              type = strtok(NULL, "=");
          } else if (strcmp(parameter, "address_length") == 0) {
              address_length = atoi(strtok(NULL, "="));
          } else if (strcmp(parameter, "LHR_length") == 0) {
              LHR_length = atoi(strtok(NULL, "="));
          } else if (strcmp(parameter, "theta") == 0) {
              theta = atoi(strtok(NULL, "="));
          } else if (strcmp(parameter, "GHR_length") == 0) {
              GHR_length = atoi(strtok(NULL, "="));
          } else if (strcmp(parameter, "w_count") == 0) {
              w_count = atoi(strtok(NULL, "="));
          } else if (strcmp(parameter, "w_length") == 0) {
              w_length = atoi(strtok(NULL, "="));
          }
      }
      if (strcmp(type, "PAgA2") == 0) {
          PAgA2_predictor = new PAgA2(address_length, LHR_length);
          current_predictor = PAgA2_predictor;
      } else if (strcmp(type, "Perceptron") == 0) {
          Perceptron_predictor = new Perceptron(theta, GHR_length, address_length, w_count, w_length);
          current_predictor = Perceptron_predictor;
      } else if (strcmp(type, "GShare") == 0) {
          GShare_predictor = new GShare(GHR_length, address_length);
          current_predictor = GShare_predictor;
      } else if (strcmp(type, "Alpha") == 0) {
          Alpha_predictor = new Alpha(GHR_length, address_length, LHR_length);
          current_predictor = Alpha_predictor;
      }
    //    printf("type: %s\naddress_length: %d\nLHR_length: %d\ntheta: %d\nGHR_length: %d\nw_count: %d\nw_length: %d\n",
    //        type, address_length, LHR_length, theta, GHR_length, w_count, w_length);
  } else
      setup_trace (NULL);

  // Initialize the predictor
  init_predictor ();

  // Read the number of instructions from the trace
  uint32_t stat_num_insts = 0;
  if (fread (&stat_num_insts, sizeof (uint32_t), 1, stream) != 1) {
    printf ("Could not read input file\n");
    return 1;
  }
  stat_num_insts = ntohl (stat_num_insts);

  // Read each branch from the trace
  while (read_branch (&pc, &outcome)) {

    pc = ntohl (pc);

    num_branches ++;

    // Make a prediction and compare with actual outcome
    if (make_prediction (pc) != outcome)
      mis_preds ++;

    // Train the predictor
    train_predictor (pc, outcome);
  }

  // Print out the mispredict statistics
  //printf ("Branches\t\t%8d\n", num_branches);
  //printf ("Incorrect\t\t%8d\n", mis_preds);
  printf("%d,%d,", mis_preds, num_branches);
  float mis_pred_rate = (float)mis_preds / float(stat_num_insts / 1000); /* Is it the definition of prediction rate? */
  //printf ("1000*wrong_cc_predicts/total insts 1000 * %8d / %8d = %7.3f\n", mis_preds, stat_num_insts, mis_pred_rate);
  printf("%f", mis_pred_rate);
  if (argc >= 2) {
    close_trace ();
    //delete current_predictor;
  }
  return 0;
}
