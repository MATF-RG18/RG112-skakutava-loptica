#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "loptica.h"

Lopta loptica;
void inicijalizacija_l(){
    loptica.s=0.3;
    loptica.w=20;
    loptica.h=20;
}

void nacrtaj_l(){
//     inicijalizacija_l();
    glPushMatrix();    
        glColor3f(1,0,0);
        glTranslatef(0,-3,0);
        glutWireSphere(loptica.s, loptica.w, loptica.h);
    glPopMatrix();
    glutPostRedisplay();
}
