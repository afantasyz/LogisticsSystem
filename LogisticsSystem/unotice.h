#ifndef UNOTICE_H
#define UNOTICE_H

#include <QDialog>
#include <QFrame>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QFormLayout>
//#include "usermainwindow.h"

namespace Ui {
class UPakageNotice;
class UPaymentNotice;
}

//定义消息栏
class PakageNoticeBox:public QFrame
{
    Q_OBJECT
public:
    explicit PakageNoticeBox(QFrame *parent = nullptr);
    ~PakageNoticeBox();
    void PakageNoticeBox_Init();

    QVBoxLayout* vlayout;
    QPushButton* button;

    QString nid;
    QString uid;
    QString key;
    QString sid;
    QString pid;

    QString sname;
    QString spos;
    bool isRead;
};

//定义支付凭证栏
class PaymentBox:public QFrame
{
    Q_OBJECT
public:
    explicit PaymentBox(QFrame *parent = nullptr);
    ~PaymentBox();
    void PaymentBox_Init();

    QFormLayout* flayout;
    QPushButton* button;

    QString payid;
    QString oid;
    float price;
    QString paytime;
    QString uid;
};

//取件消息通知

class UPakageNotice : public QDialog
{
    Q_OBJECT
public:
    explicit UPakageNotice(QDialog *parent = nullptr);
    ~UPakageNotice();
    void UPakageNotice_Init();

    QString uid;
    QVector<PakageNoticeBox*> pakageboxList;

private:
    Ui::UPakageNotice *ui;

signals:

};

//支付凭证查看
class UPaymentNotice:public QDialog
{
    Q_OBJECT
public:
    explicit UPaymentNotice(QDialog* parent = nullptr);
    ~UPaymentNotice();
    void UPaymentNotice_Init();

    QString uid;
    QVector<PaymentBox*> payboxList;

private:
    Ui::UPaymentNotice *ui;
};

#endif // UNOTICE_H
