#include "mainwindow.h"
#include "lampwidget.h"
#include "rgblampwidget.h"
#include "ui_mainwindow.h"
#include "networkhandler.h"
#include "ac.h"
#include "heater.h"
#include "rgblamp.h"
#include "thermostat.h"
#include "hvac.h"
#include "hvacwidget.h"

MainWindow::MainWindow(NetworkHandler *networkhandler, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), networkHandler(networkhandler), weatherStation(new WeatherStation(this)), currentDevice(nullptr)
{
    ui->setupUi(this);
    // Timer to update the time every second
    clockTimer = new QTimer(this);
    connect(clockTimer, &QTimer::timeout, this, &MainWindow::setTime);
    clockTimer->start(1000);

    // Timer to update the weather every 60 seconds (1 minute)
    weatherTimer = new QTimer(this);
    connect(weatherTimer, &QTimer::timeout, this, &MainWindow::updateWeatherData);
    weatherTimer->start(300000);  // Refresh weather every 5 minutes 300000

    connect(weatherStation, &WeatherStation::weatherUpdated, this, &MainWindow::setWeatherData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTime(){
    ui->timeLabel->setText(weatherStation->acquireTime());
}

void MainWindow::setWeatherData(){
    //qDebug() << "Seting weather data.";
    ui->temperatureLabel->setText(weatherStation->getTemperature());
    ui->humidityLabel->setText(weatherStation->getHumidity());
    ui->pressureLabel->setText(weatherStation->getPressure());
    ui->weatherIconLabel->setPixmap(QPixmap::fromImage(weatherStation->getIconImage()));
}

void MainWindow::updateWeatherData(){
    weatherStation->readValue();
}

void MainWindow::on_addDeviceButton_clicked()
{
    addDeviceWindow = new newdevicewindow(this);
    connect(addDeviceWindow,&newdevicewindow::newDeviceNameCheck,this,&MainWindow::on_newDeviceNameCheck);
    connect(addDeviceWindow,&newdevicewindow::newDeviceIpcheck,this,&MainWindow::on_newDeviceIpCheck);
    connect(this,&MainWindow::newDeviceNameOK,addDeviceWindow,&newdevicewindow::on_newDeviceNameOK);
    connect(this,&MainWindow::newDeviceIpOK,addDeviceWindow,&newdevicewindow::on_newDeviceIpOK);
    connect(addDeviceWindow,&newdevicewindow::newThermostatNameCheck,this,&MainWindow::on_newThermostatNameCheck);
    connect(addDeviceWindow,&newdevicewindow::newThermostatIpCheck,this,&MainWindow::on_newThermostatIpCheck);
    connect(this,&MainWindow::newThermostatNameOK,addDeviceWindow,&newdevicewindow::on_newThermostatNameOK);
    connect(this,&MainWindow::newThermostatIpOK,addDeviceWindow,&newdevicewindow::on_newThermostatIpOK);
    connect(addDeviceWindow,&newdevicewindow::checkIfIpNotInHVACDevices,this,&MainWindow::on_checkIfIpNotInHVACDevices);
    connect(this,&MainWindow::ipNotInHVACDevices,addDeviceWindow,&newdevicewindow::on_deviceIpNotInUse);
    connect(this,&MainWindow::hvacWidgetCreation,addDeviceWindow,&newdevicewindow::on_HVACWidgetCreation);
    if(hvacWidgetCreated){
        emit hvacWidgetCreation();
    }
    int result = addDeviceWindow->exec();
    QHostAddress adres;
    Device* newDevice = nullptr;  // Base pointer for any device type
    Thermostat* newThermostat = nullptr;
    if(result) {
        // Create device based on type
        switch(addDeviceWindow->getDeviceType()) {
        case 0:
            //newDevice = new AC();
            newThermostat = new Thermostat;
            newThermostat->setDeviceName(addDeviceWindow->getThermostatName());
            adres.setAddress(addDeviceWindow->getThermostatIP());
            newThermostat->setDeviceIP(adres);
            newDevice = new HVAC(newThermostat);
            //newThermostat = nullptr;
            break;
        case 1:
            //newDevice = new Heater();
            newDevice = new RGBLamp();
            break;
        case 2:
            //newDevice = new RGBLamp();
            newDevice = new Lamp();
            break;
        case 3:
            //newDevice = new Thermostat();
            break;
        case 4:
            //newDevice = new Lamp();
            break;
        }

        if(newDevice) {
            newDevice->setDeviceName(addDeviceWindow->getDeviceName());
            adres.setAddress(addDeviceWindow->getDeviceIP());
            newDevice->setDeviceIP(adres);
            networkHandler->registerDevice(newDevice);

            qDebug() << "Added device:" << addDeviceWindow->getDeviceName()
                     << "IP:" << addDeviceWindow->getDeviceIP();

            // Add device card to scroll area
            addDeviceCard(newDevice);
        }
    }
    delete addDeviceWindow;
}

void MainWindow::addDeviceCard(Device *device)
{
    if(device == nullptr) {
        return;
    }

    QFrame *deviceCard = new QFrame(ui->scrollAreaWidgetContents_2);
    deviceCard->setFrameStyle(QFrame::Panel);
    deviceCard->setLineWidth(2);

    QVBoxLayout *layout = new QVBoxLayout(deviceCard);
    DeviceWidget* deviceWidget = nullptr;

    if(auto rgbLamp = qobject_cast<RGBLamp*>(device)) {
        deviceWidget = new RGBLampWidget(rgbLamp, deviceCard);
    }
    else if(auto lamp = qobject_cast<Lamp*>(device)) {
        deviceWidget = new LampWidget(lamp, deviceCard);
    }
    else if(auto hvac = qobject_cast<HVAC*>(device)) {
        deviceWidget = new HVACWidget(hvac,deviceCard);
        connect(qobject_cast<HVACWidget*>(deviceWidget), &HVACWidget::searchDevicesNameInNetworkHandler,this,&MainWindow::on_searchDevicesNameInNetworkHandler);
        connect(qobject_cast<HVACWidget*>(deviceWidget),&HVACWidget::searchDevicesIpInNetworkHandler, this, &MainWindow::on_searchDevicesIpInNetworkHandler);
        connect(this,&MainWindow::hvacDeviceNameOK,qobject_cast<HVACWidget*>(deviceWidget),&HVACWidget::on_hvacDeviceNameOK);
        connect(this,&MainWindow::hvacDeviceIpOK,qobject_cast<HVACWidget*>(deviceWidget), &HVACWidget::on_hvacDeviceIpOK);
        connect(this,&MainWindow::checkIfIpNotInHVACDevices,qobject_cast<HVACWidget*>(deviceWidget),&::HVACWidget::on_checkIfIpNotInHVACDevices);
        connect(qobject_cast<HVACWidget*>(deviceWidget),&HVACWidget::ipNotInHVACDevices,this,&MainWindow::on_ipNotInHVACDevices);
        connect(qobject_cast<HVACWidget*>(deviceWidget),&HVACWidget::widgetCreation,this,&MainWindow::on_hvacWidgetCreation);
        hvacWidgetCreated = true;
    }
    if(deviceWidget){
        connect(deviceWidget, &DeviceWidget::removalRequested, this, &MainWindow::removeDeviceCard);
        layout->addWidget(deviceWidget);

        deviceCards[device] = deviceCard;

        QHBoxLayout* scrollLayout = qobject_cast<QHBoxLayout*>(ui->scrollAreaWidgetContents_2->layout());

        if(!scrollLayout){
            scrollLayout = new QHBoxLayout(ui->scrollAreaWidgetContents_2);
            scrollLayout->setSpacing(10);
            scrollLayout->setContentsMargins(10, 10, 10, 10);
            scrollLayout->addStretch();
        }
        scrollLayout->insertWidget(scrollLayout->count()-1, deviceCard);
    }
}

Device* MainWindow::searchForDevice(QString deviceName)
{
    Device *selectedDevice = nullptr;
    // Znalezienie urządzenia na podstawie nazwy
    for (Device* device : networkHandler->getDevices()) {
        if (device->getName() == deviceName) {
            selectedDevice = device;
            break;
        }
    }
    return selectedDevice;
}

void MainWindow::removeDeviceCard(Device *device)
{
    if(device == nullptr) {
        return;
    }

    if(deviceCards.contains(device)) {
        QFrame* deviceCard = deviceCards[device];
        QHBoxLayout* scrollLayout = qobject_cast<QHBoxLayout*>(ui->scrollAreaWidgetContents_2->layout());

        if(scrollLayout) {
            scrollLayout->removeWidget(deviceCard);
        }

        networkHandler->unregisterDevice(device);
        deviceCards.remove(device);
        deviceCard->deleteLater();
        device->deleteLater();
    }
}

void MainWindow::on_newDeviceNameCheck(QString name){
        qDebug() << "Sprawdzan nazwe";
    Device* newDevice = nullptr;
    newDevice = searchForDevice(name);
    if(newDevice == nullptr){
        emit newDeviceNameOK();
        //qDebug() << "Emituję sygnał, nazwa ok";
    }
}

void MainWindow::on_newDeviceIpCheck(QHostAddress* ip){
   // qDebug() << "Sprawdzam ip";
    Device* newDevice = nullptr;
        for (Device* device : networkHandler->getDevices()) {
            if (device->getDeviceIP() == *ip) {
                newDevice = device;
                break;
            }
        }
        if(newDevice == nullptr){
            emit newDeviceIpOK();
            //qDebug() << "Emituję sygnał, ip ok";
        }
}

void MainWindow::on_newThermostatNameCheck(QString name){
    //qDebug() << "Sprawdzam nazwę thermostatu";
    Device* newDevice = nullptr;
    newDevice = searchForDevice(name);
    if(newDevice == nullptr){
        emit newThermostatNameOK();
        //qDebug() << "Emituję sygnał, nazwa thermostat ok";
    }
}

void MainWindow::on_newThermostatIpCheck(QHostAddress* ip){
    //qDebug() << "Sprawdzam ip thermostatu";
    Device* newDevice = nullptr;
    for (Device* device : networkHandler->getDevices()) {
        if (device->getDeviceIP() == *ip) {
            newDevice = device;
            break;
        }
    }
    if(newDevice == nullptr){
        emit newThermostatIpOK();
        //qDebug() << "Emituję sygnał, ip thermostat ok";
    }
}

void MainWindow::on_searchDevicesNameInNetworkHandler(QString name){
    Device * device = nullptr;
    device = searchForDevice(name);
    if(device == nullptr){
        emit hvacDeviceNameOK();
    }
}

void MainWindow::on_searchDevicesIpInNetworkHandler(QHostAddress *ip){
    Device* sdevice = nullptr;
    for(Device * device : networkHandler->getDevices()){
        if(device->getDeviceIP() == *ip){
            qDebug() << device->getDeviceIP();
            sdevice = device;
        }
    }
    if(sdevice == nullptr){
        emit hvacDeviceIpOK();
    }
}

void MainWindow::on_checkIfIpNotInHVACDevices(QHostAddress *ip){
    qDebug() << "Jestesmy w mainwindow";
    emit checkIfIpNotInHVACDevices(ip);
}

void MainWindow::on_ipNotInHVACDevices(){
    qDebug() << "znowu jestem w mainwidow";
    emit ipNotInHVACDevices();
}

void MainWindow::on_hvacWidgetCreation(){
    qDebug() <<"WWWWWWW mainie info o utowrzeniu widgeta";
    emit hvacWidgetCreation();
}

void MainWindow::on_miasctoCoBox_currentIndexChanged(int index)
{
    weatherStation->setMiasto(ui->miasctoCoBox->itemText(index));
}

