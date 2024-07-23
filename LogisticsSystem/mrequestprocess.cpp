#include "mrequestprocess.h"
#include "ui_mrequestprocess.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QDateTime>

MRequestProcess::MRequestProcess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MRequestProcess)
{
    ui->setupUi(this);

}

void MRequestProcess::MRequestProcess_init(QString sid,QString mid)
{
    this->sid=sid;
    this->mid=mid;
    //初始化一下requestVector
    QSqlDatabase db=QSqlDatabase::database("connection01");
    QSqlQuery query(db);

    QString sql=QString("select * from orders where O_state = %1 and O_s_id = '%2' order by O_time").arg(0).arg(this->sid);
    if(!query.exec(sql))
    {
        qDebug()<<"requestVector query fail";
        return;
    }

    //依次生成条目
    while(query.next())
    {
        QString oid=query.value("O_id").toString();
        QString uid=query.value("O_u_id").toString();
        RequestBox* box=new RequestBox(nullptr,oid,uid);
        this->requestVector.push_back(box);
        ui->verticalLayout->addWidget(box);
        box->sname=query.value("O_sendPerson").toString();
        box->sphone=query.value("O_sendPhone").toString();
        box->spos=query.value("O_sendPositon").toString();
        box->rname=query.value("O_recievePerson").toString();
        box->rphone=query.value("O_recievePhone").toString();
        box->rpos=query.value("O_recievePos").toString();
        box->weight=query.value("O_weight").toFloat();
        box->time=query.value("O_time").toString();

        //连接条目信息按钮的功能函数
        connect(&box->infoButt,&QPushButton::clicked,this,[=](){
            //弹出对话框
            QDialog* dio = new QDialog();
            box->requetDiolog=dio;
            //查询订单信息

            //设置布局
            QFormLayout* flayout=new QFormLayout(dio);

            QLabel* sidLabel=new QLabel("寄件人ID");
            QLineEdit* sidLine=new QLineEdit();
            sidLine->setText(box->uid);
            flayout->addRow(sidLabel,sidLine);
            QLabel* snameLabel=new QLabel("寄件人姓名");
            QLineEdit* snameLine=new QLineEdit(box->sname);
            flayout->addRow(snameLabel,snameLine);
            QLabel* sphoneLabel=new QLabel("寄件人电话");
            QLineEdit* sphoneLine=new QLineEdit(box->sphone);
            flayout->addRow(sphoneLabel,sphoneLine);
            QLabel* sposLabel=new QLabel("寄件人地址");
            QLineEdit* sposLine=new QLineEdit(box->spos);
            flayout->addRow(sposLabel,sposLine);
            QLabel* rnameLabel=new QLabel("收件人姓名");
            QLineEdit* rnameLine=new QLineEdit(box->rname);
            flayout->addRow(rnameLabel,rnameLine);
            QLabel* rphoneLabel=new QLabel("收件人电话");
            QLineEdit* rphoneLine=new QLineEdit(box->rphone);
            flayout->addRow(rphoneLabel,rphoneLine);
            QLabel* rposLabel=new QLabel("收件人地址");
            QLineEdit* rposLine=new QLineEdit(box->rpos);
            flayout->addRow(rposLabel,rposLine);
            QLabel* weightLabel=new QLabel("重量");
            QLineEdit* weightLine=new QLineEdit(QString::number(box->weight));
            flayout->addRow(weightLabel,weightLine);

            dio->setLayout(flayout);
            dio->show();
        });

        //处理支付订单的按钮
        connect(&box->payButt,&QPushButton::clicked,this,[=](){
            //显示数据
           box->paymentDialog=new PaymentShowForm();
           PaymentShowForm* psf=box->paymentDialog;
           psf->show();
           psf->line_Id->setText(box->uid);
           psf->line_name->setText(box->sname);
           psf->line_phone->setText(box->sphone);
//           psf->line_time->setText(box->time);
           psf->label_oid->setText(box->oid);
           //生成支付单号
           //查询当前时间
           QDateTime currtime=QDateTime::currentDateTime();
           QString timestr=currtime.toString("yyyy-MM-dd hh:mm:ss");
           QString datestr=currtime.toString("yyMMdd");

           //建立数据库连接
           QSqlDatabase db1=QSqlDatabase::database("connection01");
           QSqlQuery query1(db1);

           //处理一些数据
           QString sql=QString("select count(*) from payment where pay_id like 'p%1%'").arg(datestr);
           if(!query1.exec(sql))
           {
               qDebug()<<"generate payid num fail";
               return;
           }
           query1.next();
           int paynum=query1.value(0).toInt();
           QString payid=QString("p%1%2").arg(datestr).arg(paynum+10000);

           psf->line_payid->setText(payid);
           psf->line_time->setText(timestr);

           //提交按钮,生成支付单，不打算做，要求太多改不过来
           connect(psf->button_submit,&QPushButton::clicked,this,[=](){
            QMessageBox::information(this,"success","生成支付单成功");
            psf->close();
           });
        });

        //处理需求的功能函数(新版）
        connect(&box->processButt,&QPushButton::clicked,this,[=](){
            PakageShowForm* pf=new PakageShowForm();
            pf->PakageShowForm_Init(box->oid);
            box->pakageDialog=pf;
            pf->show();
            //若需求处理完毕
            connect(pf,&PakageShowForm::PakageIsCreate,this,[=](){
                ui->verticalLayout->removeWidget(box);
                box->close();
            });
        });

        //处理需求的功能函数
//        connect(&box->processButt,&QPushButton::clicked,this,[=](){
//            QDialog* dio = new processPage();
//            box->processDiolog=dynamic_cast<processPage*>(dio);
//            dio->show();
//            //查询当前时间
//            QDateTime currtime=QDateTime::currentDateTime();
//            QString timestr=currtime.toString("yyyy-MM-dd hh:mm:ss");
//            QString datestr=currtime.toString("yyMMdd");

//            //建立数据库连接
//            QSqlDatabase db1=QSqlDatabase::database("connection01");
//            QSqlQuery query1(db1);

//            //处理一些数据
//            QString sql=QString("select count(*) from payment where pay_id like 'p%1%'").arg(datestr);
//            if(!query1.exec(sql))
//            {
//                qDebug()<<"generate payid num fail";
//                return;
//            }
//            query1.next();
//            int paynum=query1.value(0).toInt();
//            box->processDiolog->payid=QString("p%1%2").arg(datestr).arg(paynum+10000);

//            sql=QString("select count(*) from pakage where P_id like 'P%1%'").arg(datestr);
//            if(!query1.exec(sql))
//            {
//                qDebug()<<"generate pakage num fail";
//                return ;
//            }
//            query1.next();
//            int pakagenum=query1.value(0).toInt();
//            box->processDiolog->pid=QString("P%1%2").arg(datestr).arg(10000+pakagenum);
//            box->processDiolog->price=box->weight*10;
//            box->processDiolog->oid=box->oid;
//            box->processDiolog->solveTime=timestr;

//            //显示数据
//            box->processDiolog->processPage_Init();

//            //处理按钮功能函数
//            connect(box->processDiolog->commitButton,&QPushButton::clicked,this,[=](){
//                box->processDiolog->company=box->processDiolog->pcomLine->currentText();
//                //建立数据库连接
//                QSqlDatabase db2=QSqlDatabase::database("connection01");
//                QSqlQuery query2(db2);

//                db2.transaction();
//                //生成支付单
//                QString sql=QString("insert into payment values('%1','%2','%3','%4')").arg(box->processDiolog->payid).arg(box->oid).arg(box->processDiolog->price).arg(box->processDiolog->solveTime);
//                if(!query2.exec(sql))
//                {
//                    qDebug()<<"insert payment fail";
//                    return ;
//                }
//                //生成快递单
//                sql=QString("insert into pakage values('%1','%2','%3','%4','%5')").arg(box->processDiolog->pid).arg(box->processDiolog->company).arg("待派发").arg(box->oid).arg(box->processDiolog->solveTime);
//                if(!query2.exec(sql))
//                {
//                    qDebug()<<"insert pakage fail";
//                    return ;
//                }
//                //修改订单信息
//                sql=QString("update orders set O_state = 1 where O_id = '%1'").arg(box->oid);
//                if(!query2.exec(sql))
//                {
//                    qDebug()<<"update order state fail";
//                    db2.rollback();
//                    return ;
//                }

//                db2.commit();
//                //成功提示
//                QString info="快递单号："+box->processDiolog->pid+"\n支付单号："+box->processDiolog->payid;
//                PakageForm* pf=new PakageForm(nullptr,box->processDiolog->pid);
//                pf->show();
//                QMessageBox::information(box->processDiolog,"生成成功",info);
//                ui->verticalLayout->removeWidget(box);
////              box->processDiolog->close();
//            });

//            //取消订单功能函数
//            connect(box->processDiolog->cancelButton,&QPushButton::clicked,this,[=](){
//                //建立数据库连接
//                QSqlDatabase db2=QSqlDatabase::database("connection01");
//                QSqlQuery query2(db2);
//                //准备数据插入
//                QString sql=QString("delete from orders where O_id = '%1'").arg(box->oid);
//                if(!query2.exec(sql))
//                {
//                    qDebug()<<"delete orders fail";
//                    return ;
//                }
//                QString info="需求单号："+box->processDiolog->oid;
//                QMessageBox::information(box->processDiolog,"取消成功",info);
//                ui->verticalLayout->removeWidget(box);
//                box->processDiolog->close();
//            });
//        });
    }
    ui->scrollAreaWidgetContents->setLayout(ui->verticalLayout);
}

