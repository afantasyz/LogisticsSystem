#ifndef MORDERQUERY_H
#define MORDERQUERY_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class MOrderQuery;
}

class MOrderQuery : public QWidget
{
    Q_OBJECT

public:
    explicit MOrderQuery(QWidget *parent = nullptr);
    ~MOrderQuery();
    void MorderQuery_Init(QSqlTableModel* m);

    QSqlTableModel* model;

private:
    Ui::MOrderQuery *ui;
};




#endif // MORDERQUERY_H
