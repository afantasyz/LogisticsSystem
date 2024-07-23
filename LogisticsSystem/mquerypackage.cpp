#include "mquerypackage.h"
#include "ui_mquerypackage.h"
#include <QDebug>
#include <QMessageBox>

MQueryPackage::MQueryPackage(QWidget *parent,QSqlTableModel* m) :
    QWidget(parent),
    ui(new Ui::MQueryPackage)
{
    ui->setupUi(this);

    this->model=m;
    //找到错误原因了，构造函数比赋值函数更早，故此时model是一个空指针,现修改初始化函数
    //修改之后还是用不了喵，TableView也要传进来，参数太多了不如出初始表摆烂
//    this->model->clear();
//    this->model->setTable("pakageInfo");
//    this->model->select();
//    this->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //查询按钮
    connect(ui->pushButton_select,&QPushButton::clicked,this,[=](){
        //id query line
        QString id=ui->lineEdit_pid->text();
        if(!id.isEmpty())
        {
            model->setFilter(QString("P_id = '%1'").arg(id));
            qDebug()<<"id query";
            return;
        }

        //other query line
        QString pdate=ui->lineEdit_pdate->text();
        QString pstate=ui->lineEdit_pstate->text();
        QString pcompany=ui->comboBox_select_company->currentText();

        QString queryline;
        if(!pdate.isEmpty())
        {
            queryline+=QString("P_date = '%1'").arg(pdate);
        }
        if(!pstate.isEmpty())
        {
            if(!queryline.isEmpty())
                queryline+=" and ";
            queryline+=QString(QString("P_state = '%1'").arg(pstate));
        }
        if(!pcompany.isEmpty())
        {
            if(!queryline.isEmpty())
                queryline+=" and ";
            queryline+=QString(QString("P_company = '%1'").arg(pcompany));
        }
        if(queryline.isEmpty())
            return;
        model->setFilter(queryline);
        if(model->rowCount()==0)
        {
            QMessageBox::warning(this,"warning","未查找到结果");
        }
    });

    //重置按钮
    connect(ui->pushButton_selectreset,&QPushButton::clicked,this,[=](){
        model->setFilter("");
    });

    //排序按钮
    connect(ui->pushButton_sort,&QPushButton::clicked,this,[=](){
        //id排序
        int pid=ui->comboBox_pid->currentIndex();
        if(pid==1)
        {
            model->sort(0,Qt::AscendingOrder);
            return;
        }
        else if(pid==2)
        {
            model->sort(0,Qt::SortOrder::DescendingOrder);
            return;
        }

        //日期排序
        int pdate=ui->comboBox_pdate->currentIndex();
        if(pdate==1)
        {
            model->sort(4,Qt::SortOrder::AscendingOrder);
            qDebug()<<"pdate up";
            return ;
        }
        else if(pdate==2)
        {
            model->sort(4,Qt::SortOrder::DescendingOrder);
            return ;
        }

        //快递状态排序
        int state=ui->comboBox_pstate->currentIndex();
        if(state!=0)
        {
            model->sort(2,Qt::SortOrder::AscendingOrder);
            qDebug()<<"state up";
            return;
        }

        //快递公司排序
        int company=ui->comboBox_pcompany->currentIndex();
        if(company==1)
        {
            model->sort(1,Qt::SortOrder::AscendingOrder);
            qDebug()<<"company up";
        }
        else if(company==2)
        {
            model->sort(1,Qt::SortOrder::DescendingOrder);
            qDebug()<<"compamy down";
        }
    });

    //重置按钮
    connect(ui->pushButton_sortreset,&QPushButton::clicked,this,[=](){
        model->sort(0,Qt::AscendingOrder);
    });

}

MQueryPackage::~MQueryPackage()
{
    delete ui;
}
