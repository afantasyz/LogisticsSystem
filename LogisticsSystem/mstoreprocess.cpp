#include "mstoreprocess.h"
#include "ui_MInstoreProcess.h"
#include "ui_MSelectPakage.h"
#include "ui_MOutstoreProcess.h"
#include "ui_MSelectPakageForSend.h"

#include <QSqlQuery>
#include <QDebug>
#include <QLabel>
#include <QDateTime>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QRandomGenerator>

MInstoreProcess::MInstoreProcess(QDialog* parent):
    QDialog(parent),
    ui(new Ui::MInstoreProcess)
{
    ui->setupUi(this);
}

MInstoreProcess::~MInstoreProcess()
{

}

void MInstoreProcess::MInstoreProcess_Init()
{
    this->tableWidegt=ui->tableWidget;
    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    //填充数据
    ui->lineEdit_sid->setText(this->parentWid->Msid);
    ui->lineEdit_sname->setText(this->parentWid->Msname);
    ui->textEdit_spos->setText(this->parentWid->Mspos);
    //操作人员盒子
    if(!query.exec("select * from worker"))
    {
        qDebug()<<"query station fail";
        return;
    }
    while (query.next()) {
        QString text=query.value("w_id").toString();
        ui->comboBox_wid->addItem(text);
    }

    //连接按钮函数
    connect(ui->pushButton_add,&QPushButton::clicked,this,[=](){
        MSelectPakage* mwidget=new MSelectPakage();
        mwidget->mipParent=this;
        mwidget->MSelectPakage_Init();
        mwidget->show();
    });

    connect(ui->pushButton_delete,&QPushButton::clicked,this,[=](){
        ui->tableWidget->clear();
    });

    connect(ui->pushButton_commit,&QPushButton::clicked,this,[=](){
        //连接数据库
        QSqlDatabase db=QSqlDatabase::database("connection01");
        QSqlQuery query(db);
        QSqlQuery query02(db);

        db.transaction();
        //开始插入
        query.prepare("insert into instore values(:iid,:pid,:key,:itime,:sid,:wid)");
        for (auto& i : this->instoreMap ) {
            //绑定参数
            query.bindValue(":iid",i->iid);
            query.bindValue(":pid",i->pid);
            query.bindValue(":key",i->key);
            query.bindValue(":itime",i->itime);//我去，找了半天时间录不上的bug，原来是itime前面忘了加：号
            query.bindValue(":sid",this->parentWid->Msid);
            query.bindValue(":wid",ui->comboBox_wid->currentText());

            //插入数据
            if(!query.exec())
            {
                qDebug()<<"insert instore fail";
                return ;
            }

            //修改包裹表信息
            if(!query02.exec(QString("update pakage set P_state = '待取中' where P_id = '%1'").arg(i->pid)))
            {
                qDebug()<<"alter pakage fail";
                return ;
            }
        }
        QMessageBox::information(this,"添加成功",QString("成功入库包裹%1件").arg(this->instoreMap.size()));
        db.commit();
        this->tableWidegt->clearContents();
        this->instoreMap.clear();
    });

    connect(ui->pushButton_cancel,&QPushButton::clicked,this,[=](){
        this->close();
    });
}

/*                                 *****************************************                        */
//以下是MSelectPakager类成员

MSelectPakage::MSelectPakage(QDialog* parent):
    QDialog(parent),
    ui(new Ui::MSelectPakage)
{
    ui->setupUi(this);
}

MSelectPakage::~MSelectPakage()
{
    for (auto & i : this->psBoxList) {
        delete i;
    }
}

