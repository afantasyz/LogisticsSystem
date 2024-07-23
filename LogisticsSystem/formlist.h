#ifndef FORMLIST_H
#define FORMLIST_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

namespace Ui
{
class OrderForm;
class PakageForm;
class PaymentForm;
class OrderShowForm;
class PaymentShowForm;
class PakageShowForm;
};

class OrderForm:public QDialog
{
    Q_OBJECT
public:
    explicit OrderForm(QDialog* parent = nullptr);
    OrderForm(QDialog* parent = nullptr,QString oid=NULL);
    ~OrderForm();

    QString oid;

private:
    Ui::OrderForm* ui;
};

class PakageForm:public QDialog
{
    Q_OBJECT
public:
    explicit PakageForm(QDialog* parent = nullptr);
    PakageForm(QDialog* parent = nullptr,QString pid=NULL);
    ~PakageForm();

    QString pid;
    QString oid;
    QString payid;

private:
    Ui::PakageForm* ui;
};

class PaymentForm:public QDialog
{
    Q_OBJECT
public:
    explicit PaymentForm(QDialog* parent = nullptr);
    PaymentForm(QDialog* parent = nullptr,QString payid=NULL);
    ~PaymentForm();

    QString pid;
    QString oid;
    QString payid;

private:
    Ui::PaymentForm* ui;
};

class OrderShowForm:public  QDialog
{
    Q_OBJECT
public:
    explicit OrderShowForm(QDialog* parent=nullptr);
    OrderShowForm(QDialog* parent = nullptr,QString oid=NULL);
    ~OrderShowForm();

    QString oid;

private:
    Ui::OrderShowForm* ui;
};

class PaymentShowForm:public QDialog
{
    Q_OBJECT
public:
    explicit PaymentShowForm(QDialog* parent = nullptr);
//    PaymentShowForm(QDialog* parent = nullptr,QString payid=NULL);
    ~PaymentShowForm();

    QString pid;
    QString oid;
    QString payid;

    QLineEdit* line_payid;
    QLineEdit* line_price;
    QLineEdit* line_time;
    QLineEdit* line_Id;
    QLineEdit* line_phone;
    QLineEdit* line_name;
    QLabel* label_oid;
    QPushButton* button_submit;

private:
    Ui::PaymentShowForm* ui;
};

class PakageShowForm:public QDialog
{
    Q_OBJECT
public:
    PakageShowForm(QDialog* parent=nullptr);
    void PakageShowForm_Init(QString oid);
    ~PakageShowForm();

    QString pid;
    QString oid;
    QString payid;

    QString ptime;

private:
    Ui::PakageShowForm* ui;

signals:
    void PakageIsCreate();
};

#endif // FORMLIST_H
