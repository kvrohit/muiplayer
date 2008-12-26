#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QSlider>
#include <QDebug>
#include <QWheelEvent>

class VolumeSlider : public QSlider
{
    Q_OBJECT
    public:
        VolumeSlider(Qt::Orientation, QWidget *);
    
    private:
        void wheelEvent(QWheelEvent *);
};

#endif
