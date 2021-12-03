#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
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
  DIR *d;
  char buf[PATH_MAX + 1];
   struct dirent *dir;

  //number 7
  float input0[5] = {1000.0, 0.0, -1.5, 100.0, 100.0};

    d = opendir(".");
    if (d)
      {
        while ((dir = readdir(d)) != NULL)
        {
          printf("%s\n", dir->d_name);
          realpath(dir->d_name, buf);
          printf("%s\n", buf);

    if(strstr(dir->d_name, nnetExt) != NULL) {
      char buff[100] = "";
      strcpy(nnetFilePath, buf);
      strcat(buff, basename(buf));
      strncpy(nnetFileName, buff, strlen(buff)-5);
      printf("FILE NAME %s \n", nnetFileName);
      importNNet(&nnet);

  setImg(&nnet, input0);
  int classification = 0;
  classification = neuralNetPrediction(&nnet);
  printf("CLASSIFICATION : %d.\n", classification);
  for(int i = 0; i < arraySize(range); i++){
    exportANNC(&nnet, classification, range[i]);
  }
  free(nnet);
  //printNeuralNetworkDescriptors(nnet);
  }
  }
  closedir(d);
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
  printf("The program took %f seconds to execute\n", time_taken);
}
