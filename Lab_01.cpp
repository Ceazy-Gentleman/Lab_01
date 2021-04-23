#pragma execution_character_set("utf-8")
#include "Lab_01.h"
#include <QFileDialog>
#include <QLabel>
#include <QtCore/qmath.h>
#include <QMessageBox>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include<QPainter>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Lab_01::Lab_01(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //QRect rectangle(x, y, width, height);
    connect(ui.open, &QPushButton::clicked, this, &Lab_01::open_clicked);
    connect(ui.save, &QPushButton::clicked, this, &Lab_01::save_clicked);
    connect(ui.rectangle, &QPushButton::clicked, this, &Lab_01::rectangle_clicked);
    connect(ui.checkout, &QPushButton::clicked, this, & Lab_01::checkout_clicked);
    connect(ui.gray, &QPushButton::clicked, this, &Lab_01::gray_clicked);
    connect(ui.pseudo, &QPushButton::clicked, this, &Lab_01::pseudoColor_clicked);
    //connect(ui.rectangle, &QPushButton::clicked, this, &Lab_01::paintEvent);
    //ui.unIabel->installEventFilter(this);
    connect(ui.radioButton, &QRadioButton::clicked, [=]() {
        gray = true;
        histogram_Balanced();
        });
    connect(ui.radioButton_2, &QRadioButton::clicked, [=]() {
        gray = false;
        histogram_Balanced();
        });
    connect(ui.end_log, &QPushButton::clicked, [=]() {
        records.push_back(*(ui.unIabel->pixmap()));
        });
    connect(ui.end_gamma, &QPushButton::clicked, [=]() {
        records.push_back(*(ui.unIabel->pixmap()));
        });
    connect(ui.end_hsl, &QPushButton::clicked, [=]() {
        records.push_back(*(ui.unIabel->pixmap()));
        });
} 


//打开图片
void Lab_01::open_clicked() {
    imgPath = QFileDialog::getOpenFileName(this, "请选择图片", ".", "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
    double time0 = static_cast<double>(getTickCount());

    if (img.load(imgPath)) {
        Mat img1;
        srcImg= imread(imgPath.toUtf8().data());
        cvtColor(srcImg, img1, COLOR_BGR2RGB); //将imread得到的BRG Mat转换成RGB Mat
        QImage img2 = QImage((const unsigned char*)(img1.data), img1.cols, img1.rows, QImage::Format_RGB888);//将RGB Mat格式转化为QImage格式
        ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));
        img = (ui.unIabel->pixmap()->toImage()).copy();

        time0 = 1000 * ((double)getTickCount() - time0) / getTickFrequency();
        qDebug("——————————————————————————————————————————————————————————————————————");
        qDebug() << "成功打开图片" << "  耗时：" << time0 << "ms";
        ui.statusBar->showMessage("加载图片, 耗时：" + QString("%1").arg(time0) + "ms");

        //撤销记录
        QPixmap a = (*(ui.unIabel->pixmap())).copy();
        records.push_back(a);
    }
}
//绘制矩形框
void Lab_01::rectangle_clicked() {
    double time0 = static_cast<double>(getTickCount());

    //读取图片及参数
    QImage img2;
    img2 = img;
    x = ui.X->value();
    y = ui.Y->value();
    width = ui.Width->value();
    height = ui.Height->value();

    //设置绘图设备绘图
    QPainter painter(&img2);
    QPen pen(Qt::green, 2, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawRect(QRect(x, y, width, height));

    //显示图片
    ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));

    time0 = 1000*((double)getTickCount() - time0) / getTickFrequency();
    qDebug()<<"成功绘制矩形框"<<"  耗时："<<time0<<"ms";
    ui.statusBar->showMessage("绘制ROI, 耗时：" + QString("%1").arg(time0) + "ms");

}//todo 1.利用绘图事件实现鼠标画图功能

