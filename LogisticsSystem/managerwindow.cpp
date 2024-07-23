#include "managerwindow.h"
#include "ui_managerwindow.h"

#include "mquerypackage.h"
#include "malterpackage.h"
#include "minstorgerecord.h"
#include "moutstorgerecord.h"
#include "morderquery.h"
#include "mrequestprocess.h"
#include "msendmessage.h"
#include "mstoreprocess.h"

#include<QSqlDatabase>
#include<QMessageBox>
#include<QString>
#include<QSqlQuery>
#include<QSqlRecord>

#include<QPainter>
#include<QPixmap>

ManagerWindow::ManagerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagerWindow)
{
    ui->setupUi(this);

    //数据表视图显示，数据按钮,先连接数据库
    QSqlDatabase db=QSqlDatabase::database("connection01");
    this->model=new QSqlTableModel(this,db);

    //修改包裹按钮
    connect(ui->pushButton_alterPakege_fuction,&QPushButton::clicked,this,[=](){
        MAlterPackage* mwidegt=new MAlterPackage();
        mwidegt->show();
    });
    //入库查询按钮
    connect(ui->pushButton_instorge_fuction,&QPushButton::clicked,this,[=](){
        MInstorgeRecord* mwidegt=new MInstorgeRecord();
        mwidegt->MInstore_Init(this->model);
        mwidegt->show();
    });
    //出库记录查询
    connect(ui->pushButton_outstorge_function,&QPushButton::clicked,this,[=](){
        MOutstorgeRecord* mwidegt=new MOutstorgeRecord();
        mwidegt->Moutstore_Init(this->model);
        mwidegt->show();
    });

    //订单查询按钮
    connect(ui->pushButton_queryOrder_fuction,&QPushButton::clicked,this,[=](){
        MOrderQuery* mwidegt=new MOrderQuery();
        mwidegt->MorderQuery_Init(this->model);
        mwidegt->show();
    });

    //包裹查询按钮
    connect(ui->pushButton_queryPackage_function,&QPushButton::clicked,this,[=](){
        MQueryPackage* mwidegt=new MQueryPackage(nullptr,this->model);
        mwidegt->show();
    });

    //处理订单功能
    connect(ui->pushButton_processRequest_function,&QPushButton::clicked,this,[=](){
        MRequestProcess* mwidegt=new MRequestProcess();
        mwidegt->MRequestProcess_init(this->Msid,this->Maccount);
        mwidegt->show();
    });

    //发送消息功能
    connect(ui->pushButton_sendMessage_function,&QPushButton::clicked,this,[=](){
        MSendMessage* mwidegt=new MSendMessage(nullptr,this->Maccount,this->Msid);
        mwidegt->show();
    });

    //添加入库包裹
    connect(ui->pushButton_addInstore,&QPushButton::clicked,this,[=](){
        MInstoreProcess* mwidegt=new MInstoreProcess();
        mwidegt->parentWid=this;
        mwidegt->MInstoreProcess_Init();
        mwidegt->show();
    });

    //发出包裹功能
    connect(ui->pushButton_sendPakage,&QPushButton::clicked,this,[=](){
        MOutstoreProcess* mwidegt=new MOutstoreProcess();
        mwidegt->parentWid=this;
        mwidegt->MOutstoreProcess_Init();
        mwidegt->show();
    });

    //包裹按钮
    connect(ui->pushButton_12,&QPushButton::clicked,this,[=](){
        this->model->clear();
        this->model->setTable("pakageInfo");
        this->model->select();
        this->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        ui->tableView->setModel(this->model);
    });

    //订单按钮
    connect(ui->pushButton_13,&QPushButton::clicked,this,[=](){
        this->model->clear();
        this->model->setTable("orders");
        this->model->select();
        this->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        ui->tableView->setModel(this->model);
    });

    //入库按钮
    connect(ui->pushButton_11,&QPushButton::clicked,this,[=](){
        this->model->clear();
        this->model->setTable("instoreInfo");
        this->model->select();
        this->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        ui->tableView->setModel(this->model);
    });

    //出库按钮
    connect(ui->pushButton_14,&QPushButton::clicked,this,[=](){
        this->model->clear();
        this->model->setTable("outstoreInfo");
        this->model->select();
        this->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        ui->tableView->setModel(this->model);
    });

}

ManagerWindow::~ManagerWindow()
{
    delete ui;
}

void ManagerWindow::paintEvent(QPaintEvent* ev)
{
    QPainter p(this);

    QPixmap pix;
    pix.load(":/picRes/image/pakage.png");
    ui->pushButton_12->setIcon(pix);
    ui->pushButton_12->setIconSize(ui->pushButton_12->size());

    pix.load(":/picRes/image/orders.png");
    ui->pushButton_13->setIcon(pix);
    ui->pushButton_13->setIconSize(ui->pushButton_13->size());

    pix.load(":/picRes/image/instore.png");
    ui->pushButton_11->setIcon(pix);
    ui->pushButton_11->setIconSize(ui->pushButton_11->size());

    pix.load(":/picRes/image/outstore.png");
    ui->pushButton_14->setIcon(pix);
    ui->pushButton_14->setIconSize(ui->pushButton_14->size());

    pix.load(":/picRes/image/manager.png");
    ui->pushButton_manager->setIcon(pix);
    ui->pushButton_manager->setIconSize(ui->pushButton_manager->size());
}
