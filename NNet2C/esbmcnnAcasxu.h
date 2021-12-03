#ifndef _ESBMCNN_N
#define _ESBMCNN_N
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cuda_operational_model.h"
#include "structsacasxu.h"
#include "utils.h"
//#include "coveringMethods.h"
//gcc -o main esbmcnn.c
#define arraySize(x)  (int)(sizeof(x) / sizeof((x)[0]))

void setImg(esbmc_nnet** nnet, float* img) {
  int inputs = (*nnet)->layers[0].neurons;
  for(int i=0; i< inputs; i++){
    (*nnet)->inputs[i] = (img[i]-((*nnet)->means[i]))/((*nnet)->ranges[i]);
    (*nnet)->nonNormInputs[i] = img[i];
  }
  printfMatrix((*nnet)->means, 1, 6);
  printfMatrix((*nnet)->ranges, 1, 6);
  printfMatrix((*nnet)->inputs, 1, 5);
  printfMatrix((*nnet)->nonNormInputs, 1, 5);

}

void printNeuralNetworkDescriptors(esbmc_nnet* net) {
  //printf("#include <stdio.h> \n#include <math.h>\n#include <stdlib.h>\n#include <time.h>\nfloat UpLinearRelaxation(float input, float up, float low) {\n  float relaxation = (up/(up-low))*(input-low);\n  return relaxation;\n}\nfloat LowLinearRelaxation(float input, float up, float low) {\n  float relaxation = up/(up-low)*(input);\n  return relaxation;\n}\nint main(){\n    clock_t t;\n    t = clock();\n    float y = nondet_float();\n    __ESBMC_assume(y >= 0 && y <= 1);\n    float x = nondet_float();\n    __ESBMC_assume(x >= 0 && x <= 1);\n    float safeLimit = 3.8;\n");
  int neurons = net->layers[1].neurons;
  printf("LAYER:.\n");
  printf("LAYER: %d.\n",net->layersNumber);
  for(int n = 1; n < net->layersNumber; n++) {
    printf("LAYER: %d. Neurons: %d \n", n, net->layers[n].neurons);
    printf("W[%d] = {", n);
    int rows = net->layers[n].neurons;
    int columns = net->layers[n-1].neurons;
    for(int j = 0; j<rows; j++) {
      for(int i = 0; i< columns; i++) {
        printf("  %.6f,", net->layers[n].weights[i + j*columns]);
      }
      printf("\n");
    }
    printf("Bias[%d] = {", n);
    for(int j = 0; j<rows; j++) {
      printf("  %.6f,", net->layers[n].bias[j]);
    }
    printf("}\n");
  }

      //__ESBMC_assume(-3*x + 2*y > 0);\n    __ESBMC_assume(4*x + 1*y > 0);\n    __ESBMC_assert(x + 3*y - safeLimit < 0, \"Unsafe\");\n}");
}

esbmc_layer createLayer(int neurons, int inputs, float* weights, float* bias){
  float* layerWeights;
  float* layerBias;
  layerWeights = (float*)malloc(sizeof(float)*inputs*neurons);
  layerBias = (float*)malloc(sizeof(float)*inputs*neurons);
  esbmc_layer layer = (esbmc_layer)malloc(sizeof(layer));
  layer->neurons = neurons;
  layer->weights = layerWeights;
  layer->bias = layerBias;
  return layer;
}

esbmc_nnet* initializeNN(int *layersDescription, int layersNumber){
  esbmc_nnet* nnet = (esbmc_nnet*)malloc(sizeof(esbmc_nnet)+ (layersNumber+2)*sizeof(layer));
  nnet->layersNumber = layersNumber+2;
  for(int i = 1; i <= layersNumber; i++) {
    nnet->layers[i].neurons = layersDescription[i-1];
  }
  return nnet;
}

esbmc_nnet* initializeNNFromNNet(int *layersDescription, int layersNumber){
  esbmc_nnet* nnet = (esbmc_nnet*)malloc(sizeof(esbmc_nnet)+ (layersNumber)*sizeof(layer));
  nnet->layersNumber = layersNumber;
  for(int i = 0; i < layersNumber; i++) {
    nnet->layers[i].neurons = layersDescription[i];
  }
  int inputs = nnet->layers[0].neurons;
  int outputs = nnet->layers[layersNumber-1].neurons;
  nnet->inputs = (float*) malloc(sizeof(float)*inputs);
  nnet->means = (float*) malloc(sizeof(float)*(inputs+1));
  nnet->ranges = (float*) malloc(sizeof(float)*(inputs+1));
  nnet->nonNormInputs = (float*) malloc(sizeof(float)*inputs);
  nnet->outputs = (float*) malloc(sizeof(float)*outputs);
  restrictionNeuronsWidth = inputs + 1;
  for(int i = 1; i < nnet->layersNumber; i++) {
    int neurons = nnet->layers[i].neurons;
    int previous = nnet->layers[i-1].neurons;
    nnet->layers[i].weights = (float*) malloc(sizeof(float)*neurons*previous);
    nnet->layers[i].bias = (float*) malloc(sizeof(float)*neurons);
    nnet->layers[i].restrictionsHIGHER = (float*) malloc(sizeof(float)*neurons*(inputs+1));
    nnet->layers[i].restrictionsLOWER = (float*) malloc(sizeof(float)*neurons*(inputs+1));
    nnet->layers[i].concretization = (float*) malloc(sizeof(float)*neurons*2);
    nnet->layers[i].outputLayer = (float*) malloc(sizeof(float)*neurons);
    nnet->layers[i].allowsSimbolic = (unsigned short*) malloc(sizeof(unsigned short)*neurons*2);
  }
  return nnet;
}

void configNet(esbmc_nnet* net, int inputs, int outputs) {
  net->layers[0].neurons = inputs;
  if(net->layersNumber > 2){
    net->layers[net->layersNumber-1].neurons = outputs;
  } else {
    net->layersNumber = 2;
    net->layers[1].neurons = outputs;
  }
  net->isPatternNet = 0;
  net->inputs = (float*) malloc(sizeof(float)*inputs);
  net->nonNormInputs = (float*) malloc(sizeof(float)*inputs);
  net->outputs = (float*) malloc(sizeof(float)*outputs);
  restrictionNeuronsWidth = inputs + 1;
  for(int i = 1; i < net->layersNumber; i++) {
    int neurons = net->layers[i].neurons;
    int previous = net->layers[i-1].neurons;
    net->layers[i].weights = (float*) malloc(sizeof(float)*neurons*previous);
    net->layers[i].bias = (float*) malloc(sizeof(float)*neurons);
    net->layers[i].restrictionsHIGHER = (float*) malloc(sizeof(float)*neurons*(inputs+1));
    net->layers[i].restrictionsLOWER = (float*) malloc(sizeof(float)*neurons*(inputs+1));
    net->layers[i].concretization = (float*) malloc(sizeof(float)*neurons*2);
    net->layers[i].outputLayer = (float*) malloc(sizeof(float)*neurons);
    net->layers[i].allowsSimbolic = (unsigned short*) malloc(sizeof(unsigned short)*neurons*2);
  }
}

