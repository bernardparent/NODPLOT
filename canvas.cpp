// Canvas Class
//

#include "canvas.h"

#define max(a,b)   ((a) > (b) ? (a) : (b))
#define min(a,b)   ((a) < (b) ? (a) : (b))




//
// Construct the nplot with buttons.
//



Canvas::Canvas( QWidget *parent, const char *name )
    : QWidget( parent, name )
{
    long lengthx,lengthy;
    QColor c;
#include "fonts.h"
    QFontMetrics    fontm4(font4);

    //c.setHsv( 36, 36, 177 );
    //setBackgroundColor( c );
    setCursor(crossCursor);
    setMouseTracking( TRUE );


  // Create and setup the forward/backward buttons

    lengthy=(long)(fontm4.height()*1.0);
    lengthx=lengthy;
    fw = new QPushButton( "+", this );
    bw = new QPushButton( "-", this );
    fw->resize( lengthx, lengthy );
    bw->resize( lengthx, lengthy );
    fw->setFont(font4);
    bw->setFont(font4);
    connect( fw, SIGNAL(clicked()), SLOT(butfw()) );
    connect( bw, SIGNAL(clicked()), SLOT(butbw()) );


}


//
// Calls the drawing function as specified by the radio buttons.
//


void Canvas::butfw()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on the + button",
      "The + button increases by 1 the value of the index \n"
      "perpendicular to the canvas.\n");
      cv->instanthelp=FALSE; 
    }
    else { 
      cv->kk=cv->kk+1;
      if (cv->kk>cv->ke) cv->kk=cv->ke;
      drawxy();
    }	
}

void Canvas::butbw()
{
    if (cv->instanthelp) {
      QMessageBox::about( this, "NodPlot: Help on the - button",
      "The - button decreases by 1 the value of the index\n"
      "perpendicular to the canvas.\n");
      cv->instanthelp=FALSE; 
    }
    else { 
      cv->kk=cv->kk-1;
      if (cv->kk<cv->ks) cv->kk=cv->ks;
      drawxy();
    }	
}




//
// This function initializes
//

void Canvas::init()
{
#include "fonts.h"
   QFontMetrics    fontm1(font1);

   testme=10101;
   QString nstr1,nstr2;
   nstr1.sprintf( "%ld", cv->is);    
   nstr2.sprintf( "%ld", cv->ie);
   maxnodelength=max(fontm1.width(nstr1),fontm1.width(nstr2));
   nstr2.sprintf( "%ld", cv->js);
   maxnodelength=max(maxnodelength,fontm1.width(nstr2));
   nstr2.sprintf( "%ld", cv->je);
   maxnodelength=max(maxnodelength,fontm1.width(nstr2));
   nstr2.sprintf( "%ld", cv->ks);
   maxnodelength=max(maxnodelength,fontm1.width(nstr2));
   nstr2.sprintf( "%ld", cv->ke);
   maxnodelength=max(maxnodelength,fontm1.width(nstr2));
   mousebuttonpressed=FALSE;
}






//
// draws xy graph on nplot
//

