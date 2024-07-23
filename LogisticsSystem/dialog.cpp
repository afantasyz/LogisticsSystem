#include "dialog.h"
#include "ui_dialog.h"

#include "managerwindow.h"
#include "usermainwindow.h"
#include<QSqlDatabase>
#include<QMessageBox>
#include<QString>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QPainter>
#include<QPixmap>
#include <QDebug>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //连接数据库
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL","connection01");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("12345");
    db.setPort(3306);
    db.setDatabaseName("logistics");
    if(!db.open())
    {
        QMessageBox::warning(this,"warning","连接失败");
        return;
    }

    //登录按钮事件
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        QSqlDatabase db=QSqlDatabase::database("connection01");
        //测试数据

        //获取用户输入
        int str=ui->comboBox->currentIndex();
        QString account=ui->lineEdit->text();
        QString password=ui->lineEdit_2->text();

        //检查用户名和密码
        QSqlQuery query(db);

        //弹出界面
        //管理员
        if(str==0)
        {
            account="30000000";
            password="12345";
            //检查账号和密码是否合法
            if(!query.exec(QString("select * from manager where m_id = '%1'").arg(account)))
            {
                QMessageBox::warning(this,"sql","数据库查询失败");
                return;
            }
            if(query.size()==0)
            {
                QMessageBox::warning(this,"error","用户不存在");
                return;
            }
            query.next();
            if(query.value("m_password")!=password)
            {
                QMessageBox::warning(this,"error","用户密码错误");
                return;
            }
            ManagerWindow* mWindow=new ManagerWindow();
            mWindow->Maccount=account;
            mWindow->Msid=query.value("m_s_id").toString();
            mWindow->Mname=query.value("m_name").toString();
            mWindow->Mphone=query.value("m_phone").toString();
            mWindow->show();
            //补充下自身信息
            //设置默认值
            if(!query.exec(QString("select * from station where s_id = '%1'").arg(mWindow->Msid)))
            {
                qDebug()<<"query station fail";
                return;
            }
            query.next();
            mWindow->Msname=query.value("s_name").toString();
            mWindow->Mspos=query.value("s_pos").toString();
        }
        //用户
        if(str==1)
        {
            //给个测试用例
            account="10000024";
            password="12345";
            //检查账号和密码是否合法
            if(!query.exec(QString("select * from user where u_id = '%1'").arg(account)))
            {
                QMessageBox::warning(this,"sql","数据库查询失败");
                return;
            }
            if(query.size()==0)
            {
                QMessageBox::warning(this,"error","用户不存在");
                return;
            }
            query.next();
            if(query.value("u_password")!=password)
            {
                QMessageBox::warning(this,"error","用户密码错误");
                return;
            }
            UserMainwindow* mWindow=new UserMainwindow();
            mWindow->show();
            mWindow->Uaccount=account;
            mWindow->Uname=query.value("u_name").toString();
            mWindow->Uphone=query.value("u_phone").toString();
        }
    });
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *event)
{
    QPixmap pic;
    pic.load(":/picRes/image/loginBack.png");
    QPainter p(this);
//    p.drawPixmap(0,0,this->width(),this->height(),pic);

}
