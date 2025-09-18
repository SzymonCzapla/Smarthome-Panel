#ifndef WEATHERSTATION_H
#define WEATHERSTATION_H

#include "qimage.h"
#include "sensor.h"
#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

class WeatherStation : public Sensor
{
    Q_OBJECT
public:
    explicit WeatherStation(QObject *parent = nullptr);
    void setMiasto(QString miasto);

public slots:
    void readValue() override;
    QString acquireTime(); //przenieść do okna
    QString getTemperature();
    QString getHumidity();
    QString getPressure();
    QImage getIconImage();


private:
    double temperature = NULL;
    double humidity = NULL;
    double pressure = NULL;
    QString iconCode;
    QImage iconImage;
    QNetworkAccessManager *networkManager;
    void fetchWeatherData();
    void fetchWeatherIcon(const QString &iconCode);
    bool isRequestInProgress;
    QString miasto = "Katowice";


signals:
    void weatherUpdated();
};

#endif // WEATHERSTATION_H
