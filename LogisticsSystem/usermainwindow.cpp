#include "usermainwindow.h"
#include "ui_usermainwindow.h"
#include <QPixmap>
#include "ucreateorder.h"
#include "unotice.h"
#include "upakage.h"
#include <QSqlQuery>
#include <QDebug>
#include <QPainter>
#include <QMessageBox>
#include <QDateTime>

#include "formlist.h"

UserMainwindow::UserMainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserMainwindow)
{
    ui->setupUi(this);

    //查询数据库


    //生成订单按钮
    connect(ui->pushButton_order_create,&QPushButton::clicked,this,[=](){
        UCreateOrder* uwidegt=new UCreateOrder();
        uwidegt->parentWidegt=this;
        uwidegt->Info_Init();
        uwidegt->show();
    });

    //查询订单按钮
    connect(ui->pushButton_order_query,&QPushButton::clicked,this,[=](){
        UQueryOrder* uwidegt=new UQueryOrder();
        uwidegt->parentWid=this;
        uwidegt->MorderQuery_Init();
        uwidegt->show();
    });

    //取件通知按钮
    connect(ui->pushButton_notice_recieve,&QPushButton::clicked,this,[=](){
        UPakageNotice* uwidegt=new UPakageNotice();
        uwidegt->uid=this->Uaccount;
        uwidegt->UPakageNotice_Init();
        uwidegt->show();
    });

    //支付凭证按钮
    connect(ui->pushButton_payment,&QPushButton::clicked,this,[=](){
        UPaymentNotice* uwidegt = new UPaymentNotice();
        uwidegt->uid=this->Uaccount;
        uwidegt->UPaymentNotice_Init();
        uwidegt->show();
    });

    //包裹查询按钮
    connect(ui->pushButton_pakage_query,&QPushButton::clicked,this,[=](){
        UPakageQuery* uwidegt=new UPakageQuery();
        uwidegt->parentWid=this;
        uwidegt->UPakageQuery_Init();
        uwidegt->show();
    });

    //待取包裹
    connect(ui->pushButton_pakage_ready,&QPushButton::clicked,this,[=](){
        QString sql=QString("select * from pakageInfo where P_state = '待取中' and O_recievePhone = '%1'").arg(this->Uphone);
        this->Pakage_Function2(sql);
    });

    //取件记录
    connect(ui->pushButton_pakage_record,&QPushButton::clicked,this,[=](){
        QString sql=QString("select * from pakageInfo where P_state = '已取件' and O_recievePhone = '%1'").arg(this->Uphone);
        this->Pakage_Function2(sql);
    });

    //接收包裹按钮
    connect(ui->pushButton_pakage,&QPushButton::clicked,this,&UserMainwindow::Pakage_Function);

    //订单按钮
    connect(ui->pushButton_order,&QPushButton::clicked,this,&UserMainwindow::Order_Function);

    //寄包裹按钮
    connect(ui->pushButton_pakage_send,&QPushButton::clicked,this,&UserMainwindow::PakageSend_Function);

}

UserMainwindow::~UserMainwindow()
{
    for (auto& i : this->pakageList) {
        delete i;
    }
    for (auto& i : this->orderList) {
        delete i;
    }
    for (auto& i : this->spList) {
        delete i;
    }
    delete ui;
}

void UserMainwindow::UserMainwindow_clear()
{
    delete ui->gridLayout;
    ui->gridLayout=new QGridLayout();
    for (auto &i : this->pakageList) {
        delete i;
    }
    this->pakageList.clear();

    for (auto &i : this->orderList) {
        delete i;
    }
    this->orderList.clear();

    for (auto &i : this->spList) {
        delete i;
    }
    this->spList.clear();
}

