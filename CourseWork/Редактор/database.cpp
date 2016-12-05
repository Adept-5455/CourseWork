#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        dbFileName = "ZNTU_Abiturient.db";
        db.setDatabaseName( dbFileName );
    }

DataBase::~DataBase()
    {
        db.close ();
    }

bool DataBase::connectToDataBase()
    {


        if(db.open ())
            {
                qDebug() << "          Successfully connected to database." + '\n';
                qDebug() << "          Tables aviable in database: " + '\n' ;
                foreach( QString t , db.tables() )
                    qDebug() << t ;
                return true;
            }
        else
            qDebug()    << "          Database connection error." + '\n' ;
            return false ;



    }

bool DataBase::insertIntoTable(const QVariantList &data)
    {
        QSqlQuery query;

        query.prepare("INSERT INTO carinfo ( ImageLink, Brand, Name,"
                                                "Price, Color, Engine, "
                                                 "Transmission, Drivetrain, HP, timeFor100ph )"
                      "VALUES ( :ImageLink, :Brand, :Name, :Price, "
                                ":Color, :Engine, :Transmission, :Drivetrain, :HP, :timeFor100ph )");

        query.bindValue(":ImageLink", data[0]);
        query.bindValue(":Brand",   data[1]);
        query.bindValue(":Name",    data[2]);
        query.bindValue(":Price",   data[3]);
        query.bindValue(":Color",   data[4]);
        query.bindValue(":Engine",  data[5]);
        query.bindValue(":Transmission", data[6]);
        query.bindValue(":Drivetrain",   data[7]);
        query.bindValue(":HP",      data[8] );
        query.bindValue(":timeFor100ph", data[9] );

        if( !query.exec() )
            {
                qDebug() << "Error during operation INSERT into table: ";
                qDebug() << query.lastError().text();
                return false;
            }
        else
            qDebug() << "Operation 'insert' has been done.";

        return true;
    }

bool DataBase::deleteFromTable(const QVariantList &data)
    {
        QSqlQuery query;

        query.prepare("DELETE FROM carinfo WHERE "
                        "ID = :ID" );

        query.bindValue( ":ID",     data[0].toInt ());

        if( !query.exec() )
            {
                qDebug() << "Error during operation DELETE into table: ";
                qDebug() << query.lastError().text();
                return false;
            }
        else
           qDebug() << "Operation 'insert' has been done.",
           qDebug() << "Item with ID:" + data[0].toString () + " has been deleted.";

        return true;
    }

bool DataBase::updateIntoTable(const QVariantList &data)
    {
        QSqlQuery query;

        query.prepare("UPDATE carinfo SET "
                        "ImageLink=:ImageLink, Brand=:Brand, Name=:Name, "
                        "Price=:Price, Color=:Color,"
                        "Engine=:Engine, Transmission=:Transmission, "
                        "Drivetrain=:Drivetrain, HP=:HP, "
                        "timeFor100ph=:timeFor100ph "
                        "WHERE ID = :ID" );

        query.bindValue(":ID",   data[0]);
        query.bindValue(":ImageLink",   data[1]);
        query.bindValue(":Brand",       data[2]);
        query.bindValue(":Name",        data[3]);
        query.bindValue(":Price",       data[4]);
        query.bindValue(":Color",       data[5]);
        query.bindValue(":Engine",      data[6]);
        query.bindValue(":Transmission", data[7]);
        query.bindValue(":Drivetrain",   data[8]);
        query.bindValue(":HP",          data[9] );
        query.bindValue(":timeFor100ph", data[10] );

        if( !query.exec() )
            {
                qDebug() << "Error during operation UPDATE into table: ";
                qDebug() << query.lastError().text();
                return false;
            }

        return true;
    }
