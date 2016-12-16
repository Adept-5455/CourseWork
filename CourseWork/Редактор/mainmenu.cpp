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

    this->setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    QFile styleF;

    styleF.setFileName(":/Style/Style/QSS.css");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();

    qApp->setStyleSheet(qssStr);

    connect(ui->loadFromDB, SIGNAL(triggered(bool)), this, SLOT(connectToDB()));
    connect(ui->exit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->generateXML, SIGNAL(triggered(bool)), this, SLOT(generateXML()));

    connect(this, SIGNAL(needUpdate()), this, SLOT(updateTables()));
    connect(AOW.ui->addBtn, SIGNAL(clicked()), this, SLOT(updateTables()));
    connect(LW.ui->pushButton, SIGNAL(clicked()), this, SLOT(updateTables()));

    ui->generateXML->setEnabled(false);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::generateXML()
{
    QString fileName = "";
    fileName = QFileDialog::getSaveFileName(this,
        tr("Збереження XML"), "/home/ZNTU_Abiturient_XML.xml", tr("XML (*.xml)"));

    if(fileName == "")
    {
        return;
    }

    qDebug() << "XML to save:" << fileName;

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.setCodec("windows-1251");
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("database");

    xmlWriter.writeStartElement("operators");

    for(int i=0; i<ui->operatorsTable->model()->rowCount(); i++)
    {
        xmlWriter.writeStartElement("operator");
        xmlWriter.writeAttribute("login",       ui->operatorsTable->model()->data
                                                (ui->operatorsTable->model()->index(i, 0)).toString());
        xmlWriter.writeAttribute("password",    ui->operatorsTable->model()->data
                                                (ui->operatorsTable->model()->index(i, 1)).toString());
        xmlWriter.writeAttribute("fullname",    ui->operatorsTable->model()->data
                                                (ui->operatorsTable->model()->index(i, 2)).toString());
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("abiturients");
    for(int i=0; i<ui->ratingTable->model()->rowCount(); i++)
    {
        xmlWriter.writeStartElement("abiturient");
        xmlWriter.writeAttribute("surname",         ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 0)).toString());
        xmlWriter.writeAttribute("name",            ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 1)).toString());
        xmlWriter.writeAttribute("fathersname",     ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 2)).toString());
        xmlWriter.writeAttribute("dateofbirth",     ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 3)).toString());
        xmlWriter.writeAttribute("passport",        ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 4)).toString());
        xmlWriter.writeAttribute("id",              ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 5)).toString());
        xmlWriter.writeAttribute("telephone",       ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 6)).toString());
        xmlWriter.writeAttribute("address",         ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 7)).toString());
        xmlWriter.writeAttribute("level",           ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 8)).toString());
        xmlWriter.writeAttribute("rating",          ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 9)).toString());
        xmlWriter.writeAttribute("form",            ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 10)).toString());
        xmlWriter.writeAttribute("speciality",      ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 11)).toString());
        xmlWriter.writeAttribute("outsider",        ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 12)).toString());
        xmlWriter.writeAttribute("chumneed",        ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 13)).toString());
        xmlWriter.writeAttribute("dateofadding",    ui->ratingTable->model()->data
                                                    (ui->ratingTable->model()->index(i, 14)).toString());      
        xmlWriter.writeEndElement();
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

void MainMenu::updateTables()
{   
    if(LW.successEnter == false)
    {
        return;
    }

    QSqlQuery query;
    query.exec(" SELECT [ПІБ] FROM Редактори WHERE [Логін] = '" + LW.correct.login + "' ");
    query.next();
    ui->statusLabel->setText("Оператор: " + query.value(0).toString());

    ui->generateXML->setEnabled(true);
    ui->tabWidget->setEnabled(true);
    ui->addBtn->setEnabled(true);
    operatorsModel = new QSqlTableModel(this, DBI.DB());
    ratingModel = new QSqlTableModel(this, DBI.DB());

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

    DBI.connectToDB(fileName);
    DBI.selectFromDB("Оператори");

    if(DBI.DB().databaseName()!="")
    {
        ui->operatorsTable->setModel(NULL);
        ui->ratingTable->setModel(NULL);

        ui->addBtn->setEnabled(false);
        ui->delBtn->setEnabled(false);
        ui->delBtn_2->setEnabled(false);
        ui->tabWidget->setEnabled(false);
        ui->generateXML->setEnabled(false);
        ui->statusLabel->setText("Слід обрати БД для завантаження в меню \"Файл\".");

        LW.LPVector.clear();
        LW.correct.clear();

        QSqlQuery query = DBI.execQuery("SELECT * FROM Редактори");

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

    foreach(QModelIndex index, indexes)
    {
        QSqlRecord record = operatorsModel->record(index.row());
        QString toDelete = record.value("Логін").toString();

        DBI.deleteFromDB("Оператори", "Логін", toDelete);
    }

    emit needUpdate();

}

void MainMenu::on_delBtn_2_clicked()
{
    ui->delBtn_2->setEnabled(false);

    foreach(QModelIndex index, indexes)
    {
        QSqlRecord record = ratingModel->record(index.row());
        QString toDelete = record.value("Паспорт").toString();

        DBI.deleteFromDB("Абітурієнти", "Паспорт", toDelete);
    }

    emit needUpdate();
}

void MainMenu::on_ratingTable_clicked(const QModelIndex &index)
{
    ui->delBtn_2->setEnabled(true);
    indexes = ui->ratingTable->selectionModel()->selection().indexes();
}
