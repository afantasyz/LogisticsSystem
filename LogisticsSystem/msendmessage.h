#ifndef MSENDMESSAGE_H
#define MSENDMESSAGE_H

#include <QDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QFrame>

class InfoBox: public QFrame
{
    Q_OBJECT
public:
    InfoBox();
    explicit InfoBox(QFrame *parent = nullptr,QString pid=nullptr,QString uphone=nullptr,QString key=nullptr,QString time=nullptr);
    ~InfoBox();

    QHBoxLayout hlay;
    QPushButton pidLabel;
    QLabel uidLabel;
    QLabel keyLabel;
    QLabel timeLabel;
    QPushButton processButt;

    QString pid;
    QString uphone;
    QString key;
    QString time;
    QString iid;
    QString sid;
    QString uid;
    QString uname;
};

namespace Ui {
class MSendMessage;
}

class MSendMessage : public QDialog
{
    Q_OBJECT

public:
    explicit MSendMessage(QWidget *parent = nullptr,QString mid=NULL,QString sid=NULL);
    ~MSendMessage();

    //变量
    QString mid;
    QString sid;
    QVector<InfoBox*> infoVector;

private:
    Ui::MSendMessage *ui;
};

#endif // MSENDMESSAGE_H