void addLayerDescription(esbmc_nnet* net, int index, float* weights, float* bias){
  if(index == 0){
    printf("Index must be a positive integer. \n");
    exit(0);
  }
  else if (index >= net->layersNumber) {
    printf("Index can't be outside the neural net. Max index: %d. \n", net->layersNumber-1);
    exit(0);
  }
  else {
    int inputs = net->layers[index-1].neurons;
    int neurons = net->layers[index].neurons;
    memcpy(net->layers[index].weights, weights, sizeof(float) * inputs*neurons);
    memcpy(net->layers[index].bias, bias, sizeof(float)*neurons);
  }
}

void addLayer(esbmc_nnet* net, esbmc_layer layer){
  if(net->layersNumber == 0){
//    net->layers->layer = layer;
    net->layersNumber++;
  }
  else {
//    net->layers->layer->neurons = layer -> neurons;
    net->layersNumber++;
  }
}

void computeLayerSimbolicPropagation(float* restrictions, int restrictionsSize, float bias, int neuronsPrevious, float* simbolicOutput){
  for(int i = 0; i < restrictionsSize; i++) {
    for(int j = 0; j < neuronsPrevious; j++){
      if(j==0){
        simbolicOutput[i] = restrictions[i+(j*(restrictionsSize))];
        //printf("SAIDA %d : %.6f \n",i+(j*(restrictionsSize)), restrictions[i+(j*(restrictionsSize))]);
      } else{
        simbolicOutput[i] += restrictions[i+(j*(restrictionsSize))];
        //printf("SAIDA %d : %.6f \n",i+(j*(restrictionsSize)), restrictions[i+(j*(restrictionsSize))]);
        if(j == neuronsPrevious - 1) {
          //printf("SAIDA: %.6f \n",simbolicOutput[i]);
        }
      }
    }
  }
  simbolicOutput[restrictionsSize-1] += bias;
}

void positiveWeights(float* coefficients, float* positiveWeights){
  for(int i = 0; i < restrictionNeuronsWidth-1; i++) {
    if(coefficients[i] > 0)
      positiveWeights[i] = coefficients[i];
    else
      positiveWeights[i] = 0;
  }
}

void negativeWeights(float* coefficients, float* negativeWeights){
  for(int i = 0; i < restrictionNeuronsWidth-1; i++) {
    if(coefficients[i] < 0)
      negativeWeights[i] = coefficients[i];
    else
      negativeWeights[i] = 0;
  }
}

void maximumVector(float* intervals, float* maximumVector) {
  for(int i = 0; i < restrictionNeuronsWidth-1; i++) {
    maximumVector[i] = intervals[(i*2)+1];
  }
}

void minimumVector(float* intervals, float* minimumVector) {
  for(int i = 0; i < restrictionNeuronsWidth-1; i++) {
    minimumVector[i] = intervals[i*2];
  }
}

void concretize(float* interval, esbmc_nnet* net, int layer, int neuron) {
  float resultUp = 0;
  float resultLow = 0;
  float* positives;
  positives = (float*) malloc(sizeof(float)*(restrictionNeuronsWidth-1));
  float* negatives;
  negatives = (float*) malloc(sizeof(float)*(restrictionNeuronsWidth-1));
  float* maximum;
  maximum = (float*) malloc(sizeof(float)*(restrictionNeuronsWidth-1));
  float* minimum;
  minimum = (float*) malloc(sizeof(float)*(restrictionNeuronsWidth-1));

  positiveWeights(&(net->layers[layer].restrictionsHIGHER[neuron*restrictionNeuronsWidth]), positives);
  negativeWeights(&(net->layers[layer].restrictionsHIGHER[neuron*restrictionNeuronsWidth]), negatives);
  maximumVector(interval, maximum);
  minimumVector(interval, minimum);
  for(int i =0; i < restrictionNeuronsWidth-1; i++) {
    resultUp += positives[i]*maximum[i] + negatives[i]*minimum[i];
    resultLow += positives[i]*minimum[i] + negatives[i]*maximum[i];
  }
  resultUp += net->layers[layer].restrictionsHIGHER[neuron*restrictionNeuronsWidth + (restrictionNeuronsWidth-1)];
  resultLow += net->layers[layer].restrictionsLOWER[neuron*restrictionNeuronsWidth + (restrictionNeuronsWidth-1)];
  if(resultUp<=0){
    net->layers[layer].allowsSimbolic[neuron*2]= 0;
    net->layers[layer].allowsSimbolic[(neuron*2)+1]= 0;
    net->layers[layer].concretization[neuron*2] = 0;
    net->layers[layer].concretization[(neuron*2)+1]= 0;
  }
  else if(resultUp > 0 && resultLow < 0) {
    net->layers[layer].allowsSimbolic[neuron*2]= 0;
    net->layers[layer].allowsSimbolic[(neuron*2)+1]= 1;
    net->layers[layer].concretization[neuron*2] = 0;
    net->layers[layer].concretization[(neuron*2)+1]= resultUp;
  }
  else {
    net->layers[layer].allowsSimbolic[neuron*2]= 1;
    net->layers[layer].allowsSimbolic[(neuron*2)+1]= 1;
    net->layers[layer].concretization[neuron*2] = resultLow;
    net->layers[layer].concretization[(neuron*2)+1]= resultUp;
  }
  free(positives);
  free(negatives);
  free(maximum);
  free(minimum);
}

void generateSimbolicBoundedEquationsFirstLayer(float* interval, esbmc_nnet* net){
  int inputs = net->layers[0].neurons;
  int currentNeurons = net->layers[1].neurons;
  for(int j = 0; j < currentNeurons; j++){
    memcpy(&(net->layers[1].restrictionsLOWER[j*restrictionNeuronsWidth]), &(net->layers[1].weights[j*inputs]), sizeof(float) * inputs);
    memcpy(&(net->layers[1].restrictionsHIGHER[j*restrictionNeuronsWidth]), &(net->layers[1].weights[j*inputs]), sizeof(float) * inputs);
    memcpy(&(net->layers[1].restrictionsLOWER[(j*restrictionNeuronsWidth)+inputs]), &(net->layers[1].bias[j]), sizeof(float));
    memcpy(&(net->layers[1].restrictionsHIGHER[(j*restrictionNeuronsWidth)+inputs]), &(net->layers[1].bias[j]), sizeof(float));
    concretize(interval, net, 1, j);
  }
}

