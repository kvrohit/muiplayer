#ifndef METADATAWIDGET_H
#define METADATAWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QtGui>
#include <iostream>
#include <cstddef>
#include "ui_metadata.h"
#include "caudiotags.hpp"

#include "support.h"

class MetaDataWidget : public QWidget
{
    Q_OBJECT
    public:
        MetaDataWidget(QWidget *parent = 0);
        void setTag(AudioTag::GenericTag, std::string);
        
    private:
        Ui::Metadata ui;
};

#endif
