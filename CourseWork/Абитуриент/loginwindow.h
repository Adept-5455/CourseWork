#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "ui_loginwindow.h"

struct LogPass
{
    QString login;
    QString pass;

    void clear()
    {
        login = "";
        pass = "";
    }
};

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
    Ui::LoginWindow *ui;
    QVector <LogPass> LPVector;
    LogPass correct;
    bool successEnter = false;

private slots:
    void on_pushButton_clicked();

    void on_exitBtn_clicked();

};

#endif // LOGINWINDOW_H
