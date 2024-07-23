#ifndef UCREATEORDER_H
#define UCREATEORDER_H

#include <QWidget>
#include <QDialog>
#include <usermainwindow.h>

//创建订单的UI
namespace Ui {
class UCreateOrder;
}

class UCreateOrder : public QWidget
{
    Q_OBJECT

public:
    explicit UCreateOrder(QWidget *parent = nullptr);
    ~UCreateOrder();

    void Info_Init(void);
    void Creat_order();

    UserMainwindow* parentWidegt;

private:
    Ui::UCreateOrder *ui;
};

//查询订单的UI
namespace Ui {
class UQueryOrder;
}

class UQueryOrder : public QDialog
{
    Q_OBJECT

public:
    explicit UQueryOrder(QDialog *parent = nullptr);
    ~UQueryOrder();
    void MorderQuery_Init();

    UserMainwindow* parentWid;

private:
    Ui::UQueryOrder *ui;
};

#endif // UCREATEORDER_H