void UserMainwindow::Pakage_Function()
{
    //回收一下内存
    this->UserMainwindow_clear();
    //建立数据库连接
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    QString qstr=QString("select * from pakageInfo where O_recievePhone = '%1'").arg(this->Uphone);
    if(!query.exec(qstr))
    {
        qDebug()<<"pakage query fail";
        return ;
    }

    //创建出框框
    int c=0;
    while(query.next())
    {
        //赋值
        PakageBox* pb=new PakageBox();
        pb->pid=query.value("P_id").toString();
        pb->pcompany=query.value("P_company").toString();
        pb->pstate=query.value("P_state").toString();
        //如果快递已送达
        if(pb->pstate!="待取中")
        {
            QSqlQuery queryInfo(db);
            QString sql=QString("select * from instoreInfo where i_p_id = '%1'").arg(pb->pid);
            if(queryInfo.exec(sql))
            {
                if(queryInfo.next())
                {
                    pb->pkey=queryInfo.value("i_key").toString();
                    pb->sid=queryInfo.value("i_s_id").toString();
                    sql=QString("select * from station where s_id = '%1'").arg(pb->sid);
                    queryInfo.exec(sql);
                    queryInfo.next();
                    pb->sname=queryInfo.value("s_name").toString();
                }
            }
        }
        pb->PakageBox_Init();
        this->pakageList.push_back(pb);
        pb->setParent(ui->scrollAreaWidgetContents);
        ui->gridLayout->addWidget(pb,c/3,c%3);
        pb->move((c%3)*240,(c/3)*180);
        pb->show();

        c++;
    }
    //设置widegt大小，但是没用
//        ui->scrollArea->setWidgetResizable(true);
//        ui->scrollAreaWidgetContents->resize(719,180*(c+2/3));
//        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 根据需要显示垂直滚动条
//        ui->scrollAreaWidgetContents->update();
//        qDebug()<<ui->scrollAreaWidgetContents->size();
    ui->scrollAreaWidgetContents->setLayout(ui->gridLayout);
}

void UserMainwindow::Order_Function()
{
    //回收一下内存
    this->UserMainwindow_clear();
    //建立数据库连接
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    QString qstr=QString("select * from orders where O_u_id = '%1'").arg(this->Uaccount);
    if(!query.exec(qstr))
    {
        qDebug()<<"order query fail";
        return ;
    }

    //创建出框框
    int c=0;
    while(query.next())
    {
        //赋值
        OrderBox* ob=new OrderBox();
        ob->oid=query.value("O_id").toString();
        ob->rname=query.value("O_recievePerson").toString();
        ob->rphone=query.value("O_recievePhone").toString();
        ob->rAddr=query.value("O_recievePos").toString();
        ob->ostate=query.value("O_state").toInt();
        //如果快递已送达
        if(ob->ostate==1)
        {
            QSqlQuery queryInfo(db);
            QString sql=QString("select * from pakageInfo where P_O_id = '%1'").arg(ob->oid);
            if(queryInfo.exec(sql))
            {
                if(queryInfo.next())
                {
                    ob->pid=queryInfo.value("P_id").toString();
                }
            }
        }
        ob->OrderBox_Init();
        this->orderList.push_back(ob);
        ob->setParent(ui->scrollAreaWidgetContents);
        ui->gridLayout->addWidget(ob,c/2,c%2);
//        ob->move((c%3)*240,(c/3)*180);
        ob->show();

        //连接删除订单按钮
        connect(ob->deleteButton,&QPushButton::clicked,this,[=](){
            //建立数据库连接
            QSqlDatabase db=QSqlDatabase::database("connection01");
            QSqlQuery query(db);

            //检查订单是否已处理
            QString sql=QString("select * from orders where O_id = '%1'").arg(ob->oid);
            if(!query.exec(sql))
            {
                qDebug()<<"query orders state fail";
                return;
            }
            query.next();
            if(query.value("O_state").toInt()!=0)
            {
                QMessageBox::warning(this,"取消失败","该订单已发出");
                return ;
            }

            //再次确实是否删除
            QMessageBox msgb;
            msgb.setParent(ob);
            msgb.setWindowTitle("删除订单");
            msgb.setIcon(QMessageBox::Icon::Question);
            msgb.setInformativeText(QString("订单号：%1").arg(ob->oid));
            msgb.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
            int ret=msgb.exec();
            if(ret==QMessageBox::No)
            {
                return ;
            }

            //删除订单
            sql=QString("delete from orders where O_id = '%1'").arg(ob->oid);
            if(!query.exec(sql))
            {
                qDebug()<<"delete order fail";
                return ;
            }
            QMessageBox::information(this,"删除成功",QString("删除订单号:%1").arg(ob->oid));

            //删除视图
            ui->gridLayout->removeWidget(ob);
            ui->scrollAreaWidgetContents->setLayout(ui->gridLayout);
        });

        c++;
    }
    //设置widegt大小，但是没用
//        ui->scrollArea->setWidgetResizable(true);
//        ui->scrollAreaWidgetContents->resize(719,180*(c+2/3));
//        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 根据需要显示垂直滚动条
//        ui->scrollAreaWidgetContents->update();
//        qDebug()<<ui->scrollAreaWidgetContents->size();
    ui->scrollAreaWidgetContents->setLayout(ui->gridLayout);
}

