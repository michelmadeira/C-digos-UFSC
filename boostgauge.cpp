#include "boostgauge.h"

//Comentários disponíveis na classe RPMGauge (implementação similiar).

boostGauge::boostGauge()
{

    QPixmap gauge(":/resources/assets/BOOST_GAUGE.png");
    Gauge = new QGraphicsPixmapItem(gauge);
    Gauge->setTransformationMode(Qt::TransformationMode::SmoothTransformation);
    Gauge->setTransformOriginPoint(gauge.width()/2,gauge.height()/2);
    Gauge->setPos(-245,-60);

    QPixmap boostPointerpixmap(":/resources/assets/BOOST_POINTER.png");
    Pointer = new QGraphicsPixmapItem(boostPointerpixmap);
    Pointer->setTransformationMode(Qt::TransformationMode::SmoothTransformation);
    Pointer->setTransformOriginPoint(32,22);
    Pointer->setPos(-191,5);

}
