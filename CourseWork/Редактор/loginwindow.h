#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QVector>

struct LogPass
{
    QString login;
    QString pass;
};

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
    Ui::LoginWindow *ui;
    QVector <LogPass> LPVector;
    bool successEnter = false;

private slots:
    void on_pushButton_clicked();

    void on_exitBtn_clicked();
};

#endif // LOGINWINDOW_H
