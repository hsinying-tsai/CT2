#include "widget.h"
#include <QApplication>

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include "ini.h"
#include"myFuncts.h"

char buffIni[40];
char iniFile[20];

bool DEBUG_CONFIG;
uint8_t MCP2515_fOsc;
float CANBUS_SPPos;
uint16_t BOND_James;
uint8_t CAM1_exposureTime,CAM2_exposureTime,CAM3_exposureTime;
uint16_t COORDINATE_PTsX,COORDINATE_PTsY;
char picfoldpath[100];
int main(int argc, char *argv[])
{
    //192.168.1.168 8501
    if(argc>1)
    {
        strcpy(iniFile,argv[1]);
    }
    else
    {
        strcpy(iniFile,"config.ini");
    }
    if(!initFuncts(iniFile))
    {
        puts("initFuncts error");
        return EXIT_FAILURE;
    }else{
//        puts("initFuncts OK");
//        printf("Setting DEBUG_CONFIG %i\n", DEBUG_CONFIG);
//        printf("Setting MCP2515_fOsc %i\n", MCP2515_fOsc);
//        printf("Setting CANBUS_SPPos %f\n", CANBUS_SPPos);
//        printf("BOND_James %i\n", BOND_James);
        printf("Setting CAM1_exposureTime %i\n",CAM1_exposureTime);
        printf("Setting CAM2_exposureTime %i\n",CAM2_exposureTime);
        printf("Setting CAM3_exposureTime %i\n",CAM3_exposureTime);
        printf("Setting COORDINATE_PTsX %i\n", COORDINATE_PTsX);
        printf("Setting COORDINATE_PTsY %i\n", COORDINATE_PTsY);
        printf("Setting the path of picture floder: %s\n",picfoldpath);

//        return EXIT_SUCCESS;
    }
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
