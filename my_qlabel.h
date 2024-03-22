#ifndef MY_QLABEL_H
#define MY_QLABEL_H
#include"widget.h"
#include <QLabel>
#include <QWidget>
#include<QMouseEvent>
#include<vector>
#include<QWheelEvent>
#include<math.h>
#include<QObject>
#include<opencv2/opencv.hpp>
#include<QImage>
using namespace std;
class my_qlabel : public QLabel
{
    Q_OBJECT
public:

    int x,y,zoomtime = 1, frame_x = 200, frame_y = 200,paint_count,j,test = 1;
    double zoomFactor = 0.05, pre_zoomX = 0, pre_zoomY = 0
        ,pre_posX = 0,posX,posY, pre_posY = 0,del_posX, del_posY, now_posX, now_posY,press_x,press_y, zoomCustomX,zoomCustomY
        ,magnificationFactor = 1;
    struct paint_frame{

    };
    QRectF tmp_rect;
    cv::Mat cvImage;
    QImage qImage;
    qreal count_matrix_coodinateX, count_matrix_coodinateY;
    qreal customX, customY, customWidth, customHeight,zoomX = 0, zoomY= 0, zoomWidth = 3840, zoomHeight= 2160;
    QPixmap pix,zoompix, orig_pic,pix_frame;
    QPoint numDegrees;
    QVector<int> matrix_coodinate = {500,700, 800,1000, 1100,1300 , 1400,1600};
//    std::vector<std::vector<double>> matrix_coodinate2;
    explicit my_qlabel(QWidget *parent = 0);

    void setImage(const QPixmap &image);
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
    void drawRectangleOnImage(cv::Mat& image);
    void updateRectangle2();
    bool m_isDragging = false,pic_zoomOut=false;

    QPixmap mat2pixmap(cv::Mat const& src);

signals:
    int Mouse_Pos();
    int set_pic2(const int);


private:

    QList<QRect> rectangles;
    QList<QRectF> rectangles2;

signals:
    void rectangleClicked(int index);
};


#endif // MY_QLABEL_H
