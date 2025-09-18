#include "devicewidget.h"



void DeviceWidget::setupUI()
{
    removalButton = new QPushButton("X", this);
    removalButton->setFixedSize(20,20);
    removalButton->setStyleSheet(R"(
        QPushButton {
            background-color: #ff4444;
            border-radius: 10px;
            color: white;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #ff0000;
        }
    )");

    connect(removalButton, &QPushButton::clicked, this, [this](){
        emit removalRequested(m_device);
    });
}
