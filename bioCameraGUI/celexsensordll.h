#ifndef CELEXSENSORDLL_H
#define CELEXSENSORDLL_H

#include <stdint.h>
#include <vector>
#include <map>
#include <list>
#include <fstream>

#ifdef _WIN32
#ifdef CELEXSENSORDLL_EXPORTS
#define CELEXSENSORDLL_API __declspec(dllexport)
#else
#define CELEXSENSORDLL_API __declspec(dllimport)
#endif
#else
#if defined(CELEXSENSOR_LIBRARY)
#define CELEXSENSORDLL_API
#else
#define CELEXSENSORDLL_API
#endif
#endif

enum emSensorMode {
    FullPictureMode = 0,
    EventMode,
    FullPic_Event_Mode
};

enum emEventPicMode {
    EventOnlyPic = 0,
    EventAccumulationPic = 1,
    EventGrayPic,
    EventSuperimposedPic,
    EventDenoisedBinaryPic,
    EventDenoisedGrayPic,
};

class Subject;
class Observer
{
public:
    enum emDataType {
        emSpecialEvent = 0,
        emPixelData = 1,
        emPicFrameBufferData = 2
    };
    virtual ~Observer() {}
    virtual void updateData(emDataType type) = 0;

protected:
    Observer() {}
};

class Subject
{
public:
    virtual void registerData(Observer* observer, Observer::emDataType type) = 0;
    virtual void unregisterData(Observer* observer, Observer::emDataType type) = 0;
    virtual void notify(Observer::emDataType dataType) = 0;
};

class SensorData : public Subject
{
public:
    SensorData()
        : m_pObserver(NULL)
        , m_pFullPicBuffer(NULL)
        , m_pEventPicBuffer(NULL)
        , m_pEventPicBufferGray(NULL)
        , m_pEventPicBufferSuperimposed(NULL)
        , m_pEventPicBufferDenoisedBinary(NULL)
        , m_pEventPicBufferDenoisedGray(NULL)
        , m_pEventPicOpticalFlow(NULL)
        , m_pEventPicOpticalFlowDirection(NULL)
        , m_pEventPicOpticalFlowSpeed(NULL)
        , m_nRow(0)
        , m_nColumn(0)
        , m_nBrightness(0)
        , m_nTime(0)
    {

    }

    void registerData(Observer* observer, Observer::emDataType type);
    void unregisterData(Observer* observer, Observer::emDataType type);
    void notify(Observer::emDataType dataType);

    inline void setPixelData(int x, int y, int brightness, int time)
    {
        m_nRow = y;
        m_nColumn = x;
        m_nBrightness = brightness;
        m_nTime = time;
    }
    inline void setFullPicBuffer(unsigned char* buffer) { m_pFullPicBuffer = buffer; }
    inline void setEventPicBuffer(unsigned char* buffer) { m_pEventPicBuffer = buffer; }
    inline void setEventPicBufferGray(unsigned char* buffer) { m_pEventPicBufferGray = buffer; }
    inline void setEventPicBufferSuperimposed(unsigned char* buffer) { m_pEventPicBufferSuperimposed = buffer; }
    inline void setEventPicBufferDenoisedBinary(unsigned char* buffer) { m_pEventPicBufferDenoisedBinary = buffer; }
    inline void setEventPicBufferDenoisedGray(unsigned char* buffer) { m_pEventPicBufferDenoisedGray = buffer; }
    inline void setEventPicOpticalFlow(unsigned char* buffer) { m_pEventPicOpticalFlow = buffer; }
    inline void setEventPicOpticalFlowDirection(unsigned char* buffer) { m_pEventPicOpticalFlowDirection = buffer; }
    inline void setEventPicOpticalFlowSpeed(unsigned char* buffer) { m_pEventPicOpticalFlowSpeed = buffer; }

    inline int getRow() { return m_nRow; }
    inline int getColumn() { return m_nColumn; }
    inline int getBrightness() { return m_nBrightness; }
    inline int getTime() { return m_nTime; }
    inline unsigned char* getFullPicBuffer() { return m_pFullPicBuffer; }
    inline unsigned char* getEventPicBuffer() { return m_pEventPicBuffer; }
    inline unsigned char* getEventPicBufferGray() { return m_pEventPicBufferGray; }
    inline unsigned char* getEventPicBufferSuperimposed() { return m_pEventPicBufferSuperimposed; }
    inline unsigned char* getEventPicBufferDenoisedBinary() { return m_pEventPicBufferDenoisedBinary; }
    inline unsigned char* getEventPicBufferDenoisedGray() { return m_pEventPicBufferDenoisedGray; }
    inline unsigned char* getEventPicOpticalFlow() { return m_pEventPicOpticalFlow; }
    inline unsigned char* getEventPicOpticalFlowDirection() { return m_pEventPicOpticalFlowDirection; }
    inline unsigned char* getEventPicOpticalFlowSpeed() { return m_pEventPicOpticalFlowSpeed; }

private:
    std::list<Observer::emDataType> m_listDataType;
    Observer*         m_pObserver;
    unsigned char*    m_pFullPicBuffer;
    unsigned char*    m_pEventPicBuffer;
    unsigned char*    m_pEventPicBufferGray;
    unsigned char*    m_pEventPicBufferSuperimposed;
    unsigned char*    m_pEventPicBufferDenoisedBinary;
    unsigned char*    m_pEventPicBufferDenoisedGray;
    unsigned char*    m_pEventPicOpticalFlow;
    unsigned char*    m_pEventPicOpticalFlowDirection;
    unsigned char*    m_pEventPicOpticalFlowSpeed;
    int               m_nRow;
    int               m_nColumn;
    int               m_nBrightness;
    int               m_nTime;
};

