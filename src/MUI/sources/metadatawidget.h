#ifndef METADATAWIDGET_H
#define METADATAWIDGET_H

#include <QWidget>
#include <QPixmap>
#include "ui_metadata.h"
#include "caudiotags.hpp"

class MetaDataWidget : public QWidget
{
    Q_OBJECT
    public:
        MetaDataWidget(QWidget *parent = 0);
        void setTag(AudioTag::GenericTag);
        
    private:
        Ui::Metadata ui;
};

#endif
