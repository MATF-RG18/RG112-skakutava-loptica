#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "ostrvo.h"


void nacrtaj_sigurno_ostrvo(){
    glColor3f(0,0.5,0.5);
    glTranslatef(0,-5,0);
    glutSolidCube(2);
    glTranslatef(0,5,0);
}

