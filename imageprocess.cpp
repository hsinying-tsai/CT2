#include "imageprocess.h"
#include <QDebug>


imageprocess::imageprocess() {}

void imageprocess::process(QVector<ImageStruct>* x){
    qDebug()<<"imageprocess::process";
    cv::Mat wimage,PerspectiveTransform ;
    // structure + iterator 的用法
    QVector<ImageStruct>::iterator it = x->begin();
    for(; it != x->end(); it++) {
        if (it->patternName == "White"){

            wimage = this->QImageToCvMat(it->image);
            try{
                PerspectiveTransform = this->getPerspectiveTransform(&wimage);
                //                std::cout << PerspectiveTransform;
            }
            catch(const std::exception &e){
                qDebug()<< "Find PerspectiveTransform ERROR;";
            }

            break;
        }
        // it->x++; // 這時候的it代表一個structure的指標
        // it->y--;
    }


    it = x->begin();
    for(; it != x->end(); it++) {
        // std::cout << ;

        if(it->BPenable){
            qDebug()<<it->patternName<<"got BP true";
            try{
                //0628 修改structImage定義
//                it->defectPoint = calBrightPoint(this->QImageToCvMat(it->image),PerspectiveTransform);

            }
            catch(const std::exception &e){
                qDebug()<< "BP ERROR";
            }
            // qDebug() << it->patternName;
            // qDebug() << it->defectPoint;

        }
        if(it->DPenable){
            qDebug()<<it->patternName<<"got DP true";
        }

        if(it->BLenable){
            qDebug()<<it->patternName<<"got BL true";
        }
        if(it->DLenable){
            qDebug()<<it->patternName<<"got DL true";
        }
    }

    // std::cout<<x;


}
QVector<QPoint>imageprocess::calBrightPoint(Mat inputImage,Mat M)
{

    //high expoursetimes must be denoised
    Mat noiseImage = cv::imread("./noise.bmp",cv::IMREAD_GRAYSCALE);
    Mat result_image,denoise_image;
    cv::subtract(inputImage,noiseImage,denoise_image);

    /*
        step1.threshold(image_gray,image_gray,125,255,0);
        pixelSize for real object =>Size(2900,1660)
        int neighborhood_size = 20,     //邻域大小
        prominence_threshold = 5 ,  //显著性阈值
        grouping_threshold = 20;    //分组阈值
        float iou_threshold = 0.001;     //IOU阈值
     */
    QVector<QPoint> returnValue = {};

    // qDebug()<<"1213";
    // Mat image,image_gray;
    // std::vector<std::vector<Point>>contours;
    // std::vector<Point> rectPoint;




    warpPerspective(denoise_image,result_image,M,Size(2900,1660));

    Mat variance = variance_filter(result_image,3);
    // imwrite("./VVV.bmp",variance);
    Mat output_image ;
    cvtColor(result_image,output_image,COLOR_GRAY2BGR);
    //设定参数
    int neighborhood_size = 20,     //邻域大小
        prominence_threshold = 5 ,  //显著性阈值
        grouping_threshold = 20;    //分组阈值
    float iou_threshold = 0.001;     //IOU阈值
        //查找局部最大值并考虑峰值的显著性
    std::vector<Point> maximaCoordinates = find_maxima_with_prominence(variance ,neighborhood_size,prominence_threshold);
    std::vector<std::pair<cv::Point, cv::Rect>> drawn_areas;
    std::vector<std::pair<cv::Point, cv::Rect>> toDrawn_areas;
    for(const auto& point : maximaCoordinates){

        // 計算當前區域的矩形框坐標
        cv::Rect rect(point.x - neighborhood_size / 2, point.y - neighborhood_size / 2, neighborhood_size, neighborhood_size);
        // 檢查當前區域是否與已繪製區域重疊
        bool grouped = false;
        for (auto& drawn_area : drawn_areas) {
            float iou = calculate_iou(rect, drawn_area.second);
            // Mat p =output_image.col(point.y).row(point.x);
            // p = Scalar(255,255,255);
            //iou > iou_threshold
            if (iou > iou_threshold) { // 如果IOU大於閾值，則將當前區域與已繪製區域合併
                // qDebug() << iou;
                // std::cout << iou <<'\n';
                // 更新已繪製區域的坐標
                drawn_area.first = drawn_area.first;
                int start_x = std::min(rect.x, drawn_area.second.x),
                    start_y = std::min(rect.y, drawn_area.second.y),
                    end_x =(rect.x+rect.width >= drawn_area.second.x+drawn_area.second.width)?rect.x+rect.width:drawn_area.second.x+drawn_area.second.width,
                    end_y = (rect.y+rect.height >= drawn_area.second.y+drawn_area.second.height)?rect.y+rect.height:drawn_area.second.y+drawn_area.second.height;
                drawn_area.second = cv::Rect(start_x,start_y ,end_x-start_x,end_y-start_y);
                // toDrawn_areas.push_back(std::make_pair(drawn_area.first,cv::Rect(start_x,start_y ,end_x-start_x,end_y-start_y)));
                grouped = true;
                break;
            }
            else{
                // std::cout <<iou<<'\n';
            }
        }

        // 如果當前區域與已繪製區域沒有重疊，則繪製該區域
        if (!grouped) {
            // 在這裡可以執行繪製操作，例如繪製矩形
            // cv::rectangle(output_image, rect, cv::Scalar(0, 255, 0),1);
            drawn_areas.push_back(std::make_pair(point, rect)); // 將當前區域添加到已繪製區域列表中

            //(rect[0] + rect[2]) / 2
            //(rect[1] + rect[3]) / 2
            // Point center = Point(int(rect.x+rect.width/2),int(rect.y+rect.height/2));
            // Point center =(5,5);
            // circle(output_image,center,10, (0, 0, 255), 0);
        }

    }

    int fontFace =0,fontScale=1,thickness=2;
    String temp;
    Scalar color=(255,0,0);
    for (auto& drawn_area : drawn_areas) {
        cv::rectangle(output_image, drawn_area.second, cv::Scalar(0, 255, 0),1);
        returnValue.push_back(QPoint(drawn_area.first.x,drawn_area.first.y));
        temp = "(" + std::to_string(drawn_area.first.x) + ", " + std::to_string(drawn_area.first.y) + ")";
        cv::putText(output_image,temp,drawn_area.first,2,3,Scalar(255,255,255));
    }

    imwrite("./output.bmp",output_image);

    // namedWindow("Display Image", WINDOW_NORMAL );
    // imshow("Display Image", output_image);
    // waitKey(10);




    return returnValue;
}
QPoint imageprocess::calCameraCenter(Mat bigImage,cv::Point2f *topleft,cv::Point2f *bottomRight){

    // std::cout << "1234";

    float pixelSize;
    QPoint imageCenter= QPoint(bigImage.cols/2,bigImage.rows/2),panelCenter;
    // std::cout << imageCenter.x();
    // std::cout << imageCenter.y()<<'\n';
    panelCenter = QPoint((topleft->x+bottomRight->x)/2,(topleft->y+bottomRight->y)/2);
    // std::cout << panelCenter.x();
    // std::cout << panelCenter.y()<<'\n';

    pixelSize = 396.21/sqrtl(powf(bottomRight->x-topleft->x,2)+powf(bottomRight->x-topleft->x,2));
    // std::cout << pixelSize;

    // std::cout << topleft->x-imageCenter.x();
    // std::cout << topleft->y-imageCenter.y()<<'\n';
    return QPoint(topleft->x-imageCenter.x(),topleft->y-imageCenter.y());
    // namedWindow("Display Image", WINDOW_NORMAL );
    // imshow("Display Image", bigImage);
    // waitKey(10);
}

