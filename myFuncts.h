#ifndef MYFUNCTS_H
#define MYFUNCTS_H

#include<stdint.h>
#include<stdlib.h>
#include"ini.h"

extern char buffIni[40];
extern char iniFile[20];

extern bool DEBUG_CONFIG;
extern uint8_t MCP2515_fOsc;
extern float CANBUS_SPPos;
extern uint16_t BOND_James;
bool initFuncts(char* _iniFile);

#endif // MYFUNCTS_H
