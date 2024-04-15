#ifndef MYFUNCTS_H
#define MYFUNCTS_H

#include<stdint.h>
#include<stdlib.h>

extern char buffIni[40];
extern char iniFile[20];

extern bool DEBUG_CONFIG;
extern uint8_t MCP2515_fOsc;
extern float CANBUS_SPPos;
extern uint16_t BOND_James;
extern uint8_t CAM1_exposureTime,CAM1_parm2,CAM1_parm3,CAM1_parm4,CAM1_parm5;
extern uint8_t CAM2_exposureTime,CAM2_parm2,CAM2_parm3,CAM2_parm4,CAM2_parm5;
extern uint8_t CAM3_exposureTime,CAM3_parm2,CAM3_parm3,CAM3_parm4,CAM3_parm5;
extern uint16_t COORDINATE_PTsX,COORDINATE_PTsY;
extern char picfoldpath[100];
bool initFuncts(char* _iniFile);
#endif // MYFUNCTS_H
