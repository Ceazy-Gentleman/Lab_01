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

private slots:
    void open_clicked();
    void save_clicked();
    void rectangle_clicked();
    void checkout_clicked();
    void gray_clicked();
    void pseudoColor_clicked();
    void log_Changed(int);

private:
    Ui::Lab_01Class ui;

    QString imgPath;
    QString savePath;
    bool isOpenFile;
    QImage img;
    Mat srcImg, grayImg, pseImg, logImg, hslImg;
};
