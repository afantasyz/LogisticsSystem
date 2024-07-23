#include "upakage.h"
#include "ui_UPakageQuery.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

UPakageQuery::UPakageQuery(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::UPakageQuery)
{
    ui->setupUi(this);

}

UPakageQuery::~UPakageQuery()
{

}

void UPakageQuery::UPakageQuery_Init()
{
    //处理按钮
    connect(ui->pushButton_select_commit,&QPushButton::clicked,this,[=](){
        //获取输入文本
        QString pid=ui->lineEdit_select_pid->text();
        if(!pid.isEmpty())
        {
            QString sql=QString("select * from pakageInfo where p_id = '%1'").arg(pid);
            this->parentWid->Pakage_Function2(sql);
            return;
        }
        QString oid=ui->lineEdit_select_oid->text();
        if(!oid.isEmpty())
        {
            QString sql=QString("select * from pakageInfo where P_O_id = '%1'").arg(oid);
            this->parentWid->Pakage_Function2(sql);
            return;
        }
        QString company=ui->comboBox_select_company->currentText();
        if(!company.isEmpty())
        {
            QString sql=QString("select * from pakageInfo where P_company = '%1' and O_recievePhone = '%2'").arg(company).arg(this->parentWid->Uphone);
            this->parentWid->Pakage_Function2(sql);
            return;
        }
        QString state=ui->comboBox_select_state->currentText();
        if(!state.isEmpty())
        {
            QString sql=QString("select * from pakageInfo where P_state = '%1' and O_recievePhone = '%2'").arg(state).arg(this->parentWid->Uphone);
            this->parentWid->Pakage_Function2(sql);
            return;
        }
    });

    //连接排序按钮
    connect(ui->pushButton_sort_commit,&QPushButton::clicked,this,[=](){
        //获取输入
        int pid=ui->comboBox_sort_pid->currentIndex();
        int time=ui->comboBox_sort_time->currentIndex();
        int state=ui->comboBox_sort_state->currentIndex();
        int company=ui->comboBox_sort_company->currentIndex();

        //排序
        if(pid==1)
        {
            QString sql=QString("select * from pakageInfo where O_recievePhone = '%1' order by P_id").arg(this->parentWid->Uphone);
            this->parentWid->Pakage_Function2(sql);
            return;
        }
        else if(pid==2)
        {
            QString sql=QString("select * from pakageInfo where O_recievePhone = '%1' order by P_id DESC").arg(this->parentWid->Uphone);
            this->parentWid->Pakage_Function2(sql);
            return;
        }

        if(time==1)
        {
            QString sql=QString("select * from pakageInfo where O_recievePhone = '%1' order by P_time").arg(this->parentWid->Uphone);
            this->parentWid->Pakage_Function2(sql);
            return;
        }
        else if(time==2)
        {
            QString sql=QString("select * from pakageInfo where O_recievePhone = '%1' order by P_time DESC").arg(this->parentWid->Uphone);
            this->parentWid->Pakage_Function2(sql);
            return;
        }

        if(state==1)
        {
            QString sql=QString("select * from pakageInfo where O_recievePhone = '%1' order by P_state").arg(this->parentWid->Uphone);
            this->parentWid->Pakage_Function2(sql);
            return;
        }
        else if(state==2)
        {
            QString sql=QString("select * from pakageInfo where O_recievePhone = '%1' order by P_state DESC").arg(this->parentWid->Uphone);
            this->parentWid->Pakage_Function2(sql);
            return;
        }

        if(company==1)
        {
            QString sql=QString("select * from pakageInfo where O_recievePhone = '%1' order by P_company").arg(this->parentWid->Uphone);
            this->parentWid->Pakage_Function2(sql);
            return;
        }
        else if(company==2)
        {
            QString sql=QString("select * from pakageInfo where O_recievePhone = '%1' order by P_company DESC").arg(this->parentWid->Uphone);
            this->parentWid->Pakage_Function2(sql);
            return;
        }
    });

    //查询重置按钮
    connect(ui->pushButton_select_reset,&QPushButton::clicked,this,[=](){
        this->parentWid->Pakage_Function();
    });

    //排序重置按钮
    connect(ui->pushButton_sort_reset,&QPushButton::clicked,this,[=](){
        this->parentWid->Pakage_Function();
    });
}
