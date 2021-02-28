#ifndef _PRINT_STATISTICS_H_
#define _PRINT_STATISTICS_H_

#include <stdlib.h>

void printCalcError();
void errVariablesReset();
size_t getRepeatsForCalcErr();
void printStatShort(const size_t closedReq, const size_t queueLen, const double avgQueueLen);
void calcError(double modelT, double waitT, size_t openReq, size_t numOfServ);
void printStatAll(const double modelT, const double waitT, const size_t numOfServ,\
                            const size_t closedReq, const double sumTimeInQueue, const size_t openedReq);


#endif  // _PRINT_STATISTICS_H_