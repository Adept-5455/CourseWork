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

void DBInterface::addToDB(QString tableName, QStringList parameterName, QVariantList parameterValue)
{
    QSqlQuery query;
    QString queryText = "INSERT INTO " + tableName + "(";

    for(int i=0; i<parameterName.count()-1; i++)
    {
        queryText += " [" + parameterName[i] + "], ";
    }

    queryText += "[" + parameterName[parameterName.count() - 1] + "]) VALUES (";

    for(char i=0; i<parameterValue.count()-1; i++)
    {
        queryText += ":p";
        queryText += 'A'+i;
        queryText += ", ";
    }

    queryText += ":p";
    queryText += (parameterValue.count()-1 + 'A');
    queryText += ")";

    query.prepare(queryText);

    qDebug() << "Query Text: " << queryText;

    for(int i=0; i<parameterValue.count(); i++)
    {
        query.bindValue(i, parameterValue[i]);
    }

    try
    {
        if(!query.exec())
        {
            throw QException();
        }
    }

    catch(QException)
    {
        qDebug() << query.lastError().text();
    }
}

QSqlQuery DBInterface::selectWithFilters(QString defaultQueryText, QStringList parameterName, QVariantList parameterValue)
{
    QString queryText = defaultQueryText += " WHERE ";

    if(parameterName.count() == 1)
    {
        queryText += "[" + parameterName[0] + "] = " + "'" + parameterValue[0].toString() + "'";
    }

    else
    {
        for(int i=0; i<parameterName.count()-1; i++)
        {
            queryText += "[" + parameterName[i] + "] = " + "'" + parameterValue[i].toString() + "'" + " AND ";
        }

        queryText += "[" + parameterName[parameterName.count()-1] + "] = "
                + "'" + parameterValue[parameterValue.count()-1].toString() + "'";
    }

    queryText += " ORDER BY [Рейтинговий бал] DESC";
    qDebug() << queryText;

    QSqlQuery query;

    try
    {
        if(!query.exec(queryText)) throw QException();
    }

    catch(QException)
    {
        qDebug() << query.lastError().text();
    }

    return query;
}
