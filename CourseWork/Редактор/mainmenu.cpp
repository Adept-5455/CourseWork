#include "mainmenu.h"
#include "loginwindow.h"
#include "addoperatorwindow.h"

#include "ui_mainmenu.h"
#include "ui_addoperatorwindow.h"
#include "ui_loginwindow.h"

#include <QtSql>
#include <QFileDialog>

MainMenu::MainMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu)
{ 
    ui->setupUi(this);

    connect(ui->loadFromDB, SIGNAL(triggered(bool)), this, SLOT(connectToDB()));
    connect(ui->exit, SIGNAL(triggered(bool)), this, SLOT(close()));

    connect(this, SIGNAL(needUpdate()), this, SLOT(updateTables()));
    connect(AOW.ui->addBtn, SIGNAL(clicked()), this, SLOT(updateTables()));
    connect(LW.ui->pushButton, SIGNAL(clicked()), this, SLOT(updateTables()));
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::updateTables()
{   
    if(LW.successEnter == false)
    {
        return;
    }

    ui->addBtn->setEnabled(true);
    operatorsModel = new QSqlTableModel(this, db);
    ratingModel = new QSqlTableModel(this, db);

    operatorsModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    operatorsModel->setTable("Оператори");
    operatorsModel->select();

    operatorsModel->setHeaderData(0, Qt::Horizontal, tr("Логін"));
    operatorsModel->setHeaderData(1, Qt::Horizontal, tr("Пароль"));
    operatorsModel->setHeaderData(2, Qt::Horizontal, tr("ПІБ"));

    ui->operatorsTable->setModel(operatorsModel);
    ui->operatorsTable->setSelectionBehavior(QTableView::SelectRows);
    ui->operatorsTable->resizeColumnsToContents();
    ui->operatorsTable->resizeRowsToContents();

    ratingModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    ratingModel->setTable("Абітурієнти");
    ratingModel->select();

    ratingModel->setHeaderData(0, Qt::Horizontal, tr("Прізвище"));
    ratingModel->setHeaderData(1, Qt::Horizontal, tr("Ім'я"));
    ratingModel->setHeaderData(2, Qt::Horizontal, tr("По-батькові"));
    ratingModel->setHeaderData(3, Qt::Horizontal, tr("Дата народження"));
    ratingModel->setHeaderData(4, Qt::Horizontal, tr("Паспорт"));
    ratingModel->setHeaderData(5, Qt::Horizontal, tr("ІД номер"));
    ratingModel->setHeaderData(6, Qt::Horizontal, tr("Телефон"));
    ratingModel->setHeaderData(7, Qt::Horizontal, tr("Адреса"));
    ratingModel->setHeaderData(8, Qt::Horizontal, tr("ОКР"));
    ratingModel->setHeaderData(9, Qt::Horizontal, tr("Бал"));
    ratingModel->setHeaderData(10, Qt::Horizontal, tr("Форма навчання"));
    ratingModel->setHeaderData(11, Qt::Horizontal, tr("Спеціальність"));
    ratingModel->setHeaderData(12, Qt::Horizontal, tr("ПК"));
    ratingModel->setHeaderData(13, Qt::Horizontal, tr("Гуртожиток"));
    ratingModel->setHeaderData(14, Qt::Horizontal, tr("Дата внесення"));

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

void MainMenu::on_updBtn_clicked()
{
    delete operatorsModel;
    emit needUpdate();
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
