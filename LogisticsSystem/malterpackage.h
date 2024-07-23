#ifndef MALTERPACKAGE_H
#define MALTERPACKAGE_H

#include <QWidget>

namespace Ui {
class MAlterPackage;
}

class MAlterPackage : public QWidget
{
    Q_OBJECT

public:
    explicit MAlterPackage(QWidget *parent = nullptr);
    ~MAlterPackage();

    QString pid;
    QString oid;
    QString payid;

private:
    Ui::MAlterPackage *ui;
};

#endif // MALTERPACKAGE_H
