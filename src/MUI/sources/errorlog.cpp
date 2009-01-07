#include "errorlog.h"

ErrorLog::ErrorLog()
{
    ui.setupUi(this);
    
    connect(ui.buttonOk, SIGNAL(clicked()),
        this, SLOT(quitErrorDialog()));
}

void ErrorLog::quitErrorDialog()
{
    close();
}

void ErrorLog::append(QString s)
{
    ui.listWidget->addItem(s);
}

void ErrorLog::append(const char *s)
{
    ui.listWidget->addItem(QString::fromStdString(s));
}

void ErrorLog::append(std::string s)
{
    ui.listWidget->addItem(QString::fromStdString(s));
}
