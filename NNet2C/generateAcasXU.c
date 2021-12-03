#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include "cuda_operational_model.h"
#include "structsacasxu.h"
#include "utils.h"
#include "esbmcnnAcasxu.h"


int main(int argc,char* argv[]){
  clock_t t;
  t = clock();
  int range[1] = {10};
  esbmc_nnet* nnet;
  //number 7
  float input0[5] = {1000.0, 0.0, -1.5, 100.0, 100.0};
  if(argc < 2){
    printf("Error. Atleast the output path and .nnet file must be passed to the program.\n");
    exit(-1);
  } else {
    strcpy(outPutPath, argv[1]);
    strcat(outPutPath, fileName);
    if(strstr(argv[1], nnetExt) != NULL) {
      char buff[100] = "/";
      strcpy(nnetFilePath, argv[1]);
      strcat(buff, basename(argv[1]));
      strncpy(nnetFileName, buff, strlen(buff)-5);
      printf("FILE NAME %s \n", nnetFileName);
      importNNet(&nnet);
    } else {
      strcpy(nnetFileName, ANN2C);
    }
  }

  setImg(&nnet, input0);
  int classification = 0;
  classification = neuralNetPrediction(&nnet);
  printf("CLASSIFICATION : %d.\n", classification);
  for(int i = 0; i < arraySize(range); i++){
    exportANNC(&nnet, classification, range[i]);
  }
  free(nnet);
  //printNeuralNetworkDescriptors(nnet);

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
  printf("The program took %f seconds to execute\n", time_taken);
}