void generateSimbolicBoundedEquations(float* interval, int layer, esbmc_nnet* net){
  int currentNeurons = net->layers[layer].neurons;
  int previousNeurons = net->layers[layer-1].neurons;
  int inputs = net->layers[0].neurons;
  float* restrictionsAuxUp;
  float* restrictionsAuxLow;
  restrictionsAuxUp = (float*) malloc(sizeof(float)*previousNeurons*(restrictionNeuronsWidth));
  restrictionsAuxLow = (float*) malloc(sizeof(float)*previousNeurons*(restrictionNeuronsWidth));
  for(int neuron = 0; neuron < currentNeurons; neuron++){
    for(int previousNeuron = 0; previousNeuron < previousNeurons; previousNeuron++) {
      if(net->layers[layer-1].allowsSimbolic[previousNeuron*2] == 1) {
        for(int i = 0; i < restrictionNeuronsWidth; i++){
          restrictionsAuxUp[previousNeuron*restrictionNeuronsWidth + i] = net->layers[layer].weights[neuron*previousNeurons]*net->layers[layer-1].restrictionsHIGHER[previousNeuron*restrictionNeuronsWidth + i];
          restrictionsAuxLow[previousNeuron*restrictionNeuronsWidth + i] = net->layers[layer].weights[neuron*previousNeurons]*net->layers[layer-1].restrictionsLOWER[previousNeuron*restrictionNeuronsWidth + i];
        }
      } else{
        for(int i = 0; i < restrictionNeuronsWidth-1; i++){
          restrictionsAuxUp[previousNeuron*restrictionNeuronsWidth + i] = 0;
          restrictionsAuxLow[previousNeuron*restrictionNeuronsWidth + i] = 0;
        }
        if(net->layers[layer].weights[neuron*previousNeurons] >= 0){
          restrictionsAuxLow[previousNeuron*restrictionNeuronsWidth + inputs] = net->layers[layer].weights[neuron*previousNeurons]*net->layers[layer-1].concretization[previousNeuron*2];
          restrictionsAuxUp[previousNeuron*restrictionNeuronsWidth + inputs] = net->layers[layer].weights[neuron*previousNeurons]*net->layers[layer-1].concretization[(previousNeuron*2)+1];
        } else{
          restrictionsAuxLow[previousNeuron*restrictionNeuronsWidth + inputs] = net->layers[layer].weights[neuron*previousNeurons]*net->layers[layer-1].concretization[(previousNeuron*2)+1];
          restrictionsAuxUp[previousNeuron*restrictionNeuronsWidth + inputs] = net->layers[layer].weights[neuron*previousNeurons]*net->layers[layer-1].concretization[previousNeuron*2];
        }
      }
    }
    computeLayerSimbolicPropagation(restrictionsAuxLow, inputs+1 ,net->layers[layer].bias[neuron], previousNeurons, &(net->layers[layer].restrictionsLOWER[neuron*restrictionNeuronsWidth]));
    computeLayerSimbolicPropagation(restrictionsAuxUp, inputs+1 ,net->layers[layer].bias[neuron], previousNeurons,  &(net->layers[layer].restrictionsHIGHER[neuron*restrictionNeuronsWidth]));
    concretize(interval, net, layer, neuron);
  }
  free(restrictionsAuxLow);
  free(restrictionsAuxUp);
}

void printConcretizations(esbmc_nnet* net){
  int layers = net->layersNumber;
  printf("LAYER NUMBER %d \n", layers);
  for(int i = 1; i<layers; i++){
    printf("LAYER %d \n", i);
    int currentLayer = net->layers[i].neurons;
    for(int j = 0; j < currentLayer; j++) {
      printf("Allows simbolic: %d   %d Concretization: %.6f   %.6f \n", net->layers[i].allowsSimbolic[j*2],
      net->layers[i].allowsSimbolic[(j*2)+1], net->layers[i].concretization[j*2],
      net->layers[i].concretization[(j*2)+1]);
    }
    printfMatrix(&(net->layers[i].restrictionsLOWER[0]), currentLayer, restrictionNeuronsWidth);
    printfMatrix(&(net->layers[i].restrictionsHIGHER[0]), currentLayer, restrictionNeuronsWidth);
  }
}

void getSimbolicNNPropagation(esbmc_nnet* net, float* intervals){
  int inputs = net->layers[0].neurons;
  //printf("INPUTS %d \n",inputs);
  int layers = net->layersNumber;
  //printf("LAYERS %d \n",layers);
  //Luiz - 20200701 - restrictionNeuronsWidth represents the input size plus bias, that is the restriction elements.
  int totalNumberOfNeurons = 0;
  int neuronsSimbolicIndexVec[layers];
  neuronsSimbolicIndexVec[0] = 0;
  for(int i =1; i < layers; i++) {
    totalNumberOfNeurons += net->layers[i].neurons;
    neuronsSimbolicIndexVec[i] = neuronsSimbolicIndexVec[i-1] + net->layers[i].neurons;
  }
  //printf("TOTAL NEURONS %d \n",totalNumberOfNeurons);
  //printIVector(neuronsSimbolicIndexVec, layers);

  neuronsSimbolicRestrictions = (float*) malloc(sizeof(float)*restrictionNeuronsWidth*totalNumberOfNeurons);
  float* restrictionsAux;
  int currentLayerNeurons;
  int previousLayerNeurons;
  int layersNeuronIndex;
  int previousLayersNeuronIndex;
  for(int l = 1; l < layers; l++){
    //printf("RESTRICTIONS OF LAYER1: \n");
    currentLayerNeurons = net->layers[l].neurons;
    if(l==1){
      for(int j = 0; j < currentLayerNeurons; j++){
        //for(int i = 0; i < inputs; i++){
          //neuronsSimbolicRestrictions[i + j*(restrictionNeuronsWidth)] = net->layers[l].weights[i + j*inputs];
          memcpy(&(neuronsSimbolicRestrictions[j*restrictionNeuronsWidth]), &(net->layers[l].weights[j*inputs]), sizeof(float)*inputs);
          memcpy(&(neuronsSimbolicRestrictions[(j*restrictionNeuronsWidth)+inputs]), &(net->layers[l].bias[j]), sizeof(float));
          //printf("RESTRICTION %d : %.6f \n",i + j*(inputs+1), neuronsSimbolicRestrictions[i + j*(inputs+1)]);
        //}
        //neuronsSimbolicRestrictions[j*(restrictionNeuronsWidth) + inputs] = net->layers[l].bias[j];
        //printf("RESTRICTION %d : %.6f \n",j*(inputs+1) + inputs, neuronsSimbolicRestrictions[j*(inputs+1) + inputs]);
      }
      generateSimbolicBoundedEquationsFirstLayer(intervals, net);
    } else{
      //printf("RESTRICTIONS OF LAYER%d: \n", l);
      currentLayerNeurons = net->layers[l].neurons;
      previousLayerNeurons = net->layers[l-1].neurons;
      layersNeuronIndex = neuronsSimbolicIndexVec[l-1]*(restrictionNeuronsWidth);
      previousLayersNeuronIndex = neuronsSimbolicIndexVec[l-2]*(restrictionNeuronsWidth);
      //printf("previousLayersNeuronIndex %d: \n", previousLayersNeuronIndex);
      restrictionsAux = (float*) malloc(sizeof(float)*previousLayerNeurons*(restrictionNeuronsWidth));
      for(int j = 0; j < currentLayerNeurons; j++){
        for(int k = 0; k < previousLayerNeurons; k++){
          for(int i = 0; i <= inputs; i++){
            restrictionsAux[i + k*(restrictionNeuronsWidth)] = net->layers[l].weights[k + j*previousLayerNeurons]*neuronsSimbolicRestrictions[previousLayersNeuronIndex + (i + k*(restrictionNeuronsWidth))];
            //printf("RESTRICTION %d : %.6f \n",i + j*(inputs+1), restrictionsAux[i + j*(inputs+1)]);
          }
        }
        //printfMatrix(restrictionsAux, 2, 3);
        computeLayerSimbolicPropagation(restrictionsAux, inputs+1 ,net->layers[l].bias[j], previousLayerNeurons, &neuronsSimbolicRestrictions[layersNeuronIndex + j*(inputs+1)]);
      }
      generateSimbolicBoundedEquations(intervals, l, net);
    }
  }
  //printfMatrix(neuronsSimbolicRestrictions, 3, 3);
}

