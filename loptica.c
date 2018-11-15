#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "loptica.h"

void draw_b(){
    float size=0.3;
    float width=20;
    float height=20;
    glPushMatrix();    
        glColor3f(1,0,0);
        glTranslatef(0,-3,0);
        glutWireSphere(size,width, height);
    glPopMatrix();
    glutPostRedisplay();
}