MRequestProcess::~MRequestProcess()
{
    delete ui;
    for (auto i : qAsConst(this->requestVector) ) {
        delete i;
    }
}

            /*        *********************************************************************************      */
                                            //以下是requestbox类属性
RequestBox::RequestBox(QFrame* parent,QString oid,QString uid):QFrame(parent)
{
    this->oid=oid;
    this->uid=uid;
    //设置数据
    this->oidLabel.setText(oid);
    this->uidLabel.setText(uid);
    this->infoButt.setText("详情信息");
    this->processButt.setText("生成快递");
    this->payButt.setText("支付订单");

    this->hlay.addWidget(&this->oidLabel);
    this->hlay.addSpacing(10);
    this->hlay.addWidget(&this->uidLabel);
    this->hlay.addSpacing(10);
    this->hlay.addWidget(&this->infoButt);
    this->hlay.addSpacing(10);
    this->hlay.addWidget(&this->payButt);
    this->hlay.addSpacing(10);
    this->hlay.addWidget(&this->processButt);
    this->setLayout(&this->hlay);
}

RequestBox::RequestBox()
{
//    //先考虑下要不要查询
//    QSqlDatabase db=QSqlDatabase::database("connection01");
//    QSqlQuery query(db);

//    if(!query.exec(QString("select * from orders where O_id = '%1'").arg(oid)))
//    {
//        return;
//    }
//    query.next();


}