void printIntervals(float* inputsInterval){
  int inputsSize = restrictionNeuronsWidth -1;
  for(int i=0; i<inputsSize; i++){
    printf("  float x%d = nondet_float();\n", i);
    printf("  __ESBMC_assume(x%d >= %.6f && x%d <= %.6f);\n", i, inputsInterval[i*2],i, inputsInterval[(i*2)+1]);
  }
}

void exportIntervals(float* inputsInterval){
  int inputsSize = restrictionNeuronsWidth -1;
  for(int i=0; i<inputsSize; i++){
    fprintf(outputFile,"  float x%d = nondet_float();\n", i);
    fprintf(outputFile,"  __ESBMC_assume(x%d >= %.6f && x%d <= %.6f);\n", i, inputsInterval[i*2],i, inputsInterval[(i*2)+1]);
  }
}

void printRestrictions(esbmc_nnet* net){
  int inputs = net->layers[0].neurons;
  int layers = net->layersNumber;
  int neuronsSimbolicIndexVec[layers];
  neuronsSimbolicIndexVec[0]=0;
  if (layers <= 2)
    return;
  for(int i =1; i < layers; i++) {
    neuronsSimbolicIndexVec[i] = neuronsSimbolicIndexVec[i-1] + net->layers[i].neurons;
  }
  int lastNeuronIndex = neuronsSimbolicIndexVec[layers-2];
  for(int i = 0; i < lastNeuronIndex; i++){
    printf("  __ESBMC_assume(");
    for(int j = 0; j < restrictionNeuronsWidth; j++) {
      if(j==0){
          printf("%.6f*x%d ", neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], j);
      }
      else if(j < restrictionNeuronsWidth-1){
        if(neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j] < 0){
          printf("- %.6f*x%d ", (-1)*neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], j);
        }
        else if(neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j] > 0){
          printf("+ %.6f*x%d ", neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], j);
        }
      }
      else{
        if(neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j] < 0){
          printf("- %.6f > 0);\n", (-1)*neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j]);
        }
        else if(neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j] > 0){
          printf("+ %.6f > 0);\n", neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j]);
        }
        else{
          printf("> 0);\n");
        }
      }
    }
  }
}

void exportRestrictions(esbmc_nnet* net){
  int inputs = net->layers[0].neurons;
  int layers = net->layersNumber;
  int currentNeurons;
  if (layers <= 2)
    return;
  for(int l = 1; l < layers-1; l++){
    currentNeurons = net->layers[l].neurons;
    for(int i = 0; i < currentNeurons; i++){
      fprintf(outputFile,"//Low  __ESBMC_assume(");
      for(int j = 0; j < restrictionNeuronsWidth; j++) {
        if(j==0){
            //fprintf(outputFile,"%.6f*x%d ", neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], j);
            fprintf(outputFile,"%.6f*x%d ", net->layers[l].restrictionsLOWER[i*restrictionNeuronsWidth + j], j);
        }
        else if(j < restrictionNeuronsWidth-1){
          if(net->layers[l].restrictionsLOWER[i*restrictionNeuronsWidth + j] < 0){
            //fprintf(outputFile,"- %.6f*x%d ", (-1)*neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], j);
            fprintf(outputFile,"- %.6f*x%d ", (-1)*net->layers[l].restrictionsLOWER[i*restrictionNeuronsWidth + j], j);
          }
          else if(net->layers[l].restrictionsLOWER[i*restrictionNeuronsWidth + j] > 0){
            //fprintf(outputFile,"+ %.6f*x%d ", neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], j);
            fprintf(outputFile,"+ %.6f*x%d ", net->layers[l].restrictionsLOWER[i*restrictionNeuronsWidth + j], j);
          }
        }
        else{
          if(net->layers[l].restrictionsLOWER[i*restrictionNeuronsWidth + j] < 0){
            //fprintf(outputFile,"- %.6f > 0);\n", (-1)*neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j]);
            fprintf(outputFile,"- %.6f > 0);\n", (-1)*net->layers[l].restrictionsLOWER[i*restrictionNeuronsWidth + j]);
          }
          else if(net->layers[l].restrictionsLOWER[i*restrictionNeuronsWidth + j] > 0){
            //fprintf(outputFile,"+ %.6f > 0);\n", neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j]);
            fprintf(outputFile,"+ %.6f > 0);\n", net->layers[l].restrictionsLOWER[i*restrictionNeuronsWidth + j]);
          }
          else{
            fprintf(outputFile,"> 0);\n");
          }
        }
      }
    }
    for(int i = 0; i < currentNeurons; i++){
      fprintf(outputFile,"//High  __ESBMC_assume(");
      for(int j = 0; j < restrictionNeuronsWidth; j++) {
        if(j==0){
            //fprintf(outputFile,"%.6f*x%d ", neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], j);
            fprintf(outputFile,"%.6f*x%d ", net->layers[l].restrictionsHIGHER[i*restrictionNeuronsWidth + j], j);
        }
        else if(j < restrictionNeuronsWidth-1){
          if(net->layers[l].restrictionsHIGHER[i*restrictionNeuronsWidth + j] < 0){
            //fprintf(outputFile,"- %.6f*x%d ", (-1)*neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], j);
            fprintf(outputFile,"- %.6f*x%d ", (-1)*net->layers[l].restrictionsHIGHER[i*restrictionNeuronsWidth + j], j);
          }
          else if(net->layers[l].restrictionsHIGHER[i*restrictionNeuronsWidth + j] > 0){
            //fprintf(outputFile,"+ %.6f*x%d ", neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], j);
            fprintf(outputFile,"+ %.6f*x%d ", net->layers[l].restrictionsHIGHER[i*restrictionNeuronsWidth + j], j);
          }
        }
        else{
          if(net->layers[l].restrictionsHIGHER[i*restrictionNeuronsWidth + j] < 0){
            //fprintf(outputFile,"- %.6f > 0);\n", (-1)*neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j]);
            fprintf(outputFile,"- %.6f > 0);\n", (-1)*net->layers[l].restrictionsHIGHER[i*restrictionNeuronsWidth + j]);
          }
          else if(net->layers[l].restrictionsHIGHER[i*restrictionNeuronsWidth + j] > 0){
            //fprintf(outputFile,"+ %.6f > 0);\n", neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j]);
            fprintf(outputFile,"+ %.6f > 0);\n", net->layers[l].restrictionsHIGHER[i*restrictionNeuronsWidth + j]);
          }
          else{
            fprintf(outputFile,"> 0);\n");
          }
        }
      }
    }
  }
}

