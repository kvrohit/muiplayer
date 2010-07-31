#include "editstyle.h"

EditStyle::EditStyle()
{
    ui.setupUi(this);

    connect(ui.buttonApply, SIGNAL(clicked()), this, SLOT(apply()));
}

void EditStyle::apply()
{
    qApp->setStyleSheet(ui.textEdit->toPlainText());
}
