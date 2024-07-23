#ifndef MSTOREPROCESS_H
#define MSTOREPROCESS_H

#include <QDialog>
#include <QFrame>
#include <managerwindow.h>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QTableWidget>
#include <QMap>

//选项盒子
class PakageSelectBox:public QFrame
{
    Q_OBJECT
public:
    explicit PakageSelectBox(QFrame* parent = nullptr);
    ~PakageSelectBox();
    void PakageSelectBox_Init();

    QString pid;
    QString pcompany;
    QString oid;
    QString rAddr;
    QString ptime;
    QString sAddr;

    QHBoxLayout* hlayout;
    QCheckBox* checkBox;

private:

};

namespace Ui {
class MInstoreProcess;
class MSelectPakage;
class MOutstoreProcess;
class MSelectPakageForSend;
}

//入库信息结构体、
struct instoreInfoStruct
{
public:
    QString pid;
    QString iid;
    QString key;
    QString sid;
    QString itime;
    QString wid;
};


//入库处理类
class MInstoreProcess:public QDialog
{
    Q_OBJECT
public:
    explicit MInstoreProcess(QDialog* parent=nullptr);
    ~MInstoreProcess();
    void MInstoreProcess_Init();

    ManagerWindow* parentWid;
    QTableWidget* tableWidegt;

    //用map可能方面后面选项去重，但是不想做
    QMap<QString,instoreInfoStruct*> instoreMap;

private:
    Ui::MInstoreProcess* ui;

signals:

};

//选择包裹类
class MSelectPakage:public QDialog
{
    Q_OBJECT
public:
    explicit MSelectPakage(QDialog* parent=nullptr);
    ~MSelectPakage();
    void MSelectPakage_Init();

    QVector<PakageSelectBox*> psBoxList;
    QVector<PakageSelectBox*> selectpsBoxList;
    MInstoreProcess* mipParent;
    
private:
    Ui::MSelectPakage* ui;

signals:

};


//发出包裹界面
class MOutstoreProcess:public QDialog
{
    Q_OBJECT
public:
    explicit MOutstoreProcess(QDialog* parent=nullptr);
    ~MOutstoreProcess();
    void MOutstoreProcess_Init();

    ManagerWindow* parentWid;
    QTableWidget* tableWidegt;

    //用map可能方面后面选项去重，但是不想做
    QMap<QString,instoreInfoStruct*> instoreMap;

private:
    Ui::MOutstoreProcess* ui;
};

//选择包裹界面
class MSelectPakageForSend:public QDialog
{
    Q_OBJECT
public:
    explicit MSelectPakageForSend(QDialog* parent=nullptr);
    ~MSelectPakageForSend();
    void MSelectPakageForSend_Init();

    QVector<PakageSelectBox*> psBoxList;
    QVector<PakageSelectBox*> selectpsBoxList;
    MOutstoreProcess* mipParent;

private:
    Ui::MSelectPakageForSend* ui;
};

#endif // MSTOREPROCESS_H
