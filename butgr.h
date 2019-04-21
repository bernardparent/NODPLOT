//
// Button Group Header File
//

#ifndef BUTGR_H
#define BUTGR_H

#include <qmainwindow.h>
#include <qpainter.h>
#include <qpushbt.h>
#include <qbttngrp.h>
#include <qapp.h>
#include "transfo.h"
#include "canvas.h"

class ButGr : public QWidget
{
    Q_OBJECT
public:
    ButGr( QWidget *parent=0, const char *name=0 );
    void linkcanvas(typecanvas *cv2, Canvas *canvas2);

public slots:
    void   butresplus();
    void   butresminus();
    void   buti();
    void   butj();
    void   butk();
    void   butzoomplus();
    void   butzoomminus();
    void   butsnplus();
    void   butsnminus();


private:
    Transfo         transfo;
    typecanvas      *cv;
    Canvas          *canvas;
    QPushButton	  *res_plus,*res_minus;
    QPushButton	  *sn_plus,*sn_minus;
    QPushButton	  *gc_gray,*gc_color;
    QPushButton	  *zoom_plus,*zoom_minus;
    QPushButton	  *ijk_i,*ijk_j,*ijk_k;

};


#endif // BUTGR_H
