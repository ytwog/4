#include "data.h"

bool CheckTime(QString _time)
{
    if(_time[0].isDigit() && _time[1].isDigit() && _time[2] == ':' && _time[3].isDigit() && _time[4].isDigit())
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Airports Class

Airports::Airports()
{
    Name = "";
    City = "";
    connections.clear();
}

Airports::~Airports()
{
    connections.clear();
}

void Airports::operator =(const Airports &Airport)
{
    setName(Airport.getName());
    setCity(Airport.getCity());
}

void Airports::connect(QString _Flight)
{
    connections.insert(_Flight);
}

void Airports::disconnect(QString _Flight)
{
    connections.remove(_Flight);
}

void Airports::setName(QString _name)
{
    Name = _name;
}

void Airports::setCity(QString _city)
{
    City = _city;
}

QSet <QString> Airports::getConnections() const
{
    return connections;
}

QString Airports::getName() const
{
    return Name;
}

QString Airports::getCity() const
{
    return City;
}


//Flights Class


Flights::Flights()
{
    Name = "";
    LandTime = QTime::fromString("00:00", "hh:mm");
    TakeOffTime = QTime::fromString("00:00", "hh:mm");
    ModelType = "";
}

Flights::~Flights()
{

}

void Flights::operator =(const Flights &Flight)
{
    setName(Flight.getName());
    setLandTime(Flight.getLandTime());
    setTakeOffTime(Flight.getTakeOffTime());
    setModelType(Flight.getModelType());
}

void Flights::connect(QString _Ports)
{
    connections.insert(_Ports);
}

void Flights::disconnect(QString _Ports)
{
    connections.remove(_Ports);
}

void Flights::setName(QString _name)
{
    Name = _name;
}

void Flights::setTakeOffTime(QTime _time)
{
    TakeOffTime = _time;
}

void Flights::setLandTime(QTime _time)
{
    LandTime = _time;
}

void Flights::setModelType(QString _type)
{
    ModelType = _type;
}

QSet <QString> Flights::getConnections() const
{
    return connections;
}

QString Flights::getName() const
{
    return Name;
}

QTime Flights::getLandTime() const
{
    return LandTime;
}

QTime Flights::getTakeOffTime() const
{
    return TakeOffTime;
}

QString Flights::getModelType() const
{
    return ModelType;
}
