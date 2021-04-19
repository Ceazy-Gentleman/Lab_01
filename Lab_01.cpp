#include "Lab_01.h"
#include <QFileDialog>
#include <QLabel>
#include <QtCore/qmath.h>
#include <QMessageBox>
#include<QPainter>
#include <opencv2/opencv.hpp>


using namespace std;

Lab_01::Lab_01(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    isOpenFile = false;
    //QRect rectangle(x, y, width, height);
    connect(ui.open, &QPushButton::clicked, this, &Lab_01::open_clicked);
    connect(ui.save, &QPushButton::clicked, this, &Lab_01::save_clicked);
    connect(ui.rectangle, &QPushButton::clicked, this, &Lab_01::rectangle_clicked);
    connect(ui.checkout, &QPushButton::clicked, this, & Lab_01::checkout_clicked);
    connect(ui.gray, &QPushButton::clicked, this, &Lab_01::gray_clicked);
    connect(ui.pseudo, &QPushButton::clicked, this, &Lab_01::pseudoColor_clicked);
    //connect(ui.rectangle, &QPushButton::clicked, this, &Lab_01::paintEvent);
    //ui.unIabel->installEventFilter(this);
} 

//打开图片
void Lab_01::open_clicked() {
    imgPath = QFileDialog::getOpenFileName(this, "请选择图片", ".", "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
    if (img.load(imgPath)) {
        isOpenFile = true;
        Mat img1;
        hslImg=srcImg= imread(imgPath.toUtf8().data());
        cvtColor(srcImg, img1, COLOR_BGR2RGB); //将imread得到的BRG Mat转换成RGB Mat
        QImage img2 = QImage((const unsigned char*)(img1.data), img1.cols, img1.rows, QImage::Format_RGB888);//将RGB Mat格式转化为QImage格式
        ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));
    }
}
//绘制矩形框
void Lab_01::rectangle_clicked() {
    QImage img2= ui.unIabel->pixmap()->toImage();
    QPainter painter(&img2);
    QPen pen(Qt::green, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawRect(QRect(50, 50, 100, 100));
    ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));
}//todo 1.利用绘图事件实现鼠标画图功能 2.参数调整按钮

//选取矩形框
void Lab_01::checkout_clicked() {
    QImage img2 = (ui.unIabel->pixmap()->toImage()).copy(QRect(50, 50, 100, 100));
    srcImg = Mat(img2.height(), img2.width(), CV_8UC3, (void*)img2.constBits(), img2.bytesPerLine());//QImage转Mat
    ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));
}
//保存图片
void Lab_01::save_clicked() {
    savePath= QFileDialog::getSaveFileName(this,"保存图片","",tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF"));
    QImage img2 = ui.unIabel->pixmap()->toImage();
    if (savePath.isEmpty())
    {
        return;
    }
    else
    {
        if (!(img2.save(savePath))) //保存图像
        {
            QMessageBox::information(this,
                tr("Failed to save the image"),
                tr("Failed to save the image!"));
            return;
        }
        ui.statusBar->showMessage("Successfully");
        
    }
}


//灰度化
void Lab_01::gray_clicked() {
    QImage img2 = (ui.unIabel->pixmap()->toImage()).convertToFormat(QImage::Format_Grayscale8, Qt::AutoColor);
    //for (int i = 50; i < 150; i++)
    //{
    //    for (int j = 50; j < 150; j++)
    //    {
    //        QRgb pixel = img2.pixel(i, j);
    //        int r = qRed(pixel);
    //        int g = qGreen(pixel);
    //        int b = qBlue(pixel);
    //        //img2.setPixel(i, j, qGray(r, g, b));
    //        int average = (r * 0.299 + g * 0.587 + b * 0.114);//效果更好，精度问题，这样算的精度更高
    //        img2.setPixel(i,j,qRgb(average, average, average));
    //    }
    //}
    //cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);
    //Mat temp;
    //cvtColor(grayImg, temp, COLOR_GRAY2RGB); //将imread得到的BRG Mat转换成RGB Mat
    //QImage img2 = QImage((const unsigned char*)(temp.data), temp.cols, temp.rows, QImage::Format_RGB888);//将RGB Mat格式转化为QImage格式
    ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));

}//todo 整数精度教程 ,Mat 灰度图转QImage 灰度图失败也不知到什么原因


//伪彩色图
void Lab_01::pseudoColor_clicked() {
    //Mat img1;
    cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);
    applyColorMap(grayImg, pseImg, COLORMAP_HSV);
    QImage img2 = QImage((const unsigned char*)(pseImg.data), pseImg.cols, pseImg.rows, pseImg.step, QImage::Format_RGB888);//将RGB Mat格式转化为QImage格式
    //cvtColor(pseImg, img1, COLOR_GRAY2RGB); //Mat转换成RGB Mat
    //QImage img2 = QImage((const unsigned char*)(img1.data), img1.cols, img1.rows,img1.step, QImage::Format_RGB888);//将RGB Mat格式转化为QImage格式
    ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));
}

//对数变换
void Lab_01::log_Changed(int log_r)
{
      grayImg.convertTo(logImg, CV_32F, 1.0 / 255, 0);
      logImg = log_r * logImg + 1;
      log(logImg, logImg);
      logImg = logImg / log(log_r + 1);
      logImg.convertTo(logImg, CV_8U, 255, 0);
      cvtColor(logImg, logImg, COLOR_BGR2RGB);
      QImage img2((const unsigned char*)logImg.data, logImg.cols, logImg.rows, logImg.step, QImage::Format_RGB888);
      ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));
}
//绘制矩形框
//void Lab_01::paintEvent(QPaintEvent*) {
//    if (imgPath.isEmpty())
//    {
//        return;
//    }
//    else {
//        //实例化画家对象
//        QPainter painter(ui.unIabel);
//        QPen pen(QColor(255, 0, 0));
//        painter.setPen(pen);
//        pen.setWidth(2);
//        painter.drawRect(QRect(20, 20, 50, 50));
//    }
//
//
//}

//QImage Lab_01::Center(QImage  qimage, QLabel* qLabel)
//{
//    QImage image;
//    QSize imageSize = qimage.size();
//    QSize labelSize = qLabel->size();
//
//    double dWidthRatio = 1.0 * imageSize.width() / labelSize.width();
//    double dHeightRatio = 1.0 * imageSize.height() / labelSize.height();
//    if (dWidthRatio > dHeightRatio) {
//        image = qimage.scaledToWidth(labelSize.width());
//    }
//    else {
//        image = qimage.scaledToHeight(labelSize.height());
//    }
//    return image;
//}
