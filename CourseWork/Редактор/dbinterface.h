#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>

#include <QString>
#include <QException>

class DBInterface
{
    QSqlDatabase db;
public:
    DBInterface();
    DBInterface(DBInterface&);
    DBInterface(QString);

    QSqlDatabase DB()
    {
        return db;
    }

    void connectToDB(QString);
    void setDBName(QString);
    void selectFromDB(QString);
    void deleteFromDB(QString, QString, QString);
    QSqlQuery execQuery(QString);
};

#endif // DBINTERFACE_H
