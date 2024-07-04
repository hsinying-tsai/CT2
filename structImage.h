#ifndef STRUCTIMAGE_H
#define STRUCTIMAGE_H
#include <QImage>
typedef struct ImageStruct{
    bool BPenable,DPenable,BLenable,DLenable,isProcessedFlag;
    int index;
    double meanGray;
    QString patternName;
    QImage image;

    //瑕疵類別分為BP,DP,HOpen, VOpen, HShort, VShort
    enum defectType{BP,DP,HOpen, VOpen, HShort, VShort};

    //檢測類別分為BP,DP,BL,DL
    //widget.cpp Line188左右有定義defectType用於顯示表格
    QVector<QPair<defectType,QVector<QPoint>>> defectPoint;


} ImageProcess;

#endif // STRUCTIMAGE_H

