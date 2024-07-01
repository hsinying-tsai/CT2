#ifndef STRUCTIMAGE_H
#define STRUCTIMAGE_H
#include <QImage>
typedef struct ImageStruct{
    bool BPenable,DPenable,LINEenable,isProcessedFlag;
    int index;
    double meanGray;
    QString patternName;
    QImage image;

    //瑕疵類別分為BP,DP,HOpen, VOpen, HShort, VShort
    enum defectType{BP,DP,HOpen, VOpen, HShort, VShort};

    //檢測類別分為BP,DP,BL,DL
    QVector<QPair<defectType,QVector<QPoint>>> defectPoint;
} ImageProcess;

#endif // STRUCTIMAGE_H

