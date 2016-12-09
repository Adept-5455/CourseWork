#include "ui_loginwindow.h"
#include "loginwindow.h"
#include "mainmenu.h"

#include <QSqlQuery>
#include <QSqlDataBase>
#include <QDebug>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    QSqlDatabase tempDB = QSqlDatabase::addDatabase("QSQLITE");
    tempDB.setDatabaseName("C:\\Users\\Adept\\Desktop\\Abiturients.db");

    if(tempDB.open()) qDebug() << "Success!" ;
    else qDebug() << "Error:" ;

    QSqlQuery query;
    query.exec("SELECT Логін, Пароль FROM Оператори");

    while(query.next())
    {
        LogPass tmpLogPass;
        tmpLogPass.login = query.value(0).toString();
        tmpLogPass.pass =  query.value(1).toString();
        LPVector.push_back(tmpLogPass);
    }

    tempDB.close();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_pushButton_clicked()
{

    foreach(LogPass templogpass, LPVector)
    {
        if(templogpass.login == ui->loginField->text()
                && templogpass.pass == ui->passwordField->text())
        {
            this->hide();
            MainMenu* menu = new MainMenu();
            menu->show();
            this->close();
            return;
        }
    }

    QMessageBox wrongLogPass;
    wrongLogPass.setText("Невірний логін або(і) пароль!");
    wrongLogPass.setWindowTitle("Помилка вводу!");
    wrongLogPass.setIcon(QMessageBox::Critical);
    wrongLogPass.exec();
}