void Canvas::drawxy()
{
#include "fonts.h"
   QFontMetrics    fontm2(font2);
   QPainter p;
   QColor c;
   double pixelsx,pixelsy;
   double fictresi,fictresj,resi,resj;

//  Create a pixmap pm used to store the image temporarily in mem
//  before showing it to the screen
//  pm.fill is used to fill the pixmap with the widget's background color

   QApplication::setOverrideCursor( waitCursor ); // this might take time
 
   QPixmap pm( cv->kposxs-2-fontm2.width("X")/2, height() );
   pm.fill( this, cv->kposxs-2-fontm2.width("X")/2, height() );

   resj=sqrt(cv->resfact*(cv->totpixelsy*cv->zoomfacty)
                                      /(cv->totpixelsx*cv->zoomfactx));
   resi=((double)(cv->totpixelsx*cv->zoomfactx)*resj
                       /(double)(cv->totpixelsy*cv->zoomfacty));
   //fprintf(stderr,"\n\n resi=%E  resj=%E\n",resi,resj);
   //fprintf(stderr,"totpixelsx=%ld\n",cv->totpixelsx);
   //fprintf(stderr,"resfact=%E\n",cv->resfact);
   //fprintf(stderr,"zoomfactx=%E  zoomfacty=%E\n",cv->zoomfactx,cv->zoomfacty);
   fictresi=resi;
   fictresj=resj;
   if (resi>(cv->totpixelsx*cv->zoomfactx)) 
        fictresi=(cv->totpixelsx*cv->zoomfactx);
   if (resj>(cv->totpixelsy*cv->zoomfacty)) 
        fictresj=(cv->totpixelsy*cv->zoomfacty);

   pixelsx=(double)cv->totpixelsx/fictresi;       pixelsy=(double)cv->totpixelsy/fictresj;
   pixelsx=pixelsx*(double)cv->zoomfactx;          pixelsy=pixelsy*(double)cv->zoomfacty;


   (p).begin( &pm );

//  Fill drawing canvas with medium gray color
    
   c.setHsv( 36, 36, 136 );
   (p).setPen( c );
   (p).setBrush( c );
   (p).drawRect(cv->posxs,cv->posys,cv->totpixelsx,cv->totpixelsy);

    //fprintf(stderr,"\n\n fictresi=%E  fictresj=%E\n",fictresi,fictresj);
    //fprintf(stderr,"resi=%E  resj=%E\n",resi,resj);
    //fprintf(stderr,"totpixelsx=%ld  totpixelsy=%ld\n",cv->totpixelsx,cv->totpixelsy);
    //fprintf(stderr,"zoomfactx=%E  zoomfacty=%E\n",cv->zoomfactx,cv->zoomfacty);
    draw_rectangles(&p,fictresi,fictresj,pixelsx,pixelsy);
    draw_axis_legend(&p);
    draw_nodes(&p,pixelsx,pixelsy,fictresi,fictresj);
    draw_xyaxis_values(&p,pixelsx,pixelsy,fictresi,fictresj);
    draw_color_legend(&p);

    (p).end();
    
// Export the bitmap pm to the screen at position 0,0

    bitBlt( this, 0, 0, &pm );
    QApplication::restoreOverrideCursor();    // restore original cursor

    drawz((double)cv->is, (double)cv->js, FALSE);
}




//
// draws z graph on canvas
//

