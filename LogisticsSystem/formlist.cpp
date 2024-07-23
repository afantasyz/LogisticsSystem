#include "formlist.h"
#include "ui_OrderForm.h"
#include "ui_PakageForm.h"
#include "ui_PaymentForm.h"
#include "ui_OrderShowForm.h"
#include "ui_PaymentShowForm.h"
#include "ui_PakageShowForm.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

/*              ******************************************************************************                      */
//以下是订单表OrderForm类

OrderForm::OrderForm(QDialog* parent):
    QDialog(parent),
    ui(new Ui::OrderForm)
{
    ui->setupUi(this);
}

OrderForm::~OrderForm()
{

}

OrderForm::OrderForm(QDialog* parent,QString oid):
    QDialog(parent),
    ui(new Ui::OrderForm)
{
    ui->setupUi(this);
    this->oid=oid;

    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    //查询数据
    QString sql=QString("select * from orders where O_id = '%1'").arg(this->oid);
    if(!query.exec(sql))
    {
        qDebug()<<"show order fail";
        return;
    }
    if(query.size()==0)
    {
        qDebug()<<"order do not exist";
        return;
    }
    query.next();

    //显示数据
    ui->lineEdit_oid->setText(query.value("O_id").toString());
    ui->lineEdit_sendname->setText(query.value("O_sendPerson").toString());
    ui->lineEdit_sendphone->setText(query.value("O_sendPhone").toString());
    ui->lineEdit_recievename->setText(query.value("O_recievePerson").toString());
    ui->lineEdit_recievephone->setText(query.value("O_recievePhone").toString());
    ui->lineEdi_uid->setText(query.value("O_u_id").toString());
    QString ostate=query.value("O_state").toInt()==0?"未处理":"已处理";
    ui->lineEdit_ostate->setText(ostate);
    ui->textEdit_sendAddr->setText(query.value("O_sendPositon").toString());
    ui->textEdit_recieveAddr->setText(query.value("O_recievePos").toString());
    ui->lineEdit_sendtime->setText(query.value("O_time").toString());
    ui->lineEdit_station->setText(query.value("O_s_id").toString());
}

/*              ******************************************************************************                      */
//以下是包裹表PakageForm类
PakageForm::PakageForm(QDialog* parent):
    QDialog(parent),
    ui(new Ui::PakageForm)
{
    ui->setupUi(this);
}

PakageForm::~PakageForm()
{

}

PakageForm::PakageForm(QDialog* parent,QString pid):
    QDialog(parent),
    ui(new Ui::PakageForm)
{
    ui->setupUi(this);
    this->pid=pid;

    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    //查询数据
    QString sql=QString("select * from pakageInfo where P_id = '%1'").arg(this->pid);
    if(!query.exec(sql))
    {
        qDebug()<<"show pakage fail";
        return;
    }
    if(query.size()==0)
    {
        qDebug()<<"pakage do not exist";
        return;
    }
    query.next();

    ui->lineEdit_id->setText(this->pid);
    ui->lineEdit_company->setText(query.value("P_company").toString());
    ui->lineEdit_state->setText(query.value("P_state").toString());
    ui->lineEdit_code->setText("");
    ui->lineEdit_sendtime->setText(query.value("P_time").toString());
    ui->lineEdit_sendname->setText(query.value("O_sendPerson").toString());
    ui->lineEdit_sendphone->setText(query.value("O_sendPhone").toString());
    ui->lineEdit_recievename->setText(query.value("O_recievePerson").toString());
    ui->lineEdit_recievephone->setText(query.value("O_recievePhone").toString());
    ui->textEdit_sendaddr->setText(query.value("O_sendPositon").toString());
    ui->textEdit_recieveaddr->setText(query.value("O_recievePos").toString());

    //处理单据跳转
    this->oid=query.value("P_O_id").toString();
    sql=QString("select * from payment where pay_O_id = '%1'").arg(this->oid);
    if(!query.exec(sql))
    {
        qDebug()<<"query payid fail";
        return;
    }
    query.next();
    this->payid=query.value("pay_id").toString();

    ui->label_order->setText(this->oid);
    ui->label_payment->setText(this->payid);

    connect(ui->pushButton_order,&QPushButton::clicked,this,[=](){
        OrderForm* form=new OrderForm(nullptr,this->oid);
        form->show();
    });

    connect(ui->pushButton_payment,&QPushButton::clicked,this,[=](){
        PaymentForm* form=new PaymentForm(nullptr,this->payid);
        form->show();
    });
}

