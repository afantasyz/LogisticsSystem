#include "malterpackage.h"
#include "ui_malterpackage.h"
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QMessageBox>
#include<QDebug>
#include<QSqlError>

#include "formlist.h"

MAlterPackage::MAlterPackage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MAlterPackage)
{
    ui->setupUi(this);

    //连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");

    //查询按钮
    connect(ui->pushButton_select,&QPushButton::clicked,this,[=](){
        //查询对应结果
        QSqlQuery query(db);
        QString id=ui->lineEdit_id->text();
        qDebug()<<id;
        if(id.isEmpty())
            return;
        if(!query.exec(QString("select * from pakageInfo where P_id = '%1'").arg(id)))
        {
            qDebug()<<"error";
            QMessageBox::warning(this,"查询错误","查找失败");
            return;
        }
        if(query.size()==0)
        {
            qDebug()<<"warning";
            QMessageBox::warning(this,"查询错误","查找空");
            return;
        }
        qDebug()<<"success";
        query.next();
        QSqlRecord record=query.record();

        //给defalut赋值
        QString oid=record.value("P_O_id").toString();
        this->oid=oid;
        ui->label_order->setText(oid);
        ui->lineEdit_company->setText(record.value("P_company").toString());
        ui->lineEdit_recievename->setText(record.value("O_recievePerson").toString());
        ui->lineEdit_recievephone->setText(record.value("O_recievePhone").toString());
        ui->lineEdit_sendname->setText(record.value("O_sendPerson").toString());
        ui->lineEdit_sendphone->setText(record.value("O_sendPhone").toString());
        ui->lineEdit_sendtime->setText(record.value("P_time").toString());
        ui->lineEdit_state->setText(record.value("P_state").toString());
        ui->textEdit_sendaddr->setText(record.value("O_sendPositon").toString());
        ui->textEdit_recieveaddr->setText(record.value("O_recievePos").toString());

        //支付单号
        if(query.exec(QString("select * from payment where pay_O_id = '%1'").arg(oid)))
        {
            if(query.size()==0)
                return;
            qDebug()<<"query has result";
            query.next();
            this->payid=query.value("pay_id").toString();
            ui->label_payment->setText(this->payid);
        }

        //跳转支付单按钮
        connect(ui->pushButton_payment,&QPushButton::clicked,this,[=](){
            PaymentForm* payform=new PaymentForm(nullptr,this->payid);
            payform->show();
        });

        //跳转订单按钮
        connect(ui->pushButton_order,&QPushButton::clicked,this,[=](){
            OrderForm* oform=new OrderForm(nullptr,this->oid);
            oform->show();
        });

    });

    //取消按钮
    connect(ui->pushButton_quit,&QPushButton::clicked,this,[=](){
        delete this;
    });

    //确定按钮
    connect(ui->pushButton_save,&QPushButton::clicked,this,[=](){
        QString id=ui->lineEdit_id->text();
        QString oid=ui->label_order->text();
        if(id.isEmpty())
            return;
        //像包裹数据库写入数据
        QString company=ui->lineEdit_company->text();
        QString stime=ui->lineEdit_sendtime->text();
        QString state=ui->lineEdit_state->text();

        QSqlQuery query(db);
        QString command="update pakage set ";
        command+="P_company=:company, P_time=now(), P_state=:state ";
        command+=QString("where P_id= '%1'").arg(id);
        query.prepare(command);
        query.bindValue(":company",company);
//        query.bindValue(":time","now()");
        query.bindValue(":state",state);

        //开始执行
        if(!query.exec())
        {
            qDebug()<<"pakage执行失败";
            QMessageBox::warning(this,"warning","修改失败");
            QSqlError error;
            error=db.lastError();
            qDebug()<<error.text();
        }

        //向订单数据库修改数据
        QString rname=ui->lineEdit_recievename->text();
        QString rphone=ui->lineEdit_recievephone->text();
        QString sname=ui->lineEdit_sendname->text();
        QString sphone=ui->lineEdit_sendphone->text();
        QString saddr=ui->textEdit_sendaddr->toPlainText();
        QString raddr=ui->textEdit_recieveaddr->toPlainText();

        command="update orders set ";
        command+="O_recievePerson=:rname, O_recievePhone=:rphone,";
        command+="O_sendPerson=:sname, O_sendPhone=:sphone,";
        command+="O_sendPositon=:saddr, O_recievePos=:raddr ";
        command+=QString("where O_id= '%1'").arg(oid);

        query.prepare(command);
        query.bindValue(":rname",rname);
        query.bindValue(":rphone",rphone);
        query.bindValue(":sname",sname);
        query.bindValue(":sphone",sphone);
        query.bindValue(":saddr",saddr);
        query.bindValue(":raddr",raddr);

        //开始执行
        if(!query.exec())
        {
            qDebug()<<"orders执行失败";
            QMessageBox::warning(this,"warning","修改失败");
            QSqlError error;
            error=db.lastError();
            qDebug()<<error.text();
        }
        QMessageBox::information(this,"修改成功",QString("成功更新快递单：%1\n,更新订单：%2").arg(id).arg(oid));
        qDebug()<<"yes";
    });
}

MAlterPackage::~MAlterPackage()
{
    delete ui;
}
