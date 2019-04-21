//
// Button Group Class
//


#include "butgr.h"
#include <stdio.h>


ButGr::ButGr( QWidget *parent, const char *name )
    : QWidget( parent, name )
{
#include "fonts.h"
    QFontMetrics    fontm4(font4);
    
    long posy,lengthx,lengthy;

    lengthx=fontm4.width("RR")+7;
    lengthy=fontm4.height()*2/3+12;

    posy=2;

  // Create and setup the zoom buttons
    zoom_plus= new QPushButton( "Z+", this );
    zoom_minus= new QPushButton( "Z-", this );
    zoom_plus ->resize( lengthx , lengthy );
    zoom_minus ->resize( lengthx , lengthy );
    zoom_plus->move( 2, posy );
    zoom_minus->move( 2+lengthx, posy );
    zoom_plus->setFont(font4);
    zoom_minus->setFont(font4);
    connect( zoom_plus, SIGNAL(clicked()), SLOT(butzoomplus()) );
    connect( zoom_minus, SIGNAL(clicked()), SLOT(butzoomminus()) );

  // Create and setup the ijk buttons

    posy=posy+lengthy+2;
    ijk_i=new QPushButton( "i", this );
    ijk_j= new QPushButton( "j", this );
    ijk_k= new QPushButton( "k", this );
    ijk_i ->resize( (long)(lengthx*2/3) , lengthy );
    ijk_j ->resize( (long)(lengthx*2/3) , lengthy );
    ijk_k ->resize( (long)(lengthx*2/3) , lengthy );
    ijk_i->move( 2, posy );
    ijk_j->move( 2+(long)(lengthx*2/3) , posy );
    ijk_k->move( 2+(long)(lengthx*4/3), posy );
    ijk_i->setFont(font4);
    ijk_j->setFont(font4);
    ijk_k->setFont(font4);
    connect( ijk_i, SIGNAL(clicked()), SLOT(buti()) );
    connect( ijk_j, SIGNAL(clicked()), SLOT(butj()) );
    connect( ijk_k, SIGNAL(clicked()), SLOT(butk()) );




  // Create and setup the resolution button

    posy=posy+lengthy+2;
    res_plus= new QPushButton( "R+", this );
    res_minus= new QPushButton( "R-", this );
    res_plus ->resize( lengthx , lengthy );
    res_minus ->resize( lengthx , lengthy );
    res_plus->move( 2, posy );
    res_minus->move( 2+lengthx, posy );
    res_plus->setFont(font4);
    res_minus->setFont(font4);
    connect( res_plus, SIGNAL(clicked()), SLOT(butresplus()) );
    connect( res_minus, SIGNAL(clicked()), SLOT(butresminus()) );



  // Create and setup the shownode button

    posy=posy+lengthy+2;
    sn_plus= new QPushButton( "N+", this );
    sn_minus= new QPushButton( "N-", this );
    sn_plus ->resize( lengthx , lengthy );
    sn_minus ->resize( lengthx , lengthy );
    sn_plus->move( 2, posy );
    sn_minus->move( 2+lengthx, posy );
    sn_plus->setFont(font4);
    sn_minus->setFont(font4);
    connect( sn_plus, SIGNAL(clicked()), SLOT(butsnplus()) );
    connect( sn_minus, SIGNAL(clicked()), SLOT(butsnminus()) );

    resize(2*lengthx+4,posy+lengthy+2);
}

void ButGr::linkcanvas(typecanvas *cv2, Canvas *canvas2)
{
    canvas=canvas2;
    cv=cv2;		
}




void ButGr::butresplus()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on the R+ PushButton",
      "The R+ (Resolution Plus) push button increases by a factor of 4 the\n" 
      "total number of rectangles used to draw the canvas.\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->resfact=cv->resfact*4.0;
      canvas->drawxy();
    }		
}

void ButGr::butresminus()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on the R- PushButton",
      "The R- (Resolution Minus) push button decreases by a factor of 4 the\n"
      "total number of rectangles used to draw the canvas.\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->resfact=cv->resfact/4.0;
      //if (cv->resfact<100) cv->resfact=100;
      canvas->drawxy();
    }		
}

void ButGr::butsnminus()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on the N- PushButton",
      "The N- (Nodes Minus) push button results in less visible nodes.\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->shownode=cv->shownode-1;
      if (cv->shownode < 0) cv->shownode=0;
      canvas->drawxy();
    }		
}

void ButGr::butsnplus()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on the N+ PushButton",
      "The N+ (Nodes Plus) push button results in more visible nodes.\n");
      cv->instanthelp=FALSE;
    }
    else {
      cv->shownode=cv->shownode+1;
      if (cv->shownode > 2) cv->shownode=2;
      canvas->drawxy();
    }		
}




