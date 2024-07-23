#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H

#include <QMainWindow>
#include<QSqlTableModel>

namespace Ui {
class ManagerWindow;
}

class ManagerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerWindow(QWidget *parent = nullptr);
    ~ManagerWindow();
    void paintEvent(QPaintEvent* ev);

    QSqlTableModel* model;
    QString Maccount;
    QString Mname;
//    QString Mpassword;
    QString Mphone;
    QString Msid;
    QString Msname;
    QString Mspos;

private:
    Ui::ManagerWindow *ui;
};

#endif // MANAGERWINDOW_H
