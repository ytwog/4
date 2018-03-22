#include "data.h"

Data::Data()
{

}

Data::~Data()
{
    connections.clear();
}



Data::Data(QString _name, QSet<QString> _cons)
{
    name = _name;
    connections = _cons;
}

void Data::CreateConnection(QString name)
{
    connections.insert(name);
}


void Data::EraseConnection(QString name)
{
    connections.remove(name);
}

QString Data::getName()
{
    return name;
}

QSet <QString> Data::getCon()
{
    return connections;
}