void ButGr::butzoomplus()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on the Z+ PushButton",
      "The Z+ (Zoom Plus) push button multiplies by 2 both the zoom factor \n"
      "along x and the zoom factor along y. \n"
      "\n"
      "The zoom takes place at the center of the xy canvas where the cross is\n"
      "shown\n");
      cv->instanthelp=FALSE;
    }
    else {
      transfo.zoomtrans(&(cv->zoomfactx), &(cv->decalx), &(cv->resfact),2.);
      transfo.zoomtrans(&(cv->zoomfacty), &(cv->decaly), &(cv->resfact),2.);
      cv->zoomfactref=cv->zoomfactref*2.;
      canvas->drawxy();
    }		
}

void ButGr::butzoomminus()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on the Z- PushButton",
      "The Z- (Zoom Minus) push button divides by 2 both the zoom factor \n"
      "along x and the zoom factor along y. \n"
      "\n"
      "The zoom takes place at the center of the xy canvas, where the\n"
      "cross is shown\n");
      cv->instanthelp=FALSE;
    }
    else {
      transfo.zoomtrans(&(cv->zoomfactx), &(cv->decalx), &(cv->resfact),.5);
      transfo.zoomtrans(&(cv->zoomfacty), &(cv->decaly), &(cv->resfact),.5);
      cv->zoomfactref=cv->zoomfactref*.5;
      canvas->drawxy();
    }		
}




void ButGr::buti()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on the i PushButton",
      "The i push button enables to view the 3D data at a constant i.\n"
      "\n"
      "The rotation is centered around the nearest node to the middle\n"
      "of the xy canvas, where the cross is located. \n");
      cv->instanthelp=FALSE;
    }
    else {
      long ci,cj,ck;
      transfo.find_center_node(*cv,&ci,&cj,&ck);
  //    printf("i=%ld j=%ld k=%ld\n",ci,cj,ck);
      cv->is=cv->kso;
      cv->ie=cv->keo;
      cv->js=cv->jso;
      cv->je=cv->jeo;
      cv->ks=cv->iso;
      cv->ke=cv->ieo;
      cv->kk=ci;
      if (cv->kk>cv->ke) cv->kk=cv->ke;
      if (cv->kk<cv->ks) cv->kk=cv->ks;
      strcpy(cv->upaxis, "ikj");
      transfo.find_decal_from_node_pixel(cv,ck, cj, 
          (long)((cv->posxe+cv->posxs)/2.+0.5), 
          (long)((cv->posye+cv->posys)/2.+0.5));
      if (cv->proportional) transfo.find_zoom_proportional(cv);
      canvas->drawxy();
    }		
}

void ButGr::butj()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on the j PushButton",
      "The j push button enables to view the 3D data at a constant j\n"
      "\n"
      "The rotation is centered around the nearest node to the middle\n"
      "of the xy canvas, where the cross is shown.\n");
      cv->instanthelp=FALSE;
    }
    else {
      long ci,cj,ck;
      transfo.find_center_node(*cv,&ci,&cj,&ck);
//      printf("i=%ld j=%ld k=%ld\n",ci,cj,ck);
 
      cv->is=cv->kso;
      cv->ie=cv->keo;
      cv->js=cv->iso;
      cv->je=cv->ieo;
      cv->ks=cv->jso;
      cv->ke=cv->jeo;
      cv->kk=cj;
      if (cv->kk>cv->ke) cv->kk=cv->ke;
      if (cv->kk<cv->ks) cv->kk=cv->ks;
      strcpy(cv->upaxis, "jki");
      transfo.find_decal_from_node_pixel(cv,ck, ci, 
          (long)((cv->posxe+cv->posxs)/2.+0.5), 
          (long)((cv->posye+cv->posys)/2.+0.5));
      if (cv->proportional) transfo.find_zoom_proportional(cv);
      canvas->drawxy();
    }		
}

void ButGr::butk()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on the k PushButton",
      "The k push button enables to view the 3D data at a constant k.\n"
      "\n"
      "The rotation is centered around the nearest node to the middle\n"
      "of the xy canvas, where the cross is located. \n");
      cv->instanthelp=FALSE;
    }
    else {

      long ci,cj,ck;
      transfo.find_center_node(*cv,&ci,&cj,&ck);
//      printf("i=%ld j=%ld k=%ld\n",ci,cj,ck);

      cv->is=cv->iso;
      cv->ie=cv->ieo;
      cv->js=cv->jso;
      cv->je=cv->jeo;
      cv->ks=cv->kso;
      cv->ke=cv->keo;
      cv->kk=ck;
      if (cv->kk>cv->ke) cv->kk=cv->ke;
      if (cv->kk<cv->ks) cv->kk=cv->ks;
      strcpy(cv->upaxis, "kij");
      transfo.find_decal_from_node_pixel(cv, ci, cj, 
          (long)((cv->posxe+cv->posxs)/2.+0.5), 
          (long)((cv->posye+cv->posys)/2.+0.5));
      if (cv->proportional) transfo.find_zoom_proportional(cv);
      canvas->drawxy();		
    }
}



