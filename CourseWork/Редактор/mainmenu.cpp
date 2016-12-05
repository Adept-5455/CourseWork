#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "addoperatorwindow.h"

#include <QtSql>


MainMenu::MainMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu)
{ 
    ui->setupUi(this);

    connect(ui->loadFromDB, SIGNAL(triggered(bool)), this, SLOT(loadingDB()));
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::updateTable()
{
    QSqlTableModel* model = new QSqlTableModel();
    model = new QSqlTableModel(this, db);
    model->setTable("Оператори");
    model->select();
    model->setSort(0,Qt::AscendingOrder);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);

    ui->operatorsTable->setModel(model);
}

void MainMenu::loadingDB()
{  
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\Adept\\Desktop\\ZNTU_Abiturient.db"); //Прописать путь

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

    updateTable();
}

void MainMenu::on_addBtn_clicked()
{
    AddOperatorWindow* AOW = new AddOperatorWindow();
    AOW->setModal(this);
    AOW->show();

    updateTable();
}
