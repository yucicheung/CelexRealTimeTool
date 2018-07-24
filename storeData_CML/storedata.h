#include<QTimer>
#include<QDateTime>
#include<iostream>
#include<QCoreApplication>
#include"celexsensordll.h"

#ifndef STOREDATA
#define STOREDATA

#include<QObject>

class CelexSensorDLL;
class StoreData:public QObject
{
    Q_OBJECT
public:
    StoreData();
    ~StoreData();
    void startRecording();
    void stopRecording();
private:
    QTimer*             m_pPipeOutDataTimer;
    CelexSensorDLL*     m_pCelexSensor;
private slots:
    void                onPipeOutTimer();
};

#endif // STOREDATA
