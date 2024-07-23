#ifndef MQUERYPACKAGE_H
#define MQUERYPACKAGE_H

#include <QWidget>
#include<QSqlTableModel>

namespace Ui {
class MQueryPackage;
}

class MQueryPackage : public QWidget
{
    Q_OBJECT

public:
    explicit MQueryPackage(QWidget *parent = nullptr,QSqlTableModel* m=nullptr);
    ~MQueryPackage();

    QSqlTableModel* model;

private:
    Ui::MQueryPackage *ui;
};

#endif // MQUERYPACKAGE_H
