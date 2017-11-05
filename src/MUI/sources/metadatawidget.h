#ifndef METADATAWIDGET_H
#define METADATAWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QtWidgets>
#include <iostream>
#include <cstddef>

#include "ui_metadata.h"
#include "metadata.h"

#include "globals.h"

class MetaDataWidget : public QWidget
{
    Q_OBJECT
    public:
        MetaDataWidget(QWidget *parent = 0);
        void setTag(const Meta::AudioTag &tag);
        void reset();

    private:
        Ui::Metadata ui;
        QImage m_qImage;
};

#endif
