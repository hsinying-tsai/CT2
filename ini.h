#ifndef INI_H
#define INI_H
#include<stdio.h>
#include<string.h>
#include<readIni-master/readIni.h>
extern char buffIni[40];

class INI
{
public:
    INI();
    int ini_gets(const char *Section, const char *Key, const char *DefValue, char *Buffer, int BufferSize, const char *Filename);
};

#endif // INI_H
