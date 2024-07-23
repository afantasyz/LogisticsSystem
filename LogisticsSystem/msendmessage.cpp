#include "msendmessage.h"
#include "ui_msendmessage.h"
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDebug>
#include<QTime>
#include<QMessageBox>
#include<QDateTime>

#include "formlist.h"

MSendMessage::MSendMessage(QWidget *parent,QString mid,QString sid) :
    QDialog(parent),
    ui(new Ui::MSendMessage)
{
    ui->setupUi(this);
    //注入数据
    this->mid=mid;
    this->sid=sid;

    //开始生成表单
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);
    QString sql=QString("select * from instoreInfo where i_s_id ='%1' and P_state = '待取中' order by i_time").arg(sid);
    if(!query.exec(sql))
    {
        qDebug()<<"infobox query fail";
        return;
    }

    while (query.next()) {
        //插入信息
        QString pid=query.value("i_p_id").toString();
        QString uphone=query.value("O_recievePhone").toString();
        QString key=query.value("i_key").toString();
        QString time=query.value("i_time").toString();
        InfoBox* box=new InfoBox(nullptr,pid,uphone,key,time);
        this->infoVector.push_back(box);
        ui->verticalLayout->addWidget(box);       
        //给box插入数值
        box->iid=query.value("i_id").toString();
        box->sid=query.value("i_s_id").toString();

        //连接订单查询按钮
        connect(&box->pidLabel,&QPushButton::clicked,this,[=](){
            PakageForm* pform=new PakageForm(nullptr,box->pid);
            pform->show();
        });

        //连接发送消息按钮
        connect(&box->processButt,&QPushButton::clicked,this,[=](){
            QSqlDatabase db=QSqlDatabase::database("connection01");
            QSqlQuery query(db);
            //查询当前时间
            QDateTime currtime=QDateTime::currentDateTime();
            QString timestr=currtime.toString("yyyy-MM-dd hh:mm:ss");
            QString datestr=currtime.toString("yyMMdd");

            //查询用户ID
            QString sql=QString("select * from user where u_phone = '%1'").arg(box->uphone);
            if(!query.exec(sql))
            {
                qDebug()<<"sendMessage query user fail";
                return ;;
            }
            query.next();
            box->uid=query.value("u_id").toString();
            box->uname=query.value("u_name").toString();
            //生成插入数据
            //notice单的id生成
            sql=QString("select count(*) from notice where n_id like 'N%1%'").arg(datestr);
            if(!query.exec(sql))
            {
                qDebug()<<"query count num fail";
                return ;
            }
            query.next();
            int num=query.value(0).toInt()+10000;
            QString nid=QString("N%1%2").arg(datestr).arg(num);

            sql=QString("insert into notice values('%1','%2','%3','%4','%5',0)").arg(nid).arg(box->iid).arg(timestr).arg(this->mid).arg(box->uid);
            if(!query.exec(sql))
            {
                qDebug()<<"notice insert fail";
                return ;
            }
            QString message=QString("收信人：%1,发送人:%2,取件码:%3").arg(box->uid).arg(this->mid).arg(box->key);
            QMessageBox::information(this,"发送成功",message);
        });
    }
    ui->scrollAreaWidgetContents->setLayout(ui->verticalLayout);

    //包裹查询按钮
    connect(ui->pushButton_pid,&QPushButton::clicked,this,[=](){
        QLayoutItem* child;
        //清楚原来的按钮
        while((child=ui->verticalLayout->takeAt(0))!=NULL){
            if(child->widget())
            {
                child->widget()->setParent(0);
                delete child;
            }
        }
        QString pid=ui->lineEdit_pid->text();
        //如果空则重置
        if(pid.isEmpty())
        {
            ui->scrollAreaWidgetContents->resize(659,319);
            for (auto &i : this->infoVector) {
                ui->verticalLayout->addWidget(i);
            }
            return ;
        }
        //显示需要的按钮
        for (auto &i : this->infoVector) {
            if(i->pid==pid)
            {
                ui->verticalLayout->addWidget(i);
                return;
            }
        }
    });

    //用户查询按钮
    connect(ui->pushButton_uphone,&QPushButton::clicked,this,[=](){
        QString uphone=ui->lineEdit_uphone->text();
        QLayoutItem* child;
        //隐藏原来的按钮
        while((child=ui->verticalLayout->takeAt(0))!=NULL){
            if(child->widget())
            {
                child->widget()->setParent(0);
                delete child;
            }
        }
        //如果空则重置
        if(uphone.isEmpty())
        {
            ui->scrollAreaWidgetContents->resize(659,319);
            for (auto &i : this->infoVector) {
                ui->verticalLayout->addWidget(i);
            }
            return ;
        }
        //显示需要的按钮
        for (auto &i : this->infoVector) {
            if(i->uphone==uphone)
            {
                ui->verticalLayout->addWidget(i);
                return;
            }
        }
    });

}

MSendMessage::~MSendMessage()
{
    for (auto& i : this->infoVector) {
        delete i;
    }
    delete ui;
}

/* ************************************************ */
//以下是infobox的成员函数

InfoBox::InfoBox(QFrame *parent,QString pid,QString uphone,QString key,QString time):QFrame(parent)
{
    this->pid=pid;
    this->uphone=uphone;
    this->key=key;
    this->time=time;

    //设置数据
    this->pidLabel.setText(pid);
    this->uidLabel.setText(uphone);
    this->keyLabel.setText(key);
    this->timeLabel.setText(time);
    this->processButt.setText("发送通知");

    //设置布局
    this->hlay.addWidget(&this->pidLabel);
    this->hlay.addSpacing(10);
    this->hlay.addWidget(&this->uidLabel);
    this->hlay.addSpacing(10);
    this->hlay.addWidget(&this->keyLabel);
    this->hlay.addSpacing(10);
    this->hlay.addWidget(&this->timeLabel);
    this->hlay.addSpacing(10);
    this->hlay.addWidget(&this->processButt);
    this->setLayout(&this->hlay);
}

InfoBox::InfoBox()
{

}

InfoBox::~InfoBox()
{

}
