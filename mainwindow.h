#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QWidget>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QFileDialog>
#include <QUdpSocket>
#include "LabelPress.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setWidget();
    ~MainWindow();
private slots:
    void on_Clickedip1PushBtn();
    void updateImage();
    void saveFtpPushBtn();
    void onClickedPushBtn();

private:

    LabelPress *cam1;
    LabelPress *cam2;
    QWidget *centerWidget;
    QLabel *ip1Label;
    QLineEdit *ip1LineEdit;
    QPushButton *ip1PushBtn;
    QLabel *ip2Label;
    QLineEdit *ip2LineEdit;
    QPushButton *ip2PushBtn;

    QLabel *warnIPLabel;
    QLineEdit *warnIPLineEdit;
    QLabel *warnPortLabel;
    QLineEdit *warnPortLineEdit;
    QPushButton *warnPushBtn;
    QLabel *ftplabel;
    QPushButton *ftpBtn;

    QHBoxLayout *mainLayout;
    QVBoxLayout *rightVLayout;
    QVBoxLayout *leftVLayout;
    QGridLayout *ipGridLayout;
    QGridLayout *warnGridlayout;

    QGroupBox *ipGroupBox;
    QGroupBox *warnGroupBox;

    QTimer theTimer;
    cv::Mat srcImage;
    cv::VideoCapture videoCap;

    QTimer theTimer1;
    cv::Mat srcImage1;
    cv::VideoCapture videoCap1;

    QUdpSocket *mSocket;
protected:
    void paintEvent(QPaintEvent *e);

};
#endif // MAINWINDOW_H
