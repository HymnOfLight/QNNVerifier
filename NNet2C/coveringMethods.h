//#include <cuda.h>
#ifndef COVERING_METHODS_H
#define COVERING_METHODS_H
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define data 25
#define fc1 2
#define fc2 1
#define fc3 0
#define layerNumber 3
int coverageSS[fc1 + fc2 + fc3];
int coverageDS[fc1 + fc2 + fc3];
int coverageSV[fc1 + fc2 + fc3];
int coverageDV[fc1 + fc2 + fc3];

int layerSizes[3] = {fc1,fc2,fc3};

int signalChange(float v1, float v2) {
  if(v1 > 0 && v2 < 0)
    return 1;
  else if(v1 < 0 && v2 > 0)
    return 1;
  else
    return 0;
}

int distanceAbsoluteFunction(float v1, float v2, float d) {
  //precisa de validacao
  if (fabs(v1 - v2) > d)
    return 1;
  else
    return 0;
}

int valueChange(float v1, float v2, float d) {
  if((!signalChange(v1,v2) && distanceAbsoluteFunction(v1,v2,d)))
    return 1;
  else
    return 0;
}

int distanceChange(float* out1, float* out2, float normDistance, int size) {
  int i = 0;
  float normbasedDistance = 0;
  for(i=0;i<size; i++) {
      normbasedDistance = (out1[i] - out2[i])*(out1[i] - out2[i]) + normbasedDistance;
      if(signalChange(out1[i], out2[i])) {
        return 0;
      }
  }
  if(normDistance < sqrtf(normbasedDistance)) {
    return 1;
  }
  else {
    return 0;
  }
}

//Covering method
//Sign-sign Cover, or SSCover
int SSCover(float* layer1x1, float* layer1x2, float n2x1, float n2x2, int size1, int n1) {
  int i = 0;
  if (!signalChange(layer1x1[n1], layer1x2[n1]))
    return 0;
  for(i=0;i<size1;i++) {
    if (i==n1)
        continue;
    else if(signalChange(layer1x1[i], layer1x2[i]))
      return 0;
  }
  if(signalChange(n2x1, n2x2))
    return 1;
  else
    return 0;
}

void fullSSCover(float* layeri1, float* layeri2, float* layerj1, float* layerj2, int l1, int l2, int layerIndex) {
  int i = 0;
  int n1 = -1;
  int n2 = -1;
  int *sc1, *sc2;
  sc1 = malloc(l1*sizeof(int));
  sc2 = malloc(l2*sizeof(int));
  for(i =0; i <l1; i++) {
    sc1[i] = signalChange(layeri1[i], layeri2[i]);
    if((n1!=-1)&&(sc1[i]==1)) {
      return;
    }
    if((sc1[i]==1) && (n1==-1)) {
      n1 = i;
    }
    if(i==(l1-1)&&(n1==-1)) {
      return;
    }
  }
  for(i =0; i <l2; i++) {
    sc2[i] = signalChange(layerj1[i], layerj2[i]);
    if(sc2[i]) {
      if(layerIndex == 1) {
        coverageSS[n1] = 1;
        coverageSS[fc1 + i] = 1;
        n2 = i;
      }
      else {
        coverageSS[fc1 + n1] = 1;
        coverageSS[fc1 + fc2 + i] = 1;
        n2 = i;
      }
    }
    if((n2==-1)&&(i==(l2-1))){
      return;
    }


  }

}


//Covering method
//Distance-Sign Cover, or DSCover
int DSCover(float* layer1x1, float* layer1x2, float n2x1, float n2x2, int size1, int n1, int normDistance) {
  int i = 0;
  if(!distanceChange(layer1x1, layer1x2, normDistance, size1))
    return 0;
  else {
    if(signalChange(n2x1,n2x2))
      return 1;
    else
      return 0;
  }
}

void fullDSCover(float* layeri1, float* layeri2, float* layerj1, float* layerj2, int l1, int l2, int layerIndex) {
    int i = 0;
    int j = 0;
    int find = 0;
    if(!distanceChange(layeri1, layeri2, 1, l1)) {
      return;
    }

    for(i =0; i <l2; i++) {
      if(signalChange(layerj1[i], layerj2[i])){
          find=1;
          for(j=0; j<l1;j++) {
            if(layerIndex == 1) {
              coverageDS[j] = 1;
              coverageDS[fc1+i] = 1;
            } else {
              coverageDS[fc1 + j] = 1;
              coverageDS[fc1 + fc2 + i] = 1;
            }
          }
      }
    }
      if(!find){
        return;
      }
    }

