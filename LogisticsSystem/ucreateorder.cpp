#include "ucreateorder.h"
#include "ui_ucreateorder.h"
#include "ui_uqueryorder.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QDateTime>

UCreateOrder::UCreateOrder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UCreateOrder)
{
    ui->setupUi(this);
}

UCreateOrder::~UCreateOrder()
{
    delete ui;
}

//初始化输入框内容
void UCreateOrder::Info_Init(void)
{
    ui->lineEdit_sendname->setText(this->parentWidegt->Uname);
    ui->lineEdit_sendphone->setText(this->parentWidegt->Uphone);

    //按老师要求修改
        //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);
        //计算单号
    QDateTime time=QDateTime::currentDateTime();
    QString tstr=time.toString("yyMMdd");
    if(!query.exec(QString("select count(*) from orders where O_id like 'O%1%'").arg(tstr)))
    {
        qDebug()<<"genetate oid fail";
        return;
    }
    query.next();
    int num=query.value(0).toInt()+10000;
    QString oid=QString("O%1%2").arg(tstr).arg(QString::number(num));
    ui->lineEdit_Oid->setText(oid);

    //连接槽函数
    connect(ui->pushButton_commit,&QPushButton::clicked,this,&UCreateOrder::Creat_order);
    connect(ui->pushButton_cancel,&QPushButton::clicked,this,[=](){
        this->close();
    });
}

//创建订单
void UCreateOrder::Creat_order()
{
    //获取数据
    QString sname=ui->lineEdit_sendname->text();
    QString sphone=ui->lineEdit_sendphone->text();
    QString sAddr=ui->textEdit_sendAddr->toPlainText();
    QString rname=ui->lineEdit_recievename->text();
    QString rphone=ui->lineEdit_recievephone->text();
    QString rAddr=ui->textEdit_recieveAddr->toPlainText();
    QString station=ui->comboBox_station->currentText();
    QString weight=ui->lineEdit_weight->text();
    if(sname.isEmpty()||sphone.isEmpty()||sAddr.isEmpty()||rname.isEmpty()||rphone.isEmpty()||rphone.isEmpty()||rAddr.isEmpty()||station.isEmpty())
    {
        QMessageBox::warning(this,"订单创建失败","信息不能为空");
        return;
    }

    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    //计算单号
    QDateTime time=QDateTime::currentDateTime();
    QString tstr=time.toString("yyMMdd");
    QString timestr=time.toString("yyyy-MM-dd hh:mm:ss");
    if(!query.exec(QString("select count(*) from orders where O_id like 'O%1%'").arg(tstr)))
    {
        qDebug()<<"genetate oid fail";
        return;
    }
    query.next();
    int num=query.value(0).toInt()+10000;
    QString oid=QString("O%1%2").arg(tstr).arg(QString::number(num));

    //查询驿站id
    if(!query.exec(QString("select * from station where s_name = '%1'").arg(station)))
    {
        qDebug()<<"query station id fail";
        return;
    }
    query.next();
    QString sid=query.value("s_id").toString();

    //插入请求
    query.prepare("insert into orders values(:oid,:sname,:sphone,:saddr,:rname,:rphone,:raddr,:time,:weight,:uid,:sid,:state)");
    query.bindValue(":oid",oid);
    query.bindValue(":sname",sname);
    query.bindValue(":sphone",sphone);
    query.bindValue(":saddr",sAddr);
    query.bindValue(":rname",rname);
    query.bindValue(":rphone",rphone);
    query.bindValue(":raddr",rAddr);
    query.bindValue(":time",timestr);
    query.bindValue(":weight",weight);
    query.bindValue(":uid",this->parentWidegt->Uaccount);
    query.bindValue(":sid",sid);
    query.bindValue(":state",0);

    if(!query.exec())
    {
        qDebug()<<"creat order fail";
        return;
    }
    QString info=QString("订单ID:%1").arg(oid);
    QMessageBox::information(this,"寄件申请成功",info);
    this->close();
}


/*                                 *****************************************                        */
//以下是UQueryorder类成员
UQueryOrder::UQueryOrder(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::UQueryOrder)
{
    ui->setupUi(this);
}

UQueryOrder::~UQueryOrder()
{
    delete ui;
}

void UQueryOrder::MorderQuery_Init()
{
    //处理按钮
    connect(ui->pushButton_select_commit,&QPushButton::clicked,this,[=](){
        //获取输入文本
        QString oid=ui->lineEdit_oid->text();
        if(!oid.isEmpty())
        {
            QString sql=QString("select * from orders where O_u_id = '%1' and O_id = '%2'").arg(this->parentWid->Uaccount).arg(oid);
            this->parentWid->Order_Function2(sql);
            return;
        }
        QString pid=ui->lineEdit_sid->text();
        if(!pid.isEmpty())
        {
            QSqlDatabase db=QSqlDatabase::database("connection01");
            QSqlQuery query(db);
            if(!query.exec(QString("select * from pakage where P_id = '%1'").arg(pid)))
            {
                QMessageBox::information(this,"查找空","该订单尚未处理");
                return ;
            }
            query.next();
            QString oid=query.value("P_O_id").toString();
            QString sql=QString("select * from orders where O_u_id = '%1' and O_id = '%2'").arg(this->parentWid->Uaccount).arg(oid);
            this->parentWid->Order_Function2(sql);
            return;
        }
        int state=ui->comboBox_select_state->currentIndex();
        if(state==1)
        {
            QString sql=QString("select * from orders where O_u_id = '%1' and O_state = %2").arg(this->parentWid->Uaccount).arg(1);
            this->parentWid->Order_Function2(sql);
            return;
        }
        else if(state==2)
        {
            QString sql=QString("select * from orders where O_u_id = '%1' and O_state = %2").arg(this->parentWid->Uaccount).arg(0);
            this->parentWid->Order_Function2(sql);
            return;
        }
    });

    //连接排序按钮
    connect(ui->pushButton_sort_commit,&QPushButton::clicked,this,[=](){
        //获取输入
        int oid=ui->comboBox_sort_oid->currentIndex();
        int time=ui->comboBox_sort_time->currentIndex();
        int state=ui->comboBox_sort_state->currentIndex();

        //排序
        if(oid==1)
        {
            QString sql=QString("select * from orders where O_u_id = '%1' order by O_id").arg(this->parentWid->Uaccount);
            this->parentWid->Order_Function2(sql);
            return;
        }
        else if(oid==2)
        {
            QString sql=QString("select * from orders where O_u_id = '%1' order by O_id DESC").arg(this->parentWid->Uaccount);
            this->parentWid->Order_Function2(sql);
            return;
        }

        if(time==1)
        {
            QString sql=QString("select * from orders where O_u_id = '%1' order by O_time").arg(this->parentWid->Uaccount);
            this->parentWid->Order_Function2(sql);
            return;
        }
        else if(time==2)
        {
            QString sql=QString("select * from orders where O_u_id = '%1' order by O_time DESC").arg(this->parentWid->Uaccount);
            this->parentWid->Order_Function2(sql);
            return;
        }

        if(state==1)
        {
            QString sql=QString("select * from orders where O_u_id = '%1' order by O_state").arg(this->parentWid->Uaccount);
            this->parentWid->Order_Function2(sql);
            return;
        }
        else if(state==2)
        {
            QString sql=QString("select * from orders where O_u_id = '%1' order by O_state DESC").arg(this->parentWid->Uaccount);
            this->parentWid->Order_Function2(sql);
            return;
        }
    });
}