void printProperties(esbmc_nnet* net, float safeLimit){
  int layers = net->layersNumber;
  int neuronsSimbolicIndexVec[layers];
  neuronsSimbolicIndexVec[0]=0;
  for(int i =1; i < layers; i++) {
    neuronsSimbolicIndexVec[i] = neuronsSimbolicIndexVec[i-1] + net->layers[i].neurons;
  }
  int startIndex;
  int lastNeuronIndex;
  if(net->layersNumber < 2) {
    startIndex = 0;
    lastNeuronIndex = neuronsSimbolicIndexVec[layers-1];
  } else {
    startIndex = neuronsSimbolicIndexVec[layers-2];
    lastNeuronIndex = neuronsSimbolicIndexVec[layers-1];
  }
  printf("  __ESBMC_assert(");
  for(int i = startIndex; i < lastNeuronIndex; i++) {
    for(int j = 0; j < restrictionNeuronsWidth; j++){
      if(j==0){
          printf("%.6f*x%d ", neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], j);
      }
      else if(j < restrictionNeuronsWidth-1){
        if(neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j] < 0){
          printf("- %.6f*x%d ", (-1)*neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], j);
        }
        else if(neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j] > 0){
          printf("+ %.6f*x%d ", neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], j);
        }
      }
      else{
        if(neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j] < 0){
          printf("- %.6f <= %.6f, \"Safety property violated\");\n", (-1)*neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], safeLimit);
        }
        else if(neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j] > 0){
          printf("+ %.6f <= %.6f, \"Safety property violated\");\n", neuronsSimbolicRestrictions[i*restrictionNeuronsWidth + j], safeLimit);
        }
        else{
          printf("<= %.6f, \"Safety property violated\");\n", safeLimit);
        }
      }
    }
  }
}

void exportProperties(esbmc_nnet* net, float safeLimit){
  int layers = net->layersNumber - 1;
  int currentNeurons = net->layers[layers].neurons;
  for(int i = 0; i < currentNeurons; i++) {
    fprintf(outputFile,"  __ESBMC_assert(");
    for(int j = 0; j < restrictionNeuronsWidth; j++){
      if(j==0){
          fprintf(outputFile,"%.6f*x%d ", net->layers[layers].restrictionsLOWER[i*restrictionNeuronsWidth + j], j);
      }
      else if(j < restrictionNeuronsWidth-1){
        if(net->layers[layers].restrictionsLOWER[i*restrictionNeuronsWidth + j] < 0){
          fprintf(outputFile,"- %.6f*x%d ", (-1)*net->layers[layers].restrictionsLOWER[i*restrictionNeuronsWidth + j], j);
        }
        else if(net->layers[layers].restrictionsLOWER[i*restrictionNeuronsWidth + j] > 0){
          fprintf(outputFile,"+ %.6f*x%d ", net->layers[layers].restrictionsLOWER[i*restrictionNeuronsWidth + j], j);
        }
      }
      else{
        if(net->layers[layers].restrictionsLOWER[i*restrictionNeuronsWidth + j] < 0){
          fprintf(outputFile,"- %.6f <= %.6f, \"Safety property violated\");\n", (-1)*net->layers[layers].restrictionsLOWER[i*restrictionNeuronsWidth + j], safeLimit);
        }
        else if(net->layers[layers].restrictionsLOWER[i*restrictionNeuronsWidth + j] > 0){
          fprintf(outputFile,"+ %.6f <= %.6f, \"Safety property violated\");\n", net->layers[layers].restrictionsLOWER[i*restrictionNeuronsWidth + j], safeLimit);
        }
        else{
          fprintf(outputFile,"<= %.6f, \"Safety property violated\");\n", safeLimit);
        }
      }
    }
  }

  for(int i = 0; i < currentNeurons; i++) {
    fprintf(outputFile,"  __ESBMC_assert(");
    for(int j = 0; j < restrictionNeuronsWidth; j++){
      if(j==0){
          fprintf(outputFile,"%.6f*x%d ", net->layers[layers].restrictionsHIGHER[i*restrictionNeuronsWidth + j], j);
      }
      else if(j < restrictionNeuronsWidth-1){
        if(net->layers[layers].restrictionsHIGHER[i*restrictionNeuronsWidth + j] < 0){
          fprintf(outputFile,"- %.6f*x%d ", (-1)*net->layers[layers].restrictionsHIGHER[i*restrictionNeuronsWidth + j], j);
        }
        else if(net->layers[layers].restrictionsHIGHER[i*restrictionNeuronsWidth + j] > 0){
          fprintf(outputFile,"+ %.6f*x%d ", net->layers[layers].restrictionsHIGHER[i*restrictionNeuronsWidth + j], j);
        }
      }
      else{
        if(net->layers[layers].restrictionsHIGHER[i*restrictionNeuronsWidth + j] < 0){
          fprintf(outputFile,"- %.6f <= %.6f, \"Safety property violated\");\n", (-1)*net->layers[layers].restrictionsHIGHER[i*restrictionNeuronsWidth + j], safeLimit);
        }
        else if(net->layers[layers].restrictionsHIGHER[i*restrictionNeuronsWidth + j] > 0){
          fprintf(outputFile,"+ %.6f <= %.6f, \"Safety property violated\");\n", net->layers[layers].restrictionsHIGHER[i*restrictionNeuronsWidth + j], safeLimit);
        }
        else{
          fprintf(outputFile,"<= %.6f, \"Safety property violated\");\n", safeLimit);
        }
      }
    }
  }
}

