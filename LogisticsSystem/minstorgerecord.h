#ifndef MINSTORGERECORD_H
#define MINSTORGERECORD_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class MInstorgeRecord;
}

class MInstorgeRecord : public QWidget
{
    Q_OBJECT

public:
    explicit MInstorgeRecord(QWidget *parent = nullptr);
    ~MInstorgeRecord();
    void MInstore_Init(QSqlTableModel* m);

    QSqlTableModel* model;

private:
    Ui::MInstorgeRecord *ui;
};

#endif // MINSTORGERECORD_H
