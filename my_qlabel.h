#ifndef MY_QLABEL_H
#define MY_QLABEL_H

#include<QLabel>
#include<QObject>
#include<QWidget>
#include<QMouseEvent>
#include<QWheelEvent>
#include<vector>
#include<opencv2/opencv.hpp>
#include<QImage>
#include"logger.h" 
#include"myFuncts.h"
class my_qlabel : public QLabel
{
    Q_OBJECT
public:
    explicit my_qlabel(QWidget *parent = 0);
    int x,y,zoomtime = 1,labelWidth,labelHeight;
    double zoomfactor = 0.02, magnificationFactor = 1,deltaX,deltaY;
    struct frame{
        double x,y;
        qreal width, height;
        QPixmap pix;
    };
    frame paint,zoom,del,press;
    QRectF tmp_rect;
    QImage qImage;
    cv::Mat cvImage;
    QPixmap orig_pic;
    QPixmap mat2pixmap(cv::Mat const& src);
    QPoint numDegrees;
    bool m_isDragging = false;
    void setImage(const QPixmap &image,const QVector<QPoint> DefectCoordinates);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev) override;
    void updateMousePosition();
    void MouseCurrentPos();
    void updatelblPic();
    void addRectangle(const QRect &rect);
    void addRectangle2(const QRectF &rect);
    void updateBoxPosition();
    void drawRectangleOnImage(cv::Mat& image,const QVector<QPoint> DefectVectors);
    void updateRectangle2();

signals:
    int Mouse_Pos();
    int set_pic2(const int);

private:
    QList<QRect> rectangles;
    QList<QRectF> rectangles2;
    Logger logger;

signals:
    void rectangleClicked(int index);
};


#endif // MY_QLABEL_H
