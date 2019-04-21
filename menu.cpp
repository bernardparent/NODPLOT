//
//  Menu Class File
//

#include "menu.h"
#include "canvas.h"
#include <qpopmenu.h>
#include <qkeycode.h>
#include <qapp.h>
#include <qmsgbox.h>
#include <stdarg.h>
#include "nplot.h"


#define max(a,b)   ((a) > (b) ? (a) : (b))
#define min(a,b)   ((a) < (b) ? (a) : (b))

Menu::Menu( QWidget *parent, const char *name )
    : QWidget( parent, name )
{

}

void Menu::init(Nplot *nplot2)
{
#include "fonts.h"
    nplot=nplot2;
    QPopupMenu *file = new QPopupMenu();
    CHECK_PTR( file );
    file->insertItem( "New Window",  this, SLOT(newwindow()), CTRL+Key_N);
    file->insertItem( "Close Window",  this, SLOT(closewindow()), CTRL+Key_W);
    file->insertItem( "E&xit",  this, SLOT(exit()), CTRL+Key_Q );



    scaletype = new QPopupMenu();
    CHECK_PTR (scaletype);
    rainbowID=scaletype->insertItem( "Rainbow", this, SLOT(rainbow()), CTRL+Key_R );
    grayID=scaletype->insertItem( "Gray", this, SLOT(gray()), CTRL+Key_G);
    steelblueID=scaletype->insertItem( "SteelBlue", this, SLOT(steelblue()), CTRL+Key_B);
    scaletype->setCheckable( TRUE );
    if (cv->colortype==1) 
           scaletype->setItemChecked( rainbowID, TRUE );
    if (cv->colortype==2) 
           scaletype->setItemChecked( grayID, TRUE );
    if (cv->colortype==3) 
           scaletype->setItemChecked( steelblueID, TRUE );


    QPopupMenu *scalelevels = new QPopupMenu();
    CHECK_PTR (scalelevels);
    scalelevels->insertItem( "Increase", this, SLOT(incscalelevels()), CTRL+Key_I );
    scalelevels->insertItem( "Decrease", this, SLOT(decscalelevels()), CTRL+Key_D );

    QPopupMenu *scale = new QPopupMenu();
    CHECK_PTR( scale );
    scale->insertItem( "Set Min Scale Value", this, SLOT(setdatamin()) );
    scale->insertItem( "Set Max Scale Value", this, SLOT(setdatamax()) );
    scale->insertSeparator();
    scale->insertItem( "Scan Min Scale Value", this, SLOT(scandatamin()) );
    scale->insertItem( "Scan Max Scale Value", this, SLOT(scandatamax()) );
    scale->insertSeparator();
    scale->insertItem( "Scale &Type", scaletype);
    scale->insertItem( "Change Scale &Levels", scalelevels);


    QPopupMenu *prop = new QPopupMenu();
    CHECK_PTR( prop );
    propmenulineflag= (long *) malloc (cv->numprop*sizeof(long));
    propmenuline = (PropMenuLine **) malloc (cv->numprop*sizeof(PropMenuLine));
    for (long cnt=1; cnt<=cv->numprop; cnt++) {
      propmenuline[cnt-1]=new PropMenuLine(cnt-1,canvas,cv,nplot,prop,
                                           propmenulineflag);
      propmenulineflag[cnt-1]=prop->insertItem( cv->proptitle[cnt-1], 
                            propmenuline[cnt-1], SLOT(change_numprop()) );
    }
    prop->setCheckable(TRUE);
    prop->setItemChecked(propmenulineflag[cv->prop], TRUE);


    nodespacing = new QPopupMenu();
    CHECK_PTR (nodespacing);
    propID=nodespacing->insertItem( "Proportional", this, SLOT(propspacing()), CTRL+Key_P );
    nonpropID=nodespacing->insertItem( "Non Proportional", this, SLOT(nonpropspacing()), CTRL+Key_O);
    nodespacing->setCheckable( TRUE );
    if (cv->proportional) 
           nodespacing->setItemChecked( propID, TRUE );
    else   nodespacing->setItemChecked( nonpropID, TRUE );



    QPopupMenu *view = new QPopupMenu();
    CHECK_PTR( view );
    view->insertItem( "&Center xy-Canvas", this, SLOT(centerxycanvas()), CTRL+Key_C);
    view->insertItem( "Reset &Zoom Factor", this, SLOT(resetzoomfactor()), CTRL+Key_Z);
    view->insertSeparator();
    view->insertItem( "&Node Spacing", nodespacing);

    
    QPopupMenu *help = new QPopupMenu;
    CHECK_PTR( help );
    help->insertItem( "&Instant Help", this, SLOT(instanthelp()), CTRL+Key_H );
    help->insertItem( "&About Instant Help", this, SLOT(aboutinstanthelp())  );
    help->insertItem( "About &NodPlot", this, SLOT(aboutNPlot()) );
    help->insertItem( "About &Qt", this, SLOT(aboutQt()) );

    menu = new QMenuBar( this );
    CHECK_PTR( menu );
    menu->insertItem( "&File", file );
    menu->insertItem( "&Scale", scale );
    menu->insertItem( "&Prop",prop);
    menu->insertItem( "&View",view);
    menu->insertSeparator();
    menu->insertItem( "&Help", help );
    menu->setSeparator( QMenuBar::InWindowsStyle );

    
    resize(width(),menu->height());
}