//Covering method
//Sign-Value Cover, or SVCover
int SVCover(float* layer1x1, float* layer1x2, float n2x1, float n2x2, int size1, int n1, int distance) {
  int i = 0;
  if (!signalChange(layer1x1[n1], layer1x2[n1]))
    return 0;
  for(i=0;i<size1;i++) {
    if (i==n1)
        continue;
    else if(signalChange(layer1x1[i], layer1x2[i]))
      return 0;
  }
  if(valueChange(n2x1, n2x2, distance))
    return 1;
  else
    return 0;
}

void fullSVCover(float* layeri1, float* layeri2, float* layerj1, float* layerj2, int l1, int l2, int distance,int layerIndex){
  int i = 0;
  int n1 = -1;
  int n2 = -1;
  int *sc1, *sc2;
  sc1 = malloc(l1*sizeof(int));
  sc2 = malloc(l2*sizeof(int));
  for(i =0; i <l1; i++) {
    sc1[i] = signalChange(layeri1[i], layeri2[i]);
    if((n1!=-1)&&(sc1[i]==1)) {
      return;
    }
    if((sc1[i]==1) && (n1==-1)) {
      n1 = i;
    }
    if(i==(l1-1)&&(n1==-1)) {
      return;
    }
  }
  for(i =0; i <l2; i++) {
    sc2[i] = valueChange(layerj1[i], layerj2[i], distance);
    if(sc2[i]) {
      if(layerIndex == 1) {
        coverageSV[n1] = 1;
        coverageSV[fc1 + i] = 1;
        n2 = i;
      }
      else {
        coverageSV[fc1 + n1] = 1;
        coverageSV[fc1 + fc2 + i] = 1;
        n2 = i;
      }
    }
    if((n2==-1)&&(i==(l2-1))){
      return;
    }


  }
}


//Covering method
//Distance-Value Cover, or DVCover
int DVCover(float* layer1x1, float* layer1x2, float n2x1, float n2x2, int size1, int distance) {
  if(!distanceChange(layer1x1, layer1x2, distance, size1))
    return 0;
  if(valueChange(n2x1, n2x2, distance))
    return 1;
  else
    return 0;
}


void fullDVCover(float* layeri1, float* layeri2, float* layerj1, float* layerj2, int l1, int l2, int distance, int layerIndex) {
  int i = 0;
  int j = 0;
  int find = 0;
  if(!distanceChange(layeri1, layeri2, 0.1, l1)) {
    return;
  }

  for(i =0; i <l2; i++) {
    if(valueChange(layerj1[i], layerj2[i], distance)){
        find=1;
        for(j=0; j<l1;j++) {
          if(layerIndex == 1) {
            coverageDV[j] = 1;
            coverageDV[fc1+i] = 1;
          } else {
            coverageDV[fc1 + j] = 1;
            coverageDV[fc1 + fc2 + i] = 1;
          }
        }
    }
  }
    if(!find){
      return;
    }
}


float sqrtt(const float x)
{
union
{
int i;
float x;
} u;
u.x = x;
u.i = (1<<29) + (u.i>>1) - (1<<22);

u.x =    u.x + x/u.x;
u.x = 0.25f*u.x + x/u.x;

return u.x;
}

int isCloseEnough(float* img, float* adversarial, float b, int size) {
    int i = 0;
    float normDistance = 0;
    for(i=0;i<size;i++) {
      normDistance += (img[i]-adversarial[i])*(img[i]-adversarial[i]);
    }
      normDistance = sqrtt(normDistance);
      if(normDistance <= b)
        return 1;
      else
        return 0;
}

float neuronCoverageSS(){
  int i = 0;
  float sum = 0;
  float neuronsCoveredPercent = 0;
  for(i = 0; i < (fc1 + fc2 + fc3); i++) {
      sum += coverageSS[i];
  }
  neuronsCoveredPercent = sum / (fc1 + fc2 + fc3);
  return neuronsCoveredPercent;
}

float neuronCoverageDS(){
  int i = 0;
  float sum = 0;
  float neuronsCoveredPercent = 0;
  for(i = 0; i < (fc1 + fc2 + fc3); i++) {
      sum += coverageDS[i];
  }
  neuronsCoveredPercent = sum / (fc1 + fc2 + fc3);
  return neuronsCoveredPercent;
}


float neuronCoverageSV(){
  int i = 0;
  float sum = 0;
  float neuronsCoveredPercent = 0;
  for(i = 0; i < (fc1 + fc2 + fc3); i++) {
      sum += coverageSV[i];
  }
  neuronsCoveredPercent = sum / (fc1 + fc2 + fc3);
  return neuronsCoveredPercent;
}


float neuronCoverageDV(){
  int i = 0;
  float sum = 0;
  float neuronsCoveredPercent = 0;
  for(i = 0; i < (fc1 + fc2 + fc3); i++) {
      sum += coverageDV[i];
  }
  neuronsCoveredPercent = sum / (fc1 + fc2 + fc3);
  return neuronsCoveredPercent;
}
#endif
