//
// Node Plot
//
// 3D Data Visualization Program for a structured mesh.
//    The visualization is performed on a node basis,
//    independantly of the spatial distances. 
//

#include <stdio.h>

void OutputHelpInputFile(){

printf(
"\n"
"===========================================================================\n"
"FORTRAN 77 INPUT FILE TEMPLATE                                             \n"
"---------------------------------------------------------------------------\n"
"     open (unit = 3, file = 'floweg.nod', status = 'unknown')              \n"
"       write (3,*) '4'                                                     \n"
"       write (3,*) 'Temperature [k]'                                       \n"
"       write (3,*) 'Pressure [Pa]'                                         \n"
"       write (3,*) 'X-Position [m]'                                        \n"
"       write (3,*) 'Y-Position [m]'                                        \n"
"       write (3,*) is,ie,js,je,ks,ke                                       \n"
"       do k=ks,ke                                                          \n"
"         do i=is,ie                                                        \n"
"           do j=js,je                                                      \n"
"             write (3,*) T(i,j,k),P(i,j,k),posx(i,j,k),posy(i,j,k)         \n"
"           enddo                                                           \n"
"         enddo                                                             \n"
"       enddo                                                               \n"
"     close(unit=3)                                                         \n"
"\n"
"===========================================================================\n"
"PASCAL INPUT FILE TEMPLATE                                                 \n"
"---------------------------------------------------------------------------\n"
"     VAR outfile:text;                                                     \n"                                    
"     rewrite(outfile,'floweg.nod');                                        \n"
"       writeln(outfile,'4');                                               \n"
"       writeln(outfile,'Temperature [K]');                                 \n"
"       writeln(outfile,'Pressure [Pa]');                                   \n"
"       writeln(outfile,'X-Position [m]');                                  \n"
"       writeln(outfile,'Y-Position [m]');                                  \n"
"       writeln(outfile,is,ie,js,je,ks,ke);                                 \n"
"       for k:=ks to ke do                                                  \n"
"         for i:=is to ie do                                                \n"
"           for j:=js to je do                                              \n"
"             writeln(outfile,T[i,j,k],P[i,j,k],posx[i,j,k],posy[i,j,k]);   \n"
"     close(outfile);                                                       \n"
"\n"
"===========================================================================\n"
"C/C++ INPUT FILE TEMPLATE                                                      \n"
"---------------------------------------------------------------------------\n"
"     FILE *outfile;                                                        \n"
"     outfile = fopen(\"floweg.nod\", \"w\");                               \n"
"       fprintf(outfile, \"4\\n\");                                         \n"
"       fprintf(outfile, \"Temperature [K]\\n\");                           \n"
"       fprintf(outfile, \"Pressure [Pa]\\n\");                             \n"
"       fprintf(outfile, \"X-Position [m]\\n\");                            \n"
"       fprintf(outfile, \"Y-Position [m]\\n\");                            \n"
"       fprintf(outfile, \"%%12ld%%12ld%%12ld%%12ld%%12ld%%12ld\\n\",       \n"
"                          is,ie,js,je,ks,ke);                              \n"
"       for (k = ks; k <= ke; k++) {                                        \n"
"         for (i = is; i <= ie; i++) {                                      \n"
"           for (j = js; j <= je; j++)                                      \n"
"             fprintf(outfile, \"%% .5E%% .5E%% .5E%% .5E\\n\",             \n"
"                     T[i][j][k], P[i][j][k], posx[i][j][k], posy[i][j][k]);\n"
"         }                                                                 \n"
"       }                                                                   \n"
"    fclose(outfile);                                                       \n"
"\n"
"===========================================================================\n"
"\n"
"Include one of the above templates in your numerical method to output your \n"
"3D data in nodplot format. Note that the file type must be ascii and make  \n"
"certain that je!=js and ie!=is.    \n"
"\n"
"At the command line, type nodplot followed by the name of the data file(s).\n"
"All help needed to operate the software can be found in the Help menu.     \n"
"\n"
);


}


#include "nplot.h"


int chkarg(int argc, char **argv, char *arg){
  int cnt,tmp;
  tmp=0;
  for (cnt=1; cnt<argc; cnt++){
   if (strcmp(argv[cnt],arg) == 0) {
     tmp=cnt;
   }
  }  
  return(tmp);
}



int main( int argc, char **argv )
{
#include "fonts.h"
    long tmp,is,ie,js,je,ks,ke,numprop,cntfile,numfile;
    double *data;
    proptitlename *proptitle;
    Transfo transfo;
    long readfile;

    readfile=0;

    if (argc==1) {
      readfile=0;
      OutputHelpInputFile();
    } else {
      numfile=argc-1;
      tmp=0;
      if (numfile>0) {
        data=(double *)malloc(sizeof(double));
	for (cntfile=0; cntfile<numfile; cntfile++){
#ifndef NDEBUG
          printf("cntfile=%ld  numfile=%ld filename=%s\n",cntfile,numfile,argv[cntfile+1]);
#endif	  
	  transfo.input_data(argv[cntfile+1], cntfile, numfile,
                             &is, &ie, &js, &je, &ks, &ke, &numprop, &proptitle, &data);
	  if (cntfile>0 && tmp!=ke-ks) {
	    fprintf(stderr,"\nData files do not share the same ke-ks.\n\n");
	    exit(EXIT_FAILURE);
	  } 
	  tmp=ke-ks;
	  
	} 
        if (numfile==1 && ke==ks){
	  numfile=2;
	  for (cntfile=0; cntfile<numfile; cntfile++){
#ifndef NDEBUG
            printf("cntfile=%ld  numfile=%ld filename=%s\n",cntfile,numfile,argv[1]);
#endif            
	    transfo.input_data(argv[1], cntfile, numfile,
                               &is, &ie, &js, &je, &ks, &ke, &numprop, &proptitle, &data);
	  }
	}
	QApplication app( argc, argv );
        //QApplication::setFont( font5);

        Nplot nplot;
        nplot.init(argv[1],TRUE,is,ie,js,je,ks,ke,numprop,proptitle,data);
        nplot.show();
        app.setMainWidget( &nplot );
        nplot.resize(700,500);
	return app.exec();
      }
    }
    
  return(0); 
}