class FrontPanel;
class HHSequenceMgr;
class HHSequenceSlider;
class DataProcessThread;
class SensorData;
class SensorDataObserver;

// This class is exported from the CelexSensorDll.dll
class CELEXSENSORDLL_API CelexSensorDLL
{
public:
    enum emDeviceType {
        Sensor = 0,
        FPGA,
        SensorAndFPGA
    };
    enum emAdvancedBiasType {
        EVT_VL = 0,
        EVT_VH,
        ZONE_MINUS,
        ZONE_PLUS,
        REF_MINUS,
        REF_PLUS,
        REF_MINUS_H,
        REF_PLUS_H,
        EVT_DC,
        LEAK,
        CDS_DC,
        CDS_V1,
        PixBias,
        Gain,
        Clock,
        Resolution
    };
    enum ErrorCode {
        NoError = 0,
        InitializeFPGAFailed,
        PowerUpFailed,
        ConfigureFailed
    };

    typedef struct TimeInfo
    {
        int hour;
        int minute;
        int second;
    } TimeInfo;

    typedef struct ControlSliderInfo
    {
        std::string name;
        uint32_t    min;
        uint32_t    max;
        uint32_t    value;
        uint32_t    step;
        bool        bAdvanced;
    } ControlSliderInfo;

    CelexSensorDLL();
    ~CelexSensorDLL();

    ErrorCode openSensor(std::string str);
    bool isSensorReady();
    bool isSdramFull();
    void pipeOutFPGAData();

    unsigned char* getFullPicBuffer();
    unsigned char* getEventPicBuffer(emEventPicMode mode = EventOnlyPic);
    void getPixelData(int& row, int& column, int& brightness, int& time);
    void clearData();

    //--- Sensor Operating Mode Interfaces ---
    void setSensorMode(emSensorMode mode);
    emSensorMode getSensorMode();

    //--- FPN Interfaces ---
    bool setFpnFile(const std::string& fpnFile);
    void generateFPN(std::string fpnFile);

    //--- Sensor Control Interfaces ---
    void resetDevice(emDeviceType emType);
    void enableADC(bool enable);

    void setContrast(uint32_t value);
    uint32_t getContrast();

    void setBrightness(uint32_t value);
    uint32_t getBrightness();

    void setThreshold(uint32_t value);
    uint32_t getThreshold();

    void trigFullPic();

    //--- Set the lower and upper limits of A Interfaces ---
    void setLowerADC(uint32_t value);
    void setUpperADC(uint32_t value);

    //--- Record Sensor Data Interfaces ---
    void startRecording(std::string filePath);
    void stopRecording();
    TimeInfo getRecordedTime();

    //--- Playback Interfaces ---
    bool readPlayBackData(long length = 1968644);
    bool openPlaybackFile(std::string filePath);
    void play();
    void pause();
    long getPlaybackFileSize();
    bool setPlayBackOffset(long offset);
    void saveSelectedBinFile(std::string filePath, long fromPos, long toPos, int hour, int minute, int second);

    //--- Set the methods of creating pic frame Interfaces ---
    void setFullPicFrameTime(uint32_t msec); //unit: millisecond
    uint32_t getFullPicFrame();
    void setEventFrameTime(uint32_t msec); //unit: millisecond
    uint32_t getEventFrameTime();
    void setFEFrameTime(uint32_t msec); //unit: millisecond
    uint32_t getFEFrameTime();

    void setOverlapTime(uint32_t msec); //unit: millisecond
    uint32_t getOverlapTime();

    SensorData* getSensorDataObject();
    std::vector<ControlSliderInfo> getSensorControlList();

    void pauseThread(bool pause);
    bool setAdvancedBias(emAdvancedBiasType biasType);
    bool setAdvancedBias(emAdvancedBiasType biasType, int value);

    //--- for clock ---
    uint32_t getClockRate(); //unit: MHz
    void setClockRate(uint32_t value); //unit: MHz

    //--- for optical flow ---
    void enableOpticalFlow(bool enable);
    bool isOpticalFlowEnabled();
    void setOpticalFlowLatencyTime(uint32_t msec);
    void setOpticalFlowSliceCount(uint32_t count);
    unsigned char* getOpticalFlowRawBuffer();
    unsigned char* getOpticalFlowDirectionBuffer();
    unsigned char* getOpticalFlowSpeedBuffer();

    //for test
    void setResetLength(uint32_t value);
    uint32_t getPageCount();

private:
    void initializeFPGA(uint32_t value = 0);
    bool powerUp();
    bool configureSettings();
    void parseSliderSequence();
    bool excuteCommand(std::string strCommand);

private:
    std::map<std::string, uint32_t>  m_mapSliderNameValue; //All Setting Names & Initial Values
    std::vector<std::string>         m_vecAdvancedNames;   //Advanced Setting Names
    std::vector<ControlSliderInfo>   m_vecSensorControlList;
    std::ifstream                    m_ifstreamPlayback;
    std::ofstream                    m_ofstreamRecord;
    FrontPanel*                      m_pFrontPanel;
    HHSequenceMgr*                   m_pSequenceMgr;
    DataProcessThread*               m_pDataProcessThread;
    unsigned char*                   m_pData;
    uint32_t                         m_uiFrameTime;
    uint32_t                         m_uiOverlapTime;
    uint32_t                         m_uiContrast;
    uint32_t                         m_uiBrightness;
    uint32_t                         m_uiThreshold;
    uint32_t                         m_uiClockRate;
    int                              m_iTimeStartRecord;
    TimeInfo                         m_stTimeRecorded;
    bool                             m_bRecording;
    long                             m_lPlaybackFileSize;
    //for test
    uint32_t                         m_uiPageCount;
};

#endif // CELEXSENSORDLL_H
