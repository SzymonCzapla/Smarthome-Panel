#include "lampwidget.h"
#include "ui_lampwidget.h"

LampWidget::LampWidget(Lamp *lamp, QWidget *parent)
    : DeviceWidget(lamp, parent), ui(new Ui::LampWidget)
{
    ui->setupUi(this);
    ui->nameLabel->setText(lamp->getName());
    ui->IPAdressLabel->setText("IP: " + lamp->getDeviceIP().toString());
    ui->lamp_deviceIcon->setPixmap(QPixmap(":/static/icons/light-bulb.png"));
    updateUI();
}

LampWidget::~LampWidget()
{
    delete ui;
}

void LampWidget::updateUI()
{
    if (m_lamp()) {
        QString statusText;
        switch (m_lamp()->getStatus()) {
        case DeviceStatus::ON:
            statusText = "ON";
            break;
        case DeviceStatus::OFF:
            statusText = "OFF";
            break;
        case DeviceStatus::ERROR:
            statusText = "ERROR";
            break;
        }
    ui->statusLabel->setText(QString("Status: %1")
                                 .arg(statusText));
    }
    ui->toggleButton->setText(m_lamp()->getStatus() == DeviceStatus::ON ? "Turn OFF" : "Turn ON");
}

void LampWidget::on_toggleButton_clicked()
{
    m_lamp()->toggle();
    updateUI();
}


