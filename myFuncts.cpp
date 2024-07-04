#include "myFuncts.h"
#include<QDebug>
#include"ini.h"
bool initFuncts(char* _iniFile)
{

    if(ini_gets("CAM1","exposureTime","0",buffIni,2,_iniFile)){
        CAM1_exposureTime=atoi(buffIni);
    }else{
        puts("Can't set exposureTime CAM1");
        return false;
    }

    if(ini_gets("COORDINATE","PT_sizeX","768",buffIni,5,iniFile)){
        COORDINATE_PTsX=atoi(buffIni);

    }else{
        puts("Can't set parameter COORDINATE_PTsX");
        return false;
    }

    if(ini_gets("COORDINATE","PT_sizeY","432",buffIni,5,iniFile)){
        COORDINATE_PTsY=atoi(buffIni);

    }else{
        puts("Can't set parameter COORDINATE_PTsY");
        return false;
    }
    return true;
}
