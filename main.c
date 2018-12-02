	#include <stdio.h>
	#include <stdlib.h>
	#include <GL/glut.h>
	#include <time.h>
	#include <string.h>
	#include "SOIL.h"


	#define BR_OSTRVA (36)
	#define BR_OSTRVA_A 6
	#define BR_OSTRVA_B 6

	int matrica[10][20];

	static int animation=0;

	int lvl=1;

	//lopta
	float x_c=0;
	float y_c=0;
	float vis_c=0;
	float s=0.15;
	float w=20;
	float h=20;

	int pom=0;

	float x_sig=0;
	float y_sig=0;

	//ostrva
	typedef struct{
	    float x_c;
	    float d;
	    int poz;
	}Ostrva;

	float o_visina=.1;
	float o_dubina=0.1;
	float animation_f=0;
	// float time_active;
	float t=0;
	float poz_g=.6;
	Ostrva ostrva[25];

	GLuint slika_pozadine;
	void ucitaj();

	void nacrtaj_l();
	void nacrtaj_sigurno_ostrvo();
	void inicijalizacija_m_o();
	void nacrtaj_manja_ostrva();


	void idi_desno();
	void idi_levo();

	void idi_napred();
	void idi_nazad();
	void skoci();

	void resetuj();

	void postavi_sliku();

	static void on_keyboard(unsigned char key,int x,int y);
	static void on_display(void);
	static void on_timer(int value);
	static void on_reshape(int width, int height);  

	int main (int argc, char ** argv){
	    
	   
	    ucitaj();
	    
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
	    
	    slika_pozadine=SOIL_load_OGL_texture("sea.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	    if(slika_pozadine==0){
	     printf("Nije ucitana slika\n");   
	    }
	   
	   /*int i,j; 
	   for(i=0;i<10;i++){
	    for(j=0;j<20;j++){
	    printf("%d ", matrica[i][j]);   
	   }    
	      printf("\n");
	  }
	   */ 
	    inicijalizacija_m_o();

	    glutMainLoop();
	    
	    return 0;
	}

	static void on_keyboard(unsigned char key, int x, int y){
	    switch(key){
		case 27:
		    exit(0);
		    break;
		case 'l':
		    idi_desno();
		    break;
		case 'j':
		    idi_levo();
		    break;
		case 'i':
		    idi_napred();
		    break;
		case 'm':
		    idi_nazad();
		    break;
		case 's':
		    skoci();
		    break;
	    }
	    
	}
	static void on_timer(int value){
	    if(value!=0){
		return;
	    }
	    glutPostRedisplay();
	    if(animation){
	     glutTimerFunc(10,on_timer,0);   
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
	    
	    
	    
	    
	    gluLookAt(-2,3,-2.3,
		      -.5,1,1,
		      0,1,0);
	  
	    
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    
	    //glScalef(1,1,1);
        //slika pozadine
        postavi_sliku();
	    //sigurno ostrvo
	    nacrtaj_sigurno_ostrvo();
	    
	    // loptica
	    nacrtaj_l();
	   
	    //manja ostrva
	    nacrtaj_manja_ostrva();
	    
	    glFlush();
	    glutSwapBuffers();
	}


	void nacrtaj_l(){
		
	    glPushMatrix();    
		glColor3f(1,0,0);
		GLfloat  diffuse_coeffs[]={0.9,0.1,0.1,1};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glTranslatef(x_c+0.15,vis_c+0.2,y_c-1);
		glutSolidSphere(s, w,h);
	    glPopMatrix();
	    
	    glutPostRedisplay();
	}

	void nacrtaj_sigurno_ostrvo(){
	    
	    glPushMatrix();
		glColor3f(0,0.5,0.5);
		glTranslatef(0.15,-0.1,-1);
		glutSolidCube(0.35);
		glScalef(5,1,1);
	//        glTranslatef(0,0.45,0);
	    glPopMatrix();
	}

	void ucitaj(){  
	}

	void inicijalizacija_m_o(){
	    int i,j;
	    for(i=0;i<BR_OSTRVA_A;i++){
		for(j=0;j<BR_OSTRVA_B;j++){
		    ostrva[i*BR_OSTRVA_B+j].poz=i;
		        ostrva[i*BR_OSTRVA_B+j].x_c=j;            
		}
	    }
	  
	}
	void nacrtaj_manja_ostrva(){
	    int i;
	//     int poz=0;
	  /*  
		if(!time_active){
		    time_active=1;
		    glutTimerFunc(50,on_timer,0);
		}*/
	    for(i=0;i<BR_OSTRVA;i++){
		GLfloat diffuse_coeffs[]={0.7,0.7,0.1,1};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
	     
		float x=ostrva[i].x_c;
// 		float d=ostrva[i].d;
		glPushMatrix();
		    glTranslatef(-x/2+1.1,0,poz_g*ostrva[i].poz-0.5);
		    glutSolidCube(0.3);
		glPopMatrix();        
		glutPostRedisplay();
	    }    
	}

	void postavi_sliku(){
	 glPushMatrix();   
	    glEnable(GL_TEXTURE_2D);
		glRotatef(91, 1, 0, 0); 
	   glBegin(GL_QUADS);
		glBindTexture(GL_TEXTURE_2D,slika_pozadine);
		glNormal3f(0, 1, 0);
		    glTexCoord2f(0, 0);
		    glVertex3f(-5, -17, 0);
		    glTexCoord2f(1, 0);
		    glVertex3f(30, -20, 0);
		    glTexCoord2f(1, 1);
		    glVertex3f(30, 20, 0);
		    glTexCoord2f(0, 1);
		    glVertex3f(-5, 15, 0);
		
	    glEnd();

		glDisable(GL_TEXTURE_2D);

	 glPopMatrix();
	}
	void idi_desno(){
	    if(pom==0){
		x_c-=0.1;
		if(x_c<-0.3 ){ 
		    resetuj();
		}
	    }
	}
	void idi_levo(){
	    if(pom==0){
		x_c+=0.1;   
		if(x_c>0.3){
		    resetuj();
	    }
	    }
	}
	void idi_napred(){
	    if(pom==0){
		y_c+=0.1;
		if(y_c>0.3){
		    resetuj();
		}
	    }
	}
	void idi_nazad(){
	    if(pom==0){
		y_c-=0.1;
		if(y_c<-0.3){
		    resetuj();
		}
	    }
	}
	void skoci(){
	    vis_c+=.1;
	}
	void resetuj(){
	 x_c=0;
	 y_c=0;
// 	 printf("Kraj\n");
	}