/*              ******************************************************************************                      */
//以下是支付凭证paymentForm类

PaymentForm::PaymentForm(QDialog* parent):
    QDialog(parent),
    ui(new Ui::PaymentForm)
{
    ui->setupUi(this);
}

PaymentForm::~PaymentForm()
{

}

PaymentForm::PaymentForm(QDialog* parent,QString payid):
    QDialog(parent),
    ui(new Ui::PaymentForm)
{
    ui->setupUi(this);    
    this->payid=payid;

    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    //查询数据库
    QString sql=QString("select * from payInfo where pay_id = '%1'").arg(this->payid);
    if(!query.exec(sql))
    {
        qDebug()<<"query payinfo fail";
        return;
    }
    if(query.size()==0)
    {
        qDebug()<<"query has no result";
        return;
    }
    query.next();

    ui->lineEdit_payid->setText(this->payid);
    ui->lineEdit_price->setText(query.value("pay_price").toString());
    ui->lineEdit_paytime->setText(query.value("pay_time").toString());
    ui->lineEdit_uid->setText(query.value("u_id").toString());
    ui->lineEdit_uname->setText(query.value("u_name").toString());
    ui->lineEdit_uphone->setText(query.value("u_phone").toString());

    //处理单据跳转
    this->oid=query.value("pay_O_id").toString();
    ui->label_order->setText(this->oid);
    sql=QString("select * from pakage where P_O_id = '%1'").arg(this->oid);
    if(!query.exec(sql))
    {
        qDebug()<<"query pid fail";
        return;
    }
    query.next();
    this->pid=query.value("P_id").toString();
    ui->label_pakage->setText(this->pid);

    connect(ui->pushButton_order,&QPushButton::clicked,this,[=](){
        OrderForm* form= new OrderForm(nullptr,this->oid);
        form->show();
    });

    connect(ui->pushButton_pakage,&QPushButton::clicked,this,[=](){
        PakageForm* form=new PakageForm(nullptr,this->pid);
        form->show();
    });

}

/*              ******************************************************************************                      */
//以下是支付凭证OrderShowForm类
OrderShowForm::OrderShowForm(QDialog* parent):
    QDialog(parent),
    ui(new Ui::OrderShowForm)
{
    ui->setupUi(this);

}

OrderShowForm::~OrderShowForm()
{

}

OrderShowForm::OrderShowForm(QDialog* parent,QString oid):
    QDialog(parent),
    ui(new Ui::OrderShowForm)
{
    ui->setupUi(this);
    this->oid=oid;

    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    //查询数据
    QString sql=QString("select * from orders where O_id = '%1'").arg(this->oid);
    if(!query.exec(sql))
    {
        qDebug()<<"show order fail";
        return;
    }
    if(query.size()==0)
    {
        qDebug()<<"order do not exist";
        return;
    }
    query.next();

    //显示数据
    ui->lineEdit_oid->setText(query.value("O_id").toString());
    ui->lineEdit_sendname->setText(query.value("O_sendPerson").toString());
    ui->lineEdit_sendphone->setText(query.value("O_sendPhone").toString());
    ui->lineEdit_recievename->setText(query.value("O_recievePerson").toString());
    ui->lineEdit_recievephone->setText(query.value("O_recievePhone").toString());
    ui->lineEdi_uid->setText(query.value("O_u_id").toString());
    QString ostate=query.value("O_state").toInt()==0?"未处理":"已处理";
    ui->lineEdit_ostate->setText(ostate);
    ui->textEdit_sendAddr->setText(query.value("O_sendPositon").toString());
    ui->textEdit_recieveAddr->setText(query.value("O_recievePos").toString());
    ui->lineEdit_sendtime->setText(query.value("O_time").toString());
    ui->lineEdit_station->setText(query.value("O_s_id").toString());

    //提交修改功能
    connect(ui->pushButton_alter,&QPushButton::clicked,this,[=](){
        //连接数据库
        QSqlDatabase db=QSqlDatabase::database("connection01");
        QSqlQuery query(db);

        QString oid=ui->lineEdit_oid->text();
        QString sname=ui->lineEdit_sendname->text();
        QString sphone=ui->lineEdit_sendphone->text();
        QString sAddr=ui->textEdit_sendAddr->toPlainText();
        QString rname=ui->lineEdit_recievename->text();
        QString rphone=ui->lineEdit_recievephone->text();
        QString rAddr=ui->textEdit_recieveAddr->toPlainText();

        query.prepare("update orders set O_sendPerson=:sname,O_sendPhone=:sphone,O_sendPositon=:sAddr,O_recievePerson=:rname,O_recievePhone=:rphone,O_recievePos=:rAddr where O_id=:oid");
        query.bindValue(":sname",sname);
        query.bindValue(":sphone",sphone);
        query.bindValue(":sname",sAddr);
        query.bindValue(":sname",rname);
        query.bindValue(":sname",rphone);
        query.bindValue(":sname",rAddr);
        query.bindValue(":oid",oid);

        if(!query.exec())
        {
            qDebug()<<"alter order success!";
            return;
        }

        QMessageBox::information(this,"success","修改成功");
    });
}

