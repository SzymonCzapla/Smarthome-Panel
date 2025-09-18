#include "newdevicewindow.h"
#include "ui_newdevicewindow.h"

newdevicewindow::newdevicewindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::newdevicewindow)
{
    ui->setupUi(this);
    QString ipRange = R"((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5]))";
    QRegularExpression ipRegex ("^" + ipRange + "(\\." + ipRange + ")" + "(\\." + ipRange + ")" + "(\\." + ipRange + ")$");
    QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(ipRegex,this);
    ui->deviceNameLnEdit->setPlaceholderText("New device");
    ui->thermostatNameliEd->setPlaceholderText("New Thermostat");
    ui->deviceIPLnEdit->setPlaceholderText("192.168.1.2");
    ui->thermostatIpLiEd->setPlaceholderText("192.168.1.4");
    ui->deviceIPLnEdit->setValidator(ipValidator);
    ui->thermostatIpLiEd->setValidator(ipValidator);
    ui->buttonBox->hide();
}

newdevicewindow::~newdevicewindow()
{
    delete ui;
}

QString newdevicewindow::getDeviceName(){
    return ui->deviceNameLnEdit->text();
}

QString newdevicewindow::getDeviceIP(){
    return ui->deviceIPLnEdit->text();
}
int newdevicewindow::getDeviceType(){
    return ui->deviceTypecBox->currentIndex();
}

void newdevicewindow::on_deviceTypecBox_activated(int index)
{
    if(index == 0){
        ui->thermostatIpLabel->show();
        ui->thermostatNameLabel->show();
        ui->thermostatNameliEd->show();
        ui->thermostatIpLiEd->show();
    } else {
        ui->thermostatNameLabel->hide();
        ui->thermostatIpLabel->hide();
        ui->thermostatNameliEd->hide();
        ui->thermostatIpLiEd->hide();
    }
}

QString newdevicewindow::getThermostatName(){
    return ui->thermostatNameliEd->text();
}

QString newdevicewindow::getThermostatIP(){
    return ui->thermostatIpLiEd->text();
}

void newdevicewindow::on_deviceNameLnEdit_textEdited(const QString &arg1)
{
    if(!ui->deviceNameLnEdit->text().isEmpty()){
        if(ui->deviceTypecBox->currentIndex() != 0){
            newThermostatNameOK = true;
            emit newDeviceNameCheck(arg1);
        } else {
            emit newDeviceNameCheck(arg1);
        }
    } else {
        ui->buttonBox->hide();
    }
}


void newdevicewindow::on_deviceIPLnEdit_textEdited(const QString &arg1)
{
    if(ui->deviceIPLnEdit->hasAcceptableInput() && ui->deviceIPLnEdit->text() == "127.0.0.1"){
        newDeviceIpOK = true;
        if(ui->deviceTypecBox->currentIndex() != 0){
            newThermostatIpOK = true;
        }
        checkIfInputsOK();
    } else{
        if(ui->deviceIPLnEdit->hasAcceptableInput()){
            QHostAddress *address = new QHostAddress;
            address->setAddress(arg1);
            qDebug() <<"Sprawdzam czy w hvacu";
            emit checkIfIpNotInHVACDevices(address);
            if(ui->deviceTypecBox->currentIndex() != 0){
                newThermostatIpOK = true;
                emit newDeviceIpcheck(address);
            }
            else {
                if( ui->deviceIPLnEdit->text() != ui->thermostatIpLiEd->text()){
                    emit newDeviceIpcheck(address);
                }
            }
        } else{
            ui->buttonBox->hide();
        }
    }

}


void newdevicewindow::on_thermostatNameliEd_textEdited(const QString &arg1)
{
    if(ui->deviceTypecBox->currentIndex() == 0){
        if(!ui->thermostatNameliEd->text().isEmpty()){
            emit newThermostatNameCheck(arg1);
        }
    }
}


void newdevicewindow::on_thermostatIpLiEd_textEdited(const QString &arg1)
{

    if(ui->deviceTypecBox->currentIndex() == 0){
        if(ui->thermostatIpLiEd->hasAcceptableInput() && !ui->thermostatIpLiEd->text().isEmpty()){
            if(ui->thermostatIpLiEd->text() != ui->deviceIPLnEdit->text()){
                QHostAddress *address = new QHostAddress;
                address->setAddress(arg1);
                qDebug() << "Emituję sprawdz ip thermostatu";
                emit newThermostatIpCheck(address);
            }
            else if(ui->thermostatIpLiEd->text() == "127.0.0.1"){
                qDebug() <<"Dociera" + ui->thermostatIpLiEd->text();
                newThermostatIpOK = true;
                checkIfInputsOK();
            }
        }

        else{
            ui->buttonBox->hide();
        }
    }
}

void newdevicewindow::on_newDeviceNameOK(){
    newDeviceNameOK = true;
    checkIfInputsOK();
}

void newdevicewindow::on_newDeviceIpOK(){
    newDeviceIpOK = true;
    checkIfInputsOK();
}

void newdevicewindow::checkIfInputsOK(){
    qDebug() <<"Name OK: ";
    qDebug() << newDeviceNameOK;
    qDebug() << "Ip OK:" ;
    qDebug() << newDeviceIpOK;
    qDebug() << "Thermostat name ok";
    qDebug() << newThermostatNameOK;
    qDebug() << "Thermostat ip ok";
    qDebug() << newThermostatIpOK;
    qDebug() <<"Not in hvac";
    qDebug() << ipNotUsedByHvacDevices;

    if(newDeviceNameOK && newDeviceIpOK && newThermostatNameOK && newThermostatIpOK && ipNotUsedByHvacDevices ){
        ui->buttonBox->show();
        ipNotUsedByHvacDevices = false; //tu

    } else {
        ui->buttonBox->hide();
    }
}

void newdevicewindow::on_newThermostatNameOK(){
    newThermostatNameOK = true;
    checkIfInputsOK();
}

void newdevicewindow::on_newThermostatIpOK(){
    newThermostatIpOK = true;
    checkIfInputsOK();
}

void newdevicewindow::on_deviceIpNotInUse(){
    qDebug() << "Dotarlismy";
    ipNotUsedByHvacDevices = true;
}

void newdevicewindow::on_HVACWidgetCreation() {
    qDebug() << "HVAC WIDGET CREATED";
    ipNotUsedByHvacDevices = false;
}