void UserMainwindow::PakageSend_Function()
{
    //回收一下内存
    this->UserMainwindow_clear();
    //建立数据库连接
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    QString qstr=QString("select * from pakageInfo where O_sendPhone = '%1'").arg(this->Uphone);
    if(!query.exec(qstr))
    {
        qDebug()<<"pakage query fail";
        return ;
    }

    //创建出框框
    int c=0;
    while(query.next())
    {
        //赋值
        PakageSendBox* pb=new PakageSendBox();
        pb->pid=query.value("P_id").toString();
        pb->pcompany=query.value("P_company").toString();
        pb->pstate=query.value("P_state").toString();
        pb->rname=query.value("O_recievePerson").toString();
        pb->rphone=query.value("O_recievePhone").toString();
        pb->raddr=query.value("O_recievePos").toString();
        //如果快递已送达
        if(pb->pstate=="待取中")
        {
            QSqlQuery queryInfo(db);
            QString sql=QString("select * from instoreInfo where i_p_id = '%1'").arg(pb->pid);
            if(queryInfo.exec(sql))
            {
                if(queryInfo.next())
                {
//                    pb->pkey=queryInfo.value("i_key").toString();
                    pb->sid=queryInfo.value("i_s_id").toString();
                    sql=QString("select * from station where s_id = '%1'").arg(pb->sid);
                    queryInfo.exec(sql);
                    queryInfo.next();
                    pb->sname=queryInfo.value("s_name").toString();
                }
            }
        }
        pb->PakageSendBox_Init();
        this->spList.push_back(pb);
        pb->setParent(ui->scrollAreaWidgetContents);
        ui->gridLayout->addWidget(pb,c/3,c%3);
        pb->move((c%3)*240,(c/3)*180);
        pb->show();

        c++;
    }
    //设置widegt大小，但是没用
//        ui->scrollArea->setWidgetResizable(true);
//        ui->scrollAreaWidgetContents->resize(719,180*(c+2/3));
//        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 根据需要显示垂直滚动条
//        ui->scrollAreaWidgetContents->update();
//        qDebug()<<ui->scrollAreaWidgetContents->size();
    ui->scrollAreaWidgetContents->setLayout(ui->gridLayout);
}

void UserMainwindow::Pakage_Function2(QString sql)
{
    //回收一下内存
    this->UserMainwindow_clear();
    //建立数据库连接
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

//    QString qstr=QString("select * from pakageInfo where O_recievePhone = '%1'").arg(this->Uphone);
    if(!query.exec(sql))
    {
        qDebug()<<"pakage query fail";
        return ;
    }

    //创建出框框
    int c=0;
    while(query.next())
    {
        //赋值
        PakageBox* pb=new PakageBox();
        pb->pid=query.value("P_id").toString();
        pb->pcompany=query.value("P_company").toString();
        pb->pstate=query.value("P_state").toString();
        //如果快递已送达
        if(pb->pstate=="待取中")
        {
            QSqlQuery queryInfo(db);
            QString sql=QString("select * from instoreInfo where i_p_id = '%1'").arg(pb->pid);
            if(queryInfo.exec(sql))
            {
                if(queryInfo.next())
                {
                    pb->pkey=queryInfo.value("i_key").toString();
                    pb->sid=queryInfo.value("i_s_id").toString();
                    sql=QString("select * from station where s_id = '%1'").arg(pb->sid);
                    queryInfo.exec(sql);
                    queryInfo.next();
                    pb->sname=queryInfo.value("s_name").toString();
                }
            }
        }
        pb->PakageBox_Init();
        this->pakageList.push_back(pb);
        pb->setParent(ui->scrollAreaWidgetContents);
        ui->gridLayout->addWidget(pb,c/3,c%3);
        pb->move((c%3)*240,(c/3)*180);
        pb->show();

        c++;
    }
    //设置widegt大小，但是没用
//        ui->scrollArea->setWidgetResizable(true);
//        ui->scrollAreaWidgetContents->resize(719,180*(c+2/3));
//        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 根据需要显示垂直滚动条
//        ui->scrollAreaWidgetContents->update();
//        qDebug()<<ui->scrollAreaWidgetContents->size();
    ui->scrollAreaWidgetContents->setLayout(ui->gridLayout);
}

