#pragma once
#include <QtWidgets/QMainWindow>
#include"ui_Lab_01.h"
#include <opencv2/opencv.hpp>
using namespace cv;

class Lab_01 : public QMainWindow
{
    Q_OBJECT

public:
    Lab_01(QWidget *parent = Q_NULLPTR);
    void outputMessage(QtMsgType, const QMessageLogContext&, const QString&);


private slots:
    void open_clicked();
    void save_clicked();
    void rectangle_clicked();
    void checkout_clicked();
    void gray_clicked();
    void pseudoColor_clicked();
    void log_Changed(int);
    void gamma_Changed(double);
    void hsl_Changed(int);
    void histogram_Balanced();
private:
    Ui::Lab_01Class ui;

    bool gray ;
    QString imgPath;
    QString savePath;
    int x, y, width, height;
    QImage img,img2;
    Mat srcImg, grayImg, pseImg, logImg, gammaImg, hslImg;

};
