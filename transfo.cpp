//
// transformation subroutines needed by menu, canvas or button group
//

//
//  find node value (i,j) from screen pixel
//

#include "transfo.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void Transfo::input_data(char *filename,
                       long cntfile,
		       long numfile,
                       long *is, long *ie, 
                       long *js, long *je,
                       long *ks, long *ke,
                       long *numprop,
                       proptitlename **proptitle,
                       double **data1)
{
  long ii, iimax, i, j, k, ks1, ke1;
  long prop,propdecal;
  char *TEMP;
  FILE *inputfile;
  double *datatmp;

  inputfile = fopen(filename, "r");
  if (inputfile==NULL) {
    fprintf(stderr,"\nCould not open file %s.\n\n",filename);
    exit(EXIT_FAILURE);
  }
  if (fscanf(inputfile, "%ld%*[^\n]", numprop)!=1) {
    fprintf(stderr,"\nProblem reading data in file %s.\n\n",filename);
    exit(EXIT_FAILURE);
  }
  getc(inputfile);

  *proptitle = (proptitlename *) malloc( (*numprop)*sizeof(proptitlename));
  datatmp   = (double *) malloc ( (*numprop)*sizeof(double));

  for (prop=1; prop<=*numprop; prop++) {
    if (fgets((*proptitle)[prop-1],30,inputfile)==NULL){
      fprintf(stderr,"\nProblem reading data in file %s.\n\n",filename);
      exit(EXIT_FAILURE);          
    }
    TEMP = strchr((*proptitle)[prop - 1], '\n');
    if (TEMP != NULL) *TEMP = 0;
#ifndef NDEBUG    
    printf("%s\n",(*proptitle)[prop-1]);     //puts((*proptitle)[prop-1]);
#endif
  }
  if (fscanf(inputfile, "%ld%ld%ld%ld%ld%ld%*[^\n]", is, ie, js, je, &ks1, &ke1)!=6){
    fprintf(stderr,"\nProblem reading data in file %s.\n\n",filename);
    exit(EXIT_FAILURE);    
  }
  getc(inputfile);
  
  *ks=1;
  *ke=*ks+numfile*(ke1-ks1+1)-1;
  *data1 = (double *) realloc(*data1, (*numprop)*(*ie-*is+1)*(*je-*js+1)*(*ke-*ks+1)*sizeof(double));
  propdecal=(*ke-*ks+1)*(*je-*js+1)*(*ie-*is+1);
  ii = 0+cntfile*(*ie-*is+1)*(*je-*js+1)*(ke1-ks1+1);
  for (k = *ks+cntfile*(ke1-ks1+1); k <= *ks+(cntfile+1)*(ke1-ks1+1)-1; k++) {
#ifndef NDEBUG
    printf("Reading k=%ld ..\n",k);
#endif    
    for (i = *is; i <= *ie; i++) {
      for (j = *js; j <= *je; j++) {
        ii++;
  	
        for (prop=1; prop<=*numprop-1; prop++) {
	    if (fscanf(inputfile, "%lg", &(datatmp[prop-1]))!=1) {
              fprintf(stderr,"\nProblem reading data in file %s.\n\n",filename);
              exit(EXIT_FAILURE);    	      
	    }
        }
	if (fscanf(inputfile, "%lg%*[^\n]", &(datatmp[*numprop-1]))!=1){
          fprintf(stderr,"\nProblem reading data in file %s.\n\n",filename);
          exit(EXIT_FAILURE);    	      	  
	}
        for (prop=1; prop<=*numprop; prop++) {
          (*data1)[ii-1+(prop-1)*propdecal]=datatmp[prop-1];
        }
	getc(inputfile);
      }
    }
  }
  if (inputfile != NULL)
    fclose(inputfile);
  inputfile = NULL;
  iimax = ii;
#ifndef NDEBUG
  printf("Number of Nodes in Input File = %ld \n",iimax);
#endif  
  free(datatmp);
}




void Transfo::find_node_from_pixel(typecanvas cv, long posx, long posy, 
                                 double *nodei, double *nodej)
{
  *nodei=(double)(((posx-cv.posxs-cv.decalx*cv.totpixelsx)/
           (cv.totpixelsx*cv.zoomfactx))*(cv.ie-cv.is)+cv.is);
  *nodej=(double)(((jinv(cv,posy)-cv.posys-cv.decaly*cv.totpixelsy)
          /(cv.totpixelsy*cv.zoomfacty))*(cv.je-cv.js)+cv.js);
}

//
//  find decalx,y from nodei,j and pixelx,y
//
void Transfo::find_decal_from_node_pixel (typecanvas *cv, long nodei, 
                                          long nodej, long posx, long posy)
{
  cv->decalx=(-(double)(nodei-cv->is)/(cv->ie-cv->is)*
            (cv->totpixelsx*cv->zoomfactx)+posx-cv->posxs)/(cv->totpixelsx);
  cv->decaly=(-(double)(nodej-cv->js)/(cv->je-cv->js)*
            (cv->totpixelsy*cv->zoomfacty)+jinv(*cv,posy)-cv->posys)/cv->totpixelsy;
}