void UserMainwindow::Order_Function2(QString sql)
{
    //回收一下内存
    this->UserMainwindow_clear();
    //建立数据库连接
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

//    QString qstr=QString("select * from orders where O_u_id = '%1'").arg(this->Uaccount);
    if(!query.exec(sql))
    {
        qDebug()<<"order query fail";
        return ;
    }

    //创建出框框
    int c=0;
    while(query.next())
    {
        //赋值
        OrderBox* ob=new OrderBox();
        ob->oid=query.value("O_id").toString();
        ob->rname=query.value("O_recievePerson").toString();
        ob->rphone=query.value("O_recievePhone").toString();
        ob->rAddr=query.value("O_recievePos").toString();
        ob->ostate=query.value("O_state").toInt();
        //如果快递已送达
        if(ob->ostate==1)
        {
            QSqlQuery queryInfo(db);
            QString sql=QString("select * from pakageInfo where P_O_id = '%1'").arg(ob->oid);
            if(queryInfo.exec(sql))
            {
                if(queryInfo.next())
                {
                    ob->pid=queryInfo.value("P_id").toString();
                }
            }
        }
        ob->OrderBox_Init();
        this->orderList.push_back(ob);
        ob->setParent(ui->scrollAreaWidgetContents);
        ui->gridLayout->addWidget(ob,c/2,c%2);
//        ob->move((c%3)*240,(c/3)*180);
        ob->show();

        //连接删除订单按钮
        connect(ob->deleteButton,&QPushButton::clicked,this,[=](){
            //建立数据库连接
            QSqlDatabase db=QSqlDatabase::database("connection01");
            QSqlQuery query(db);

            //检查订单是否已处理
            QString sql=QString("select * from orders where O_id = '%1'").arg(ob->oid);
            if(!query.exec(sql))
            {
                qDebug()<<"query orders state fail";
                return;
            }
            query.next();
            if(query.value("O_state").toInt()!=0)
            {
                QMessageBox::warning(this,"取消失败","该订单已发出");
                return ;
            }

            //再次确实是否删除
            QMessageBox msgb;
            msgb.setParent(ob);
            msgb.setWindowTitle("删除订单");
            msgb.setIcon(QMessageBox::Icon::Question);
            msgb.setInformativeText(QString("订单号：%1").arg(ob->oid));
            msgb.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
            int ret=msgb.exec();
            if(ret==QMessageBox::No)
            {
                return ;
            }

            //删除订单
            sql=QString("delete from orders where O_id = '%1'").arg(ob->oid);
            if(!query.exec(sql))
            {
                qDebug()<<"delete order fail";
                return ;
            }
            QMessageBox::information(this,"删除成功",QString("删除订单号:%1").arg(ob->oid));

            //删除视图
            ui->gridLayout->removeWidget(ob);
        });

        c++;
    }
    //设置widegt大小，但是没用
//        ui->scrollArea->setWidgetResizable(true);
//        ui->scrollAreaWidgetContents->resize(719,180*(c+2/3));
//        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 根据需要显示垂直滚动条
//        ui->scrollAreaWidgetContents->update();
//        qDebug()<<ui->scrollAreaWidgetContents->size();
    ui->scrollAreaWidgetContents->setLayout(ui->gridLayout);
}

