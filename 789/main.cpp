#include <QCoreApplication>
#include <opencv2/opencv.hpp>
#include "imageprocess.h"
#include <QVector>
#include "structImage.h"
using namespace cv;
int main(int argc, char *argv[])
{
    imageprocess process;
    QCoreApplication a(argc, argv);
    QVector<ImageStruct> V_Q;
    ImageStruct p ;
    p.BPenable =0;
    p.DPenable =0;
    p.index = 0;
    p.patternName="White";
    p.image.load("./EXP45606_WHITE.bmp");

    ImageStruct p2 ;
    p2.BPenable =1;
    p2.DPenable =0;
    p2.index = 1;
    p2.patternName="black";
    p2.image.load("./denoise.bmp");


    V_Q.push_back(p);
    V_Q.push_back(p2);

    //using process for CT2defect
    process.process(&V_Q);
    // p.process(&V_Q);
    // std::cout << V_Q[0].x<<std::endl;
    // std::cout << V_Q[0].y;
    // qDebug() << V_Q[1].defectPoint;
    return a.exec();
}
