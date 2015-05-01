#ifndef ABOUT_H
#define ABOUT_H

#include <QtWidgets>
#include "ui_about.h"
#include "cfmod.hpp"
#include "cplaylist.hpp"
#include "caudiotags.hpp"
#include "globals.h"

class About : public QDialog
{
    Q_OBJECT
    public:
        About(QWidget *parent = 0);

    private:
        Ui::AboutDialog ui;
        QStandardItemModel model;
};
#endif
