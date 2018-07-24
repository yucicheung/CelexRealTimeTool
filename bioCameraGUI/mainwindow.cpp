#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "celexsensordll.h"
#include<QTimer>
#include<QDebug>
#include<QLabel>
#include<QComboBox>
#include<QPushButton>
#include<QDateTime>

const int FRAME_COLS = 768;
const int FRAME_ROWS = 640;
const int FRAME_TIME = 40;
const int THRESHOLD = 60;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_imageBinaryPic(FRAME_COLS,FRAME_ROWS,QImage::Format_RGB32),
    m_imageGrayPic(FRAME_COLS,FRAME_ROWS,QImage::Format_RGB32),
    m_imageAccumulatedPic(FRAME_COLS,FRAME_ROWS,QImage::Format_RGB32),
    m_imageDenoisedGrayPic(FRAME_COLS,FRAME_ROWS,QImage::Format_RGB32),
    m_imageDenoisedBinaryPic(FRAME_COLS,FRAME_ROWS,QImage::Format_RGB32),
    bPressed(false)
{
    ui->setupUi(this);
    resize(800,720);

    m_pstorebutton= new QPushButton("开始录制",this);
    m_pstorebutton->setGeometry(250, 10, 100, 30);
    m_pstorebutton->resize(100,30);
    m_pstorebutton->setStyleSheet("QPushButton {background-color: #EEEEEE; background-image: url(:/images/player_pause.png); "
                                   "border-style: outset; border-width: 2px; border-radius: 2px; border-color: #FFFFFF;} "
                                   "QPushButton:pressed {background: #992F6F; background-image: url(:/images/player_pause.png); }");
    connect(m_pstorebutton,SIGNAL(pressed()),this,SLOT(onPressed()));


    m_pimageEventPic = &m_imageBinaryPic;
    m_pComboBox = new QComboBox(this);
    m_pComboBox->setGeometry(20,10,200,30);
    setComboBox();
    connect(m_pComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onEventPicModeChanged(QString)));

    m_pCelexSensor = new CelexSensorDLL;

    m_pCelexSensor->openSensor("");

    m_pCelexSensor->setSensorMode(EventMode);
    m_pCelexSensor->setThreshold(THRESHOLD);
    m_pCelexSensor->setEventFrameTime(FRAME_TIME);

    m_pPipeOutDataTimer = new QTimer(this);
    m_pPipeOutDataTimer->setSingleShot(true);
    connect(m_pPipeOutDataTimer,SIGNAL(timeout()),this,SLOT(onPipeoutDataTimer()));
    m_pPipeOutDataTimer->start(10);

    m_pDisplayTimer = new QTimer(this);
    connect(m_pDisplayTimer,SIGNAL(timeout()),this,SLOT(onDisplayTimer()));
    m_pDisplayTimer->start(FRAME_TIME);

    m_pLabelPic = new QLabel(this);
    m_pLabelPic->show();
    m_pLabelPic->setGeometry(20,60,FRAME_COLS,FRAME_ROWS);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPipeoutDataTimer()
{
    if(!m_pCelexSensor->isSdramFull())
    {
        m_pCelexSensor->pipeOutFPGAData();
    }
    m_pPipeOutDataTimer->start(10);
}

void MainWindow::onDisplayTimer()
{
    pBinaryPicBuffer = m_pCelexSensor->getEventPicBuffer(EventOnlyPic);
    pGrayPicBuffer = m_pCelexSensor->getEventPicBuffer(EventGrayPic);
    pAccumulatedPicBuffer = m_pCelexSensor->getEventPicBuffer(EventAccumulationPic);
    pDenoisedGrayPicBuffer = m_pCelexSensor->getEventPicBuffer(EventDenoisedGrayPic);
    pDenoisedBinaryPicBuffer = m_pCelexSensor->getEventPicBuffer(EventDenoisedBinaryPic);

    int index = 0;
    for(int row = 0; row<FRAME_ROWS;++row)
    {
        for(int col=0;col<FRAME_COLS;++col)
        {
            m_imageBinaryPic.setPixel(col,row,qRgb(pBinaryPicBuffer[index],pBinaryPicBuffer[index],pBinaryPicBuffer[index]));
            m_imageGrayPic.setPixel(col,row,qRgb(pGrayPicBuffer[index],pGrayPicBuffer[index],pGrayPicBuffer[index]));
            m_imageAccumulatedPic.setPixel(col,row,qRgb(pAccumulatedPicBuffer[index],pAccumulatedPicBuffer[index],pAccumulatedPicBuffer[index]));
            m_imageDenoisedBinaryPic.setPixel(col,row,qRgb(pDenoisedBinaryPicBuffer[index],pDenoisedBinaryPicBuffer[index],pDenoisedBinaryPicBuffer[index]));
            m_imageDenoisedGrayPic.setPixel(col,row,qRgb(pDenoisedGrayPicBuffer[index],pDenoisedGrayPicBuffer[index],pDenoisedGrayPicBuffer[index]));
            ++index;
        }
        m_pLabelPic->setPixmap(QPixmap::fromImage(*m_pimageEventPic));
        update();
    }
}

void MainWindow::setComboBox()
{
    m_pComboBox->insertItem(0, "BinaryPic");
    m_pComboBox->insertItem(1, "AccumulationPic");
    m_pComboBox->insertItem(2, "GrayPic");
    m_pComboBox->insertItem(3, "DenoisedBinarypic");
    m_pComboBox->insertItem(4, "DenoisedGraypic");
    connect(m_pComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onEventPicModeChanged(QString)));
    m_pComboBox->setCurrentText("BinaryPic");
    QString style1 = "QComboBox {font: 15px Calibri; color: white; border: 1px solid gray; "
                     "border-radius: 5px; background: lightblue;}";
    QString style2 = "QComboBox:editable {background: white;}";
    m_pComboBox->setStyleSheet(style1 + style2);
    update();

}

void MainWindow::onEventPicModeChanged(QString mode)
{
    if(mode=="BinaryPic"){
        m_pimageEventPic=&m_imageBinaryPic;
    }else if(mode=="AccumulationPic"){
        m_pimageEventPic=&m_imageAccumulatedPic;
    }else if(mode=="GrayPic"){
        m_pimageEventPic=&m_imageGrayPic;
    }else if(mode=="DenoisedBinarypic"){
        m_pimageEventPic=&m_imageDenoisedGrayPic;
    }else if(mode=="DenoisedGraypic"){
        m_pimageEventPic=&m_imageDenoisedGrayPic;
    }
    update();

}

void MainWindow::onPressed()
{
    if(bPressed){
        m_pstorebutton->setText("停止录制");
        const QDateTime now = QDateTime::currentDateTime();
        const QString timestamp = now.toString(QLatin1String("yyyyMMdd_hhmmsszzz"));
        std::string filepath  = (QCoreApplication::applicationDirPath()+"/Recording_"+timestamp+".bin").toStdString();
        m_pCelexSensor->startRecording(filepath);}
    else{
        m_pstorebutton->setText("开始录制");
        m_pCelexSensor->stopRecording();
    }
    bPressed=!bPressed;
    update();
}

