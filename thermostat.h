#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include "sensor.h"
class Thermostat : public Sensor
{
    Q_OBJECT
private:
    const float LOWEST_TEMP = -10.0F;
    const float HIGHEST_TEMP = 50.0F;
    float temperatureSetting;
    float temperatureReading = 20;

    void performAction();
    //agregate heater and AC?
public:
    explicit Thermostat(QObject *parent = nullptr);

    void readValue() override; //to fire network command of temperature retrieval
    void setTemperatureSetting(float temp);
    void setTemperatureReading(float temp);

    float getTemperatureReading(){
        return temperatureReading;
    }
    float getTemperatureSetting(){
        return temperatureSetting;
    }

    float getLowest_temp(){ return LOWEST_TEMP; }
    float getHighest_temp(){ return HIGHEST_TEMP; }
    void chngTempSetting();

    void sendCommand() override;
public slots:
    void handleResponse(const QByteArray& response) override;
signals:
    void temperatureChanged(float newTemperature);
    void temperatureSettingChanged(float newTemperatureSetting); //send to actuator like heater or AC
    void temperatureReadingChanged(float newTemperatureReading);
    void temperatureError(const QString& errorMessage);
    void turnAC_ON();
    void turnHeater_ON();
    void turnHVACs_OFF();

};

#endif // THERMOSTAT_H
