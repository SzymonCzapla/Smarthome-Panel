#include "weatherstation.h"

WeatherStation::WeatherStation(QObject *parent)
    : Sensor{parent}, iconCode(""),networkManager(new QNetworkAccessManager(this)),isRequestInProgress(false)
{
    // Connect the network manager to handle the weather data after it is fetched
    connect(networkManager, &QNetworkAccessManager::finished, this, &WeatherStation::fetchWeatherData);
    fetchWeatherData();
}
//przenieść do okna
QString WeatherStation::acquireTime()
{
    return QTime::currentTime().toString("hh:mm:ss");
}

void WeatherStation::readValue(){
    fetchWeatherData();
}

QString WeatherStation::getTemperature(){
    qDebug() << "Temperature: " << temperature;
    return QString::number(temperature, 'f', 0) + "°C";
}

QString WeatherStation::getHumidity(){
    qDebug() << "Humidity: " << humidity;
    return QString::number(humidity, 'f', 0) + "%";
}

QString WeatherStation::getPressure(){
    qDebug() << "Pressure: " << pressure;
    return QString::number(pressure, 'f', 0) + "hPa";
}

QImage WeatherStation::getIconImage(){
    return iconImage;
}

void WeatherStation::fetchWeatherData(){
    //qDebug() << "Fetching weather data.";
// Prevent fetching weather if a request is in progress
    if (isRequestInProgress) {
        return;
    }
    isRequestInProgress = true;  // Mark that the request is in progress

    QString apiKey = "d1ae366f3a0eff9cf7500da05d365082"; // tu jest moj api key
    //QString city = "Katowice";
    //Construct the API URL
    QString url = QString("http://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric")
                          .arg(miasto)
                          .arg(apiKey);

    qDebug() << "Constructed URL:" << url; // Debug output to verify the URL
    // Validate the URL
    QUrl apiUrl(url);
    if (!apiUrl.isValid()) {
        qDebug() << "Invalid URL:" << apiUrl.errorString();
        isRequestInProgress = false;  // Reset flag
        return;
    }
    qDebug() << "Sending get request";
    // Send the GET request
    QNetworkRequest request(apiUrl);
    qDebug() << "Receiving reply";
    QNetworkReply *reply = networkManager->get(request);
    qDebug() << "connecting";
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            qDebug() << "Reading response";

            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            QJsonObject jsonObj = jsonDoc.object();

            temperature = jsonObj["main"].toObject()["temp"].toDouble();
            humidity = jsonObj["main"].toObject()["humidity"].toDouble();
            pressure = jsonObj["main"].toObject()["pressure"].toDouble();

            QString currentIconCode = "";
            QJsonArray weatherArray = jsonObj["weather"].toArray();
            if (!weatherArray.isEmpty()) {
                QJsonObject weatherObj = weatherArray.at(0).toObject();
                currentIconCode = weatherObj["icon"].toString();
            }

            if (currentIconCode != iconCode) {
                iconCode = currentIconCode;
                fetchWeatherIcon(iconCode);
            }

            emit weatherUpdated(); // 🔥 Emitujemy sygnał po wczytaniu danych
        } else {
            qDebug() << "Error fetching weather data:" << reply->errorString();
        }

        isRequestInProgress = false;
        reply->deleteLater();
    });
}

void WeatherStation::fetchWeatherIcon(const QString &iconCode){
// Construct the URL for the weather icon
    QString iconUrl = QString("https://openweathermap.org/img/wn/%1@2x.png").arg(iconCode);
    qDebug() << "Icon URL: " << iconUrl;  // Debug to ensure the URL is correct
// Send the GET request to fetch the icon image
    QNetworkRequest iconRequest((QUrl(iconUrl)));
    QNetworkReply *iconReply = networkManager->get(iconRequest);
    connect(iconReply, &QNetworkReply::finished, [this, iconReply]() {
        if (iconReply->error() == QNetworkReply::NoError) {
            QByteArray iconData = iconReply->readAll();
            qDebug() << "Icon data size:" << iconData.size();  // Debug: check if data is received
            //QImage iconImage;
            if (iconImage.loadFromData(iconData)) {
                qDebug() << "Icon image loaded successfully!";
            } else {
                qDebug() << "Failed to load icon image!";
            }
        } else {
            qDebug() << "Error fetching icon:" << iconReply->errorString();
        }
// Clean up the icon reply
     iconReply->deleteLater();
    });
}

void WeatherStation::setMiasto(QString m){
    miasto = m;
    qDebug() << "Miasto";
    fetchWeatherData();
}
