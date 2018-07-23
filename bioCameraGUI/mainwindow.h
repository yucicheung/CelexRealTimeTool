#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class CelexSensorDLL;
class QLabel;
class QComboBox;
class QPushButton;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onPipeoutDataTimer();
    void onDisplayTimer();
    void onEventPicModeChanged(QString);
    void onPressed();

private:
    Ui::MainWindow *ui;
    QTimer*             m_pPipeOutDataTimer;
    QTimer*             m_pDisplayTimer;

    CelexSensorDLL*     m_pCelexSensor;

    QImage*             m_pimageEventPic;

    QImage              m_imageBinaryPic;
    QImage              m_imageGrayPic;
    QImage              m_imageAccumulatedPic;
    QImage              m_imageDenoisedGrayPic;
    QImage              m_imageDenoisedBinaryPic;

    QLabel*             m_pLabelPic;


    unsigned char*      pBinaryPicBuffer;
    unsigned char*      pGrayPicBuffer;
    unsigned char*      pAccumulatedPicBuffer;
    unsigned char*      pDenoisedGrayPicBuffer;
    unsigned char*      pDenoisedBinaryPicBuffer;


    QComboBox*          m_pComboBox;
    void                setComboBox();

    QPushButton*        m_pstorebutton;
    bool                pressed;
};

#endif // MAINWINDOW_H
