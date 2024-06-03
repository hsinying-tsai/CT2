#include "my_qlabel.h"
#include "widget.h"
#include<QDebug>
#include<QCursor>
#include<QGraphicsScene>
#include<QColor>
#include<QToolTip>
#include<QPixmap>

my_qlabel::my_qlabel(QWidget *parent)
    :QLabel(parent){
    paint.width = 50;
    paint.height = 50;
    zoom.width = 3840;
    zoom.height = 2160;
    connect(this,&my_qlabel::rectangleClicked,[this](int index){
        qDebug()<<"recatngle"<<index+1<<"is clicked";
//        this->rectangleClicked(index);
        logger.writeLog(Logger::Info,"User clicked rectangle number " + QString::number(index+1)+".");
    });
    updateRectangle2();

}

void my_qlabel::setImage(const QPixmap &image,const QVector<QPoint> DefectCoordinates)
{
    magnificationFactor = 1;
    zoomtime = 1;
    zoom.width = 3840;
    zoom.height = 2160;
    zoom.x = 0;
    zoom.y = 0;
    orig_pic = image;
    //Qpixmap to QImage
    qImage = image.toImage();
    //QImage::Format_RGB32
    cv::Mat cvImage(qImage.height(), qImage.width(), CV_8UC4, const_cast<uchar*>(qImage.bits()), qImage.bytesPerLine());
    drawRectangleOnImage(cvImage,DefectCoordinates);
    this->setPixmap(paint.pix);
    paint.pix = paint.pix.copy(0,0,3840,2160);
}

void my_qlabel::drawRectangleOnImage(cv::Mat &image,const QVector<QPoint> DefectVectors)
{
    rectangles.clear();
    foreach(const QPoint &coodinate,DefectVectors){
//        qDebug()<<coodinate.x()<<coodinate.y();
        addRectangle(QRect(coodinate.x()-10,coodinate.y()-10,paint.width,paint.height));
    }
    for(const QRect& rect : rectangles){
        cv::Rect frame(rect.x(),rect.y(),paint.width,paint.height);
        cv::rectangle(image, frame, cv::Scalar(255,0,0),3);
    }
    paint.pix = mat2pixmap(image);
    updateRectangle2();
}

void my_qlabel::updateRectangle2()
{
    rectangles2.clear();
    qDebug()<<"-------------";
    qDebug()<<"Info:"<<zoom.x+20<<zoom.y+20<<magnificationFactor<<zoomtime;
    for(const QRect &rect:rectangles){
        QRectF tmp_rect;
        tmp_rect.setX((rect.x()-zoom.x) *0.15 *magnificationFactor + magnificationFactor*zoomtime);
        tmp_rect.setY((rect.y()-zoom.y) *0.15 *magnificationFactor + magnificationFactor*zoomtime);
//        tmp_rect.setX(zoom.x+rect.x()+20);
//        tmp_rect.setY(zoom.y+rect.y()+20);

        tmp_rect.setWidth(paint.width+10);
        tmp_rect.setHeight(paint.height+10);
//        qDebug()<<(rect.x()-zoom.x) *0.15 *magnificationFactor + magnificationFactor*zoomtime<<(rect.y()-zoom.y) *0.15 *magnificationFactor + magnificationFactor*zoomtime;
        qDebug()<<tmp_rect;
        addRectangle2(QRectF(tmp_rect.x(),tmp_rect.y(),tmp_rect.width(),tmp_rect.height()));
    }
}

QPixmap my_qlabel::mat2pixmap(const cv::Mat &src)
{
    cv::Mat tmp;
    cv::cvtColor(src, tmp,cv::COLOR_BGR2RGB);
    QImage dest((const uchar *) tmp.data,tmp.cols,tmp.rows,tmp.step,QImage::Format_RGB888);
    dest.bits();
    return QPixmap::fromImage(dest);
}


void my_qlabel::mouseMoveEvent(QMouseEvent *ev)
{
    updateRectangle2();
    this->x = ev->x();
    this->y = ev->y();
    qDebug()<<"Mouse"<<ev->x()<<ev->y();
    emit Mouse_Pos();
    for(const QRectF& rect : rectangles2){
        if(rect.contains((ev->pos()))){
            QToolTip::showText(ev->globalPos(),"點選放大");
        }else{
            QToolTip::hideText();

        }
    }
    updateMousePosition();
    if(m_isDragging){
        del.x = ev->x() - press.x;
        del.y = ev->y() - press.y;

        if(magnificationFactor>1){
//            qDebug()<<"delposX"<<del.x<<"delposY"<<del.y;
            if(del.x>zoom.x){
                del.x = zoom.x;
            }else if(del.x>0){
                del.x = del.x;
            }else if((zoom.x-del.x+zoom.width)>3840){
                del.x = 0;
            }
            if(del.y>zoom.y){
                del.y = zoom.y;
            }else if(del.y>0){
                del.y = del.y;
            }else if((zoom.y-del.y+zoom.height)>2160){
                del.y = 0;
            }
            zoom.pix = paint.pix.copy(zoom.x-del.x, zoom.y-del.y, zoom.width, zoom.height);
            this->setPixmap(zoom.pix);
        }

    }
}