void MSelectPakage::MSelectPakage_Init()
{
    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    //获取信息栏
    QString sql=QString("select * from pakageInfo where P_state = '运输中' and O_recievePos like '%1%'").arg(this->mipParent->parentWid->Mspos);
    if(!query.exec(sql))
    {
        qDebug()<<"query pakageInfo fail";
        return;
    }
    while (query.next()) {
        //给盒子变量赋值
        PakageSelectBox* psb=new PakageSelectBox();
        psb->pid=query.value("P_id").toString();
        psb->pcompany=query.value("P_company").toString();
        psb->oid=query.value("P_O_id").toString();
        psb->rAddr=query.value("O_recievePos").toString();
        QDateTime time=query.value("P_time").toDateTime();
        psb->ptime=time.toString("yyyy-MM-dd hh:mm:ss");
        psb->PakageSelectBox_Init();

        ui->verticalLayout_show->addWidget(psb);
        this->psBoxList.push_back(psb);

        //连接槽函数
        connect(psb->checkBox,&QCheckBox::stateChanged,this,[=](){
            //如果被选中
            if(psb->checkBox->checkState()==Qt::Checked)
            {
                ui->verticalLayout_select->addWidget(psb);
                this->selectpsBoxList.push_back(psb);
            }
            else if(psb->checkBox->checkState()==Qt::Unchecked)
            {
                ui->verticalLayout_show->addWidget(psb);
                this->selectpsBoxList.remove(this->selectpsBoxList.indexOf(psb));
            }
        });
    }
    ui->scrollAreaWidgetContents_show->setLayout(ui->verticalLayout_show);
    ui->scrollAreaWidgetContents_select->setLayout(ui->verticalLayout_select);

    //确定提交按钮
    connect(ui->pushButton_commit,&QPushButton::clicked,this,[=](){
        //连接数据库
        QSqlDatabase db=QSqlDatabase::database("connection01");
        QSqlQuery query(db);
        QDateTime qdt=QDateTime::currentDateTime();
        QString datetimeStr=qdt.toString("yyyy-MM-dd hh:mm:ss");
        QString datestr=qdt.toString("yyMMdd");
        if(!query.exec(QString("select count(*) from instore where i_id like 'I%1%'").arg(datestr)))
        {
            qDebug()<<"query instore num fail";
            return ;
        }
        query.next();
        int inum=10000+query.value(0).toInt();

        for (auto& i : this->selectpsBoxList ) {
            this->mipParent->tableWidegt->insertRow(0);
            this->mipParent->tableWidegt->setItem(0,0,new QTableWidgetItem(i->pid));
            this->mipParent->tableWidegt->setItem(0,1,new QTableWidgetItem(i->pcompany));
            this->mipParent->tableWidegt->setItem(0,2,new QTableWidgetItem(i->rAddr));
            //取件码
            QString rkey=QString("%1-%2-%3").arg(QRandomGenerator::global()->generate()%100).arg(QRandomGenerator::global()->generate()%16).arg(QRandomGenerator::global()->generate()%9000+1000);
            this->mipParent->tableWidegt->setItem(0,3,new QTableWidgetItem(rkey));
            //入库单
            QString tiid=QString("I%1%2").arg(datestr).arg(inum);
            inum++;
            this->mipParent->tableWidegt->setItem(0,4,new QTableWidgetItem(tiid));
            //删除按钮
            QPushButton* button=new QPushButton("删除");
            this->mipParent->tableWidegt->setCellWidget(0,5,button);
            connect(button,&QPushButton::clicked,this->mipParent->tableWidegt,[=](){
                int row=this->mipParent->tableWidegt->currentRow();
                //草，找到twi是空指针的bug了，应该先取到item的值再给它删除,现调整顺序
                QTableWidgetItem* twi=this->mipParent->tableWidegt->item(row,0);
                QString keystr=twi->text();
                this->mipParent->instoreMap.remove(keystr);
                this->mipParent->tableWidegt->removeRow(row);
            });
            //添加到map
            instoreInfoStruct* iis=new instoreInfoStruct();
            iis->pid=i->pid;
            iis->iid=tiid;
            iis->itime=datetimeStr;
            iis->key=rkey;
            this->mipParent->instoreMap.insert(i->pid,iis);
        }
//        QMessageBox::information(this,"添加成功",QString("成功添加%1项记录").arg(this->selectpsBoxList.size()));
        this->close();
    });

    //查询按钮
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){

    });

}

/*                                 *****************************************                        */
//以下是快递单盒子PakageSelectBox类成员

PakageSelectBox::PakageSelectBox(QFrame* parent):QFrame(parent)
{

}

PakageSelectBox::~PakageSelectBox()
{

}

void PakageSelectBox::PakageSelectBox_Init()
{
    this->hlayout=new QHBoxLayout();
    this->checkBox=new QCheckBox();

    //添加布局
    this->hlayout->addWidget(this->checkBox);

    QLabel* pidLabel=new QLabel(this->pid);
    this->hlayout->addWidget(pidLabel);

    QLabel* companyLabel=new QLabel(this->pcompany);
    this->hlayout->addWidget(companyLabel);

    QLabel* rAddrLabel=new QLabel(this->rAddr);
    this->hlayout->addWidget(rAddrLabel);

    this->setLayout(this->hlayout);
}

/*  *********************以下是寄件界面的部分**********************   */
MOutstoreProcess::MOutstoreProcess(QDialog* parent):
    QDialog(parent),
    ui(new Ui::MOutstoreProcess)
{
    ui->setupUi(this);
}

MOutstoreProcess::~MOutstoreProcess()
{

}

void MOutstoreProcess::MOutstoreProcess_Init()
{
    this->tableWidegt=ui->tableWidget;
    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    //填充数据
    ui->lineEdit_sid->setText(this->parentWid->Msid);
    ui->lineEdit_sname->setText(this->parentWid->Msname);

    //连接按钮函数
    connect(ui->pushButton_add,&QPushButton::clicked,this,[=](){
        MSelectPakageForSend* mwidget=new MSelectPakageForSend();
        mwidget->mipParent=this;
        mwidget->MSelectPakageForSend_Init();
        mwidget->show();
    });

    connect(ui->pushButton_delete,&QPushButton::clicked,this,[=](){
        ui->tableWidget->clear();
    });

    connect(ui->pushButton_commit,&QPushButton::clicked,this,[=](){
        //连接数据库
        QSqlDatabase db=QSqlDatabase::database("connection01");
        QSqlQuery query(db);

        db.transaction();
        //开始插入
        query.prepare("update Pakage set P_state = '运输中' where P_id = :pid");
        for (auto& i : this->instoreMap ) {
            //绑定参数
            query.bindValue(":pid",i->pid);

            //插入数据
            if(!query.exec())
            {
                qDebug()<<"update pakage fail";
                return ;
            }
        }
        QMessageBox::information(this,"发件成功",QString("成功发出包裹%1件").arg(this->instoreMap.size()));
        db.commit();
        this->tableWidegt->clearContents();
        this->instoreMap.clear();
    });

    connect(ui->pushButton_cancel,&QPushButton::clicked,this,[=](){
        this->close();
    });
}

