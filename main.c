	#include <stdio.h>
	#include <stdlib.h>
	#include <GL/glut.h>
	#include <time.h>
	#include <string.h>
    #include <math.h>
	#include "SOIL.h"


	#define BR_OSTRVA_A 6
    #define TIMER_ID 0
    #define TIMER_ID1 1
    #define TIMER_INTERVAL (20)
	static int animation=0;
    static int animation2=0;
//     static int animation_skok=0;
    
    //prikazuje nam koji skok zelimo
    int pomocna_animation=0;

	//lopta
	float x_c=0;
	float y_c=0;
	float vis_c=0;
	float s=0.15;
	float w=20;
	float h=20;

    //Pocetne pozicije sigurnog ostrva
    float sigurno_x=-1.5;
    float sigurno_y=-.35;
    float sigurno_z=-1.5;
    
	int pom=0;
    int pom_linija=0;
    
    //pomocna koja nam prikazuje broj trenutne linije manjeg ostrva (i)
    int pom_linija2=0;

    int pom_k=0;
    
    //pomocna koja nam prikazuje broj trenutne linije manjeg ostrva (j)
    int pom_k2=3;
    
    
    float pomeranje_kocke=0;
    int broj_milisec=0;
    int broj_sekundi=0;
    
	float x_sig=0;
	float y_sig=0;

    int matrica_ostrva[BR_OSTRVA_A][BR_OSTRVA_A];
    
    
	GLuint slika_pozadine;

    void postavi_sliku();

	void nacrtaj_l();
	void nacrtaj_sigurno_ostrvo();
	void inicijalizacija_m_o();
	void nacrtaj_manja_ostrva();


	void idi_desno();
	void idi_levo();
    void skok_desno();
                
    
	void idi_napred();
	void idi_nazad();
	void skok_uvis();
    void skok_napred();
    void skok_levo();
    void skok_nazad();
       
    void resetuj();


	static void on_keyboard(unsigned char key,int x,int y);
	static void on_display(void);
	static void on_timer(int value);
    static void on_timer2(int value);
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
	 
	    slika_pozadine=SOIL_load_OGL_texture("sea.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	    if(slika_pozadine==0){
	     printf("Nije ucitana slika\n");   
	    }
      
	    glutMainLoop();
	    
	    return 0;
	}

	static void on_keyboard(unsigned char key, int x, int y){
	    switch(key){
            case 27:
                exit(0);
                break;
             //POCETAK--->START!!!!
            case 'o':
                if(!animation2){
                    animation2=1;
                    glutTimerFunc(60,on_timer2,TIMER_ID1);
                }
                break;
            case 'p':
                if(animation2==1){
                 animation2=0;   
                }
                break;
            case 'l':
                if(animation2==1){
                    idi_desno();
                }
                break;
            case 'j':
                if(animation2==1){
                    idi_levo();
                }
                break;
            case 'i':
                if(animation2==1){
                    idi_napred();
                }
                break;
            case 'm':
                if(animation2==1){
                    idi_nazad();
                }
                break;
            case 's':
//                skok_uvis();
                if(animation2==1){
                    if(animation==0){
                        animation=1;
                        pomocna_animation=1;
                        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                    }
                }
                break;
            case 'w':
//              skok_napred();
                 if(animation2==1){
                    if(animation==0){
                        pomocna_animation=2;
                        animation=1;
                        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                    }
                 }
                
                break;
            case 'a':
//                 skok_levo();
                 if(animation2==1){
                    if(!animation){
                        pomocna_animation=3;
                        animation=1;
                        glutTimerFunc(20,on_timer,TIMER_ID);
                    }
                 }
                break;
            case 'd':
                skok_desno();
                 if(animation2==1){
                    if(!animation){
                        pomocna_animation=4;
                        animation=1;
                        glutTimerFunc(20,on_timer,TIMER_ID);
                    
                    }
                 }
                break;
            case 'x':
//                 skok_nazad();
                 if(animation2==1){
                    if(!animation){
                        pomocna_animation=5;
                        animation=1;
                        glutTimerFunc(20,on_timer,TIMER_ID);
                    }
                 }
                break;            
            case 'q':
                 if(animation2==1){
                    if(!animation){
                        pomocna_animation=6;
                        animation=1;
                        glutTimerFunc(20,on_timer,TIMER_ID);
                    }
                 }
                break;
                
        }
	    
	}
	static void on_timer2(int value){
        if(value!=TIMER_ID1)
            return;
        
        sigurno_y-=0.002;
        if(sigurno_y<=-1){
         sigurno_y=-1;   
         
        }   
         /*
	    broj_milisec+=70;
        if(broj_milisec>1000){
         broj_sekundi+=1;
         broj_milisec=0;           
        }
	    if(broj_sekundi>2){
         broj_sekundi=0;
         if(pomeranje_kocke>1.6){
            pomeranje_kocke=pomeranje_kocke-0.8;
         }
         else{
           pomeranje_kocke=pomeranje_kocke+0.8;
         } */
        glutPostRedisplay();
        if(animation2==1){
         glutTimerFunc(60,on_timer2,TIMER_ID1);   
        }
        
    }
	
	static void on_timer(int value){
	    if(value!=TIMER_ID && value!=TIMER_ID1){
            return;
	    }
	    
	    
        /*
	    broj_milisec+=70;
        if(broj_milisec>1000){
         broj_sekundi+=1;
         broj_milisec=0;           
        }
	    if(broj_sekundi>2){
         broj_sekundi=0;
         if(pomeranje_kocke>1.6){
            pomeranje_kocke=pomeranje_kocke-0.8;
         }
         else{
           pomeranje_kocke=pomeranje_kocke+0.8;
         } */
        switch(pomocna_animation){
            case 1:
                skok_uvis();
                glutPostRedisplay();
                 if(animation){
                   glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);   
                 }
                break;   
                
            case 2:
                 skok_napred();
                 glutPostRedisplay();
                 if(animation){
                   glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);   
                 }
                 break;                
            case 3:
                skok_levo();
                glutPostRedisplay();
                if(animation){
                    glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);   
                }
                break;
               
            case 4:
                skok_desno();
                glutPostRedisplay();
                if(animation){
                    glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);   
                }
                break;
            case 5:
                skok_nazad();
                glutPostRedisplay();
                if(animation==1){
                glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);   
                }
                break;
            default:
                glutPostRedisplay();
                if(animation==1){
                    glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);   
                }
        }
	}
	static void on_reshape(int width , int height){
	 
	    glViewport(0,0,width, height);
	    glMatrixMode(GL_PROJECTION);
	    
	    glLoadIdentity();
	    
	    gluPerspective(60,(float)width/(float)height,1,1000);
	    glutPostRedisplay();
	}

	static void on_display(void){
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    
	    GLfloat light_position[]={0.5,1.8,-3,1};
	    
	    GLfloat light_ambient[]={0.2,0.3,0.1,1};
	    GLfloat light_diffuse[]={0.5,0.6,0.8,1};
	    GLfloat light_specular[]={0.6,0.8,0.9,1};
	    
	    
	    glEnable(GL_LIGHTING);
	    glEnable(GL_LIGHT0);
	    
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
	    
	    
	    
	    
	    gluLookAt(-1,4,-5,
                   -2,1,1,
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
		glTranslatef(x_c-1.5,vis_c+0.2,y_c-1.5);
		glutSolidSphere(0.15,20,20);
	    glPopMatrix();
	    
	    glutPostRedisplay();
	}

	void nacrtaj_sigurno_ostrvo(){
	    
	    glPushMatrix();
            glColor3f(0,0.5,0.5);
            glTranslatef(sigurno_x,sigurno_y,sigurno_z);
//             glScalef(1,0,2   .2);
            glutSolidCube(1);
	    glPopMatrix();
	}

void nacrtaj_manja_ostrva(){
     int i,j;
     for(i=0;i<6;i++){
         for(j=0;j<6;j++){
            matrica_ostrva[i][j]=1;
//             printf("%d ", matrica_ostrva[i][j]);
         }
         
     }
     
     for(i=0;i<BR_OSTRVA_A;i++){
      for(j=0;j<BR_OSTRVA_A;j++){
          GLfloat diffuse_coeffs[]={0.7,0.7,0.1,1};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        if(matrica_ostrva[i][j]==1){
            glPushMatrix();
                 glTranslatef(-j+0.5+pomeranje_kocke,0,i-0.5);
                 glutSolidCube(0.3);
            glPopMatrix();            
        }
      }
     }
    }

	void postavi_sliku(){
	 glPushMatrix();   
       GLfloat diffuse_coeffs[]={0.1,0.1,0.8,0};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        
	    glEnable(GL_TEXTURE_2D);
        glTranslatef(-3,0,0);
	   glBegin(GL_QUADS);
		glBindTexture(GL_TEXTURE_2D,slika_pozadine);
		glNormal3f(0, 0, 1);
		    glTexCoord2f(0, 0);
		    glVertex3f(50, 0, 50);
		    glTexCoord2f(1, 0);
		    glVertex3f(-70, 0, 70);
		    glTexCoord2f(0,1);
		    glVertex3f(-70, 0, -70);
		    glTexCoord2f(1,1);
		    glVertex3f(70, 0, -70);
		
	    glEnd();

		glDisable(GL_TEXTURE_2D);

	 glPopMatrix();
	}
	void idi_desno(){
        x_c-=0.1;
	    if(pom==0){
            if(x_c<-0.3 ){ 
                resetuj();
            }
	    }
	}
	void idi_levo(){
        x_c+=0.1;   
	    if(pom==0){
            if(x_c>0.3){
                resetuj();
            }
	    }
	    
	}
	void idi_napred(){
        y_c+=0.1;
	    if(pom==0){
            if(y_c>0.3){
                resetuj();
            }
	    }
	    
	}
	void idi_nazad(){
        y_c-=0.1;
	    if(pom==0){
            if(y_c<-0.3){
                resetuj();
            }
	    }
	}
	void skok_uvis(){
	    vis_c+=.1;
//         printf("vis_c : %f\n", vis_c);
        if(vis_c>=0.5){
         vis_c=0.02;
         animation=0;
        }
        
	}
    void skok_napred(){
        pom++;
        y_c+=0.1;
        vis_c+=0.08;
        pom_linija++;
//         printf("p1 :%d\n", pom_linija);
        if(pom_linija==10){
                vis_c=0.02;
            pom_linija2++;
            pom_linija=0;
            animation=0;
            if(pom_linija2>=7){
             resetuj();   
            }
        }
     }
    void skok_levo(){

        vis_c+=0.08;
        x_c+=0.1;
        pom_k++;
        //ako pokusamo da skocimo sa sigurnog ostrva ulevo 
        if(pom==0){ 
          resetuj();   
        }
        
        if(pom_k==10){
                vis_c=0.02;
                pom_k2--;
                pom_k=0;
                animation=0;
                if(pom_k2<=0){
                    resetuj();   
                }
        }
        
    }
    void skok_desno(){
        vis_c+=0.08;
        x_c-=0.1;
        pom_k++;
        //ako pokusamo da skocimo sa sigurnog ostrva udesno 
        if(pom==0){
         resetuj();   
        }
        if(pom_k==10){
            vis_c=0.02;
            pom_k2++;
            pom_k=0;
  /*            printf("k: %d\n", pom_k);
            printf("k2: %d\n", pom_k2);
              */
            animation=0;
            if(pom_k2>=7){
              resetuj();   
            }
        }
    }
    void skok_nazad(){
        vis_c+=0.08;
        y_c-=0.1;
        pom_linija++;
        if(pom==0){
        resetuj();   
        }
        if(pom_linija==10){
            vis_c=0.02;
            pom_linija2--;
            pom_linija=0;
            animation=0;
            if(pom_linija2<=0){
            resetuj();   
            }
        }
        
    }
    
	void resetuj(){
	 x_c=0;
	 y_c=0;
     vis_c=0;
     pom=0;
     pom_k=0;
     pom_k2=3;
     pom_linija=0;
     pom_linija2=0;
 	 printf("Kraj, resetovano\n");
	}
