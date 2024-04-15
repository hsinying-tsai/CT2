#include "myFuncts.h"
#include<QDebug>
#include"ini.h"
bool initFuncts(char* _iniFile)
{
    if(ini_gets("DEBUG","CONFIG","0",buffIni,2,_iniFile)){
        DEBUG_CONFIG=atoi(buffIni);
    }else{
        puts("Can't set parameter DEBUG_CONFIG");
        return false;
    }

    if(ini_gets("MCP2515","fOsc","20",buffIni,3,iniFile)){
        MCP2515_fOsc=atoi(buffIni);
    }else{
        puts("Can't set parameter MCP2515_fOsc");
        return false;
    }

    if(ini_gets("CANBUS","SPPos","0.75",buffIni,5,iniFile)){
        CANBUS_SPPos=atof(buffIni);
    }else{
        puts("Can't set parameter CANBUS_SPPos");
        return false;
    }

    if(ini_gets("BOND","James","65535",buffIni,6,iniFile)){
        BOND_James=atoi(buffIni);
    }else{
        puts("Can't set parameter BOND_James");
        return false;
    }

    if(ini_gets("CAM1","exposureTime","0",buffIni,2,iniFile)){
        CAM1_exposureTime=atoi(buffIni);
    }else{
        puts("Can't set exposureTime CAM1");
        return false;
    }
    if(ini_gets("CAM2","exposureTime","0",buffIni,2,iniFile)){
        CAM2_exposureTime=atoi(buffIni);

    }else{
        puts("Can't set exposureTime CAM2");
        return false;
    }

    if(ini_gets("CAM3","exposureTime","0",buffIni,2,iniFile)){
        CAM3_exposureTime=atoi(buffIni);

    }else{
        puts("Can't set exposureTime CAM3");
        return false;
    }


    if(ini_gets("COORDINATE","PT_sizeX","576",buffIni,5,iniFile)){
        COORDINATE_PTsX=atoi(buffIni);

    }else{
        puts("Can't set parameter COORDINATE_PTsX");
        return false;
    }


    if(ini_gets("COORDINATE","PT_sizeY","324",buffIni,5,iniFile)){
        COORDINATE_PTsY=atoi(buffIni);

    }else{
        puts("Can't set parameter COORDINATE_PTsY");
        return false;
    }


    if(ini_gets("PICTURE","pic_fold_path"," ",buffIni,sizeof(buffIni),iniFile)){
        strncpy(picfoldpath, buffIni,sizeof(buffIni)-1);
        picfoldpath[sizeof(picfoldpath) - 1] = '\0';
    }else{
        puts("Can't set parameter COORDINATE_PTsY");
        return false;
    }

    return true;
}