/*              ******************************************************************************                      */
//以下是支付凭证PaymentShowForm类
PaymentShowForm::PaymentShowForm(QDialog* parent):
    QDialog(parent),
    ui(new Ui::PaymentShowForm)
{
    ui->setupUi(this);

    this->line_Id=ui->lineEdit_uid;
    this->line_payid=ui->lineEdit_payid;
    this->line_phone=ui->lineEdit_uphone;
    this->line_price=ui->lineEdit_price;
    this->line_time=ui->lineEdit_paytime;
    this->line_name=ui->lineEdit_uname;
    this->label_oid=ui->label_order;
    this->button_submit=ui->pushButton_submit;
}

PaymentShowForm::~PaymentShowForm()
{

}

//PaymentShowForm::PaymentShowForm(QDialog* parent,QString payid):
//    QDialog(parent),
//    ui(new Ui::PaymentShowForm)
//{
//    ui->setupUi(this);

//    this->payid=payid;

//}

/*              ******************************************************************************                      */
//以下是支付凭证PakageShowForm类
PakageShowForm::PakageShowForm(QDialog* parent):
    QDialog(parent),
    ui(new Ui::PakageShowForm)
{
    ui->setupUi(this);
}

PakageShowForm::~PakageShowForm()
{

}

void PakageShowForm::PakageShowForm_Init(QString oid)
{
    this->oid=oid;
    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    //查询当前时间
    QDateTime currtime=QDateTime::currentDateTime();
    QString timestr=currtime.toString("yyyy-MM-dd hh:mm:ss");
    QString datestr=currtime.toString("yyMMdd");
    this->ptime=timestr;

    QString sql=QString("select count(*) from pakage where P_id like 'P%1%'").arg(datestr);
    if(!query.exec(sql))
    {
        qDebug()<<"generate pakage num fail";
        return ;
    }
    query.next();
    int pakagenum=query.value(0).toInt();
    this->pid=QString("P%1%2").arg(datestr).arg(10000+pakagenum);

    //查询数据
    sql=QString("select * from orders where O_id = '%1'").arg(this->oid);
    if(!query.exec(sql))
    {
        qDebug()<<"show order fail";
        return;
    }
    if(query.size()==0)
    {
        qDebug()<<"pakage do not exist";
        return;
    }
    query.next();

    ui->lineEdit_id->setText(this->pid);
    ui->lineEdit_sendtime->setText(timestr);
    ui->label_order->setText(this->oid);
    ui->lineEdit_sendname->setText(query.value("O_sendPerson").toString());
    ui->lineEdit_sendphone->setText(query.value("O_sendPhone").toString());
    ui->lineEdit_recievename->setText(query.value("O_recievePerson").toString());
    ui->lineEdit_recievephone->setText(query.value("O_recievePhone").toString());
    ui->textEdit_sendaddr->setText(query.value("O_sendPositon").toString());
    ui->textEdit_recieveaddr->setText(query.value("O_recievePos").toString());

    //处理生成按钮
    connect(ui->pushButton_submit,&QPushButton::clicked,this,[=](){
        //建立数据库连接
        QSqlDatabase db2=QSqlDatabase::database("connection01");
        QSqlQuery query2(db2);

        //生成快递单
        QString sql=QString("insert into pakage values('%1','%2','%3','%4','%5')").arg(this->pid).arg(ui->comboBox_company->currentText()).arg("待派发").arg(this->oid).arg(this->ptime);
        if(!query2.exec(sql))
        {
            qDebug()<<"insert pakage fail";
            return ;
        }
        //修改订单信息
        sql=QString("update orders set O_state = 1 where O_id = '%1'").arg(this->oid);
        if(!query2.exec(sql))
        {
            qDebug()<<"update order state fail";
            db2.rollback();
            return ;
        }

        QMessageBox::information(this,"success","生成快递单成功");
        emit this->PakageIsCreate();
        this->close();
    });
}