//选取矩形框
void Lab_01::checkout_clicked() {
    double time0 = static_cast<double>(getTickCount());

    img2 = (ui.unIabel->pixmap()->toImage()).copy(QRect(x, y, width, height));
    srcImg = Mat(img2.height(), img2.width(), CV_8UC4, (void*)img2.constBits(), img2.bytesPerLine()).clone();//QImage转Mat,图像数据共享内存了。。。
    imshow("显示图像", srcImg);
    //imshow("显示图像", srcImg);
    ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));

    time0 = 1000 * ((double)getTickCount() - time0) / getTickFrequency();
    qDebug() << "成功选取选定区域" << "  耗时：" << time0 << "ms";
    ui.statusBar->showMessage("选取ROI, 耗时：" + QString("%1").arg(time0) + "ms");

    //撤销记录
    QPixmap a = (*(ui.unIabel->pixmap())).copy();
    records.push_back(a);

}
//保存图片
void Lab_01::save_clicked() {
    savePath= QFileDialog::getSaveFileName(this,"保存图片","",tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF"));
    double time0 = static_cast<double>(getTickCount());

    img2 = ui.unIabel->pixmap()->toImage();
    if (savePath.isEmpty())
    {
        return;
    }
    else
    {
        if (!(img2.save(savePath))) //保存图像
        {
            QMessageBox::information(this,"error",tr("Failed to save the image!"));
            return;
        }

        time0 = 1000 * ((double)getTickCount() - time0) / getTickFrequency();
        ui.statusBar->showMessage("保存图片, 耗时："+ QString("%1").arg(time0)+"ms");
        qDebug() << "成功保存图片" << "  耗时：" << time0 << "ms";

    }
}


//灰度化
void Lab_01::gray_clicked() {
    double time0 = static_cast<double>(getTickCount());

    cvtColor(srcImg, grayImg, CV_BGR2GRAY);
    imshow("显示图像", srcImg);
    img2 = QImage((const unsigned char*)(grayImg.data), grayImg.cols, grayImg.rows, grayImg.step, QImage::Format_Grayscale8);
    if (img2.isNull()) {
        QMessageBox::warning(this, "提示", "图片转换错误", QMessageBox::Yes | QMessageBox::Yes);
    }
    ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));

    time0 = 1000 * ((double)getTickCount() - time0) / getTickFrequency();
    qDebug() << "灰度处理" << "  耗时：" << time0 << "ms";
    ui.statusBar->showMessage("灰度变换, 耗时：" + QString("%1").arg(time0) + "ms");


    QPixmap a = (*(ui.unIabel->pixmap())).copy();
    records.push_back(a);
}


//伪彩色图
void Lab_01::pseudoColor_clicked() {
    double time0 = static_cast<double>(getTickCount());

    applyColorMap(grayImg, pseImg, COLORMAP_JET);
    img2 = QImage((const unsigned char*)(pseImg.data), pseImg.cols, pseImg.rows, pseImg.step, QImage::Format_RGB888);//将RGB Mat格式转化为QImage格式
    ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));

    time0 = 1000 * ((double)getTickCount() - time0) / getTickFrequency();
    ui.statusBar->showMessage("伪彩色图变换, 耗时：" + QString("%1").arg(time0) + "ms");
    qDebug() << "生成伪彩色图" << "  耗时：" << time0 << "ms";

    QPixmap a = (*(ui.unIabel->pixmap())).copy();
    records.push_back(a);

}

//对数变换
void Lab_01::log_Changed(int c)
{
    double time0 = static_cast<double>(getTickCount());

    //预处理
    imshow("对数图片", grayImg);
    Mat grayImg1 = grayImg.clone();
    logImg = Mat::zeros(grayImg1.size(), grayImg1.type());

    //计算
    add(grayImg1, Scalar(1.0), grayImg1);//计算r+1
    grayImg1.convertTo(grayImg1, CV_32F);//转为单精度浮点数
    log(grayImg1, logImg);//计算log（r+1)
    logImg = c * logImg;
    normalize(logImg, logImg, 0, 255, NORM_MINMAX);//归一化处理

    //图像转换
    logImg.convertTo(logImg, CV_8UC1);
    img2 = QImage((const unsigned char*)logImg.data, logImg.cols, logImg.rows, logImg.step, QImage::Format_Grayscale8);
    ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));

    time0 = 1000 * ((double)getTickCount() - time0) / getTickFrequency();
    ui.statusBar->showMessage("对数变换, 耗时：" + QString("%1").arg(time0) + "ms");
    qDebug() << "对数变换" << "  耗时：" << time0 << "ms";
}

//伽马变换

void Lab_01::gamma_Changed(double r)
{
    double time0 = static_cast<double>(getTickCount());

    //预处理
    Mat grayImg1 = grayImg.clone();
    imshow("对数图片", grayImg1);
    gammaImg = Mat::zeros(grayImg1.size(), grayImg1.type());
    grayImg1.convertTo(grayImg1, CV_64FC1);//转为双精度浮点数

    //计算
    cv::pow(grayImg1, r, gammaImg);

    //图像转换
    gammaImg.convertTo(gammaImg, CV_8UC1);
    img2 = QImage((const unsigned char*)gammaImg.data, gammaImg.cols, gammaImg.rows, gammaImg.step, QImage::Format_Grayscale8);
    ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));

    time0 = 1000 * ((double)getTickCount() - time0) / getTickFrequency();
    ui.statusBar->showMessage("伽马变换, 耗时：" + QString("%1").arg(time0) + "ms");
    qDebug() << "伽马变换" << "  耗时：" << time0 << "ms";

}