void UserMainwindow::PakageSend_Function2(QString sql)
{
    //回收一下内存
    this->UserMainwindow_clear();
    //建立数据库连接
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

//    QString qstr=QString("select * from pakageInfo where O_sendPhone = '%1'").arg(this->Uphone);
    if(!query.exec(sql))
    {
        qDebug()<<"pakage query fail";
        return ;
    }

    //创建出框框
    int c=0;
    while(query.next())
    {
        //赋值
        PakageSendBox* pb=new PakageSendBox();
        pb->pid=query.value("P_id").toString();
        pb->pcompany=query.value("P_company").toString();
        pb->pstate=query.value("P_state").toString();
        pb->rname=query.value("O_recievePerson").toString();
        pb->rphone=query.value("O_recievePhone").toString();
        pb->raddr=query.value("O_recievePos").toString();
        //如果快递已送达
        if(pb->pstate=="待取中")
        {
            QSqlQuery queryInfo(db);
            QString sql=QString("select * from instoreInfo where i_p_id = '%1'").arg(pb->pid);
            if(queryInfo.exec(sql))
            {
                if(queryInfo.next())
                {
//                    pb->pkey=queryInfo.value("i_key").toString();
                    pb->sid=queryInfo.value("i_s_id").toString();
                    sql=QString("select * from station where s_id = '%1'").arg(pb->sid);
                    queryInfo.exec(sql);
                    queryInfo.next();
                    pb->sname=queryInfo.value("s_name").toString();
                }
            }
        }
        pb->PakageSendBox_Init();
        this->spList.push_back(pb);
        pb->setParent(ui->scrollAreaWidgetContents);
        ui->gridLayout->addWidget(pb,c/3,c%3);
        pb->move((c%3)*240,(c/3)*180);
        pb->show();

        c++;
    }
    //设置widegt大小，但是没用
//        ui->scrollArea->setWidgetResizable(true);
//        ui->scrollAreaWidgetContents->resize(719,180*(c+2/3));
//        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 根据需要显示垂直滚动条
//        ui->scrollAreaWidgetContents->update();
//        qDebug()<<ui->scrollAreaWidgetContents->size();
    ui->scrollAreaWidgetContents->setLayout(ui->gridLayout);
}

void UserMainwindow::paintEvent(QPaintEvent* ev)
{
    QPixmap pix;
    pix.load(":/picRes/image/instore.png");
    ui->pushButton_pakage->setIcon(pix);
    ui->pushButton_pakage->setIconSize(ui->pushButton_pakage->size());

    pix.load(":/picRes/image/orders.png");
    ui->pushButton_order->setIcon(pix);
    ui->pushButton_order->setIconSize(ui->pushButton_pakage->size());

    pix.load(":/picRes/image/outstore.png");
    ui->pushButton_pakage_send->setIcon(pix);
    ui->pushButton_pakage_send->setIconSize(ui->pushButton_pakage->size());

    pix.load(":/picRes/image/user.png");
    ui->pushButton_user->setIcon(pix);
    ui->pushButton_user->setIconSize(ui->pushButton_pakage->size());
}

/*           ************************************************************************************                         */
//以下是pakageBox
PakageBox::PakageBox(QFrame* parent):
    QFrame(parent)
{
    this->flayout=new QFormLayout();
    this->infoButton=new QPushButton("详情信息");
    this->outstoreButton=new QPushButton("包裹出库");

    this->resize(240,180);
}

PakageBox::~PakageBox()
{

}