Mat imageprocess::variance_filter(const Mat& image,double radius){
    cv::Mat floatImage;
    image.convertTo(floatImage,CV_8S);
    // 定義卷積核
    int kernel_size = 2 * radius + 1;
    cv::Mat kernel = cv::Mat::ones(kernel_size, kernel_size, CV_32F) / (kernel_size * kernel_size);

    // 對圖像進行方框濾波
    cv::Mat mean;
    cv::filter2D(image, mean, -1, kernel);

    // 計算圖像的方差
    // cv::Mat squared_diff = (image - mean).mul (image - mean);
    cv::Mat squared_diff;
    cv::Mat diff ;
    cv::Mat floatmean;
    mean.convertTo(floatmean,CV_8S);
    diff = floatImage-floatmean;
    diff = diff.mul(diff);
    diff.convertTo(squared_diff,CV_8U);
    // image.convertTo(image,CV_8S);
    // mean.convertTo(mean,CV_8S);
    // diff = (floatImage - mean);
    // std::cout << diff;

    cv::Mat variance;
    cv::filter2D(squared_diff, variance, -1, kernel);

    return variance;
}
std::vector<Point> imageprocess::find_maxima_with_prominence(const cv::Mat& image, int neighborhood_size, double prominence_threshold){
    // 使用局部極大值濾波器找到局部極大值
    cv::Mat local_max;
    cv::dilate(image, local_max, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(neighborhood_size, neighborhood_size)));
    // qDebug()<<local_max.type();
    // 找到圖像中的局部最小值
    cv::Mat local_min;
    cv::erode(image, local_min, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(neighborhood_size, neighborhood_size)));

    // 計算每個峰值與相鄰谷底之間的距離
    cv::Mat distance_to_min;
    cv::distanceTransform(local_min, distance_to_min, cv::DIST_L2, 3);
    cv::normalize(distance_to_min, distance_to_min, 0, 255, cv::NORM_MINMAX);
    // qDebug()<<distance_to_min.type();
    // 計算峰值的顯著性
    cv::Mat prominence;
    subtract(local_max,distance_to_min,prominence,noArray(),1);

    // 找到顯著性大於閾值的峰值位置
    cv::Mat maxima_mask = prominence > prominence_threshold;
    std::vector<Point> maxima_coordinates;
    cv::findNonZero(maxima_mask, maxima_coordinates);
    imwrite("./find_maxima_with_prominence.bmp",prominence);
    return maxima_coordinates;
}
// 定義計算 IOU 的函數
float imageprocess::calculate_iou(const cv::Rect& rect1, const cv::Rect& rect2) {
    // 計算矩形1的面積
    int area1 = (rect1.width) * (rect1.height);
    // 計算矩形2的面積
    int area2 = (rect2.width) * (rect2.height);

    // 計算矩形1和矩形2的交集部分的坐標
    int inter_top_left_x = std::max(rect1.x, rect2.x);
    int inter_top_left_y = std::max(rect1.y, rect2.y);
    int inter_bottom_right_x = std::min(rect1.x + rect1.width, rect2.x + rect2.width);
    int inter_bottom_right_y = std::min(rect1.y + rect1.height, rect2.y + rect2.height);

    //    inter_area = max(0, inter_bottom_right[0] - inter_top_left[0]) * max(0, inter_bottom_right[1] - inter_top_left[1])
    // 計算交集部分的面積
    int inter_width = std::max(0, inter_bottom_right_x - inter_top_left_x);
    int inter_height = std::max(0, inter_bottom_right_y - inter_top_left_y);
    int inter_area = abs(inter_width * inter_height);

    // 計算並返回 IOU
    float iou = static_cast<float>(inter_area) / static_cast<float>(area1 + area2 - inter_area);

    return abs(iou);
}
cv::Mat imageprocess::getPerspectiveTransform(cv::Mat* whiteImage){
    // std::cout <<std::endl<< whiteImage->channels()<<std::endl;
    Mat image_gray;
    image_gray= whiteImage->clone();
    std::vector<std::vector<Point>>contours;
    std::vector<Point> rectPoint;

    // cvtColor(*whiteImage,image_gray,COLOR_BGR2GRAY);
    threshold(image_gray,image_gray,125,255,0);
    findContours(image_gray,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    // qDebug()<<contours.size();

    // Draw rectangles around the contours
    for (size_t i = 0; i < contours.size(); i++) {
        // Get the approximate polygon of the contour
        double epsilon = 0.01 * cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], contours[i], epsilon, true);

        // Get the bounding box coordinates
        cv::Rect boundingRect = cv::boundingRect(contours[i]);

        // Draw a rectangle around each contour
        // cv::rectangle(image, boundingRect, cv::Scalar(0, 255, 0), 2);

        // Get the four points of the contour
        for (size_t j = 0; j < contours[i].size(); j++) {
            rectPoint.push_back(contours[i][j]);
            // cv::circle(image, contours[i][j], 3, cv::Scalar(255, 0, 0), -1); // Draw circles at the vertices
        }
    }

    // 找到左上角、左下角、右下角和右上角
    cv::Point2f topLeft, bottomLeft, bottomRight, topRight;
    double sumX=0,sumY=0;
    for (const auto& point : rectPoint) {
        sumX += point.x;
        sumY += point.y;
    }

    double meanX = sumX/rectPoint.size(),meanY = sumY/rectPoint.size();


    for (const auto& point : rectPoint) {
        if (point.x <meanX && point.y < meanY){
            topLeft = point;
        }
        else if(point.x <meanX && point.y > meanY){
            bottomLeft = point;
        }
        else if(point.x >meanX && point.y < meanY){
            topRight = point;
        }
        else if(point.x >meanX && point.y > meanY){
            bottomRight = point;
        }
    }

    this->calCameraCenter(image_gray,&topLeft,&bottomRight);

    std::vector<cv::Point2f> sortedRectPoint = {topLeft, bottomLeft, bottomRight, topRight};
    // 定義目標長方形的四個角點坐標（扭正後的長方形）
    std::vector<cv::Point2f> dst_pts = {{0, 0}, {0, 1660},{2900, 1660} ,{2900, 0} }; // 替換為您的目標坐標值

    // 計算透視變換矩陣
    cv::Mat M = cv::getPerspectiveTransform(sortedRectPoint, dst_pts);

    // std::cout <<M;
    return M;
}
cv::Mat imageprocess::QImageToCvMat(const QImage& image) {
    QImage::Format format = image.format();
    if (format == QImage::Format_RGB32) {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        return mat.clone(); // Make a deep copy
    } else if (format == QImage::Format_RGB888) {
        cv::Mat mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        cv::Mat matRGB;
        cv::cvtColor(mat, matRGB, cv::COLOR_BGR2RGB);
        return matRGB;
    } else if (format == QImage::Format_Indexed8) {
        cv::Mat mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        return mat.clone(); // Make a deep copy
    } else {
        return cv::Mat(); // Return an empty cv::Mat if the format is not supported
    }
}
