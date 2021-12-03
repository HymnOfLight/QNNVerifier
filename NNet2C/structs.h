#ifndef STRUCTS_H
#define STRUCTS_H

#include "dsverifier/initialization.h"

fxp_t *neuronsSimbolicRestrictions;
int restrictionNeuronsWidth;
FILE *outputFile;
FILE *nnetFile;
char outPutPath[200];
char nnetFilePath[200];
char images_path[200];
char fileName[22] = "/adversarialChecking.c";
char nnetExt[20] = ".nnet";

typedef struct
{
  int neurons;
  fxp_t *weights;
  fxp_t *bias;
  fxp_t *restrictionsHIGHER;
  fxp_t *restrictionsLOWER;
  fxp_t *concretization;
  fxp_t *outputLayer;
  unsigned short *allowsSimbolic;
} layer;

typedef layer *esbmc_layer;

typedef struct nnet
{
  fxp_t *inputs;
  fxp_t *outputs;
  unsigned short isPatternNet;
  layer layers[0];
  int layersInstanciatedIndex;
  int layersNumber;
} esbmc_nnet;

#endif
