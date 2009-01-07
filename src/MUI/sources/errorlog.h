#ifndef ERRORLOG_H
#define ERRORLOG_H

#include <QListWidget>
#include <string>

#include "ui_errorlog.h"

class ErrorLog : public QDialog
{
    Q_OBJECT
    public:
        ErrorLog();
        void append(QString);
        void append(const char *);
        void append(std::string);
        
    private slots:
        void quitErrorDialog();
        
    private:
        Ui::ErrorDialog ui;
};

#endif
