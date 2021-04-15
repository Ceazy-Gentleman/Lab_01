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
    connect(ui.open, &QPushButton::clicked, this, &Lab_01::open_clicked);
    connect(ui.save, &QPushButton::clicked, this, &Lab_01::save_clicked);
    //connect(ui.rectangle, &QPushButton::clicked, this, &Lab_01::paintEvent);
    //ui.unIabel->installEventFilter(this);
    
} 

//��ͼƬ
void Lab_01::open_clicked() {
    imgPath = QFileDialog::getOpenFileName(this, "��ѡ��ͼƬ", ".", "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
    if (img.load(imgPath)) {
        isOpenFile = true;
        Mat img1;
        hslImg=srcImg= imread(imgPath.toUtf8().data());
        cvtColor(srcImg, img1, COLOR_BGR2RGB); //��imread�õ���BRG Matת����RGB Mat
        QImage img2 = QImage((const unsigned char*)(img1.data), img1.cols, img1.rows, QImage::Format_RGB888);//��RGB Mat��ʽת��ΪQImage��ʽ
        ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));
    }
}

//����ͼƬ
void Lab_01::save_clicked() {
    savePath= QFileDialog::getSaveFileName(this,"����ͼƬ","",tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF"));
    QImage img2 = ui.unIabel->pixmap()->toImage();
    if (savePath.isEmpty())
    {
        return;
    }
    else
    {
        if (!(img2.save(savePath))) //����ͼ��
        {
            QMessageBox::information(this,
                tr("Failed to save the image"),
                tr("Failed to save the image!"));
            return;
        }
        ui.statusBar->showMessage("Successfully");
    }
}


//���ƾ��ο�
//void Lab_01::paintEvent(QPaintEvent*) {
//    if (imgPath.isEmpty())
//    {
//        return;
//    }
//    else {
//        //ʵ�������Ҷ���
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
