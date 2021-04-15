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
    //�ۺ���
    void open_clicked();
    void save_clicked();
    void rectangle_clicked();
    void gray_clicked();
    
    //��ͼ�¼�
    //void paintEvent(QPaintEvent *);
    //bool eventFilter(QObject* obj, QEvent* event);//�¼�������

private:
    Ui::Lab_01 ui;

    QString imgPath;
    QString savePath;
    bool isOpenFile;
    QImage img;
    Mat srcImg,hslImg;
};