void Menu::newwindow()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on New Window",
      "New Window creates a new window with the same data structure as the \n"
      "main one. The data structure excepted, the new window is not linked to\n"
      "the previous one, hence enabling the simultaneous visualization of two\n"
      "or more properties in different areas of the ijk space.\n"
      "\n"
      "Note that the new window is initialized with default values of the zoom\n"
      "factor, scale type, resolution factor, etc.\n");
      cv->instanthelp=FALSE;
    }
    else {
      Nplot *nplot2=new Nplot;
      nplot2->init(cv->inputfilename,FALSE,cv->iso,cv->ieo,cv->jso,cv->jeo,cv->kso,cv->keo,cv->numprop,
                  cv->proptitle,cv->data);
      nplot2->show();
    }
}


void Menu::closewindow()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Close Window",
      "Close Window deletes the present window but does not exit, unless \n"
      "the present window is the main widget, meaning it was the first \n"
      "one to appear when nodplot was lauched\n");
      cv->instanthelp=FALSE;
    }
    else {
      if (nplot->mainwidget) {
        exit();
      }
      else nplot->hide();
    }
}


void Menu::exit()
{ 
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Quit",
      "Quit deletes all windows and exits.\n");
      cv->instanthelp=FALSE;
    }
    else {
      free(propmenuline);
      free(propmenulineflag);
      free(cv->proptitle);
      free(cv->data);
      free(cv->scandatamin);
      free(cv->scandatamax);
      qApp->quit();
    }
}



void Menu::rainbow()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Rainbow",
      "Rainbow enables a scale type made of the rainbow colors, i.e. blue\n"
      "for low values, red for high values and shades of green and yellow\n"
      "in between. By default the rainbow scale type is enabled.\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->colortype=1;
      scaletype->setItemChecked( rainbowID, TRUE );
      scaletype->setItemChecked( grayID, FALSE );
      scaletype->setItemChecked( steelblueID, FALSE );
      canvas->drawxy();
    }       
}



void Menu::steelblue()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on SteelBlue",
      "SteelBlue enables a scale type made primarily of blueishcolors, \n"
      "i.e. lightblue for low values and darkblue for high values.\n"
      "By default the rainbow scale type is enabled.\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->colortype=2;
      scaletype->setItemChecked( rainbowID, FALSE );
      scaletype->setItemChecked( grayID, FALSE );
      scaletype->setItemChecked( steelblueID, TRUE );
      canvas->drawxy();
    }       
}



void Menu::gray()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Gray",
      "Gray enables a grayshading-like scale, i.e. white for low values\n"
      "and black for high values. By default, the rainbow scale type is\n"
      "enabled.\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->colortype=3;
      scaletype->setItemChecked( rainbowID, FALSE );
      scaletype->setItemChecked( grayID, TRUE );
      scaletype->setItemChecked( steelblueID, FALSE );
      canvas->drawxy();
    }     
}


void Menu::setdatamin()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Set Min Scale Value",
      "Set Min Scale Value permits the user to input the desired minimum\n"
      "value for the scale. Clicking on apply will activate the input,\n"
      "while a click on dismiss will hide the input window.");
      cv->instanthelp=FALSE;
    }
    else {
      GetData *getdata;
      getdata=new GetData(" Input Minimum Scale Value: ",&(cv->datamin),canvas);     
      getdata->show();
    }
}

