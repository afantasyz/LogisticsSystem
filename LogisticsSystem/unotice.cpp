#include "unotice.h"
#include "ui_UPakageNotice.h"
#include "ui_UPaymentNotice.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>

#include "formlist.h"

UPakageNotice::UPakageNotice(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::UPakageNotice)
{
    ui->setupUi(this);
}

UPakageNotice::~UPakageNotice()
{

}

void UPakageNotice::UPakageNotice_Init()
{
    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    //查询添加信息
    QString sql=QString("select * from noticeinfo where n_u_id = '%1'").arg(this->uid);
    if(!query.exec(sql))
    {
        qDebug()<<"query unread notice fail";
        return;
    }

    //添加未读信息
    while(query.next())
    {
        PakageNoticeBox* pb=new PakageNoticeBox();
        pb->nid=query.value("n_id").toString();
        pb->pid=query.value("i_p_id").toString();
        pb->uid=this->uid;
        pb->key=query.value("i_key").toString();
        pb->sid=query.value("i_s_id").toString();

        //再查询一下驿站信息
        QSqlQuery squery(db);
        if(!squery.exec(QString("select * from station where s_id = '%1'").arg(pb->sid)))
        {
            qDebug()<<"query station info fail";
            return;
        }
        squery.next();
        pb->sname=squery.value("s_name").toString();
        pb->spos=squery.value("s_pos").toString();

        pb->PakageNoticeBox_Init();
        //根据是否已读添加到不同菜单
        if(query.value("n_state").toInt()==0)
        {
            ui->verticalLayout_unread->addWidget(pb);
            pb->isRead=false;
        }
        else {
            ui->verticalLayout_read->addWidget(pb);
            pb->isRead=true;
        }

        this->pakageboxList.push_back(pb);

        //处理标为已读按钮
        connect(pb->button,&QPushButton::clicked,this,[=](){
            //连接数据库
            QSqlDatabase db=QSqlDatabase::database("connection01");
            QSqlQuery query(db);

            //修改通知单内容
            QString sql=QString("update notice set n_state = %1 where n_id = '%2'").arg((int)(!pb->isRead)).arg(pb->nid);
            if(!query.exec(sql))
            {
                qDebug()<<"update notice state fail";
                return ;
            }

            //刷新界面显示
            ui->verticalLayout_unread->removeWidget(pb);
            ui->verticalLayout_read->addWidget(pb);
        });

    }
    ui->scrollAreaWidgetContents->setLayout(ui->verticalLayout_unread);
    ui->scrollAreaWidgetContents_2->setLayout(ui->verticalLayout_read);

}

/*              ******************************************************************************                      */
//以下是支付凭证payment类

UPaymentNotice::UPaymentNotice(QDialog* parent ) :
    QDialog(parent),
    ui(new Ui::UPaymentNotice)
{
    ui->setupUi(this);
}

UPaymentNotice:: ~UPaymentNotice()
{

}

void UPaymentNotice::UPaymentNotice_Init()
{
    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    //查询添加信息
    QString sql=QString("select * from payinfo where u_id = '%1' ").arg(this->uid);
    if(!query.exec(sql))
    {
        qDebug()<<"query payment fail";
        return;
    }

    while(query.next())
    {
        PaymentBox* payb=new PaymentBox();
        payb->payid=query.value("pay_id").toString();
        payb->uid=this->uid;
        payb->oid=query.value("pay_O_id").toString();
        payb->price=query.value("pay_price").toFloat();
        payb->paytime=query.value("pay_time").toString();

        payb->PaymentBox_Init();
        ui->verticalLayout->addWidget(payb);
        this->payboxList.push_back(payb);

    }
    ui->scrollAreaWidgetContents->setLayout(ui->verticalLayout);

    //订单查询功能实现
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        QString payid=ui->lineEdit->text();
        //移除所有box
        QLayoutItem* child;
        while((child=ui->verticalLayout->takeAt(0))!=NULL){
            if(child->widget())
            {
                child->widget()->setParent(0);
                delete child;
            }
        }
        //如果空则重置
        if(payid.isEmpty())
        {
            for (auto& i : this->payboxList ) {
                ui->verticalLayout->addWidget(i);
            }
            ui->scrollAreaWidgetContents->setLayout(ui->verticalLayout);
            return;
        }
        //显示对应IDbox
        for (auto& i : this->payboxList ) {
            if(i->payid==payid)
            {
                ui->verticalLayout->addWidget(i);
                ui->scrollAreaWidgetContents->setLayout(ui->verticalLayout);
                return;
            }
        }
    });
}

/*              ******************************************************************************                      */
//以下是消息盒子类
PakageNoticeBox::PakageNoticeBox(QFrame* parent):
    QFrame(parent)
{

}

PakageNoticeBox::~PakageNoticeBox()
{

}

void PakageNoticeBox::PakageNoticeBox_Init()
{
    this->vlayout=new QVBoxLayout();

    QLabel* nidLabel=new QLabel(QString("通知单号： %1").arg(this->nid));
    this->vlayout->addWidget(nidLabel);

    QTextEdit* infoEdit=new QTextEdit();
    QString info=QString("用户%1你好，您的快递单号:%2 的快递已送至驿站:%3 ，请及时取走包裹\n驿站地址:%4\n取件码:%5").arg(this->uid).arg(this->pid).arg(this->sname).arg(this->spos).arg(this->key);
    infoEdit->setText(info);
    this->vlayout->addWidget(infoEdit);

    this->button=new QPushButton();
    this->button->setText("标为已读");
    this->vlayout->addWidget(this->button);

    this->setLayout(this->vlayout);
}

/*              ******************************************************************************                      */
//以下是支付单盒子类
PaymentBox::PaymentBox(QFrame* parent):
    QFrame(parent)
{

}

PaymentBox::~PaymentBox()
{

}

void PaymentBox::PaymentBox_Init()
{
    this->flayout=new QFormLayout();

    QLabel* pidLabel=new QLabel("支付单号");
    QLabel* pidLine=new QLabel(this->payid);
    this->flayout->addRow(pidLabel,pidLine);

    QLabel* priceLabel=new QLabel("价格");
    QLabel* priceLine=new QLabel(QString::number(this->price));
    this->flayout->addRow(priceLabel,priceLine);

    QLabel* timeLabel=new QLabel("支付时间");
    QLabel* timeLine=new QLabel(this->paytime);
    this->flayout->addRow(timeLabel,timeLine);

    QLabel* infoLabel=new QLabel("详情信息");
    this->button=new QPushButton("点我");
    this->flayout->addRow(infoLabel,this->button);

    //连接详情信息槽函数
    connect(this->button,&QPushButton::clicked,this,[=](){
        PaymentForm* payform=new PaymentForm(nullptr,this->payid);
        payform->show();
    });

    this->setLayout(this->flayout);
}

