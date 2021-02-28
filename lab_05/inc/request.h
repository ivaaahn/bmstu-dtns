#ifndef _REQUEST_H_
#define _REQUEST_H_

#include <stdlib.h>

typedef struct {
    double handlT;
    double deliverT; 
} Request;


typedef struct {
    double handlTimeMin;
    double handlTimeMax;
    double delayMin;
    double delayMax;
    double probability;
    size_t targetAmount;
} TimeConstants;


extern TimeConstants timeConstants;

void requestGenerate(Request *request, double currTime);
void requestReGenerate(Request *request, double currTime);


int setHandlingInterval();
int setDelayInterval();
int setTarget();
int setProbability();
int setDelayInterval();

int timeConstReset();
int printAllConst();





#endif // _REQUEST_H_
