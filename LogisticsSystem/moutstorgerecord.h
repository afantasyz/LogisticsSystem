#ifndef MOUTSTORGERECORD_H
#define MOUTSTORGERECORD_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class MOutstorgeRecord;
}

class MOutstorgeRecord : public QWidget
{
    Q_OBJECT

public:
    explicit MOutstorgeRecord(QWidget *parent = nullptr);
    ~MOutstorgeRecord();
    void Moutstore_Init(QSqlTableModel* m);

    QSqlTableModel* model;

private:
    Ui::MOutstorgeRecord *ui;
};

#endif // MOUTSTORGERECORD_H
