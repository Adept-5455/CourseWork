#include "ui_loginwindow.h"
#include "loginwindow.h"
#include "mainmenu.h"

#include <QSqlQuery>
#include <QSqlDataBase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_pushButton_clicked()
{
    foreach(LogPass templogpass, LPVector)
    {
        qDebug() << templogpass.login << ' ' << templogpass.pass;
        if(templogpass.login == ui->loginField->text()
                && templogpass.pass == ui->passwordField->text())
        {
            successEnter = true;
            correct = templogpass;
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

void LoginWindow::on_exitBtn_clicked()
{
    this->close();
}
