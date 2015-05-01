#ifndef EDITSTYLE_H
#define EDITSTYLE_H

#include <QtWidgets>
#include "ui_editstyle.h"

class EditStyle : public QDialog
{
    Q_OBJECT
public:
    EditStyle();

private:
    Ui::EditStyleDialog ui;

private slots:
    void apply();
};

#endif // EDITSTYLE_H
