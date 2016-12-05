#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QVector>

struct LogPass
{
    QString login;
    QString pass;
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

private slots:
    void on_pushButton_clicked();

private:
    Ui::LoginWindow *ui;
    QVector <LogPass> LPVector;
};

#endif // LOGINWINDOW_H