/*  *********************以下是寄件选择包裹界面的部分**********************   */
MSelectPakageForSend::MSelectPakageForSend(QDialog* parent):
    QDialog(parent),
    ui(new Ui::MSelectPakageForSend)
{
    ui->setupUi(this);
}

MSelectPakageForSend::~MSelectPakageForSend()
{

}

void MSelectPakageForSend::MSelectPakageForSend_Init()
{
    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    //获取信息栏
    QString sql=QString("select * from pakageInfo where P_state = '待派发' and O_sendPositon like '%1%'").arg(this->mipParent->parentWid->Mspos);
    if(!query.exec(sql))
    {
        qDebug()<<"query pakageInfo fail";
        return;
    }
    while (query.next()) {
        //给盒子变量赋值
        PakageSelectBox* psb=new PakageSelectBox();
        psb->pid=query.value("P_id").toString();
        psb->pcompany=query.value("P_company").toString();
        psb->oid=query.value("P_O_id").toString();
        psb->rAddr=query.value("O_recievePos").toString();
        psb->sAddr=query.value("O_sendPositon").toString();
        QDateTime time=query.value("P_time").toDateTime();
        psb->ptime=time.toString("yyyy-MM-dd hh:mm:ss");
        psb->PakageSelectBox_Init();

        ui->verticalLayout_show->addWidget(psb);
        this->psBoxList.push_back(psb);

        //连接槽函数
        connect(psb->checkBox,&QCheckBox::stateChanged,this,[=](){
            //如果被选中
            if(psb->checkBox->checkState()==Qt::Checked)
            {
                ui->verticalLayout_select->addWidget(psb);
                this->selectpsBoxList.push_back(psb);
            }
            else if(psb->checkBox->checkState()==Qt::Unchecked)
            {
                ui->verticalLayout_show->addWidget(psb);
                this->selectpsBoxList.remove(this->selectpsBoxList.indexOf(psb));
            }
        });
    }
    ui->scrollAreaWidgetContents_show->setLayout(ui->verticalLayout_show);
    ui->scrollAreaWidgetContents_select->setLayout(ui->verticalLayout_select);

    //确定提交按钮
    connect(ui->pushButton_commit,&QPushButton::clicked,this,[=](){
        //连接数据库
        QSqlDatabase db=QSqlDatabase::database("connection01");
        QDateTime qdt=QDateTime::currentDateTime();
        QString datetimeStr=qdt.toString("yyyy-MM-dd hh:mm:ss");
//        QString datestr=qdt.toString("yyMMdd");

        for (auto& i : this->selectpsBoxList ) {
            this->mipParent->tableWidegt->insertRow(0);
            this->mipParent->tableWidegt->setItem(0,0,new QTableWidgetItem(i->pid));
            this->mipParent->tableWidegt->setItem(0,1,new QTableWidgetItem(i->pcompany));
            this->mipParent->tableWidegt->setItem(0,2,new QTableWidgetItem(i->sAddr));
            this->mipParent->tableWidegt->setItem(0,3,new QTableWidgetItem(i->rAddr));
            //时间
            this->mipParent->tableWidegt->setItem(0,4,new QTableWidgetItem(i->ptime));
            //删除按钮
            QPushButton* button=new QPushButton("删除");
            this->mipParent->tableWidegt->setCellWidget(0,5,button);
            connect(button,&QPushButton::clicked,this->mipParent->tableWidegt,[=](){
                int row=this->mipParent->tableWidegt->currentRow();
                //草，找到twi是空指针的bug了，应该先取到item的值再给它删除,现调整顺序
                QTableWidgetItem* twi=this->mipParent->tableWidegt->item(row,0);
                QString keystr=twi->text();
                this->mipParent->instoreMap.remove(keystr);
                this->mipParent->tableWidegt->removeRow(row);
            });
            //添加到map
            instoreInfoStruct* iis=new instoreInfoStruct();
            iis->pid=i->pid;
            iis->itime=datetimeStr;
            this->mipParent->instoreMap.insert(i->pid,iis);
        }
//        QMessageBox::information(this,"添加成功",QString("成功添加%1项记录").arg(this->selectpsBoxList.size()));
        this->close();
    });

    //查询按钮
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){

    });
}