void Canvas::drawz(double i, double j, bool active)
{
#include "fonts.h"
   QFontMetrics    fontm1(font1);
   QFontMetrics    fontm2(font2);
   QFontMetrics    fontm4(font4);

   QPainter p;
   QColor c;
   long cntz,fictresy,fictresx,z1,z2,cnt;
   double datap,datap1,datap2,z;
   QString nstr;
   long posx1,posx2,posy1,posy2;

//  Create a pixmap pm used to store the image temporarily in mem
//  before showing it to the screen
//  pm.fill is used to fill the pixmap with the widget's background color

   QPixmap pm( cv->kposxe-cv->kposxs+maxnodelength+10+fontm2.width("X")/2, cv->kposye-cv->kposys+fontm4.height()*2+fontm2.height()+50 );
   pm.fill( this, cv->kposxe-cv->kposxs+maxnodelength+10+fontm2.width("X")/2, cv->kposye-cv->kposys+fontm4.height()*2+fontm2.height()+50 );
   fictresy=cv->kposye-cv->kposys+1;
   fictresx=cv->kposxe-cv->kposxs+1;

   posx1=fontm2.width("X")/2;
   posy1=cv->kposye-cv->kposys+fw->height()+fontm2.height()+10;

   posx2=posx1+(cv->kposxe-cv->kposxs);
   posy2=posy1-(cv->kposye-cv->kposys);
 
   (p).begin( &pm );

   if (active) {
// draw rectangles
     for (cntz=1; cntz<=fictresy; cntz++){
        z=((double)(cntz-1)/(fictresy-1)*(cv->ke-cv->ks)+cv->ks);
        z1=(long)z;
        z2=z1+1;
        if (z2>cv->ke) {
          datap=find_data(i,j,z1);
        }
        else {
          datap1=find_data(i,j,z1);
          datap2=find_data(i,j,z2);
          datap=(z-(double)z1)*datap2+(1.-z+(double)z1)*datap1;
        }
        find_color((double)((datap-cv->datamin)/(cv->datamax-cv->datamin)),&c);
        (p).setPen( c );
        (p).setBrush( c );
        (p).drawRect( posx1, posy1+1-cntz,  fictresx, 1);		
     }
   }
   else {
     c.setHsv( 36, 36, 136 );
     (p).setPen(c);
     (p).setBrush( c );
     (p).drawRect( posx1, posy2,  fictresx, fictresy);		
   }
// Draw axis and legends

     (p).setPen( black );             
     (p).drawLine(posx1-1,posy1+3+fw->height(),posx1-1,posy2-5-fw->height());    
     (p).drawLine(posx1-2,posy1+3+fw->height(),posx1-2,posy2-5-fw->height());    
     (p).drawLine(posx1-1,posy2-1,posx2+1,posy2-1);    
     (p).drawLine(posx2+1,posy1+1,posx2+1,posy2-1);    
     (p).drawLine(posx1-1,posy1+1,posx2+1,posy1+1);    

   for (cnt=1; cnt<=3; cnt++) {
      (p).drawLine(posx1-(4-cnt)-1,posy2-cnt-2-fw->height(),posx1+(4-cnt)-2,posy2-cnt-2-fw->height());    
     }

      (p).setFont(font2);    
      (p).setPen( black );             
      nstr.sprintf( "%c", cv->upaxis[0]);
      (p).drawText(posx1-fontm2.width(nstr)/2, posy2-fontm2.height()/3-3-fw->height(), nstr ); 

// show and draw the depth position (eg k=22)

      (p).setFont(font1);    
      (p).setPen( black );             
      cntz=(long)((double)(cv->kk-cv->ks)/(double)(cv->ke-cv->ks)*(fictresy-1.)+1.);
      (p).drawLine(posx1, posy1+1-cntz, posx2+3,posy1+1-cntz);		
      for (long ii=1; ii<=3; ii++) {
        (p).drawLine(posx2+1+ii, posy1+1-cntz-ii  ,posx2+1+ii, posy1+1-cntz+ii  );
      }
      nstr.sprintf( "%ld", cv->kk );
      (p).drawText(posx2+7, posy1+1-cntz+(fontm1.height())/3, nstr ); 


   (p).end();
    
// Export the bitmap pm to the screen at position cv->posxe+5,0

    bitBlt( this, cv->kposxs-posx1, cv->kposye-posy1, &pm );

}


//
// This function finds the value at a certain pi,pj with kk predefined
//
double Canvas::find_data(double pi, double pj, long pk)
{
  double val,val5,val6;
  long i,j;
  i=(long)pi;
  j=(long)pj;
  if (i==cv->ie && pi<(double)cv->ie+0.5) i=cv->ie-1;
  if (j==cv->je && pj<(double)cv->je+0.5) j=cv->je-1;
  if (i==cv->is-1 && pi>(double)cv->is-0.5) i=cv->is;
  if (j==cv->js-1 && pj>(double)cv->js-0.5) j=cv->js;
  if (i < cv->is || i >= cv->ie || j < cv->js || j >= cv->je){
#ifndef NDEBUG      
     printf("out of range: i= %10ld  j= %10ld\n",i,j); 
     printf("              is= %5ld  ie= %5ld  js= %5ld  je= %5ld\n",cv->is,cv->ie,cv->js,cv->je); 
#endif
     if (i<cv->is) i=cv->is;
     if (j<cv->js) j=cv->js;
     if (i>cv->ie-1) i=cv->ie-1;
     if (j>cv->je-1) j=cv->je-1;
     }  

  val5=(pj-j)*cv->data[transfo.ai(cv,i,j+1,pk)]+(1-pj+j)*
              cv->data[transfo.ai(cv,i,j,pk)];
  val6=(pj-j)*cv->data[transfo.ai(cv,i+1,j+1,pk)]+(1-pj+j)*
              cv->data[transfo.ai(cv,i+1,j,pk)];
  val=(pi-i)*val6+(1-pi+i)*val5;
  return(val);
}








