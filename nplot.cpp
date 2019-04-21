// nplot program -> takes care of showing the menu, canvas, status
// and button groups widgets

#include "nplot.h"


//
// Construct the main window with buttons.
//



Nplot::Nplot()
{
#include "fonts.h"
      QFontMetrics    fontm5(font5);
      setMinimumSize( fontm5.height()*20, fontm5.height()*17 );
}


//
// Called when the widget has been resized.
// Moves the button group to the upper right corner
// of the widget.

void Nplot::resizeEvent( QResizeEvent * )
{


    menu->move(0,0);
    menu->resize(width(),menu->height());

    butgr->move( width()-butgr->width(), 
                 height()-status->height()-butgr->height() );
    butgr->resize( butgr->width(), butgr->height());

    canvas->resize( width()-butgr->width(),
                    height()-menu->height()-status->height()-1);
    canvas->move(0,menu->height()+1);

    status->setGeometry(0, height() - status->height(),
                    width(), status->height());


}




//
// This function initializes
//

void Nplot::init(char *inputfilename1,bool mainwidget1, long is1, long ie1,
                       long js1, long je1,
                       long ks1, long ke1 ,
                       long numprop, proptitlename *proptitle,
                       double *data1)
{
   mainwidget=mainwidget1;

   long i,j,k;
#include "fonts.h"
   QFontMetrics    fontm3(font3);
   
   cv.is=is1; cv.iso=cv.is;
   cv.ie=ie1; cv.ieo=cv.ie;
   cv.js=js1; cv.jso=cv.js;
   cv.je=je1; cv.jeo=cv.je;
   cv.ks=ks1; cv.kso=cv.ks;
   cv.ke=ke1; cv.keo=cv.ke;
   strcpy(cv.upaxis,"kij");
   cv.data=data1;


   cv.numprop=numprop;
   cv.proptitle=proptitle;

   cv.scandatamin= (double *) malloc( (cv.numprop+1)*sizeof(double));
   cv.scandatamax= (double *) malloc( (cv.numprop+1)*sizeof(double));
   for (cv.prop=0; cv.prop<cv.numprop; cv.prop++) {
     cv.scandatamin[cv.prop]=1.0e99;
     cv.scandatamax[cv.prop]=-1.0e99;
   }
   for (cv.prop=0; cv.prop<cv.numprop; cv.prop++) {
     for (i=cv.is; i<=cv.ie; i++) {
       for (j=cv.js; j<=cv.je; j++) {
         for (k=cv.ks; k<=cv.ke; k++) {
           if (cv.data[transfo.ai(&cv,i,j,k)] < cv.scandatamin[cv.prop]) 
                 cv.scandatamin[cv.prop]=cv.data[transfo.ai(&cv,i,j,k)];
           if (cv.data[transfo.ai(&cv,i,j,k)] > cv.scandatamax[cv.prop]) 
                 cv.scandatamax[cv.prop]=cv.data[transfo.ai(&cv,i,j,k)];  
         }
       }
     }
   }
   cv.prop=0;
   cv.datamin=cv.scandatamin[cv.prop];
   cv.datamax=cv.scandatamax[cv.prop];

   cv.decalx=0;
   cv.decaly=0;
   cv.kk=(cv.ke+cv.ks)/2;
   cv.resfact=6400.0*100.0;
   cv.scalelevels=32;
   cv.colortype=1;
   cv.zoomfactx=1.0;
   cv.zoomfacty=1.0;
   cv.zoomfactref=1.0;
   cv.proportional=TRUE;
   cv.shownode=0;
   cv.instanthelp=FALSE;
   cv.inputfilename=inputfilename1;

    status = new QLabel(this);
    status->setFrameStyle( QFrame::WinPanel | QFrame::Sunken );
    status->setFont(font3);
    status->setFixedHeight( fontm3.height() + 4 );
    status->setCursor(arrowCursor);

    canvas=new Canvas(this);
    canvas->linkcanvas(&cv,status);
    canvas->init();

    butgr=new ButGr(this);
    butgr->linkcanvas(&cv,canvas);

    menu = new Menu(this);
    menu->move( 0, 0 );
    menu->linkcanvas(&cv,canvas);
    menu->init(this);    

    char newcaptionname[256],captionname[256],captionname2[256];
    if (mainwidget) {
       strcpy(captionname,"nodplot");
    }
    else {
       strcpy(captionname,"nodplot");
    }
    sprintf(captionname2, "%s : %s", captionname, cv.inputfilename);
    sprintf(newcaptionname, "%s : %s", captionname2, proptitle[0]);
    
    setCaption( newcaptionname );

//    QColor c;
//    c.setHsv( 36, 36, 177 );
//    setBackgroundColor( c );



}