void printSimbolicPropagationCode(esbmc_nnet* net, float* inputsInterval){
  printf("#include <stdio.h>\n#include <math.h>\n#include <stdlib.h>\n#include <time.h>\n  float UpLinearRelaxation(float input, float up, float low) {\n    float relaxation = (up/(up-low))*(input-low);\n    return relaxation;\n  }\n\n  float LowLinearRelaxation(float input, float up, float low) {\n    float relaxation = up/(up-low)*(input);\n    return relaxation;\n  }\n\n");
  printf("int main(){\n");
  //printing intervals;
  printIntervals(inputsInterval);
  printf("//RESTRICTIONS \n");
  printRestrictions(net);
  printf("//PROPERTIES \n");
  printProperties(net, 5.5);
  printf("}\n");
}

void exportSimbolicPropagationCode(esbmc_nnet* net, float* inputsInterval){
  fprintf(outputFile,"#include <stdio.h>\n#include <math.h>\n#include <stdlib.h>\n#include <time.h>\n\n");
  //fprintf(outputFile,"float UpLinearRelaxation(float input, float up, float low) {\n    float relaxation = (up/(up-low))*(input-low);\n    return relaxation;\n  }\n\n  float LowLinearRelaxation(float input, float up, float low) {\n    float relaxation = up/(up-low)*(input);\n    return relaxation;\n  }\n\n");
  fprintf(outputFile,"int main(){\n");
  exportIntervals(inputsInterval);
  fprintf(outputFile,"//RESTRICTIONS \n");
  exportRestrictions(net);
  fprintf(outputFile,"//PROPERTIES \n");
  exportProperties(net, 5.5);
  fprintf(outputFile,"}\n");
}

void generateOutputFileForESBMC(esbmc_nnet* net, float* inputsInterval){
  outputFile = fopen(outPutPath, "w");
  exportSimbolicPropagationCode(net, inputsInterval);
  fclose(outputFile);
}

void importNNet(esbmc_nnet** nnet){
  nnetFile = fopen(nnetFilePath, "r");
  char str[800000];
  char line[800000];
  char *nnetElement;
  int lineIndex = 0;
  int layersNumber = 0;
  int layerIndex = 1;
  int input = 0;
  int output = 0;
  int * layersDescriptor;
  float * weightsBuffer;
  float * biasBuffer;
  int weightDim = 0;
  int biasDim = 0;
  int counter = 0;
  unsigned short isLayerBeginning = 0;
  unsigned short isBiasBeginning = 0;
  if(nnetFile == NULL) {
    perror("Error opening file");
    exit(-1);
  }
  while(fgets (str, 800000, nnetFile)!=NULL){
    //Reading 1st line containing header
    while(strstr(str, "//") != NULL) {
      fgets(str, 800000, nnetFile);
    }
    if( lineIndex == 0) {
      //Reading 2nd line containing NumberOfLayers,InputSize,OutputSize,MaximumLayerSize
      lineIndex++;
      strcpy(line, str);
      nnetElement = strtok(line, ",");
      layersNumber = atof(nnetElement);
      nnetElement = strtok(NULL, ",");
      input = atof(nnetElement);
      nnetElement = strtok(NULL, ",");
      output = atof(nnetElement);
      //Reading 3rd line containing LayerDimnesions
      fgets (str, 80000, nnetFile);
      strcpy(line, str);
      layersDescriptor = (int*) malloc(sizeof(int)*(layersNumber+1));
      nnetElement = strtok(line, ",");
      for(int i = 0; i <= layersNumber; i++){
        layersDescriptor[i] = atof(nnetElement);
        nnetElement = strtok(NULL, ",");
      }
      *nnet = initializeNNFromNNet(layersDescriptor, layersNumber+1);
      isLayerBeginning = 1;
      //Reading 4th line containing deprecated flags

      fgets (str, 80000, nnetFile);

      //Reading 5th line containing minimum values
      fgets (str, 80000, nnetFile);

      //Reading 6th line containing maximum values
      fgets (str, 80000, nnetFile);

      //Reading 7th line containing mean values
      fgets (str, 80000, nnetFile);
      strcpy(line, str);
      nnetElement = strtok(line, ",");
      for(int i = 0; i <= input; i++){
        (*nnet)->means[i] = atof(nnetElement);
        nnetElement = strtok(NULL, ",");
      }

      //Reading 8th line containing range values
      fgets (str, 80000, nnetFile);
      strcpy(line, str);
      nnetElement = strtok(line, ",");
      for(int i = 0; i <= input; i++){
        (*nnet)->ranges[i] = atof(nnetElement);
        nnetElement = strtok(NULL, ",");
      }

    } else {
      strcpy(line, str);
      if(isLayerBeginning){
        isLayerBeginning = 0;
        weightDim = (*nnet)->layers[layerIndex-1].neurons*((*nnet)->layers[layerIndex].neurons);
        biasDim = (*nnet)->layers[layerIndex].neurons;
        weightsBuffer = (float*) malloc(sizeof(float)*weightDim);
        biasBuffer = (float*) malloc(sizeof(float)*biasDim);
        nnetElement = strtok(line, ",");
        while(nnetElement!=NULL){
          if(strcmp(nnetElement, "") != 0 && strlen(nnetElement) > 0 && strcmp(nnetElement, "\n") != 0){
            weightsBuffer[counter] = atof(nnetElement);
            counter++;
          }
          nnetElement = strtok(NULL, ",");
        }
        if(counter >= weightDim) {
          isBiasBeginning = 1;
          counter = 0;
        }
      } else{
        if(isBiasBeginning){
          nnetElement = strtok(line, ",");
          while(nnetElement!=NULL){
            if(strcmp(nnetElement, "") != 0 && strlen(nnetElement) > 0 && strcmp(nnetElement, "\n") != 0){
              biasBuffer[counter] = atof(nnetElement);
              counter++;
            }
            nnetElement = strtok(NULL, ",");
          }
          if(counter >= biasDim){
            counter = 0;
            isBiasBeginning = 0;
            isLayerBeginning = 1;
            addLayerDescription(*nnet, layerIndex, weightsBuffer, biasBuffer);
            free(weightsBuffer);
            free(biasBuffer);
            layerIndex++;
          }
        } else{
          nnetElement = strtok(line, ",");
          while(nnetElement!=NULL){
            if(strcmp(nnetElement, "") != 0 && strlen(nnetElement) > 0 && strcmp(nnetElement, "\n") != 0){
              weightsBuffer[counter] = atof(nnetElement);
              counter++;
            }
            nnetElement = strtok(NULL, ",");
          }
          if(counter >= weightDim) {
            isBiasBeginning = 1;
            counter = 0;
          }
        }
      }
    }
  }
  fclose(nnetFile);
  //printNeuralNetworkDescriptors(nnet);
}