void Canvas::find_color(double val, QColor *c)
{
  long col;
  double fact;
  fact=255./(double)cv->scalelevels;
  col=(long)floor(val*(double)cv->scalelevels+0.5);
  col=255-(long)((double)col*fact);
  if (col<0) col=0;
  if (col>255) col=255;
//  c->setHsv( 0, 0, col ); //default to gray 
  if (cv->colortype==3) c->setHsv(0, 0, col);
  if (cv->colortype==1) c->setHsv( col, 255, 255 );
  if (cv->colortype==2) c->setHsv(225, 255-col, 200);

}







//
//  Draw color contours with rectangles
//
void Canvas::draw_rectangles(QPainter *p, double fictresi,
                                double fictresj, double pixelsx, double pixelsy)
{    
  long cnti,cntj;
  long rect1x,rect1y,rect2x,rect2y;
  double datap;
  double pi,pj; 
  QColor c,c2;
  bool add_rect;
  
  
//  Find the limits for the i and j loops to be included in the drawing canvas

   long cntis=max(0,(long)((cv->posxs-(cv->decalx*cv->totpixelsx)-cv->posxs)/pixelsx-2));
   long cntie=min((long)fictresi,(long)((cv->posxe-(cv->decalx*cv->totpixelsx)-cv->posxs)/pixelsx+2));
   long cntjs=max(0,(long)((transfo.jinv(*cv,cv->posye)-(cv->decaly*cv->totpixelsy)-cv->posys)/pixelsy-2));
   long cntje=min((long)fictresj,(long)((transfo.jinv(*cv,cv->posys)-(cv->decaly*cv->totpixelsy)-cv->posys)/pixelsy+2));

//  Draw the rectangles in drawing canvas

   for ( cnti=cntis; cnti<cntie; cnti++ ) {		
      for ( cntj=cntjs; cntj<cntje; cntj++ ) {
        rect1x=(long)(cv->posxs+cnti*pixelsx+cv->decalx*cv->totpixelsx);
        rect1y=transfo.jinv(*cv,(long)(cv->posys+cntj*pixelsy+cv->decaly*cv->totpixelsy));
        rect2x=(long)(cv->posxs+(cnti+1)*pixelsx+1*0+cv->decalx*cv->totpixelsx);
        rect2y=transfo.jinv(*cv,(long)(cv->posys+(cntj+1)*pixelsy+1*0+cv->decaly*cv->totpixelsy));
        pi=(((cnti+.5)/fictresi)*(cv->ie-cv->is)+cv->is);
        pj=(((cntj+.5)/fictresj)*(cv->je-cv->js)+cv->js);
        datap=find_data(pi,pj,cv->kk);
        find_color((double)((datap-cv->datamin)/(cv->datamax-cv->datamin)),&c);

        do {
          pi=(((cnti+.5)/fictresi)*(cv->ie-cv->is)+cv->is);
          pj=(((cntj+1+.5)/fictresj)*(cv->je-cv->js)+cv->js);
          datap=find_data(pi,pj,cv->kk);
          find_color((double)((datap-cv->datamin)/(cv->datamax-cv->datamin)),&c2);
          add_rect=FALSE;
          if (c2==c && cntj<cntje-1) {
            //printf("hello!!\n");
            add_rect=TRUE;
            cntj++;
            rect2y=transfo.jinv(*cv,(long)(cv->posys+(cntj+1)*pixelsy+1*0+cv->decaly*cv->totpixelsy));
          }
        } while (add_rect);

        if (rect1x < cv->posxs) rect1x=cv->posxs;
        if (rect2y < cv->posys) rect2y=cv->posys;
        if (rect2x > cv->posxe) rect2x=cv->posxe;
        if (rect1y > cv->posye) rect1y=cv->posye;
        if (rect2x>=rect1x && rect1y>=rect2y) {
          (*p).setPen( c );
          (*p).setBrush( c );
	  //fprintf(stderr,".");
          (*p).drawRect( rect1x, rect2y, rect2x-rect1x+1, rect1y-rect2y+1 );		
        }
      }
    }
}



