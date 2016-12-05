#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QSqlTableModel>

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
    void on_addBtn_clicked();
    void loadingDB();
    void updateTable();

private:
    Ui::MainMenu *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
};

#endif // MAINMENU_H
