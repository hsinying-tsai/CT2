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
