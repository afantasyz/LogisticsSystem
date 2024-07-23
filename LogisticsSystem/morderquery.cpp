#include "morderquery.h"
#include "ui_morderquery.h"

MOrderQuery::MOrderQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MOrderQuery)
{
    ui->setupUi(this);
}

MOrderQuery::~MOrderQuery()
{
    delete ui;
}

void MOrderQuery::MorderQuery_Init(QSqlTableModel* m)
{
    this->model=m;
    //处理按钮
    connect(ui->pushButton_select_commit,&QPushButton::clicked,this,[=](){
        //获取输入文本
        QString oid=ui->lineEdit_oid->text();
        if(!oid.isEmpty())
        {
            model->setFilter(QString("O_id='%1'").arg(oid));
            return;
        }
        QString pid=ui->lineEdit_uid->text();
        if(!pid.isEmpty())
        {
            model->setFilter(QString("O_u_id='%1'").arg(pid));
            return;
        }
        int state=ui->comboBox_select_state->currentIndex();
        if(state==1)
        {
            model->setFilter(QString("O_state= 0 "));
            return;
        }
        else if(state==2)
        {
            model->setFilter(QString("O_state= 1 "));
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
            model->sort(0,Qt::SortOrder::AscendingOrder);
            return;
        }
        else if(oid==2)
        {
            model->sort(2,Qt::SortOrder::DescendingOrder);
            return;
        }

        if(time==1)
        {
            model->sort(7,Qt::SortOrder::AscendingOrder);
            return;
        }
        else if(time==2)
        {
            model->sort(7,Qt::SortOrder::DescendingOrder);
            return;
        }

        if(state==1)
        {
            model->sort(11,Qt::SortOrder::AscendingOrder);
            return;
        }
        else if(state==2)
        {
            model->sort(11,Qt::SortOrder::DescendingOrder);
            return;
        }
    });
}