//hsl调整
void Lab_01::hsl_Changed(int value) 
{
    Q_UNUSED(value);
    double time0 = static_cast<double>(getTickCount());


    //预处理
    Mat srcImg1,final;
    vector<Mat> channels(3);
    cvtColor(srcImg, srcImg1, CV_BGRA2BGR);
    float delta_h = ui.HspinBox->value();
    float delta_s = ui.SspinBox_2->value();
    float delta_l = ui.LspinBox_3->value();
    //imshow("原图", srcImg1);

    //图像转化处理
    srcImg1.convertTo(srcImg1, CV_32F, 1.0 / 255, 0);
    cvtColor(srcImg1, hslImg, COLOR_BGR2HLS);
    split(hslImg, channels);

    //单层计算
    add(channels.at(0), Scalar(delta_h), channels.at(0));
    add(channels.at(1), Scalar(delta_l/100.0), channels.at(1));
    add(channels.at(2), Scalar(delta_s/100.0), channels.at(2));

    //合成显示
    merge(channels, hslImg);
    //imshow("变换后hsl图", hslImg);
    cvtColor(hslImg, hslImg, COLOR_HLS2RGB);
    hslImg.convertTo(hslImg, CV_8U,255,0);
    img2 = QImage((const unsigned char*)hslImg.data, hslImg.cols, hslImg.rows, hslImg.step, QImage::Format_RGB888);
    ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));

    time0 = 1000 * ((double)getTickCount() - time0) / getTickFrequency();
    ui.statusBar->showMessage("hsl变换, 耗时：" + QString("%1").arg(time0) + "ms");
    qDebug() << "hsl调整" << "  耗时：" << time0 << "ms";

}

//直方图均衡
void Lab_01::histogram_Balanced() {

    Mat equalImg;   
    vector<Mat> channels;
    if (gray) {
        double time0 = static_cast<double>(getTickCount());

        imshow("灰度图", grayImg);
        equalizeHist(grayImg, equalImg);
        img2 = QImage((const unsigned char*)(grayImg.data), grayImg.cols, grayImg.rows, grayImg.step, QImage::Format_Grayscale8);
        ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));

        time0 = 1000 * ((double)getTickCount() - time0) / getTickFrequency();
        ui.statusBar->showMessage("灰度直方图均衡, 耗时：" + QString("%1").arg(time0) + "ms");
        qDebug() << "灰度直方图均衡" << "  耗时：" << time0 << "ms";

        records.push_back(*(ui.unIabel->pixmap()));
    }
    else {
        double time0 = static_cast<double>(getTickCount());

        //图像转换
        cvtColor(srcImg, equalImg, CV_BGRA2BGR);
        imshow("k", equalImg);
        cvtColor(equalImg, equalImg, CV_BGR2YCrCb);

        //均衡处理
        split(equalImg, channels);
        equalizeHist(channels[0], channels[0]);
        merge(channels, equalImg);

        //图像显示
        cvtColor(equalImg, equalImg, CV_YCrCb2RGB);
        img2 = QImage((const unsigned char*)(equalImg.data), equalImg.cols, equalImg.rows, equalImg.step, QImage::Format_RGB888);
        ui.unIabel->setPixmap(QPixmap::fromImage(img2.scaled(ui.unIabel->size(), Qt::KeepAspectRatio)));

        time0 = 1000 * ((double)getTickCount() - time0) / getTickFrequency();
        ui.statusBar->showMessage("彩色直方图均衡, 耗时：" + QString("%1").arg(time0) + "ms");
        qDebug() << "彩色直方图均衡" << "  耗时：" << time0 << "ms";

        records.push_back(*(ui.unIabel->pixmap()));
    }
}

void Lab_01::withdraw_clicked() {
    double time0 = static_cast<double>(getTickCount());
    //--rd;?为什么啊，为什么这样使用就有问题啊
    records.pop_back();
    rd = records.end();
    --rd;
    ui.unIabel->setPixmap(*rd);

    time0 = 1000 * ((double)getTickCount() - time0) / getTickFrequency();
    ui.statusBar->showMessage("撤销操作, 耗时：" + QString("%1").arg(time0) + "ms");
    qDebug() << "撤销操作" << "  耗时：" << time0 << "ms";
}


