#include "volumeslider.h"

VolumeSlider::VolumeSlider(Qt::Orientation orientation, QWidget * parent = 0) : QSlider(orientation, parent)
{
    setMinimum(0);
    setMaximum(100);
    setValue(100);
}

void VolumeSlider::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    
    if(numSteps > 0) setValue(value() + 4);
    else setValue(value() - 4);
    
    emit sliderMoved(value());
    event->accept();
}
