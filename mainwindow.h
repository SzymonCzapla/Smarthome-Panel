#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkhandler.h"
#include "newdevicewindow.h"
#include "qframe.h"
#include "weatherstation.h"
#include "device.h"
#include <QMap>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(NetworkHandler *networkHandler = nullptr, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setTime();
    void setWeatherData();
    void updateWeatherData();
    void on_newDeviceNameCheck(QString name);
    void on_newDeviceIpCheck(QHostAddress *ip);
    void on_newThermostatNameCheck(QString name);
    void on_newThermostatIpCheck(QHostAddress *ip);
    void on_searchDevicesNameInNetworkHandler(QString name);
    void on_searchDevicesIpInNetworkHandler(QHostAddress *ip);
    void on_checkIfIpNotInHVACDevices(QHostAddress *ip);
    void on_ipNotInHVACDevices();
    void on_hvacWidgetCreation();

private:
    Ui::MainWindow *ui;
    QMap<Device*, QFrame*> deviceCards;

    newdevicewindow *addDeviceWindow;
    NetworkHandler *networkHandler;
    WeatherStation *weatherStation;
    QTimer *clockTimer;
    QTimer *weatherTimer;
    Device *currentDevice;

    Device* searchForDevice(QString deviceName);
    void removeDeviceCard(Device* device);
    bool hvacWidgetCreated = false;



private slots:
    void on_addDeviceButton_clicked();
    void addDeviceCard(Device* device);

    void on_miasctoCoBox_currentIndexChanged(int index);

signals:
    void newDeviceNameOK();
    void newDeviceIpOK();
    void newThermostatNameOK();
    void newThermostatIpOK();
    void hvacDeviceNameOK();
    void hvacDeviceIpOK();
    void checkIfIpNotInHVACDevices(QHostAddress *ip);
    void ipNotInHVACDevices();
    void hvacWidgetCreation();

};
#endif // MAINWINDOW_H