RequestBox::~RequestBox()
{
    delete this;
}

/*        *********************************************************************************      */
                                //以下是processPage类属性
processPage::processPage(QDialog *parent) :
    QDialog(parent)
{
    QFormLayout* flayout=new QFormLayout();

    //添加元素
    QLabel* oidLabel=new QLabel("处理订单号");
    this->oidLine=new QLineEdit("");
    flayout->addRow(oidLabel,oidLine);
    QLabel* pidLabel=new QLabel("生成快递单号");
    this->pidLine=new QLineEdit("");
    flayout->addRow(pidLabel,pidLine);
    QLabel* payidLabel=new QLabel("生成支付单号");
    this->payidLine=new QLineEdit("");
    flayout->addRow(payidLabel,payidLine);
    QLabel* pcomLabel=new QLabel("快递公司");
    this->pcomLine=new QComboBox();
    this->pcomLine->addItems(QStringList({ "中通","申通","圆通","韵达" ,"极兔" ,"京东","顺丰","中国邮政"}));
    flayout->addRow(pcomLabel,pcomLine);
    QLabel* priceLabel=new QLabel("支付价格");
    this->priceLine=new QLabel("");
    flayout->addRow(priceLabel,priceLine);
    QLabel* timeLabel=new QLabel("处理时间");
    this->timeLine=new QLabel("");
    flayout->addRow(timeLabel,timeLine);
    QPushButton* commitBut=new QPushButton(this);
    commitBut->setText("提交处理");
    QPushButton* cancelBut=new QPushButton(this);
    cancelBut->setText("取消订单");
    commitBut->move(30,180);
    cancelBut->move(commitBut->width()+50,180);
    this->commitButton=commitBut;
    this->cancelButton=cancelBut;

    this->setLayout(flayout);
    this->resize(this->height(),this->commitButton->y()+50);
}

void processPage::processPage_Init()
{
    this->oidLine->setText(this->oid);
    this->payidLine->setText(this->payid);
    this->pidLine->setText(this->pid);
    this->priceLine->setText(QString::number(this->price));
    this->timeLine->setText(this->solveTime);
}

processPage::~processPage()
{

}