void Menu::scandatamin()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Scan Min Scale Value",
      "Scan Min Scale Value activates a scan of the entire mesh finding\n"
      "the minimum value for the present property and fixing the minimum\n"
      "scale value to it..\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->datamin=cv->scandatamin[cv->prop];
      canvas->drawxy();
    }
}

void Menu::scandatamax()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Scan Max Scale Value",
      "Scan Max Scale Value activates a scan of the entire mesh finding\n"
      "the maximum value for the present property and fixing the maximum\n"
      "scale value to it.\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->datamax=cv->scandatamax[cv->prop];
      canvas->drawxy();
    }
}




void Menu::setdatamax()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Set Max Scale Value",
      "Set Max Scale Value permits the user to input the desired maximum\n"
      "value for the scale. Clicking on apply will activate the input,\n"
      "while a click on dismiss will hide the input window.");
      cv->instanthelp=FALSE;
    }
    else {
      GetData *getdata;
      getdata=new GetData(" Input Maximum Scale Value: ",&(cv->datamax),canvas);     
      getdata->show();
    }
}

void Menu::incscalelevels()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Increase Scale Levels",
      "Multiplies by a factor of 2 the number of levels present on the scale.\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->scalelevels=cv->scalelevels*2;
      if (cv->scalelevels > 255) cv->scalelevels=255;
      canvas->drawxy();
    }
}

void Menu::decscalelevels()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Decrease Scale Levels",
      "Divides by a factor of 2 the number of levels present on the scale.\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->scalelevels=cv->scalelevels/2;
      if (cv->scalelevels < 4) cv->scalelevels=4;
      canvas->drawxy();
    }
}

void Menu::propspacing()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Proportional Spacing",
      "Proportional Spacing fixes the distance (in pixels) between the nodes\n"
      "to a constant on any of the i,j or k axis.\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->proportional=TRUE;
      transfo.find_zoom_proportional(cv);
      nodespacing->setItemChecked( propID, TRUE );
      nodespacing->setItemChecked( nonpropID, FALSE );

      canvas->drawxy();
    }
}



void Menu::centerxycanvas()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Center xy-Canvas",
      "Center xy-Canvas will simply move the xy-canvas's center to the\n"
      "center. Note that this applies independantly of the zoomfactor,\n"
      "of Proportional/Non-Proportional Viewing and does not affect the \n"
      "value of the z-depth.\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->decalx=-(cv->zoomfactx-1.)/2.;
      cv->decaly=-(cv->zoomfacty-1.)/2.;
      canvas->drawxy();
    }
}



void Menu::resetzoomfactor()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Reset Zoom Factor",
      "As its name indicates, Reset Zoom Factor resets the zoom factor\n"
      "to its original value, that is, 1. Note that this function can be\n"
      "used both in the Proportional and Non-Proportional Spacing View\n"
      "modes.\n");
      cv->instanthelp=FALSE;
    }
    else {
      transfo.zoomtrans(&(cv->zoomfactx), &(cv->decalx), &(cv->resfact),
                       (double)(1./cv->zoomfactref));
      transfo.zoomtrans(&(cv->zoomfacty), &(cv->decaly), &(cv->resfact),
                       (double)(1./cv->zoomfactref));
      cv->zoomfactref=1.0;
      canvas->drawxy();
    }
}





void Menu::nonpropspacing()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Proportional Spacing",
      "Non Proportional Spacing does not fix the distance (in pixels) between\n"
      "the nodes to be necessarily constant. Rather, the entire domain will\n"
      "occupy the maximum space of the canvas for a zoom factor of 1, indepen-\n"
      "dantly of the size of the window. By default this option is selected\n"
      "instead of Proportional Spacing.\n");
      cv->instanthelp=FALSE;
    }
    else {
      double fact;
      cv->proportional=FALSE;
      fact=cv->zoomfactref/cv->zoomfactx;
      transfo.zoomtrans(&(cv->zoomfactx), &(cv->decalx), &(cv->resfact),fact);
      fact=cv->zoomfactref/cv->zoomfacty;
      transfo.zoomtrans(&(cv->zoomfacty), &(cv->decaly), &(cv->resfact),fact);
      nodespacing->setItemChecked( propID, FALSE );
      nodespacing->setItemChecked( nonpropID, TRUE );
      canvas->drawxy();
    }
}