//
//  Draw axis legend
//  Draw x-y axis and i-j legends
//
void Canvas::draw_axis_legend(QPainter *p)
{    
#include "fonts.h"
      QFontMetrics    fontm2(font2);
      QColor c;
      long cnt;           
      QString nstr;

      c.setHsv( 0, 0, 0 );
      (*p).setPen( c );
      (*p).setBrush( c );
      long crosscx=(long)((cv->posxs+cv->posxe)/2.+0.5);
      long crosscy=(long)((cv->posys+cv->posye)/2.+0.5);

      (*p).drawLine(crosscx-9,crosscy,crosscx-3,crosscy);
      (*p).drawLine(crosscx+3,crosscy,crosscx+9,crosscy);
      (*p).drawLine(crosscx,crosscy-9,crosscx,crosscy-3);
      (*p).drawLine(crosscx,crosscy+3,crosscx,crosscy+9);
      (*p).drawRect(cv->posxs-2,cv->posys-2,2,cv->posye-cv->posys+7);
      (*p).drawRect(cv->posxs-4,cv->posye+1,cv->posxe-cv->posxs+7,2);
      (*p).drawLine(cv->posxs-1,cv->posys-1,cv->posxe+1,cv->posys-1);    
      (*p).drawLine(cv->posxe+1,cv->posys-1,cv->posxe+1,cv->posye+1);    
      for (cnt=1; cnt<=3; cnt++) {
        (*p).drawLine(cv->posxe+cnt+2,cv->posye-(4-cnt)+2,cv->posxe+cnt+2,cv->posye+(4-cnt)+1);    
        (*p).drawLine(cv->posxs-(4-cnt)-1,cv->posys-cnt-2,cv->posxs+(4-cnt)-2,cv->posys-cnt-2);    
      }
      
      (*p).setFont(font2);   
      (*p).setPen( black );             
      char upaxis_tmp=cv->upaxis[1]; 
      (*p).drawText( cv->posxe+8, cv->posye+fontm2.height()/3, &upaxis_tmp, 1); 
      upaxis_tmp=cv->upaxis[2];
      (*p).drawText( cv->posxs-fontm2.width(cv->upaxis[2])/2-1, cv->posys-fontm2.height()/3-3, &upaxis_tmp,1); 

}




//
//  Draw nodes with small crosshairs
//
void Canvas::draw_nodes(QPainter *p, double pixelsx,
                           double pixelsy, double fictresi, double fictresj)
{
      long posx,posy;
      if (cv->shownode>0) {
        for (long i=cv->is; i<=cv->ie; i++) {
          for (long j=cv->js; j<=cv->je; j++) {       
          transfo.find_pixel_from_node(*cv,i, j, pixelsx, pixelsy,
                      fictresi, fictresj, &posx, &posy);
          if (posx>cv->posxs && posx<cv->posxe && posy>cv->posys && posy<cv->posye) {
            if (cv->shownode==2) {
            (*p).drawLine(posx-1,posy,posx+1,posy);
            (*p).drawLine(posx,posy-1,posx,posy+1);
            }
            if (cv->shownode==1) {
            (*p).drawLine(posx,posy,posx,posy);
            }
            
            }
          }
        }
      }
}


