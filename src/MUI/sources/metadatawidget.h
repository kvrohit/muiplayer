#ifndef METADATAWIDGET_H
#define METADATAWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QtWidgets>
#include <iostream>
#include <cstddef>

#include "ui_metadata.h"
#include "caudiotags.hpp"

#include "globals.h"

class MetaDataWidget : public QWidget
{
    Q_OBJECT
    public:
        MetaDataWidget(QWidget *parent = 0);
        void setTag(const AudioTag::GenericTag &tag, const std::string &path);
        void reset();

    private:
        Ui::Metadata ui;
        QImage m_qImage;
};

#endif