int neuralNetPrediction(esbmc_nnet** nnet){
  int classification = 0;
  int layers = (*nnet)->layersNumber;
  int inputs = (*nnet)->layers[0].neurons;
  int outputs = (*nnet)->layers[(*nnet)->layersNumber - 1].neurons;
  int neurons;
  int previous;
  cublasHandle_t cublasHandle;
  cublasCreate(&cublasHandle);

  float alpha;
  float beta;
  alpha = 1;
  beta = 0;
  float *onevec;
  onevec = (float*) malloc(sizeof(float)*inputs);
  setVectorValue(onevec, 1, inputs);
  float c1;
  float c2;
  //printfVector(nnet->inputs, inputs);
  //normalizef((*nnet)->inputs, inputs);
  //printfVector((*nnet)->inputs, inputs);
  for(int i=1; i < layers; i++) {
    neurons = (*nnet)->layers[i].neurons;
    previous = (*nnet)->layers[i-1].neurons;
    if(i==1) {
      cublasSgemm(cublasHandle, CUBLAS_OP_N, CUBLAS_OP_N, neurons, 1, inputs, &alpha,
                  (*nnet)->layers[1].weights, inputs, (*nnet)->inputs, 1, &beta, (*nnet)->layers[1].outputLayer, 1);
      cublasSgemm(cublasHandle, CUBLAS_OP_N, CUBLAS_OP_N, neurons, 1, 1, &alpha, (*nnet)->layers[1].bias,
                  1, onevec, 1, &alpha, (*nnet)->layers[1].outputLayer, 1);
      RELU((*nnet)->layers[1].outputLayer, neurons);
      printfLayerResults((*nnet)->layers[i].outputLayer, neurons, i);
    } else {
      cublasSgemm(cublasHandle, CUBLAS_OP_N, CUBLAS_OP_N, neurons, 1, previous, &alpha,
                  (*nnet)->layers[i].weights, previous, (*nnet)->layers[i-1].outputLayer, 1, &beta, (*nnet)->layers[i].outputLayer, 1);
      cublasSgemm(cublasHandle, CUBLAS_OP_N, CUBLAS_OP_N, neurons, 1, 1, &alpha, (*nnet)->layers[i].bias,
                  1, onevec, 1, &alpha, (*nnet)->layers[i].outputLayer, 1);
      if(i != layers -1){
        RELU((*nnet)->layers[i].outputLayer, neurons);
      } else{
        RELU((*nnet)->layers[i].outputLayer, neurons);
        c1 = (*nnet)->ranges[outputs];
        c2 = (*nnet)->means[outputs];
        for(int w=0; w < outputs; w++){
          (*nnet)->layers[i].outputLayer[w] = ((*nnet)->layers[i].outputLayer[w]*c1) + c2;
        }
      }
      printfLayerResults((*nnet)->layers[i].outputLayer, neurons, i);
     }
  }
  classification = smallest((*nnet)->layers[layers-1].outputLayer, outputs);
  return classification;
  //printfLayerResults(nnet->layers[layers-1].outputLayer, neurons, 3);
  //printf("CLASSIFICATION %d \n", biggest(nnet->layers[nnet->layersNumber - 1].outputLayer, outputs));
}

void exportAssumes(esbmc_nnet **nnet, int range, int size) {
  int min = 0;
  int max = 0;

  fprintf(ann2cFile,"float x0 = nondet_float();\n");
  fprintf(ann2cFile,"__ESBMC_assume(x0 >= 36000 && x0 <= 60760);\n");
  fprintf(ann2cFile,"float x1 = nondet_float();\n");
  fprintf(ann2cFile,"__ESBMC_assume(x1 >= 0.7 && x1 <= 3.141592);\n");
  fprintf(ann2cFile,"float x2 = nondet_float();\n");
  fprintf(ann2cFile,"__ESBMC_assume(x2 >= -3.141592 && x2 <= (-3.141592+0.01));\n");
  fprintf(ann2cFile,"int x3 = nondet_int();\n");
  fprintf(ann2cFile,"__ESBMC_assume(x3 >= 900 && x3 <= 1200);\n");
  fprintf(ann2cFile,"int x4 = nondet_int();\n");
  fprintf(ann2cFile,"__ESBMC_assume(x4 >= 600 && x4 <= 1200);\n\n");


  //0.0,-3.141593,-3.141593,100.0,0.0,
  //60760.0,3.141593,3.141593,1200.0,1200.0,

  fprintf(ann2cFile,"float c1 = 7.518884;\n");
  fprintf(ann2cFile,"float c2 = 373.949921;\n\n");

  fprintf(ann2cFile,"float i[%d];\n", size);
  fprintf(ann2cFile,"i[%d] = (x%d-%.5e)/(%.5e);\n", 0, 0, (*nnet)->means[0], (*nnet)->ranges[0]);
  fprintf(ann2cFile,"i[%d] = (x%d-%.5e)/(%.5e);\n", 1, 1, (*nnet)->means[1], (*nnet)->ranges[1]);
  fprintf(ann2cFile,"i[%d] = (x%d-%.5e)/(%.5e);\n", 2, 2, (*nnet)->means[2], (*nnet)->ranges[2]);
  fprintf(ann2cFile,"i[%d] = (((float)x%d)-%.5e)/(%.5e);\n", 3, 3, (*nnet)->means[3], (*nnet)->ranges[3]);
  fprintf(ann2cFile,"i[%d] = (((float)x%d)-%.5e)/(%.5e);\n", 4, 4, (*nnet)->means[4], (*nnet)->ranges[4]);
  fprintf(ann2cFile,"\n\n");
}

