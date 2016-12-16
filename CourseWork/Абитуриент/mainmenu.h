#ifndef MAINMENU_H
#define MAINMENU_H

#include "loginwindow.h"

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>

#include <QFile>

#include "dbinterface.h"

namespace Ui {
class MainMenu;
}

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = 0);
    ~MainMenu();

private slots:
    void on_surnameCheckBox_clicked();
    void on_specialityCheckBox_clicked();
    void on_dateCheckBox_clicked();
    void on_addBtn_clicked();

    void connectToDB();
    void updateTable();
    void insertData();
    void checkInput();
    void changeFindBtnState();
    void setHeaderData();
    void generateXML();

    void on_findBtn_clicked();

    void on_refreshBtn_clicked();

private:
    Ui::MainMenu *ui;
    LoginWindow LW;
    DBInterface DBI;
    QSqlQueryModel* abitListModel;
};

#endif // MAINMENU_H