//
//  Print yaxis and xaxis node values
//
void Canvas::draw_xyaxis_values(QPainter *p, double pixelsx,
                           double pixelsy, double fictresi, double fictresj)
{
#include "fonts.h"
      QFontMetrics    fontm1(font1);

      QString nstr;
      long minpixspacing=fontm1.height()/3*2*2;
      long posy1,posy2;
      long posx1,posx2;
      long cnti,cntj;
      double onepixspacing;
      long nodespacing,posx,posy;
      
      (*p).setFont(font1);    
      (*p).setPen( black );             
      transfo.find_pixel_from_node(*cv, cv->is, cv->js, pixelsx, pixelsy, fictresi, fictresj, &posx, &posy1);
      transfo.find_pixel_from_node(*cv, cv->is, cv->je, pixelsx, pixelsy, fictresi, fictresj, &posx, &posy2);
      onepixspacing=(posy1-posy2)/(double)(cv->je-cv->js);
      nodespacing=(long)(minpixspacing/onepixspacing)+1;
      for (cntj=cv->js; cntj<=cv->je; cntj=cntj+nodespacing) {
        transfo.find_pixel_from_node(*cv, cv->is, cntj, pixelsx, pixelsy, fictresi, fictresj, &posx, &posy);
        if (posy>=cv->posys && posy<=cv->posye) {
          (*p).drawLine(cv->posxs,posy,cv->posxs+3,posy);
          (*p).drawLine(cv->posxe,posy,cv->posxe-3,posy);
          nstr.sprintf( "%ld", cntj );
          (*p).drawText(cv->posxs-fontm1.width(nstr)-4, posy+(fontm1.height())/3, nstr ); 
          }    
      }
      
      transfo.find_pixel_from_node(*cv, cv->is, cv->js, pixelsx, pixelsy, fictresi, fictresj, &posx1, &posy);
      transfo.find_pixel_from_node(*cv, cv->ie, cv->js, pixelsx, pixelsy, fictresi, fictresj, &posx2, &posy);
      onepixspacing=(posx2-posx1)/(double)(cv->ie-cv->is);
      nodespacing=(long)(minpixspacing/onepixspacing)+1;
      for (cnti=cv->is; cnti<=cv->ie; cnti=cnti+nodespacing) {
        transfo.find_pixel_from_node(*cv, cnti, cv->js, pixelsx, pixelsy, fictresi, fictresj, &posx, &posy);
        if (posx>=cv->posxs && posx<=cv->posxe) {
          (*p).drawLine(posx,cv->posys,posx,cv->posys+3);
          (*p).drawLine(posx,cv->posye,posx,cv->posye-3);
          nstr.sprintf( "%ld", cnti );
          QWMatrix matrix2,matrix3;
          matrix3.translate(posx , cv->posye); 
          matrix3.rotate( -90);       
          (*p).setWorldMatrix( matrix3 );
          (*p).drawText(-fontm1.width(nstr)-4, fontm1.height()/3, nstr ); 
          (*p).setWorldMatrix(matrix2);
          }    
      }

}





