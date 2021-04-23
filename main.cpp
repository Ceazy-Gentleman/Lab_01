#include "Lab_01.h"
#include <QtWidgets/QApplication>

//生成日志
void outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString text;
    switch (type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }
    //QDateTime current_date_time = QDateTime::currentDateTime();
    //QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
    //QString message = text.append(msg).append("(").append(current_date).append(")");
    QString message = text.append(msg);

    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(outputMessage);

    // 清空之前已有的日志文件
    QFile file("log.txt");
    if (file.exists()) {
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    Lab_01 w;
    w.show();
    return a.exec();
}