void my_qlabel::mousePressEvent(QMouseEvent *ev)
{
    m_isDragging = true;
    updateMousePosition();
    press.x = ev->pos().x();
    press.y = ev->pos().y();
    qDebug()<<"左鍵按下";
    qDebug()<<ev->pos();
    for(int i = 0; i<rectangles2.size();i++){
        if(rectangles2[i].contains((ev->pos()))){
            emit rectangleClicked(i);
            emit set_pic2(i+1);
        }
    }

}

void my_qlabel::mouseReleaseEvent(QMouseEvent *ev)
{
    // qDebug()<<"mouseReleaseEvent!";
    zoom.x = zoom.x-del.x;
    zoom.y = zoom.y-del.y;

    m_isDragging = false;
    qDebug()<<"左鍵放掉";
    updateMousePosition();
}

void my_qlabel::updateMousePosition()
{
    if(magnificationFactor>1){
        if(m_isDragging){
            this->setCursor(Qt::ClosedHandCursor);
        }else{
            this->setCursor(Qt::OpenHandCursor);
        }
    }else{
        this->setCursor(Qt::CrossCursor);
    }
    if(zoom.x<0){
        zoom.x = 0;
    }else if(zoom.x>3840){
        zoom.x = 3840;
    }
    if(zoom.y<0){
        zoom.y = 0;
    }else if(zoom.y>2160){
        zoom.y = 2160;
    }

}

void my_qlabel::addRectangle(const QRect &rect)
{
    rectangles.append(rect);
    update();
}

void my_qlabel::addRectangle2(const QRectF &rect)
{
    rectangles2.append(rect);
    update();
}

void my_qlabel::wheelEvent(QWheelEvent *ev)
{
    updateMousePosition();
    qDebug()<<"magnificationFactor"<<magnificationFactor;
    numDegrees = ev->angleDelta();
    if(numDegrees.y()>0){
        if(zoomtime == 40){
            zoom.pix = paint.pix.copy(zoom.x, zoom.y, zoom.width, zoom.height);
            this->setPixmap(zoom.pix);
        }else{
            qDebug()<<"forward";
            //放大的倍率
            magnificationFactor += zoomfactor;
            qDebug()<<"放大的倍率:"<<magnificationFactor;
            zoom.x += (3840*zoomfactor)*((x*(3840/labelWidth))/(zoom.width));
            zoom.y += (2160*zoomfactor)*((y*(2160/labelHeight))/(zoom.height));
            zoom.width -= 3840*zoomfactor;
            zoom.height -= 2160*zoomfactor;
            // qDebug()<<"放大後的左上角座標:"<<zoom.x<<","<<zoom.y;
            zoom.pix = paint.pix.copy(zoom.x, zoom.y, zoom.width, zoom.height);
            this->setPixmap(zoom.pix);
            zoomtime++;
        }
    }else{
        qDebug()<<"backward";
        // 圖片最小是原圖大小
        if(zoomtime<0){
            qDebug()<<"已縮至原大小";
            zoom.x = 0;
            zoom.y = 0;
            zoom.width = 3840;
            zoom.height = 2160;
            magnificationFactor=1;
            zoom.pix = paint.pix.copy(zoom.x,zoom.y,zoom.width, zoom.height);
            this->setPixmap(zoom.pix);
        }else{
            qDebug()<<zoomtime;
            zoom.width += 3840* zoomfactor;
            zoom.height += 2160* zoomfactor;
            zoom.x -= (3840*zoomfactor)*(1-(x/labelWidth));
            zoom.y -= (2160*zoomfactor)*(1-(y/labelHeight));
            zoom.pix = paint.pix.copy(zoom.x,zoom.y,zoom.width, zoom.height);
            this->setPixmap(zoom.pix);
            magnificationFactor -= zoomfactor;
            zoomtime--;
        }
    }
    updateRectangle2();
}

