#ifndef UPAKAGE_H
#define UPAKAGE_H

#include <QWidget>
#include <QDialog>
#include <usermainwindow.h>

namespace Ui {
class UPakageQuery;
};

//包裹查询页
class UPakageQuery : public QDialog
{
    Q_OBJECT
public:
    explicit UPakageQuery(QDialog *parent = nullptr);
    ~UPakageQuery();
    void UPakageQuery_Init();

    UserMainwindow* parentWid;

private:
    Ui::UPakageQuery* ui;

signals:

};

#endif // UPAKAGE_H