void PakageBox::PakageBox_Init()
{
   //添加控件
    QLabel* pidLabel=new QLabel("快递单号");
    QLabel* pidInfo=new QLabel(this->pid);
    this->flayout->addRow(pidLabel,pidInfo);

    QLabel* pcompanyLabel=new QLabel("快递公司");
    QLabel* pcompanyInfo=new QLabel(this->pcompany);
    this->flayout->addRow(pcompanyLabel,pcompanyInfo);

    QLabel* pstateLabel=new QLabel("快递状态");
    QLabel* pstateInfo=new QLabel(this->pstate);
    this->flayout->addRow(pstateLabel,pstateInfo);

    QLabel* keyLab=new QLabel("取件码");
    QLabel* keyLabel=new QLabel(this->pkey,this);
    this->flayout->addRow(keyLab,keyLabel);

    QLabel* stationLab=new QLabel("驿站");
    QLabel* stationLabel=new QLabel(this->sname);
    this->flayout->addRow(stationLab,stationLabel);

    this->flayout->addRow(this->infoButton,this->outstoreButton);
    this->flayout->setAlignment(Qt::AlignHCenter);

    this->setLayout(this->flayout);
//    this->infoButton->move(50,130);
    this->infoButton->show();

    //连接信号槽
    connect(this->infoButton,&QPushButton::clicked,this,[=](){
        PakageForm* pform=new PakageForm(nullptr,this->pid);
        pform->show();
    });

    //连接出库信号槽
    connect(this->outstoreButton,&QPushButton::clicked,this,[=](){
        //建立数据库连接
        QSqlDatabase db=QSqlDatabase::database("connection01");
        QSqlQuery query(db);

        //检查快递是否送至驿站
        QString sql=QString("select * from pakage where P_id = '%1'").arg(this->pid);
        if(!query.exec(sql))
        {
            qDebug()<<"query pakage state fail";
            return ;
        }
        query.next();
        if(query.value("P_state").toString()!="待取中")
        {
            QMessageBox::warning(this,"出库失败","该快递尚未送至驿站");
            return ;
        }
        if(query.value("P_state").toString()!="已取件")
        {
            QMessageBox::warning(this,"出库失败","该快递已取出");
            return ;
        }

        //修改数据库信息
        sql=QString("update pakage set P_state='已取件' where P_id = '%1'").arg(this->pid);
        if(!query.exec(sql))
        {
            qDebug()<<"outstore fail";
            return ;
        }

        //添加出库单
        QDateTime currtime=QDateTime::currentDateTime();
        QString dtime=currtime.toString("yyyy-MM-dd hh:mm:ss");
        QString timestr=currtime.toString("yyMMdd");
        if(!query.exec(QString("select count(*) from outstore where o_id like 'o%1%'").arg(timestr)))
        {
            qDebug()<<"generate oid fail";
            return ;
        }
        query.next();
        int onum=query.value(0).toInt();

        sql=QString("select * from pakageInfo where P_id = '%1'").arg(this->pid);
        if(!query.exec(sql))
        {
            qDebug()<<"query uid fail";
            return ;
        }
        query.next();
        QString uid=query.value("O_u_id").toString();

        query.prepare("insert into outstore values(:o_id,:o_p_id,:o_u_id,:o_time)");
        QString oid=QString("o%1%2").arg(timestr).arg(10000+onum);
        query.bindValue(":o_id",oid);
        query.bindValue(":o_p_id",this->pid);
        query.bindValue(":o_u_id",uid);
        query.bindValue(":o_time",dtime);
        if(!query.exec())
        {
            qDebug()<<"insert outstore fail";
            return ;
        }

        //弹出提示
        sql=QString("出库单号：%1").arg(oid);
        QMessageBox::information(this,"出库成功",sql);
    });
}