void Transfo::find_pixel_from_node(typecanvas cv,
                      long i, long j, double pixelsx, double pixelsy,
                      double fictresi, double fictresj, long *posx, long *posy)
{
   double id,jd;
   id=(double)(i-cv.is)/(double)(cv.ie-cv.is)*fictresi-0.5;
   *posx=(long)((double)(cv.posxs+(id)*pixelsx)+(double)(cv.decalx*cv.totpixelsx)
              +(double)(pixelsx/2)+0.0);
   jd=(double)(j-cv.js)/(double)(cv.je-cv.js)*fictresj-0.5;
   *posy=jinv(cv,(long)((double)(cv.posys+(jd)*pixelsy)+(double)(cv.decaly*
             cv.totpixelsy)
              +(double)(pixelsy/2)+0.0));
}


#include <stdio.h>
void Transfo::find_center_node(typecanvas cv, long *i, long *j, long *k)
{
  long posx,posy;
  double nodei,nodej;
  posx=(long)((cv.posxe+cv.posxs)/2.+0.5);
  posy=(long)((cv.posye+cv.posys)/2.+0.5);
  find_node_from_pixel(cv,posx,posy,&nodei,&nodej);
  switch(cv.upaxis[0]) {
    case 'k':
      *i=(long)(nodei+0.5);
      *j=(long)(nodej+0.5);
      *k=cv.kk;
      break;
    case 'i':
      *k=(long)(nodei+0.5);
      *j=(long)(nodej+0.5);
      *i=cv.kk;
      break;
    case 'j':
      *k=(long)(nodei+0.5);
      *i=(long)(nodej+0.5);
      *j=cv.kk;
      break;
  }
//  printf("i=%ld j=%ld k=%ld\n",*i,*j,*k);
}


long Transfo::jinv(typecanvas cv, long pix1)
{
   long temp;
   temp=cv.posye-pix1+cv.posys;
   return(temp);
}




long Transfo::ai(typecanvas *cv, long i, long j, long k)
{
  long ii=0;
  if (cv->upaxis[0]=='k') ii=(k-cv->ks)*(cv->je-cv->js+1)*(cv->ie-cv->is+1)+
                          (i-cv->is)*(cv->je-cv->js+1)+(j-cv->js);
  if (cv->upaxis[0]=='j') ii=(i-cv->is)*(cv->ke-cv->ks+1)*(cv->je-cv->js+1)+
                          (j-cv->js)*(cv->ke-cv->ks+1)+(k-cv->ks);
  if (cv->upaxis[0]=='i') ii=(i-cv->is)*(cv->je-cv->js+1)*(cv->ke-cv->ks+1)+
                          (k-cv->ks)*(cv->je-cv->js+1)+(j-cv->js);
  ii=ii+cv->prop*(cv->ke-cv->ks+1)*(cv->je-cv->js+1)*(cv->ie-cv->is+1);
  return(ii);
}

#include <stdio.h>

void Transfo::zoomtrans(double *zoomfact, double *decal, double *resfact,
                       double fact)
{
    *zoomfact=(*zoomfact)*(fact);
    *decal=(*decal)*(fact)-(fact-1.)/2.;
    *resfact=*resfact*fact+0.5;      
//    printf("resfact=%ld\n",*resfact);
}



void Transfo::find_zoom_proportional(typecanvas *cv)
{
long numnodesref,numnodesi,numnodesj;
long numpixelsx,numpixelsy,numpixelsref;
double zoomfactx,zoomfacty,tmp1,tmp2;

// find numnodesref and numpixelsref

   numnodesi=cv->ie-cv->is;
   numnodesj=cv->je-cv->js;
   numpixelsx=cv->posxe-cv->posxs+1;
   numpixelsy=cv->posye-cv->posys+1;
   tmp1=(double)numpixelsx/(double)numnodesi;
   tmp2=(double)numpixelsy/(double)numnodesj;
   if (tmp1<tmp2) {
     numpixelsref=numpixelsx;
     numnodesref=numnodesi;
   } else {
     numpixelsref=numpixelsy;
     numnodesref=numnodesj;
   }


// get zoomfactx and zoomfacty from cv->zoomfactref
   
   zoomtrans(&(cv->zoomfactx), &(cv->decalx), &(cv->resfact), 
             cv->zoomfactref/cv->zoomfactx);
   zoomfactx=(double)numnodesi/(double)numnodesref
            *(double)numpixelsref/(double)numpixelsx;
   zoomtrans(&(cv->zoomfactx), &(cv->decalx), &(cv->resfact), zoomfactx);
//   printf("zoomfactx=%E  decalx=%E  factor=%E\n",cv->zoomfactx,cv->decalx,zoomfactx);

   zoomtrans(&(cv->zoomfacty), &(cv->decaly), &(cv->resfact), 
             cv->zoomfactref/cv->zoomfacty);
   zoomfacty=(double)numnodesj/(double)numnodesref
            *(double)numpixelsref/(double)numpixelsy;
   zoomtrans(&(cv->zoomfacty), &(cv->decaly), &(cv->resfact), zoomfacty);
}

