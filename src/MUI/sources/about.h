#ifndef ABOUT_H
#define ABOUT_H

#include "ui_about.h"
#include "cfmod.hpp"
#include "cplaylist.hpp"

class About : public QWidget
{
    Q_OBJECT
    public:
        About();
        
    private:
        Ui::AboutDialog ui;
};
#endif