//
//  Draw color legend
//
void Canvas::draw_color_legend(QPainter *p)
{    
#include "fonts.h"
    QFontMetrics    fontm1(font1);
    QColor c;
    QString minstr;
    QString maxstr;

    long lenclx,lencly,posclx,poscly,maxw;
    long cnti;
    maxstr.sprintf( "%.3E",cv->datamax);
    minstr.sprintf( "%.3E",cv->datamin);
    (*p).setFont(font1);    
    maxw=fontm1.width(maxstr);
    lenclx=cv->sposxe-cv->sposxs;
    lencly=cv->sposye-cv->sposys;
    posclx=cv->sposxs;
    poscly=cv->sposys;
    
    (*p).drawText( cv->sposxs-1-fw->width(), poscly-7, minstr); 
    (*p).drawText( cv->sposxe+1-maxw+fw->width()+1, poscly-7, maxstr); 
    for ( cnti=0; cnti<=lenclx; cnti++ ) {		
        find_color((double)cnti/lenclx,&c);
        (*p).setPen( c );
        (*p).setBrush( c );
        (*p).drawRect( (long)(posclx+cnti), (long)poscly, 
                     (long)1, (long)lencly );		
    }
    (*p).setPen( black );             
    (*p).drawLine(posclx-1,poscly-1,posclx+lenclx+1,poscly-1);
    (*p).drawLine(posclx-1,poscly+lencly,posclx+lenclx+1,poscly+lencly);
    (*p).drawLine(posclx-1,poscly-4,posclx-1,poscly+lencly);
    (*p).drawLine(posclx+lenclx+1,poscly-4,posclx+lenclx+1,poscly+lencly);
    (*p).drawLine(posclx-2,poscly-2,posclx,poscly-2);
    (*p).drawLine(posclx-3,poscly-3,posclx+1,poscly-3);
    (*p).drawLine(posclx-4,poscly-4,posclx+2,poscly-4);
    (*p).drawLine(posclx+lenclx,poscly-2,posclx+lenclx+2,poscly-2);
    (*p).drawLine(posclx+lenclx-1,poscly-3,posclx+lenclx+3,poscly-3);
    (*p).drawLine(posclx+lenclx-2,poscly-4,posclx+lenclx+4,poscly-4);
}




//
// Called when the widget needs to be updated.
//

void Canvas::paintEvent( QPaintEvent * )
{
    drawxy();
}





void Canvas::mouseMoveEvent( QMouseEvent *e )
{
      double pi,pj;
      long nodei,nodej;
      QString message;
      double val;
      long npickx,npicky;
      bool drawzflag;
      double drawznodei,drawznodej;
      npickx = e->x();
      npicky = e->y();
      drawzflag=FALSE;
      drawznodei=(double)cv->is;
      drawznodej=(double)cv->js;

      if (cv->instanthelp) {
       
      }
      else {
     
        if (npickx>=cv->posxs && npickx<=cv->posxe && npicky>=cv->posys && npicky<=cv->posye){
          transfo.find_node_from_pixel(*cv, npickx,npicky,&pi,&pj);
          nodei=(long)(pi+0.5);
          nodej=(long)(pj+0.5);
          if (pi>=cv->is && pi<=cv->ie && pj>=cv->js && pj<=cv->je) {
            val=find_data((double)nodei,(double)nodej,cv->kk);
            message.sprintf("[%c=%ld   %c=%ld   %c=%ld]    Value=%E", cv->upaxis[0], cv->kk, cv->upaxis[1], 
                           nodei, cv->upaxis[2], nodej, val);
            drawzflag=TRUE;
            drawznodei=pi;
            drawznodej=pj;
            }
          else
            message.sprintf(" ");
          }
        else 
          if (npickx>=cv->kposxs && npickx<=cv->kposxe && npicky>=cv->kposys && npicky<=cv->kposye){
            long ktmp=(long)(0.5+(double)(cv->kposye-npicky)/(cv->kposye-cv->kposys)*(cv->ke-cv->ks)+cv->ks);
            if (mousebuttonpressed) {
              if (ktmp!=cv->kk) {
                cv->kk=ktmp;
                if (cv->kk>cv->ke) cv->kk=cv->ke;
                if (cv->kk<cv->ks) cv->kk=cv->ks;
                drawxy();
              }
            }
            else {
              message.sprintf("Click for Depth %c=%ld",cv->upaxis[0],ktmp);
            }
          }
          else {
            message.sprintf(" ");
          }
        status->setText(message);
        drawz(drawznodei,drawznodej,drawzflag);
      }

}

