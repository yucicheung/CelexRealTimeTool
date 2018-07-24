#ifndef STOREBIN
#define STOREBIN


//class CelexSensorDLL;

class storeBin{
public:
    storeBin();
    void openDVS();
    void beginRecording();
    void stopRecording();
private:
    CelexSensorDLL* pCelexSensor;
};

#endif // STOREBIN
