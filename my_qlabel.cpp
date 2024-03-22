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
    int t = 0;
    for(int i=0; i<matrix_coodinate.size()/2;i++){
        addRectangle(QRect(matrix_coodinate[t],matrix_coodinate[t+1],frame_x,frame_y));
        t = t + 2;
    }
    connect(this,&my_qlabel::rectangleClicked,[this](int index){
//        this->rectangleClicked(index);
        qDebug()<<"recatngle"<<index+1<<"is clicked";
    });
    updateRectangle2();
}

void my_qlabel::setImage(const QPixmap &image)
{
    magnificationFactor = 1;
    zoomtime = 1;
    zoomWidth = 3840;
    zoomHeight = 2160;
    zoomX = 0;
    zoomY = 0;
    orig_pic = image;
    //Qpixmap to QImage
    qImage = image.toImage();
    //QImage::Format_RGB32
    cv::Mat cvImage(qImage.height(), qImage.width(), CV_8UC4, const_cast<uchar*>(qImage.bits()), qImage.bytesPerLine());
    drawRectangleOnImage(cvImage);
    this->setPixmap(pix_frame);
    pix = pix_frame.copy(0,0,3840,2160);
}

void my_qlabel::drawRectangleOnImage(cv::Mat &image)
{
    qDebug()<<rectangles2;
    for(const QRect& rect : rectangles){
        cv::Rect frame(rect.x(),rect.y(),frame_x,frame_y);
        cv::rectangle(image, frame, cv::Scalar(255,0,0),5);
    }
    pix_frame = mat2pixmap(image);
}

void my_qlabel::updateRectangle2()
{
    rectangles2.clear();
    for(const QRect &rect:rectangles){
        QRectF tmp_rect;
        tmp_rect.setX((rect.x()-zoomX) *0.15 *magnificationFactor + magnificationFactor*zoomtime);
        tmp_rect.setY((rect.y()-zoomY) *0.15 *magnificationFactor + magnificationFactor*zoomtime);
        tmp_rect.setWidth(rect.width() *0.15 *magnificationFactor);
        tmp_rect.setHeight(rect.height()*0.15*magnificationFactor);
        addRectangle2(QRectF(tmp_rect.x(),tmp_rect.y(),tmp_rect.width(),tmp_rect.height()));
    }
    qDebug()<<rectangles2;
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
    // qDebug()<<"Mouse Moving!";
    updateRectangle2();
    this->x = ev->x();
    this->y = ev->y();
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
        del_posX = ev->x() - press_x;
        del_posY = ev->y() - press_y;

        if(magnificationFactor>1){
            qDebug()<<"delposX"<<del_posX<<"del_posY"<<del_posY;
            if(del_posX>zoomX){
                del_posX = zoomX;
            }else if(del_posX>0){
                del_posX = del_posX;
            }else if((zoomX-del_posX+zoomWidth)>3840){
                del_posX = 0;
            }
            if(del_posY>zoomY){
                del_posY = zoomY;
            }else if(del_posY>0){
                del_posY = del_posY;
            }else if((zoomY-del_posY+zoomHeight)>2160){
                del_posY = 0;
            }
            zoompix = pix.copy(zoomX-del_posX, zoomY-del_posY, zoomWidth, zoomHeight);
            this->setPixmap(zoompix);
        }

    }
}

void my_qlabel::mousePressEvent(QMouseEvent *ev)
{
    m_isDragging = true;
    updateMousePosition();
    press_x = ev->pos().x();
    press_y = ev->pos().y();
    // qDebug()<<"mousePressEvent!";
    qDebug()<<"左鍵按下";
//        for(QRectF&rect : rectangles2){
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
    zoomX = zoomX-del_posX;
    zoomY = zoomY-del_posY;

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
    now_posX = x;
    now_posY = y;
    posX = (now_posX - pre_posX)/magnificationFactor+pre_posX;
    posY = (now_posY - pre_posY)/magnificationFactor+pre_posY;
    pre_posX = posX;
    pre_posY = posY;
    if(zoomX<0){
        zoomX = 0;
    }else if(zoomX>3840){
        zoomX = 3840;
    }
    if(zoomY<0){
        zoomY = 0;
    }else if(zoomY>2160){
        zoomY = 2160;
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
        if(zoomtime == 10){
            zoompix = pix.copy(zoomX, zoomY, zoomWidth, zoomHeight);
            this->setPixmap(zoompix);
        }else{
            pic_zoomOut = false;
            qDebug()<<"forward";
            //放大的倍率
            magnificationFactor += zoomFactor;
            qDebug()<<"放大的倍率:"<<magnificationFactor;
            zoomX += (3840*zoomFactor)*((x*(3840/576))/(zoomWidth));
            zoomY += (2160*zoomFactor)*((y*(2160/324))/(zoomHeight));
            zoomWidth -= 3840*zoomFactor;
            zoomHeight -= 2160*zoomFactor;
            // qDebug()<<"放大後的左上角座標:"<<zoomX<<","<<zoomY;
            zoompix = pix.copy(zoomX, zoomY, zoomWidth, zoomHeight);
            this->setPixmap(zoompix);
            zoomtime++;
        }
    }else{
        qDebug()<<"backward";
        // 圖片最小是原圖大小
        if(zoomtime<0){
            qDebug()<<"已縮至原大小";
            zoomX = 0;
            zoomY = 0;
            zoomWidth = 3840;
            zoomHeight = 2160;
            magnificationFactor=1;
            zoompix = pix.copy(zoomX,zoomY,zoomWidth, zoomHeight);
            this->setPixmap(zoompix);
        }else{
            qDebug()<<zoomtime;
            pic_zoomOut = true;
            zoomWidth += 3840* zoomFactor;
            zoomHeight += 2160* zoomFactor;
            zoomX -= (3840*zoomFactor)*(1-(x/576));
            zoomY -= (2160*zoomFactor)*(1-(y/324));
            zoompix = pix.copy(zoomX,zoomY,zoomWidth, zoomHeight);
            this->setPixmap(zoompix);
            magnificationFactor -= zoomFactor;
            zoomtime--;
        }
    }
    updateRectangle2();
}

