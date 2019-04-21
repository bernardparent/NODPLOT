//
// Menu Class Header File
//




#ifndef MENU_H
#define MENU_H

#include <qwidget.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <qmlined.h>
#include <qlined.h>
#include <qstring.h>
#include <qmainwindow.h>
#include "transfo.h"
#include "canvas.h"





class PropMenuLine: public QWidget {
  Q_OBJECT 

friend class Nplot;
private:
   Nplot *nplot;
   long thisline;
   Canvas *canvas;
   typecanvas *cv;
   QPopupMenu *prop;
   long *propmenulineflag;

public :
   PropMenuLine(long thisline1, Canvas *canvas2, typecanvas *cv2, Nplot *nplot2,
                    QPopupMenu *prop1, long *propmenulineflag1){
     thisline=thisline1;
     nplot=nplot2;
     canvas=canvas2;
     cv=cv2;
     prop=prop1;
     propmenulineflag=propmenulineflag1;
   }

public slots:
   void change_numprop();
};




class GetData : public QMainWindow {
    Q_OBJECT
public:
    GetData(char *messtr, double *inputnumber1, Canvas *canvas1);

private:
    QPushButton     *apply,*dismiss;
    QLabel          *message;
    QLineEdit       *le;
    long            err;
    double          *inputnumber;
    double          inputnumber_tmp;
    Canvas          *canvas;
private slots:
    void slotDismiss();
    void slotApply();
    void slotTextChanged(const QString & newText);
};










class Menu : public QWidget
{
    Q_OBJECT
friend class Nplot;
public:
    Menu( QWidget *parent=0, const char *name=0 );
    void init(Nplot *nplot2);
    void linkcanvas(typecanvas *cv2,Canvas *canvas2);
private slots:
    void newwindow();
    void closewindow();
    void aboutNPlot();
    void aboutinstanthelp();
    void aboutQt();
    void instanthelp();
    void centerxycanvas();
    void resetzoomfactor();
    void setdatamin();
    void setdatamax();
    void scandatamin();
    void scandatamax();
    void rainbow();
    void steelblue();
    void gray();
    void exit();
    void propspacing();
    void nonpropspacing();
    void incscalelevels();
    void decscalelevels();
private:
    Nplot *nplot;
    QMenuBar *menu;
    typecanvas *cv;
    Transfo  transfo;
    int rainbowID,steelblueID,grayID,propID,nonpropID;
    QPopupMenu *scaletype;
    QPopupMenu *nodespacing;
    Canvas          *canvas;
    long propmenu;
    PropMenuLine **propmenuline;
    long *propmenulineflag;
};


#endif // MENU_H



