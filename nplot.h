// nplot header file

#ifndef NPLOT_H
#define NPLOT_H


#include <stdio.h>
#include <stdlib.h>
#include <qmainwindow.h>
#include <qpainter.h>
#include <qpushbt.h>
#include <qbttngrp.h>
#include <qapp.h>
#include <math.h>
#include <qpixmap.h>

#include <qmsgbox.h>
#include <qlabel.h>

#include "menu.h"
#include "butgr.h"
#include "canvas.h"

//******************************************************************************
// Drawing subroutines: nplot Class 
//******************************************************************************


class Nplot : public QMainWindow
{
    Q_OBJECT
public:
    Nplot();
    void init(char *inputfilename1, bool mainwidget1, long is1, long ie1,
                 long js1, long je1,
                 long ks1, long ke1, long numprop, 
                 proptitlename *proptitle, double *data1);
    bool mainwidget;
    QLabel          *status;
     
protected:
    void   resizeEvent( QResizeEvent * );
private:
    typecanvas      cv;
    Transfo         transfo;         
    Menu            *menu;
    ButGr           *butgr;
    Canvas          *canvas;
};

#endif // NPLOT_H
