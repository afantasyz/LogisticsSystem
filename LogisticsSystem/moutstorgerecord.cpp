#include "moutstorgerecord.h"
#include "ui_moutstorgerecord.h"
#include <QMessageBox>

MOutstorgeRecord::MOutstorgeRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MOutstorgeRecord)
{
    ui->setupUi(this);
}

MOutstorgeRecord::~MOutstorgeRecord()
{
    delete ui;
}

void MOutstorgeRecord::Moutstore_Init(QSqlTableModel* m)
{
    this->model=m;
    //处理按钮
    connect(ui->pushButton_select_commit,&QPushButton::clicked,this,[=](){
        //获取输入文本
        QString oid=ui->lineEdit_select_oid->text();
        if(!oid.isEmpty())
        {
            model->setFilter(QString("o_id='%1'").arg(oid));
            return;
        }
        QString pid=ui->lineEdit_select_pid->text();
        if(!pid.isEmpty())
        {
            model->setFilter(QString("o_p_id='%1'").arg(pid));
            return;
        }

        QString time=ui->lineEdit_select_time->text();
        QString company=ui->comboBox_select_company->currentText();

        //开始处理
        QString queryline;

        if(!time.isEmpty())
        {
            queryline=QString("o_time='%1'").arg(time);
        }

        if(!company.isEmpty())
        {
            if(!queryline.isEmpty())
                queryline+=" and ";
            queryline+=QString("P_company='%1'").arg(company);
        }

        model->setFilter(queryline);
        if(model->rowCount()==0)
        {
            QMessageBox::warning(this,"warning","未查找到结果");
        }
    });

    //连接排序按钮
    connect(ui->pushButton_sort_commit,&QPushButton::clicked,this,[=](){
        //获取输入
        int oid=ui->comboBox_sort_oid->currentIndex();
        int time=ui->comboBox_sort_time->currentIndex();
        int company=ui->comboBox_sort_company->currentIndex();
        int pid=ui->comboBox_sort_pid->currentIndex();

        //排序
        if(oid==1)
        {
            model->sort(0,Qt::SortOrder::AscendingOrder);
            return;
        }
        else if(oid==2)
        {
            model->sort(2,Qt::SortOrder::DescendingOrder);
            return;
        }

        if(pid==1)
        {
            model->sort(1,Qt::SortOrder::AscendingOrder);
            return;
        }
        else if(pid==2)
        {
            model->sort(1,Qt::SortOrder::DescendingOrder);
            return;
        }

        if(time==1)
        {
            model->sort(3,Qt::SortOrder::AscendingOrder);
            return;
        }
        else if(time==2)
        {
            model->sort(3,Qt::SortOrder::DescendingOrder);
            return;
        }

        if(company==1)
        {
            model->sort(4,Qt::SortOrder::AscendingOrder);
            return;
        }
        else if(company==2)
        {
            model->sort(4,Qt::SortOrder::DescendingOrder);
            return;
        }
    });

    connect(ui->pushButton_select_reset,&QPushButton::clicked,this,[=](){
        model->setFilter("");
    });

    connect(ui->pushButton_sort_reset,&QPushButton::clicked,this,[=](){
        model->setFilter("");
    });

}
