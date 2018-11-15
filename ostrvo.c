#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "ostrvo.h"

void draw_i(){
    glColor3f(0,0.5,0.5);
    glTranslatef(0,-5,0);
    glutSolidCube(2);
    glTranslatef(0,5,0);
}

