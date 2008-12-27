#ifndef ABOUT_H
#define ABOUT_H

#include <QtGui>
#include "ui_about.h"
#include "cfmod.hpp"
#include "cplaylist.hpp"
#include "caudiotags.hpp"

#define ROWHEIGHT 21

class About : public QWidget
{
    Q_OBJECT
    public:
        About();		
        
    private:
        Ui::AboutDialog ui;		
		QStandardItemModel model;
};
#endif
