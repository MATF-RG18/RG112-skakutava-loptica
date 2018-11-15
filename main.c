#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include "loptica.h"
#include "ostrvo.h"

static void on_keyboard(unsigned char key,int x,int y);
static void on_display(void);
static void on_reshape(int width, int height);  

int main (int argc, char ** argv){
    
    glutInit(&argc, argv);
    glutInitWindowSize(700,700);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Skakutava loptica");
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    glClearColor(0.1,0.1,0.2,0);
    glEnable(GL_DEPTH_TEST);
 
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    
    glutMainLoop();
    
    return 0;
}

static void on_keyboard(unsigned char key, int x, int y){
    switch(key){
        case 27:
            exit(0);
            break;
    }
}
static void on_reshape(int width , int height){
 
    glViewport(0,0,width, height);
    glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();
    
    gluPerspective(65,(float)width/(float)height,1,100);
    glutPostRedisplay();
}

static void on_display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //GLfloat light_position[]={0.4,.3,-4,1};
    
    gluLookAt(0,2,-2,
              0,0,0,
              0,1,0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glScalef(1,1,1);
    
    //sigurno ostrvo
    draw_i();
    // loptica
    draw_b();
    
//     glFlush();
    glutSwapBuffers();
}
