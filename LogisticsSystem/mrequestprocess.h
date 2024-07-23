#ifndef MREQUESTPROCESS_H
#define MREQUESTPROCESS_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QFrame>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

#include "formlist.h"

class processPage:public QDialog
{
    Q_OBJECT
public:
    explicit processPage(QDialog *parent = nullptr);
    ~processPage();
    void processPage_Init();

    QPushButton* commitButton;
    QPushButton* cancelButton;
    QLineEdit* oidLine;
    QLineEdit* pidLine;
    QLineEdit* payidLine;
    QComboBox* pcomLine;
    QLabel* priceLine;
    QLabel* timeLine;

    QString oid;
    QString payid;
    QString company;
    QString pid;
    QString solveTime;
    float price;

private:

};

class RequestBox: public QFrame
{
    Q_OBJECT
public:
    RequestBox();
    explicit RequestBox(QFrame *parent = nullptr,QString oid=nullptr,QString uid=nullptr);
    ~RequestBox();
    void RequestBox_init();

//    QFrame hframe;
    QHBoxLayout hlay;
    QLabel oidLabel;
    QLabel uidLabel;
    QLabel timeLabel;
    QPushButton infoButt;
    QPushButton payButt;
    QPushButton processButt;

    QString oid;
    QString uid;
    QString sname;
    QString sphone;
    QString spos;
    QString rname;
    QString rphone;
    QString rpos;
    float weight;
    QString time;

    QDialog* requetDiolog;
    processPage* processDiolog;
    PaymentShowForm* paymentDialog;
    PakageShowForm* pakageDialog;
};

namespace Ui {
class MRequestProcess;
}

class MRequestProcess : public QWidget
{
    Q_OBJECT

public:
    explicit MRequestProcess(QWidget *parent = nullptr);
    void MRequestProcess_init(QString sid,QString mid);
    ~MRequestProcess();

    QVector<RequestBox*> requestVector;
    QString sid;
    QString mid;

private:
    Ui::MRequestProcess *ui;
};

#endif // MREQUESTPROCESS_H
