#ifndef RPMGAUGE_H
#define RPMGAUGE_H

#include <QGraphicsPixmapItem>

class RPMGauge
{

public:

    RPMGauge();

    QGraphicsPixmapItem *Pointer;  //Classe QGraphicsPixmapItem utilizada para processar imagens.
    QGraphicsPixmapItem *Gauge;


};

#endif // RPMGAUGE_H
