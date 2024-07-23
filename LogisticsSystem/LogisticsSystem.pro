QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    formlist.cpp \
    main.cpp \
    dialog.cpp \
    malterpackage.cpp \
    managerwindow.cpp \
    minstorgerecord.cpp \
    morderquery.cpp \
    moutstorgerecord.cpp \
    mquerypackage.cpp \
    mrequestprocess.cpp \
    msendmessage.cpp \
    mstoreprocess.cpp \
    ucreateorder.cpp \
    unotice.cpp \
    upakage.cpp \
    usermainwindow.cpp

HEADERS += \
    dialog.h \
    formlist.h \
    malterpackage.h \
    managerwindow.h \
    minstorgerecord.h \
    morderquery.h \
    moutstorgerecord.h \
    mquerypackage.h \
    mrequestprocess.h \
    msendmessage.h \
    mstoreprocess.h \
    ucreateorder.h \
    unotice.h \
    upakage.h \
    usermainwindow.h

FORMS += \
    MInstoreProcess.ui \
    MOutstoreProcess.ui \
    MSelectPakage.ui \
    MSelectPakageForSend.ui \
    OrderForm.ui \
    OrderShowForm.ui \
    PakageForm.ui \
    PakageShowForm.ui \
    PaymentForm.ui \
    PaymentShowForm.ui \
    UPakageNotice.ui \
    UPakageQuery.ui \
    UPaymentNotice.ui \
    dialog.ui \
    malterpackage.ui \
    managerwindow.ui \
    minstorgerecord.ui \
    morderquery.ui \
    moutstorgerecord.ui \
    mquerypackage.ui \
    mrequestprocess.ui \
    msendmessage.ui \
    ucreateorder.ui \
    uqueryorder.ui \
    usermainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    picRes.qrc