void exportANNC(esbmc_nnet** nnet, int classification, int range){
  int layers = (*nnet)->layersNumber;
  int inputs = (*nnet)->layers[0].neurons;
  int outputs = (*nnet)->layers[(*nnet)->layersNumber - 1].neurons;
  char sufix[100];
  int neurons;
  int previous;
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current working dir: %s\n", cwd);
  } else {
    perror("Unable to get current path. getcwd() error");
    exit(1);
  }
  strcpy(ANN2CPath, cwd);
  strcat(ANN2CPath, "/generated/");
  strcat(ANN2CPath, nnetFileName);
  sprintf(sufix, "annP10.c");
  strcat(ANN2CPath, sufix);
  printf("path: %s\n", ANN2CPath);
  ann2cFile = fopen(ANN2CPath, "w");
  fprintf(ann2cFile,"#include <stdio.h>\n#include <math.h>\n#include <stdlib.h>\n#include <time.h>\n#include \"utils.h\"\n//#include \"__fc_builtin.h\" \n\n");
  //fprintf(outputFile,"float UpLinearRelaxation(float input, float up, float low) {\n    float relaxation = (up/(up-low))*(input-low);\n    return relaxation;\n  }\n\n  float LowLinearRelaxation(float input, float up, float low) {\n    float relaxation = up/(up-low)*(input);\n    return relaxation;\n  }\n\n");
  fprintf(ann2cFile,"int main(){\n");
  exportAssumes(nnet, range, inputs);

  for(int i=1; i < layers; i++) {
    neurons = (*nnet)->layers[i].neurons;
    previous = (*nnet)->layers[i-1].neurons;
    //transposeMatrix((*nnet)->layers[i].weights, neurons, previous);
    fprintf(ann2cFile, "float layer%d[%d];\n", i, neurons);
    if(i==1) {
      for(int j = 0; j < neurons; j++) {
        fprintf(ann2cFile, "layer%d[%d]= ", i, j);
        for(int k =0; k < previous; k++){
          fprintf(ann2cFile, "(%.5e)*i[%d] + ", (*nnet)->layers[i].weights[j*inputs + k], k);
        }
          fprintf(ann2cFile, "(%.5e);\n", (*nnet)->layers[i].bias[j]);
          fprintf(ann2cFile, "if (layer%d[%d] < 0) layer%d[%d] = 0;\n", i, j, i, j);
      }
    } else {
      for(int j = 0; j < neurons; j++) {
        fprintf(ann2cFile, "layer%d[%d]= ", i, j);
        for(int k =0; k < previous; k++){
          fprintf(ann2cFile, "(%.5e)*layer%d[%d] + ", (*nnet)->layers[i].weights[j*previous + k],i-1, k);
        }
          fprintf(ann2cFile, "(%.5e);\n", (*nnet)->layers[i].bias[j]);

      if(i != layers -1){
        fprintf(ann2cFile, "if (layer%d[%d] < 0) layer%d[%d] = 0;\n", i, j, i, j);
      } else {
        if(j!=0) {
          fprintf(ann2cFile, "if (layer%d[%d] < 0) layer%d[%d] = 0;\n", i, j, i, j);
          fprintf(ann2cFile, "layer%d[%d] = (layer%d[%d]*c2)+c1;\n", i, j, i, j);
          fprintf(ann2cFile, "if (layer%d[%d] < layer%d[r]) r = %d;\n", i, j, i, j);
        }
        else{
          fprintf(ann2cFile, "if (layer%d[%d] < 0) layer%d[%d] = 0;\n", i, j, i, j);
          fprintf(ann2cFile, "layer%d[%d] = (layer%d[%d]*c2)+c1;\n", i, j, i, j);
          fprintf(ann2cFile, "int r = 0;\n");
        }
      }
    }
     }
  }
  fprintf(ann2cFile, "__ESBMC_assert(r == 0 , \"Classification is not safe.\");\n");
  //fprintf(ann2cFile, "//@assert(r == %d);\n", classification);
  // for(int n =0; n < outputs; n++){
  //   if(n != classification)
  //     fprintf(ann2cFile, "__ESBMC_assert(layer%d[%d] > layer%d[%d], \"Classification is not a %d anymore. It is %d.\");\n", layers-1, classification, layers-1, n, classification, n);
  // }
  fprintf(ann2cFile, "}\n");
  fclose(ann2cFile);
}

void exportANNCFWL(esbmc_nnet** nnet, int classification, int range){
  int layers = (*nnet)->layersNumber;
  int inputs = (*nnet)->layers[0].neurons;
  int outputs = (*nnet)->layers[(*nnet)->layersNumber - 1].neurons;
  char sufix[100];
  int neurons;
  int previous;
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current working dir: %s\n", cwd);
  } else {
    perror("Unable to get current path. getcwd() error");
    exit(1);
  }
  strcpy(ANN2CPath, cwd);
  strcat(ANN2CPath, nnetFileName);
  sprintf(sufix, "_L%d_R%d.c", classification, range);
  strcat(ANN2CPath, sufix);
  printf("path: %s\n", ANN2CPath);
  ann2cFile = fopen(ANN2CPath, "w");
  fprintf(ann2cFile,"#include <stdio.h>\n#include <math.h>\n#include <stdlib.h>\n#include <time.h>\n\n");
  //fprintf(outputFile,"float UpLinearRelaxation(float input, float up, float low) {\n    float relaxation = (up/(up-low))*(input-low);\n    return relaxation;\n  }\n\n  float LowLinearRelaxation(float input, float up, float low) {\n    float relaxation = up/(up-low)*(input);\n    return relaxation;\n  }\n\n");
  fprintf(ann2cFile,"int main(){\n");
  fprintf(ann2cFile,"float norm = (float)1/(float)255;\n");
  exportAssumes(nnet, range, inputs);

  for(int i=1; i < layers; i++) {
    neurons = (*nnet)->layers[i].neurons;
    previous = (*nnet)->layers[i-1].neurons;
    fprintf(ann2cFile, "float layer%d[%d];\n", i, neurons);
    if(i==1) {
      for(int j = 0; j < neurons; j++) {
        fprintf(ann2cFile, "layer%d[%d]= ", i, j);
        for(int k =0; k < previous; k++){
          fprintf(ann2cFile, "(%.6f)*i[%d] + ", (*nnet)->layers[i].weights[j*inputs + k], k);
        }
          fprintf(ann2cFile, "(%.6f);\n", (*nnet)->layers[i].bias[j]);
          fprintf(ann2cFile, "if (layer%d[%d] < 0) layer%d[%d]=0;\n", i, j, i, j);
      }
    } else {
      for(int j = 0; j < neurons; j++) {
        fprintf(ann2cFile, "layer%d[%d]= ", i, j);
        for(int k =0; k < previous; k++){
          fprintf(ann2cFile, "(%.6f)*layer%d[%d] + ", (*nnet)->layers[i].weights[j*previous + k],i-1, k);
        }
          fprintf(ann2cFile, "(%.6f);\n", (*nnet)->layers[i].bias[j]);

      if(i != layers -1){
        fprintf(ann2cFile, "if (layer%d[%d] < 0) layer%d[%d]=0;\n", i, j, i, j);
      }
    }
     }
  }
  for(int n =0; n < outputs; n++){
    if(n != classification)
      fprintf(ann2cFile, "__ESBMC_assert(layer%d[%d] > layer%d[%d], \"Classification is not a %d anymore. It is %d.\");\n", layers-1, classification, layers-1, n, classification, n);
  }
  fprintf(ann2cFile, "}\n");
  fclose(ann2cFile);
}


#endif
