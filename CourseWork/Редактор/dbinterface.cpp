#include "dbinterface.h"

DBInterface::DBInterface()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

DBInterface::DBInterface(QString dbName)
{
    connectToDB(dbName);
}

DBInterface::DBInterface(DBInterface &Other)
{
    this->db.addDatabase("QSQLITE");
    this->db.setDatabaseName(Other.db.databaseName());

    try
    {
        if(!db.open()) throw QException();
        else qDebug() << "DB Successfully opened!";
    }

    catch(QException)
    {
        qDebug() << db.lastError();
    }
}

void DBInterface::connectToDB(QString dbName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    qDebug() << db.databaseName();

    try
    {
        if(!db.open()) throw QException();
        else
        {
            qDebug() << "DB Successfully opened!";

            qDebug() << "Avaiable tables: ";
            foreach(QString table, db.tables())
            {
                qDebug() << table ;
            }
        }
    }

    catch(QException)
    {
        qDebug() << db.lastError();
    }
}

void DBInterface::selectFromDB(QString tableName)
{
    QSqlQuery query;

    try
    {
        if(!query.exec("SELECT * FROM " + tableName)) throw QException();
    }

    catch(QException)
    {
        qDebug() << query.lastError();
    }

}

void DBInterface::deleteFromDB(QString tableName, QString parameter, QString value)
{
    QSqlQuery query;

    query.prepare("DELETE FROM " + tableName + " WHERE " +
                  "[" + parameter + "]" + " = :value");
    query.bindValue(":value", value);
    query.exec();
}

QSqlQuery DBInterface::execQuery(QString queryText)
{
    QSqlQuery query;

    try
    {
        if(!query.exec(queryText)) throw QException();
    }

    catch(QException)
    {
        qDebug() << query.lastError();
    }

    return query;
}

void DBInterface::setDBName(QString dbName)
{
    db.setDatabaseName(dbName);
}
