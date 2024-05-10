#ifndef MYFUNCTS_H
#define MYFUNCTS_H

#include<stdint.h>
#include<stdlib.h>

extern char buffIni[40];
extern char iniFile[20];
extern uint8_t CAM1_exposureTime;

extern uint16_t COORDINATE_PTsX,COORDINATE_PTsY;


bool initFuncts(char* _iniFile);
#endif // MYFUNCTS_H
