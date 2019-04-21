// Canvas Class Header File

#ifndef CANVAS_H
#define CANVAS_H


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

#include "transfo.h"
//#include "qwidget.h"

//******************************************************************************
// Drawing subroutines: Canvas Class 
//******************************************************************************

//extern Canvas canv;                // global application object

class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas( QWidget *parent=0, const char *name=0 );
    long      testme;
    void   drawxy();
    void linkcanvas(typecanvas *cv2,QLabel *status2);
    void init();
private slots:
    void   butfw();
    void   butbw();
protected:
    void   resizeEvent( QResizeEvent * );
    void   paintEvent( QPaintEvent * );
    void   mouseMoveEvent( QMouseEvent *e );
    void   mousePressEvent( QMouseEvent *e );
    void   mouseReleaseEvent(QMouseEvent *);
private:
    typecanvas      *cv;
    Transfo         transfo;         
    QLabel          *status;
    QPushButton	  *fw,*bw;
    QPushButton	  *sl_plus,*sl_minus;
    long		  drawindex,maxindex;
    void            find_color(double val,QColor *c);
    double          find_data(double pi, double pj, long pk);
    void            draw_color_legend(QPainter *p);
    void            draw_xyaxis_values(QPainter *p, double pixelsx,
                           double pixelsy, double fictresi, double fictresj);
    void            draw_nodes(QPainter *p, double pixelsx,
                           double pixelsy, double fictresi, double fictresj);
    void            draw_axis_legend(QPainter *p);
    void            draw_rectangles(QPainter *p, double fictresi,
                                double fictresj, double pixelsx, double pixelsy);
    bool            mousebuttonpressed;
    void            drawz(double i, double j, bool active);
    long            maxnodelength;  

};

#endif // CANVAS_H
