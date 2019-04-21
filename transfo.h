//
// Transformation subroutines header file
//

#ifndef TRANSFO_H
#define TRANSFO_H

typedef char proptitlename[30];

typedef struct typecanvas {
     long posxs,posxe,posys,posye;
     long totpixelsx,totpixelsy,shownode,scalelevels;
     double resfact,decalx,decaly,zoomfactx,zoomfacty,zoomfactref;
     long is,ie,ks,ke,js,je,kk;
     char upaxis[4];
     bool proportional;
     long iso,ieo,jso,jeo,kso,keo;
     long kposxs,kposxe,kposys,kposye;
     long sposxs,sposxe,sposys,sposye;
     double datamin,datamax;
     double *scandatamin, *scandatamax;
     double *data;
     proptitlename *proptitle;
     long numprop,prop;
     bool instanthelp;
     long colortype;     
     char *inputfilename;
} typecanvas;




class Transfo 
{
   
public:

    void input_data(char *filename,
                       long cntfile,
		       long numfile,
                       long *is, long *ie, 
                       long *js, long *je,
                       long *ks, long *ke,
                       long *numprop,
                       proptitlename **proptitle,
                       double **data1);

    void            find_pixel_from_node(typecanvas cv, long i, long j, 
                                         double pixelsx, double pixelsy,
                      double fictresi, double fictresj, long *posx, long *posy);
    void            find_node_from_pixel(typecanvas cv, long posx, long posy, double *nodei, double *nodej);
    void            find_decal_from_node_pixel (typecanvas *cv, long nodei, long nodej, long posx, long posy);
    void            find_center_node(typecanvas cv, long *i, long *j, long *k);
    long            jinv(typecanvas cv, long pix1);
    long            ai(typecanvas *cv, long i, long j, long k);
    void            zoomtrans(double *zoomfact, double *decalx, double *resfact,
                       double fact);
    void            find_zoom_proportional(typecanvas *cv);


};


#endif // TRANSFO_H
