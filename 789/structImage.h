#ifndef STRUCTIMAGE_H
#define STRUCTIMAGE_H
#include <QImage>
typedef struct ImageStruct{
    bool BPenable,DPenable,isProcessedFlag;
    int index;
    double meanGray;
    QString patternName;
    QImage image;
    QVector<QPoint> defectPoint;//如果沒有瑕疵座標->null,size()為0
} ImageProcess;

#endif // STRUCTIMAGE_H