void Menu::instanthelp()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on Instant Help",
      "Clicking on Instant Help first and then on a particular button or\n"
      "pull-down menu item will give the user the relevant help on the \n"
      "button or item pressed.\n");
      cv->instanthelp=FALSE;
    }
    else {
      nplot->status->setText("Click on a Button or Menu Item for Instant Help");
      cv->instanthelp=TRUE;
    }
}


void Menu::aboutinstanthelp()
{
    if (cv->instanthelp) cv->instanthelp=FALSE;
    QMessageBox::about( this, "NodPlot: About Instant Help",
      "Clicking on Instant Help first and then on a button or\n"
      "pull-down menu item will yield the relevant help.\n");
}


void Menu::aboutNPlot()
{
    cv->instanthelp=FALSE;
    QMessageBox::about( this, "About NodPlot",
      "NodPlot version 1.0\n"
      "\n"
	"Nodplot was developped to ease the task of visualizing rapidly\n"
	"properties in 3D structured data sets. 2D meshes can\n" 
        "be visualized by repeating twice the data in the input\n"
        "file.\n"
        "\n"
        "The visualization is performed on a node basis, to easily have access\n"
        "to properties at any node.\n"
        "\n"
        "The author can be reached by email for bug reports, comments or\n"
        "code improvement suggestions at bernparent@hotmail.com.\n"  
        "\n"
	"Copyright, 1998-2005, Bernard Parent" );
}


void Menu::aboutQt()
{
    cv->instanthelp=FALSE;
    QMessageBox::aboutQt( this, "Qt Menu Example" );
}


void Menu::linkcanvas(typecanvas *cv2, Canvas *canvas2)
{
    cv=cv2;		
    canvas=canvas2;
}

GetData::GetData(char *messtr,double *inputnumber1 , Canvas *canvas1)
    {
#include "fonts.h"
      QFontMetrics    fontm5(font5);
      err=1;
      canvas=canvas1;
      inputnumber=inputnumber1;
      setCaption( "NodPlot: Input Value" );
      resize(max(fontm5.width(" Apply  Dismiss "),fontm5.width(messtr)+3),
             (long)(fontm5.height()*4.5));
      setMinimumSize( width(), height() );
      setMaximumSize( width(), height() );


      apply= new QPushButton( "Apply", this );
      dismiss= new QPushButton( "Dismiss", this );
      apply ->resize( width()/2 , (long)(fontm5.height()*1.5) );
      dismiss ->resize( width()/2 , (long)(fontm5.height()*1.5) );
      apply->move(1, height()-apply->height());
      dismiss->move(width()/2,height()-dismiss->height() );
      connect( apply, SIGNAL(clicked()), this, SLOT(slotApply()) );
      connect( dismiss, SIGNAL(clicked()), this, SLOT(slotDismiss()) );

      message=new QLabel (messtr,this);
      message->resize(width(),(long)(fontm5.height()*1.5));
      message->move(1,1);

    le = new QLineEdit( this );
    le->setFixedHeight((long)(fontm5.height()*1.5));
    le->resize(width(),le->height());
    le->move(1,message->height());
    connect( le, SIGNAL(textChanged(const QString &)), this,
               SLOT(slotTextChanged(const QString &)) );
    connect( le, SIGNAL(returnPressed()), this,
               SLOT(slotApply()) );



    }




void GetData::slotTextChanged(const QString & newText)
{
    err = (sscanf(newText, "%lg", &inputnumber_tmp) == 0);
}


void GetData::slotDismiss()
{
   hide();
}


void GetData::slotApply()
{
   if (err==0) {
     *inputnumber=inputnumber_tmp;
     canvas->drawxy();  
   }
  
}


void PropMenuLine::change_numprop(){



    char newcaptionname[256],captionname[256],captionname2[256];
    if (nplot->mainwidget) {
       strcpy(captionname,"nodplot");
    }
    else {
       strcpy(captionname,"nodplot");
    }
    sprintf(captionname2, "%s : %s", captionname, cv->inputfilename);
    sprintf(newcaptionname, "%s : %s", captionname2, cv->proptitle[thisline]);
    
    nplot->setCaption( newcaptionname );




     cv->prop=thisline;
     cv->datamin=cv->scandatamin[cv->prop];
     cv->datamax=cv->scandatamax[cv->prop];
     for (long cnt=0; cnt<cv->numprop; cnt++) {
       if (cnt==cv->prop) {
         prop->setItemChecked(propmenulineflag[cnt], TRUE);
       }
       else
         prop->setItemChecked(propmenulineflag[cnt], FALSE);
     }
     canvas->drawxy();
   }





