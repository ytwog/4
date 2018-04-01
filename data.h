#ifndef DATA_H
#define DATA_H

#include <QSet>
#include <QVector>
#include <QString>
#include <QTime>


bool CheckTime(QString _time);

class Airports
{
public:
    Airports();
    ~Airports();
    void operator=(const Airports&);
    void connect(QString);
    void disconnect(QString);
    void setName(QString _name);
    void setCity(QString _city);

    QSet <QString> getConnections() const;
    QString getName() const;
    QString getCity() const;
private:
    QString Name;
    QString City;
    QSet <QString> connections;
};

class Flights
{
public:
    Flights();
    ~Flights();
    void operator=(const Flights&);
    void connect(QString);
    void disconnect(QString);
    void setName(QString _name);
    void setTakeOffTime(QTime _time);
    void setLandTime(QTime _time);
    void setModelType(QString _type);

    QSet <QString> getConnections() const;
    QString getName() const;
    QTime getLandTime() const;
    QTime getTakeOffTime() const;
    QString getModelType() const;
private:
    QString Name;
    QTime LandTime;
    QTime TakeOffTime;
    QString ModelType;
    QSet <QString> connections;
};


#endif // DATA_H
