#include "minstorgerecord.h"
#include "ui_minstorgerecord.h"
#include <QMessageBox>

MInstorgeRecord::MInstorgeRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MInstorgeRecord)
{
    ui->setupUi(this);

}

MInstorgeRecord::~MInstorgeRecord()
{
    delete ui;
}

void MInstorgeRecord::MInstore_Init(QSqlTableModel* m)
{
    this->model=m;
    //处理按钮
    connect(ui->pushButton_select,&QPushButton::clicked,this,[=](){
        //获取输入文本
        QString iid=ui->lineEdit_select_iid->text();
        if(!iid.isEmpty())
        {
            model->setFilter(QString("i_id='%1'").arg(iid));
            return;
        }
        QString pid=ui->lineEdit_select_pid->text();
        if(!pid.isEmpty())
        {
            model->setFilter(QString("i_p_id='%1'").arg(pid));
            return;
        }

        QString key=ui->lineEdit_select_key->text();
        QString time=ui->lineEdit_select_time->text();
        QString company=ui->comboBox_select_company->currentText();

        //开始处理
        QString queryline;
        if(!key.isEmpty())
        {
            queryline=QString("i_key='%1'").arg(key);
        }

        if(!time.isEmpty())
        {
            if(!queryline.isEmpty())
                queryline+=" and ";
            queryline+=QString("i_time='%1'").arg(time);
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
    connect(ui->pushButton_sort,&QPushButton::clicked,this,[=](){
        //获取输入
        int iid=ui->comboBox_sort_iid->currentIndex();
        int key=ui->comboBox_sort_key->currentIndex();
        int time=ui->comboBox_sort_time->currentIndex();
        int company=ui->comboBox_sort_company->currentIndex();

        //排序
        if(iid==1)
        {
            model->sort(0,Qt::SortOrder::AscendingOrder);
            return;
        }
        else if(iid==2)
        {
            model->sort(2,Qt::SortOrder::DescendingOrder);
            return;
        }

        if(key==1)
        {
            model->sort(2,Qt::SortOrder::AscendingOrder);
            return;
        }
        else if(key==2)
        {
            model->sort(0,Qt::SortOrder::DescendingOrder);
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
            model->sort(6,Qt::SortOrder::AscendingOrder);
            return;
        }
        else if(company==2)
        {
            model->sort(6,Qt::SortOrder::DescendingOrder);
            return;
        }
    });
}