void PakageBox::paintEvent(QPaintEvent* ev)
{
    QPixmap pix(":/picRes/image/pakageBox.png");
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

/*           ************************************************************************************                         */
//以下是orderBox
OrderBox::OrderBox(QFrame* parent):
    QFrame(parent)
{
    this->flayout=new QFormLayout();
    this->infoButton=new QPushButton("详情信息");
    this->deleteButton=new QPushButton("取消订单");

    this->resize(240,180);
}

OrderBox::~OrderBox()
{

}

void OrderBox::OrderBox_Init()
{
    //添加控件
     QLabel* oidLabel=new QLabel("请求单号");
     QLabel* oidInfo=new QLabel(this->oid);
     this->flayout->addRow(oidLabel,oidInfo);

     QLabel* rnameLabel=new QLabel("收件人");
     QLabel* rnameInfo=new QLabel(this->rname);
     this->flayout->addRow(rnameLabel,rnameInfo);

     QLabel* rphoneLabel=new QLabel("收件人电话");
     QLabel* rphoneInfo=new QLabel(this->rphone);
     this->flayout->addRow(rphoneLabel,rphoneInfo);

     QLabel* rAddrLab=new QLabel("收件人地址");
     QLabel* rAddrLabel=new QLabel(this->rAddr,this);
     this->flayout->addRow(rAddrLab,rAddrLabel);

     QLabel* ostateLab=new QLabel("订单状态");
     QString str=this->ostate==1?"已处理":"未处理";
     QLabel* ostateLabel=new QLabel(str);
     this->flayout->addRow(ostateLab,ostateLabel);

     QLabel* snameLab=new QLabel("处理驿站");
     QLabel* snameLabel=new QLabel(this->sname);
     this->flayout->addRow(snameLab,snameLabel);

     this->flayout->addRow(this->deleteButton,this->infoButton);
     this->flayout->setAlignment(Qt::AlignHCenter);

     this->setLayout(this->flayout);
 //    this->infoButton->move(50,130);
     this->infoButton->show();

     //连接信号槽
     connect(this->infoButton,&QPushButton::clicked,this,[=](){
//         OrderForm* oform=new OrderForm(nullptr,this->oid);
//         oform->show();
         OrderShowForm* oform=new OrderShowForm(nullptr,this->oid);
         oform->show();
     });
     //删除订单 此功能已移至order_function函数，因为要对表格视图进行操作
//     connect(this->deleteButton,&QPushButton::clicked,this,[=](){
//         //建立数据库连接
//         QSqlDatabase db=QSqlDatabase::database("connection01");
//         QSqlQuery query(db);

//         //检查订单是否已处理
//         QString sql=QString("select * from orders where O_id = '%1'").arg(this->oid);
//         if(!query.exec(sql))
//         {
//             qDebug()<<"query orders state fail";
//             return;
//         }
//         query.next();
//         if(query.value("O_state").toInt()!=0)
//         {
//             QMessageBox::warning(this,"取消失败","该订单已发出");
//             return ;
//         }

//         //删除订单
//         sql=QString("delete from orders where O_id = '%1'").arg(this->oid);
//         if(!query.exec(sql))
//         {
//             qDebug()<<"delete order fail";
//             return ;
//         }
//         QMessageBox::information(this,"删除成功",QString("删除订单号:%1").arg(this->oid));

//         //删除视图

//     });
}

void OrderBox::paintEvent(QPaintEvent *ev)
{
    QPixmap pix(":/picRes/image/pakageBox.png");
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

/*           ************************************************************************************                         */
//以下是pakageSendBox
PakageSendBox::PakageSendBox(QFrame* parent):
    QFrame(parent)
{
    this->flayout=new QFormLayout();
    this->infoButton=new QPushButton("详情信息");

    this->resize(240,180);
}

PakageSendBox::~PakageSendBox()
{

}

void PakageSendBox::PakageSendBox_Init()
{
   //添加控件
    QLabel* pidLabel=new QLabel("快递单号");
    QLabel* pidInfo=new QLabel(this->pid);
    this->flayout->addRow(pidLabel,pidInfo);

    QLabel* pcompanyLabel=new QLabel("快递公司");
    QLabel* pcompanyInfo=new QLabel(this->pcompany);
    this->flayout->addRow(pcompanyLabel,pcompanyInfo);

    QLabel* pstateLabel=new QLabel("快递状态");
    QLabel* pstateInfo=new QLabel(this->pstate);
    this->flayout->addRow(pstateLabel,pstateInfo);

    QLabel* rnameLab=new QLabel("收件人");
    QLabel* rnameLabel=new QLabel(this->rname,this);
    this->flayout->addRow(rnameLab,rnameLabel);

//    QLabel* raddrLab=new QLabel("收件地址");
//    QLabel* raddrLabel=new QLabel(this->raddr);
//    this->flayout->addRow(raddrLab,raddrLabel);

    QLabel* rphoneLab=new QLabel("收件人电话");
    QLabel* rphoneLabel=new QLabel(this->rphone);
    this->flayout->addRow(rphoneLab,rphoneLabel);

    this->flayout->addWidget(this->infoButton);
    this->flayout->setAlignment(Qt::AlignHCenter);

    this->setLayout(this->flayout);
//    this->infoButton->move(50,130);
    this->infoButton->show();

    //连接信号槽
    connect(this->infoButton,&QPushButton::clicked,this,[=](){
        PakageForm* pform=new PakageForm(nullptr,this->pid);
        pform->show();
    });

}

void PakageSendBox::paintEvent(QPaintEvent* ev)
{
    QPixmap pix(":/picRes/image/pakageBox.png");
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
