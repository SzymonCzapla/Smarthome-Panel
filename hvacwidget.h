#ifndef HVACWIDGET_H
#define HVACWIDGET_H

#include <QWidget>
#include "devicewidget.h"
#include "hvac.h"
#include "QListWidgetItem"
#include <QRegularExpression>

namespace Ui {
class HVACWidget;
}

class HVACWidget : public DeviceWidget
{
    Q_OBJECT

public:
    explicit HVACWidget(HVAC* unitHVAC, QWidget *parent = nullptr);
    ~HVACWidget();
    void updateUI() override;

private:
    Ui::HVACWidget *ui;
    HVAC* m_unitHVAC() { return qobject_cast<HVAC*>(m_device);}

    void listHVACdevices();
    Device* searchDevice(QString deviceName);
    void newDeviceNameAndIpOK();
    bool checkIfIpNotInUse(QHostAddress address);
    bool hvacDeviceNameOK = false;
    bool hvacDeviceIpOK = false;
    void sendTemperatureToServer();
    void sendDeviceStatusToServer();
public slots:
    void onTemperatureChange();
    void on_hvacDeviceNameOK();
    void on_hvacDeviceIpOK();
    void on_checkIfIpNotInHVACDevices(QHostAddress *ip);

private slots:

    void on_setTemperatureButton_clicked();

    void on_addHVACdeviceButton_clicked();

    void on_devicesHVAClistWidget_itemClicked(QListWidgetItem *item);

    void on_newDeviceNameLiEd_textEdited(const QString &arg1);

    void on_newDeviceIpLiEd_textEdited(const QString &arg1);


signals:
    void sendTemperature(float temperature);
    QString searchDevicesNameInNetworkHandler(QString name);
    void searchDevicesIpInNetworkHandler(QHostAddress *ip);
    void ipNotInHVACDevices();
    void widgetCreation();
};

#endif // HVACWIDGET_H
