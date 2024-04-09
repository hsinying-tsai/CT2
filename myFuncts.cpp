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

    if(ini_gets("CAM1","parmeter1","0",buffIni,8,iniFile)){
        CAM1_parm1=atoi(buffIni);
    }else{
        puts("Can't set parameter CAM1_1");
        return false;
    }
    if(ini_gets("CAM1","parmeter2","0",buffIni,2,iniFile)){
        CAM1_parm2=atoi(buffIni);

    }else{
        puts("Can't set parameter CAM1_2");
        return false;
    }



    return true;
}