void Canvas::mousePressEvent( QMouseEvent *e )
{
    long kktmp,npickx,npicky;
    mousebuttonpressed=TRUE;
    npickx = e->x();
    npicky = e->y();

    if (cv->instanthelp) {
      if (npickx>=cv->posxs && npickx<=cv->posxe && npicky>=cv->posys && npicky<=cv->posye){
      QMessageBox::about( this, "NodPlot: Help on xy Canvas",
      "Clicking somewhere on the xy canvas will translate the canvas such \n"
      "that the point clicked will be at the center.\n");
      cv->instanthelp=FALSE; 
      mousebuttonpressed=FALSE;
      }  
      if (npickx>=cv->kposxs && npickx<=cv->kposxe && npicky>=cv->kposys && npicky<=cv->kposye){
      QMessageBox::about( this, "NodPlot: Help on z Canvas",
      "Clicking on a point in the z canvas changes the value of the index \n"
      "perpendicular to the canvas.\n\n"
      "Keeping the mouse button pressed while scrolling through the z canvas\n"
      "will create a dynamic visualization of all nodes at any depth. Also,\n"
      "scrolling along the xy canvas with the mouse pointer will make \n"
      "the z canvas show the property at the xy point of interest.\n"
      "\n");
      cv->instanthelp=FALSE;
      mousebuttonpressed=FALSE;
      }
    }
    else {
      if (npickx>=cv->posxs && npickx<=cv->posxe && npicky>=cv->posys && npicky<=cv->posye){
          cv->decalx=cv->decalx-(double)(npickx-(cv->posxs+cv->posxe)/2.)/(cv->posxe-cv->posxs);
          cv->decaly=cv->decaly+(double)(npicky-(cv->posys+cv->posye)/2.)/(cv->posye-cv->posys);
          drawxy();
          }
      if (npickx>=cv->kposxs && npickx<=cv->kposxe && npicky>=cv->kposys && npicky<=cv->kposye){
          kktmp=(long)(0.5+(double)(cv->kposye-npicky)/(cv->kposye-cv->kposys)*(cv->ke-cv->ks)+cv->ks);
          if (kktmp!=cv->kk){
            cv->kk=kktmp;
            if (cv->kk>cv->ke) cv->kk=cv->ke;
            if (cv->kk<cv->ks) cv->kk=cv->ks;
            drawxy();
          }
          }
    }
}


void Canvas::mouseReleaseEvent(QMouseEvent *)
{
      mousebuttonpressed=FALSE;
}

void Canvas::linkcanvas(typecanvas *cv2, QLabel *status2)
{
    cv=cv2;		
    status=status2;
}




//
// Called when the widget has been resized.
// Moves the button group to the upper right corner
// of the widget.

void Canvas::resizeEvent( QResizeEvent * )
{
#include "fonts.h"
    QFontMetrics    fontm1(font1);    
    QFontMetrics    fontm2(font2);


    cv->sposys=fontm2.height()+5;
    cv->sposye=cv->sposys+(long)(fw->height()/2.75);

    cv->posxs=maxnodelength+10;                cv->posys=cv->sposye+fw->height()/2;
    cv->posye=height()-maxnodelength-10;
    
    cv->kposxe=width()-maxnodelength-10;
    cv->kposxs=cv->kposxe-fw->width()+2;
    cv->kposys=fw->height()+fontm2.height()+5;
    cv->kposye=height()-bw->height()-6;

    cv->posxe=cv->kposxs-(long)(fontm2.width("X")*3./2.)-10;

    cv->sposxs=cv->posxs+fontm1.width("000")+20;
    cv->sposxe=cv->posxe-fw->width();
    if (cv->sposxs>cv->sposxe-20) cv->sposxs=cv->sposxe-20;
    if (cv->sposxe-cv->sposxs>255) cv->sposxs=cv->sposxe-255;
    
    //fprintf(stderr,"\nresizeevent\n");
    cv->totpixelsx=cv->posxe-cv->posxs+1;  
    cv->totpixelsy=cv->posye-cv->posys+1;

    fw->move( cv->kposxs, cv->kposys-fw->height()-1);
    bw->move( cv->kposxs, cv->kposye+2);

    if (cv->proportional) transfo.find_zoom_proportional(cv);

}
