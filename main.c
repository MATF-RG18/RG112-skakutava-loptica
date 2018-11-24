#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>

#include "loptica.h"
#include "ostrvo.h"
#include "ostrva.h"

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
    
    
    inicijalizacija_l();
    inicijalizacija_m_o();
    
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
    
    GLfloat light_position[]={0.5,1.8,-3,1};
    
    GLfloat light_ambient[]={0.2,0.3,0.1,1};
    GLfloat light_diffuse[]={0.5,0.6,0.8,1};
    GLfloat light_specular[]={0.6,0.8,0.9,1};
    
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    GLfloat ambient_coeffs[]={0.4,0.2,0.3,1};
    GLfloat specular_coeffs[]={0.6,0.7,0.6,1};
    GLfloat diffuse_coeffs[]={0.9,0.8,0.9,1};
    GLfloat shininess=70;
    
    glMaterialfv(GL_FRONT,GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    
    
    
    gluLookAt(0,2,-2,
              0,0,0,
              0,1,0);
  
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //glScalef(1,1,1);
    
    //sigurno ostrvo
    nacrtaj_sigurno_ostrvo();
    
    // loptica
    nacrtaj_l();
   
    //manja ostrva
     nacrtaj_manja_ostrva();
    
    glFlush();
    glutSwapBuffers();
}
