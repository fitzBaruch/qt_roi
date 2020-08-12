#include "mainwindow.h"
#include <opencv2/imgproc.hpp>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("GZ");
    //this->resize(800,500);
    setWidget();

    mSocket = new QUdpSocket();
    centerWidget=new QWidget;
    mainLayout=new QHBoxLayout(centerWidget);
    setCentralWidget(centerWidget);

    rightVLayout=new QVBoxLayout;
    rightVLayout->addWidget(ipGroupBox);
    ipGridLayout=new QGridLayout(ipGroupBox);
    ipGridLayout->addWidget(ip1Label,0,0);
    ipGridLayout->addWidget(ip1LineEdit,0,1);
    ipGridLayout->addWidget(ip1PushBtn,0,2);

    ipGridLayout->addWidget(ip2Label,1,0);
    ipGridLayout->addWidget(ip2LineEdit,1,1);
    ipGridLayout->addWidget(ip2PushBtn,1,2);

    rightVLayout->addWidget(warnGroupBox);
    warnGridlayout=new QGridLayout(warnGroupBox);
    warnGridlayout->addWidget(warnIPLabel,0,0);
    warnGridlayout->addWidget(warnIPLineEdit,0,1);
    warnGridlayout->addWidget(warnPortLabel,1,0);
    warnGridlayout->addWidget(warnPortLineEdit,1,1);
    warnGridlayout->addWidget(warnPushBtn,1,2);
    warnGridlayout->addWidget(ftpBtn,2,0,3,1);

    leftVLayout=new QVBoxLayout;
    leftVLayout->addWidget(cam1);
    leftVLayout->addWidget(cam2);

    mainLayout->addLayout(rightVLayout);
    mainLayout->addLayout(leftVLayout);
    connect(ip1PushBtn,SIGNAL(clicked()),this,SLOT(on_Clickedip1PushBtn()));
    connect(&theTimer,&QTimer::timeout,this,&MainWindow::updateImage);
    connect(ip2PushBtn,&QPushButton::clicked,[=](){
        QString rtspAddress1=ip2LineEdit->text();
        if(videoCap1.open(rtspAddress1.toStdString())){
            srcImage1=cv::Mat::zeros(videoCap1.get(CV_CAP_PROP_FRAME_HEIGHT),videoCap1.get(CV_CAP_PROP_FRAME_WIDTH),CV_8UC3);
            theTimer1.start(33);
        }
    });
    connect(&theTimer1,&QTimer::timeout,[=](){
        videoCap1>>srcImage1;
        if(srcImage1.data){
           cv::cvtColor(srcImage1,srcImage1,CV_BGR2RGB);
           this->update();//发送刷新消息
        }
    });
    connect(ftpBtn,SIGNAL(clicked()),this,SLOT(saveFtpPushBtn()));
    connect(warnPushBtn,SIGNAL(clicked()),this,SLOT(onClickedPushBtn()));
}

void MainWindow::setWidget()
{
    ipGroupBox=new QGroupBox;
    ipGroupBox->setTitle(QString::fromLocal8Bit("IP"));
    ipGroupBox->setMaximumSize(240,150);
    ip1Label=new QLabel("IP1:");
    ip1LineEdit=new QLineEdit;
    ip1LineEdit->setMaximumWidth(110);
    ip1PushBtn=new QPushButton(QString::fromLocal8Bit("连接"));

    ip2Label=new QLabel("IP2:");
    ip2LineEdit=new QLineEdit;
    ip2LineEdit->setMaximumWidth(110);
    ip2PushBtn=new QPushButton(QString::fromLocal8Bit("连接"));

    warnGroupBox=new QGroupBox;
    warnGroupBox->setTitle(QString::fromLocal8Bit("报警"));
    warnGroupBox->setMaximumSize(240,150);
    warnIPLabel=new QLabel(QString::fromLocal8Bit("报警IP:"));
    warnIPLineEdit=new QLineEdit;
    warnIPLineEdit->setMaximumWidth(110);
    warnPortLabel=new QLabel(QString::fromLocal8Bit("Port:"));
    warnPortLineEdit=new QLineEdit;
    warnPortLineEdit->setMaximumWidth(30);
    warnPushBtn=new QPushButton(QString::fromLocal8Bit("连接"));
    ftpBtn = new QPushButton(QString::fromLocal8Bit("选择一个文件传入ftp"));
    ftpBtn->setMinimumWidth(200);

    cam1=new LabelPress(this);
    cam1->setMinimumSize(400,300);
    cam2=new LabelPress(this);
    cam2->setMinimumSize(400,300);




}

MainWindow::~MainWindow()
{
}

void MainWindow::on_Clickedip1PushBtn()
{
//    cv::Mat img=cv::imread("/home/fit/safe.png");
//    cv::namedWindow("src");
//    cv::imshow("src",img);
    QString rtspAddress=ip1LineEdit->text();
    if(videoCap.open(rtspAddress.toStdString())){
        srcImage=cv::Mat::zeros(videoCap.get(CV_CAP_PROP_FRAME_HEIGHT),videoCap.get(CV_CAP_PROP_FRAME_WIDTH),CV_8UC3);
        theTimer.start(33);
    }

}
void MainWindow::updateImage()
{
    videoCap>>srcImage;
    if(srcImage.data){
       cv::cvtColor(srcImage,srcImage,CV_BGR2RGB);
       this->update();//发送刷新消息
    }
}

void MainWindow::saveFtpPushBtn()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(QString::fromLocal8Bit("打开图片"));
    fileDialog->setDirectory("/home/fitz/Pictures");
    fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);

    QStringList fileNmaes;
    QStringList file_full;
    QFileInfo fi;
    if(fileDialog->exec()){
        fileNmaes = fileDialog->selectedFiles();
        file_full = fileDialog->getOpenFileNames();
    }
    QString name;
    QString fileNmae;
    QString destDir = "/home/fitz/Music/";
    for(auto tmp:fileNmaes){
        qDebug()<<tmp<<endl;
        fi= QFileInfo(tmp);
        fileNmae = fi.fileName();
        if(QFile::copy(tmp,destDir+fileNmae)){
            QString obs=destDir+fileNmae;
            qDebug()<<tr("success");
            qDebug()<<obs;
        }else {
            qDebug()<<tr("faild to copy");
        }

    }
}

void MainWindow::onClickedPushBtn()
{
    const char *str1 = "This is warning signal";

    QString  ip = warnIPLineEdit->text();
    quint16 port = warnPortLineEdit->text().toUShort();
    qint64 len= mSocket->writeDatagram(str1,QHostAddress(ip),port);
    if(len>0){

    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QImage image1 = QImage((uchar*)(srcImage.data), srcImage.cols, srcImage.rows, QImage::Format_RGB888);
    cam1->setPixmap(QPixmap::fromImage(image1));
    cam1->resize(image1.size());
    cam1->show();
    QImage image2 = QImage((uchar*)(srcImage1.data), srcImage1.cols, srcImage1.rows, QImage::Format_RGB888);
    cam2->setPixmap(QPixmap::fromImage(image2));
    cam2->resize(image2.size());
    cam2->show();
}


