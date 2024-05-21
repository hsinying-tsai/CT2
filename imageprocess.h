#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include <opencv2/opencv.hpp>
#include <QPoint>
#include <QVector>
#include "mystruct.h"
#include "structImage.h"
using namespace cv;
class imageprocess
{
public:
    imageprocess();
    void process(QVector<ImageStruct>* x);



private:
    Mat variance_filter(const Mat& image,double radius);
    std::vector<Point> find_maxima_with_prominence(const cv::Mat& image, int neighborhood_size, double prominence_threshold);
    float calculate_iou(const cv::Rect& rect1, const cv::Rect& rect2);
    QPoint calCameraCenter(Mat bigImage,cv::Point2f *topleft,cv::Point2f *bottomRight);
    QVector<QPoint>calBrightPoint(Mat inputImage,Mat M);
    QVector<QPoint>calDarkPoint(Mat inputImage,Mat M);
    cv::Mat getPerspectiveTransform(cv::Mat* whiteImage);
    cv::Mat QImageToCvMat(const QImage& image);
    //For imageprocessing para



};

#endif // IMAGEPROCESS_H
