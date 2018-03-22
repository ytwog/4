#ifndef DATA_H
#define DATA_H

#include <QSet>
#include <QVector>
#include <QString>

class Data
{
public:
    Data();
    Data(QString _name, QSet <QString> _cons);
    ~Data();
    void CreateConnection(QString name);
    void EraseConnection(QString name);
    QString getName();
    QSet <QString> getCon();
private:
    QString name;
    QSet <QString> connections;
};



#endif // DATA_H
