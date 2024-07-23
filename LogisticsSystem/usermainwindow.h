#ifndef USERMAINWINDOW_H
#define USERMAINWINDOW_H

#include <QMainWindow>
#include<QSqlTableModel>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>

//包裹块
class PakageBox:public QFrame
{
    Q_OBJECT
public:
    explicit PakageBox(QFrame* parent = nullptr);
    ~PakageBox();
    void PakageBox_Init();
    void paintEvent(QPaintEvent* ev);

    QFormLayout* flayout;
    QPushButton* infoButton;
    QPushButton* outstoreButton;
    int selfwidth;
    int selfheight;

    QString pid;
    QString pcompany;
    QString pstate;
    QString pkey="暂无";
    QString sid;
    QString sname="暂无";

private:

};

//订单块
class OrderBox:public QFrame
{
    Q_OBJECT
public:
    explicit OrderBox(QFrame* parent = nullptr);
    ~OrderBox();
    void OrderBox_Init();
    void paintEvent(QPaintEvent* ev);

    QFormLayout* flayout;
    QPushButton* infoButton;
    QPushButton* deleteButton;
    int selfwidth;
    int selfheight;

    QString oid;
    QString rname;
    QString rphone;
    QString rAddr;
    QString sid;
    QString sname;
    QString sphone;
    int ostate;
    QString pid;

private:

};

//寄件块
class PakageSendBox:public QFrame
{
    Q_OBJECT
public:
    explicit PakageSendBox(QFrame* parent = nullptr);
    ~PakageSendBox();
    void PakageSendBox_Init();
    void paintEvent(QPaintEvent* ev);

    QFormLayout* flayout;
    QPushButton* infoButton;
    int selfwidth;
    int selfheight;

    QString pid;
    QString pcompany;
    QString pstate;
    QString pkey="暂无";
    QString sid;
    QString sname="暂无";
    QString rname;
    QString rphone;
    QString raddr;

private:

};

//主界面
namespace Ui {
class UserMainwindow;
}

class UserMainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserMainwindow(QWidget *parent = nullptr);
    ~UserMainwindow();
    void paintEvent(QPaintEvent* ev);
    void Pakage_Function();
    void Order_Function();
    void PakageSend_Function();
    void UserMainwindow_clear();
    //来个重载版
    void Pakage_Function2(QString sql);
    void Order_Function2(QString sql);
    void PakageSend_Function2(QString sql);

    QSqlTableModel* model;
    QString Uaccount;
    QString Uname;
    QString Upassword;
    QString Uphone;

    QVector<PakageBox*> pakageList;
    QVector<OrderBox*> orderList;
    QVector<PakageSendBox*> spList;

private:
    Ui::UserMainwindow *ui;
};

#endif // USERMAINWINDOW_H
