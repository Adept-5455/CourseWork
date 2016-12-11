#include "mainmenu.h"
#include "loginwindow.h"
#include "addoperatorwindow.h"

#include "ui_mainmenu.h"
#include "ui_addoperatorwindow.h"
#include "ui_loginwindow.h"

#include <QtSql>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>

#include <QFileDialog>

#include <QVariant>
#include <QStandardItemModel>
#include <QAbstractItemModel>

MainMenu::MainMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu)
{ 
    ui->setupUi(this);

    connect(ui->loadFromDB, SIGNAL(triggered(bool)), this, SLOT(connectToDB()));
    connect(ui->exit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->generateXML, SIGNAL(triggered(bool)), this, SLOT(generateXML()));
    connect(ui->action_XML, SIGNAL(triggered(bool)), this, SLOT(loadFromXML()));

    connect(this, SIGNAL(needUpdate()), this, SLOT(updateTables()));
    connect(AOW.ui->addBtn, SIGNAL(clicked()), this, SLOT(updateTables()));
    connect(LW.ui->pushButton, SIGNAL(clicked()), this, SLOT(updateTables()));
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::generateXML()
{

    QSqlQuery query;
    query.exec("SELECT * FROM Редактори");

    QFile file("C:\\Users\\Adept\\Desktop\\test.xml");
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    //xmlWriter.setCodec("windows-1251");
    xmlWriter.writeStartElement("database");
    xmlWriter.writeStartElement("redactors");

    while(query.next())
    {
        xmlWriter.writeStartElement("redactor");
        xmlWriter.writeAttribute("login", query.value(0).toString());
        xmlWriter.writeAttribute("password", query.value(1).toString());
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();

    query.exec("SELECT * FROM Оператори");
    xmlWriter.writeStartElement("operators");

    while(query.next())
    {
        xmlWriter.writeStartElement("operator");
        xmlWriter.writeAttribute("login", query.value(0).toString());
        xmlWriter.writeAttribute("password", query.value(1).toString());
        xmlWriter.writeAttribute("fullname", query.value(2).toString());
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();

    query.exec("SELECT * FROM Абітурієнти");
    xmlWriter.writeStartElement("abiturients");

    while(query.next())
    {
        xmlWriter.writeStartElement("abiturient");
        xmlWriter.writeAttribute("surname", query.value(0).toString());
        xmlWriter.writeAttribute("name", query.value(1).toString());
        xmlWriter.writeAttribute("fathersname", query.value(2).toString());
        xmlWriter.writeAttribute("dateofbirth", query.value(3).toString());
        xmlWriter.writeAttribute("passport", query.value(4).toString());
        xmlWriter.writeAttribute("id", query.value(5).toString());
        xmlWriter.writeAttribute("telephone", query.value(6).toString());
        xmlWriter.writeAttribute("address", query.value(7).toString());
        xmlWriter.writeAttribute("level", query.value(8).toString());
        xmlWriter.writeAttribute("rating", query.value(9).toString());
        xmlWriter.writeAttribute("form", query.value(10).toString());
        xmlWriter.writeAttribute("speciality", query.value(11).toString());
        xmlWriter.writeAttribute("outsider", query.value(12).toString());
        xmlWriter.writeAttribute("chumneed", query.value(13).toString());
        xmlWriter.writeAttribute("dateofadding", query.value(14).toString());
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();

    xmlWriter.writeEndDocument();

    file.close();
}

void MainMenu::setHeadersData(QAbstractItemModel* abits, QAbstractItemModel* operators)
{
    operators->setHeaderData(0, Qt::Horizontal, tr("Логін"));
    operators->setHeaderData(1, Qt::Horizontal, tr("Пароль"));
    operators->setHeaderData(2, Qt::Horizontal, tr("ПІБ"));

    abits->setHeaderData(0, Qt::Horizontal, tr("Прізвище"));
    abits->setHeaderData(1, Qt::Horizontal, tr("Ім'я"));
    abits->setHeaderData(2, Qt::Horizontal, tr("По-батькові"));
    abits->setHeaderData(3, Qt::Horizontal, tr("Дата народження"));
    abits->setHeaderData(4, Qt::Horizontal, tr("Паспорт"));
    abits->setHeaderData(5, Qt::Horizontal, tr("ІД номер"));
    abits->setHeaderData(6, Qt::Horizontal, tr("Телефон"));
    abits->setHeaderData(7, Qt::Horizontal, tr("Адреса"));
    abits->setHeaderData(8, Qt::Horizontal, tr("ОКР"));
    abits->setHeaderData(9, Qt::Horizontal, tr("Бал"));
    abits->setHeaderData(10, Qt::Horizontal, tr("Форма навчання"));
    abits->setHeaderData(11, Qt::Horizontal, tr("Спеціальність"));
    abits->setHeaderData(12, Qt::Horizontal, tr("ПК"));
    abits->setHeaderData(13, Qt::Horizontal, tr("Гуртожиток"));
    abits->setHeaderData(14, Qt::Horizontal, tr("Дата внесення"));
}

void MainMenu::loadFromXML()
{
    QFile file("C:\\Users\\Adept\\Desktop\\test.xml");
    file.open(QIODevice::ReadOnly);

    QXmlStreamReader* xmlReader = new QXmlStreamReader;
    xmlReader->setDevice(&file);

    QStandardItemModel* operatorsXMLModel = new QStandardItemModel();
    QStandardItemModel* abitXMLModel = new QStandardItemModel();

    xmlReader->readNext();
    qDebug() << xmlReader->name();

    while(xmlReader->name() != "operators" )
        xmlReader->readNext();

    xmlReader->readNext();
    qDebug() << xmlReader->name();

    while(!(xmlReader->atEnd()))
    {
        qDebug() << xmlReader->name();
        if(xmlReader->name() == "operator" && xmlReader->isStartElement())
        {
            QList <QStandardItem*> dataToPush;
            foreach(const QXmlStreamAttribute &attr, xmlReader->attributes())
            {
                QStandardItem* item = new QStandardItem(attr.value().toString());
                dataToPush << item;
            }

            operatorsXMLModel->insertRow(operatorsXMLModel->rowCount(), dataToPush);
            dataToPush.clear();
        }

        else if(xmlReader->name() == "abiturient" && xmlReader->isStartElement())
        {
            QList <QStandardItem*> dataToPush;
            foreach(const QXmlStreamAttribute &attr, xmlReader->attributes())
            {
                QStandardItem* item = new QStandardItem(attr.value().toString());
                dataToPush << item;
                qDebug() << item->text();
            }

            abitXMLModel->insertRow(abitXMLModel->rowCount(), dataToPush);
            dataToPush.clear();
        }

        xmlReader->readNext();

    }



    setHeadersData(abitXMLModel, operatorsXMLModel);

    qDebug() << "headers";

    ui->operatorsTable->setModel(operatorsXMLModel);
    ui->operatorsTable->resizeColumnsToContents();
    ui->operatorsTable->resizeRowsToContents();

    ui->ratingTable->setModel(abitXMLModel);
    ui->ratingTable->resizeRowsToContents();
    ui->ratingTable->resizeColumnsToContents();

    file.close();

    ui->tabWidget->setEnabled(true);
    ui->addBtn->setEnabled(true);
}

void MainMenu::updateTables()
{   
    if(LW.successEnter == false)
    {
        return;
    }

    ui->tabWidget->setEnabled(true);
    ui->addBtn->setEnabled(true);
    operatorsModel = new QSqlTableModel(this, db);
    ratingModel = new QSqlTableModel(this, db);

    operatorsModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    operatorsModel->setTable("Оператори");
    operatorsModel->select();

    ratingModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    ratingModel->setTable("Абітурієнти");
    ratingModel->select();

    setHeadersData(ratingModel, operatorsModel);

    ui->operatorsTable->setModel(operatorsModel);
    ui->operatorsTable->setSelectionBehavior(QTableView::SelectRows);
    ui->operatorsTable->resizeColumnsToContents();
    ui->operatorsTable->resizeRowsToContents();

    ui->ratingTable->setModel(ratingModel);
    ui->ratingTable->setSelectionBehavior(QTableView::SelectRows);
    ui->ratingTable->resizeColumnsToContents();
    ui->ratingTable->resizeRowsToContents();

}

void MainMenu::connectToDB()
{
    QString fileName = "";
    fileName = QFileDialog::getOpenFileName(this,
        tr("Відкриття бази даних"), "/home/ZNTU_Abiturient_DB.db", tr("База даних (*.db)"));

    if(fileName == "")
    {
        return;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    qDebug() << db.databaseName();

    if(db.open())
    {
        qDebug() << "Success!";

        foreach(QString t, db.tables())
        {
            qDebug() << t ;
        }
    }

    else
    {
        qDebug() << "Fail!";
    }


    if(db.databaseName()!="")
    {
        ui->operatorsTable->setModel(NULL);
        ui->ratingTable->setModel(NULL);

        ui->addBtn->setEnabled(false);
        ui->delBtn->setEnabled(false);
        ui->delBtn_2->setEnabled(false);
        ui->tabWidget->setEnabled(false);

        LW.LPVector.clear();

        QSqlQuery query;
        query.exec("SELECT Логін, Пароль FROM Редактори");

        while(query.next())
        {

            LogPass tmpLogPass;

            tmpLogPass.login = query.value(0).toString();
            tmpLogPass.pass =  query.value(1).toString();

            LW.LPVector.push_back(tmpLogPass);

            qDebug() << tmpLogPass.login << ' ' << tmpLogPass.pass;
        }

        LW.successEnter = false;

        LW.ui->loginField->clear();
        LW.ui->passwordField->clear();

        LW.setModal(this);
        LW.show();
    }

}

void MainMenu::on_addBtn_clicked()
{
    AOW.ui->loginField->clear();
    AOW.ui->passwordField->clear();
    AOW.ui->nameField->clear();
    AOW.ui->fathersNameField->clear();;
    AOW.ui->surnameField->clear();

    AOW.setModal(this);
    AOW.setWindowTitle("Новий оператор");
    AOW.show();
}


void MainMenu::on_operatorsTable_clicked(const QModelIndex &index)
{
    ui->delBtn->setEnabled(true);
    indexes = ui->operatorsTable->selectionModel()->selection().indexes();
}

void MainMenu::on_delBtn_clicked()
{
    ui->delBtn->setEnabled(false);

    QSqlQuery query;

    foreach(QModelIndex index, indexes)
    {
        QSqlRecord record = operatorsModel->record(index.row());
        QString toDelete = record.value("Логін").toString();

        query.prepare("DELETE FROM Оператори WHERE Логін = :log");
        query.bindValue(":log", toDelete);
        query.exec();
    }

    emit needUpdate();

}

void MainMenu::on_delBtn_2_clicked()
{
    ui->delBtn_2->setEnabled(false);

    QSqlQuery query;

    foreach(QModelIndex index, indexes)
    {
        QSqlRecord record = ratingModel->record(index.row());
        QString toDelete = record.value("Паспорт").toString();

        query.prepare("DELETE FROM Абітурієнти WHERE  Паспорт = :passport");
        query.bindValue(":passport", toDelete);
        query.exec();
    }

    emit needUpdate();
}

void MainMenu::on_ratingTable_clicked(const QModelIndex &index)
{
    ui->delBtn_2->setEnabled(true);
    indexes = ui->ratingTable->selectionModel()->selection().indexes();
}
