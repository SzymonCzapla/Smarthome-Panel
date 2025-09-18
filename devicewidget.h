#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include "device.h"
#include "qpushbutton.h"

class DeviceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceWidget(Device* device, QWidget *parent = nullptr)
        : QWidget(parent), m_device(device)
    {
        setupUI();
    }
    virtual void updateUI() = 0;
protected:
    Device* m_device;
    QPushButton *removalButton;

    virtual void setupUI();

signals:
    void removalRequested(Device* device);
};

#endif // DEVICEWIDGET_H
