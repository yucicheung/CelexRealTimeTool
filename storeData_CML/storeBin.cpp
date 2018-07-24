#include"storeBin.h"


storeBin::storeBin(){
    pCelexSensor =new CelexSensorDLL;
}
void storeBin::openDVS()
{
    pCelexSensor->openSensor("");
    if(!pCelexSensor->isSensorReady()){
        std::cerr<<"Sensor not ready!"<<std::endl;
        exit(1);
    }
    std::cout<<"Sensor is ready."<<std::endl;
    return;
}

void storeBin::beginRecording()
{
     const QDateTime now = QDateTime::currentDateTime();
     const QString timestamp = now.toString(QLatin1String("yyyyMMdd_hhmmsszzz"));
     std::string filepath  = (QCoreApplication::applicationDirPath()+"/Recording_"+timestamp+".bin").toStdString();
     pCelexSensor->startRecording(filepath);
     std::cout<<"Recording started successfully!"<<std::endl;
     std::cout<<"Saving bin file to "<<filepath<<std::endl;
     return;
}

void storeBin::stopRecording()
{
    pCelexSensor->stopRecording();
    std::cout<<"Recording stopped successfully!"<<std::endl;
    return;
}

