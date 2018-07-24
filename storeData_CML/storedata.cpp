#include"storedata.h"

const int THRESHOLD = 50;
const int FRAMETIME = 30;
const int PIPEOUTTIME = 10;

StoreData::StoreData():
    QObject(nullptr)
{
    m_pCelexSensor = new CelexSensorDLL;

    m_pCelexSensor->openSensor("");
    if (!m_pCelexSensor->isSensorReady())
    {
        std::cerr<<"Sensor not ready"<<std::endl;
        exit(1);
    }
    else{
        std::cout<<"Sensor is ready"<<std::endl;
    }
    // set Sensor mode
    m_pCelexSensor->setSensorMode(EventMode);
    m_pCelexSensor->setThreshold(THRESHOLD);
    m_pCelexSensor->setEventFrameTime(FRAMETIME);

    m_pPipeOutDataTimer = new QTimer(this);
    m_pPipeOutDataTimer->setSingleShot(true);
    connect(m_pPipeOutDataTimer,SIGNAL(timeout()),this,SLOT(onPipeoutDataTimer()));
    m_pPipeOutDataTimer->start(PIPEOUTTIME);
}

StoreData::~StoreData()
{

}

void StoreData::onPipeOutTimer()
{
    if(!m_pCelexSensor->isSdramFull())
    {
        m_pCelexSensor->pipeOutFPGAData();
    }
    m_pPipeOutDataTimer->start(10);
}

void StoreData::startRecording()
{
    const QDateTime now = QDateTime::currentDateTime();
    const QString timestamp = now.toString(QLatin1String("yyyyMMdd_hhmmsszzz"));
    std::string filepath  = (QCoreApplication::applicationDirPath()+"/Recording_"+timestamp+".bin").toStdString();
    m_pCelexSensor->startRecording(filepath);
    std::cout<<"Recording started successfully!"<<std::endl;
    std::cout<<"Saving bin file to "<<filepath<<std::endl;
}

void StoreData::stopRecording()
{
    m_pCelexSensor->stopRecording();
    std::cout<<"Recording stopped successfully!"<<std::endl;
}
