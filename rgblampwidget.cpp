#include "rgblampwidget.h"
#include "ui_RGBLampWidget.h"
#include <QColorDialog>

RGBLampWidget::RGBLampWidget(RGBLamp* lamp, QWidget* parent)
    : DeviceWidget(lamp, parent), ui(new Ui::RGBLampWidget)
{
    ui->setupUi(this);
    //already connected by Ui, no need for that
    // connect(ui->colorPickerButton, &QPushButton::clicked,
    //         this, &RGBLampWidget::on_colorPickerButton_clicked);
    // connect(ui->brightnessSlider, &QSlider::valueChanged,
    //         this, &RGBLampWidget::on_brightnessSlider_valueChanged);
    // connect(ui->toggleButton, &QPushButton::clicked,
    //         this, &RGBLampWidget::on_toggleButton_clicked);

    ui->brightnessSlider->setRange(0, 255);
    ui->nameLabel->setText(lamp->getName());
    ui->IPAddressLabel->setText("IP: " + lamp->getDeviceIP().toString());
    ui->RGBLamp_deviceIcon->setPixmap(QPixmap(":/static/icons/rgb-light-bulb.png"));
    updateUI();
}

RGBLampWidget::~RGBLampWidget()
{
    delete ui;
}

void RGBLampWidget::updateUI()
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
    ui->brightnessSlider->setValue(m_lamp()->getBrightness());
}

void RGBLampWidget::on_colorPickerButton_clicked()
{
    QColor color = QColorDialog::getColor(m_lamp()->getColor(), this);
    if (color.isValid()) {
        m_lamp()->mixColors(color.red(), color.green(), color.blue());
        m_lamp()->chngColor();
        m_lamp()->sendCommand();
        updateUI();
    }
}


void RGBLampWidget::on_brightnessSlider_valueChanged(int value)
{
    m_lamp()->setBrightness(value);
    m_lamp()->chngBrightness();
    m_lamp()->sendCommand();
    updateUI();
}


void RGBLampWidget::on_toggleButton_clicked()
{
    m_lamp()->toggle();
    updateUI();
}

