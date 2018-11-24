#include <stdio.h>
#include <GL/glut.h>

#include "ostrva.h"

float o_visina=.1;
float o_dubina=0.1;

float poz_g=.6;

Ostrva ostrva[BR_OSTRVA];

void inicijalizacija_m_o(){
    int i,j;
    for(i=0;i<BR_OSTRVA_A;i++){
        for(j=0;j<BR_OSTRVA_B;j++){
            ostrva[i*BR_OSTRVA_B+j].poz=i-1;
                ostrva[i*BR_OSTRVA_B+j].x_c=-j+2;            
        }
    }
  
}


void nacrtaj_manja_ostrva(){
    /*    inicijalizacija_li_s();*/
    int i;
//     int poz=0;
    for(i=0;i<BR_OSTRVA;i++){
        
    /*    GLfloat diffuse_coeffs[]={0.9,.9,.9,1};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
       */ 
        float x=ostrva[i].x_c;
        
        glPushMatrix();
            glTranslatef(x,o_visina/3,poz_g*ostrva[i].poz);
//         glScalef(duzina,o_visina, 1.5*o_visina);
            
            glutSolidCube(0.3);
        glPopMatrix();
        
        glutPostRedisplay();
    }    
}
