#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "image.h"

#define BR_OSTRVA 6
#define TIMER_ID 0
#define TIMER_ID1 1
#define TIMER_INTERVAL (20)
#define L_TEXTURE "images.bmp"


static void initializeTexture(void);
/*Animacijski parametri za pokretanje animacija: */
static int animation=0;
static int animation2=0;
    
/* prikazuje nam koji skok zelimo */
int pomocna_animation=0;

/* interval za on_timer2 */
int timer_interval2=70;
    
/* pocetne pozicije lopte */
float lopta_x=0;
float lopta_z=0;
float lopta_y=0;
	
/* Pocetne pozicije sigurnog ostrva*/
float sigurno_x=-1.5;
float sigurno_y=-.35;
float sigurno_z=-1.5;

//promenljiva koja nam pokazuje da li smo na pocetnom ostrvu, kada skocimo sa njega, ona se uveca.
int pom=0;

int pom_duzina_skoka=0;
    
/* pomocna koja nam prikazuje broj trenutne linije manjeg ostrva (i) */
int pom_linija2=0;

/* pomocna koja nam prikazuje broj trenutne linije manjeg ostrva (j) */
int pom_k2=3;
int pomocna_j=0;
    
/* promenljiva za pomeranje manjih ostrva */
float pomeranje_kocke=0;
float pomeranje_kocke2=0;
/* promenljive za racunanje sekundi */
int broj_milisec=0;
int broj_sekundi=0;
    
//promenljiva koja nam odredjujemo znak kretanja ostrva
int znak=1;

/* matrica pomocu koje iscrtavamo manja ostrva */
int matrica_ostrva[BR_OSTRVA][BR_OSTRVA];
float matrica_ostrva_x[BR_OSTRVA][BR_OSTRVA];    
float matrica_ostrva_y[BR_OSTRVA][BR_OSTRVA];


/*promenljiva i funkcije za texture */

GLuint loptica_texture;
void texture_pod(GLuint loptica_texture);
static void initializeTexture(void);

/*Funkcije za iscrtavanje: */
void nacrtaj_l();
void nacrtaj_sigurno_ostrvo();
void nacrtaj_manja_ostrva();

/*funkcije za kretanje levo/desno/napred/nazad */
void idi_desno();
void idi_levo();
void idi_napred();
void idi_nazad();
    
/*funkcije za skokove */
void skok_desno();
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
	    
    /* inicijalizacija   */
    glutInit(&argc, argv);
	glutInitWindowSize(700,700);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Skakutava loptica");
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	    
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE); 
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    
	
    glClearColor(0.1,0.1,0.2,0);
      
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
                    glutTimerFunc(timer_interval2,on_timer2,TIMER_ID1);
                }
                break;
            //PAUZA!!!!!!!!!!!
            case 'p':
                if(animation2==1){
                 animation2=0;   
                }
                break;
            //pomeranje levo --> idi_desno
            case 'l':
                if(animation2==1){
                    idi_desno();
                }
                break;
            
            //pomeranje levo --> idi_levo
            case 'j':
                if(animation2==1){
                    idi_levo();
                }
                break;
                
            //pomeranje levo --> idi_napred
            case 'i':
                if(animation2==1){
                    idi_napred();
                }
                break;
                
            //pomeranje levo --> idi_nazad
            case 'm':
                if(animation2==1){
                    idi_nazad();
                }
                break;
                
            // skok_uvis();
            case 's':
                if(animation2==1){
                    if(animation==0){
                        animation=1;
                        pomocna_animation=1;
                        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                    }
                }
                break;
        
            //skok_napred();
            case 'w':
                 if(animation2==1){
                    if(animation==0){
                        pomocna_animation=2;
                        animation=1;
                        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                    }
                 }
                
                break;
            //skok_levo();
            case 'a':
                 if(animation2==1){
                    if(!animation){
                        pomocna_animation=3;
                        animation=1;
                        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                    }
                 }
                break;
            //skok_desno();
            case 'd':
                 if(animation2==1){
                    if(!animation){
                        pomocna_animation=4;
                        animation=1;
                        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                    
                    }
                 }
                break;
            //skok_nazad();
            case 'x':
                 if(animation2==1){
                    if(!animation){
                        pomocna_animation=5;
                        animation=1;
                        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                    }
                 }
                break;  
                
        }
	    
}
static void on_timer2(int value){
        if(value!=TIMER_ID1)
            return;
        
        sigurno_y-=0.002;
        /*ukoliko loptica nije skocila na vreme, gubi zivot.  */
        if(pom==0){
            if(sigurno_y<=-.505){
              printf("Potonula, nije skocila na vreme.");
             resetuj();
            }
        }
        if(sigurno_y<=-1){
         sigurno_y=-10;   
         
        }   
        /*Na svakih 10 sekundi smanjujemo timer_interval2, tj.interval za pomeranje manjih ostrva,
        kako bi dobili efekat da se brze krecu. */
	    broj_milisec+=70;
        if(broj_milisec>1000){
         broj_sekundi+=1;
         broj_milisec=0;           
        }
        //ako je proslo 10 sekundi smanjujemo timer_interval2 za 1 i ponovo pocinje brojanje sekundi
	    if(broj_sekundi>10){
         broj_sekundi=0;
         timer_interval2-=1;         
        }
//         printf("Broj sekundi : %d\n",broj_sekundi);
        
        pomeranje_kocke+=0.01;
        if(pomeranje_kocke>=1){
            pomeranje_kocke=1;
            pomeranje_kocke2+=0.01;
            if(pomeranje_kocke2>=1){
                pomeranje_kocke=0;
                 pomeranje_kocke2=0;
            }
            znak=znak*(-1);
        }
        
        glutPostRedisplay();
        if(animation2==1){
         glutTimerFunc(timer_interval2,on_timer2,TIMER_ID1);   
        }
        
    }
	
static void on_timer(int value){
	    if(value!=TIMER_ID){
            return;
	    }
	    //Zavisno od slova koje smo pritisnuli je pomocna_animation postavljena na odredjenu vrednost. Pomocu nje mozemo izvrsiti zeljenu funkciju.
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
/*funkcija za postavljanje glViewport-a i perspektive */
static void on_reshape(int width , int height){
	 
	    glViewport(0,0,width, height);
	    glMatrixMode(GL_PROJECTION);
	    
	    glLoadIdentity();
	    
	    gluPerspective(60,(float)width/(float)height,1,1000);
	    glutPostRedisplay();
}


static void on_display(void){
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        /*Koeficijenti za osvetljenje: pozicija svetla, ambientalno, difuzno i spekularno */
	    GLfloat light_position[]={0.5,1.8,-3,1};
	    GLfloat light_ambient[]={0.2,0.3,0.1,1};
	    GLfloat light_diffuse[]={0.5,0.6,0.8,1};
	    GLfloat light_specular[]={0.6,0.8,0.9,1};
	    
	    /*Ukljucivanje svetla: */
	    glEnable(GL_LIGHTING);
	    glEnable(GL_LIGHT0);
        
	    /*Postavljanje svetla: */
	    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	    
        /*Koeficijenti za materijale: */
	    GLfloat ambient_coeffs[]={0.4,0.2,0.3,1};
	    GLfloat specular_coeffs[]={0.6,0.7,0.6,1};
	    GLfloat diffuse_coeffs[]={0.9,0.8,0.9,1};
	    GLfloat shininess=70;
	    
        /*Postavljanje materijala: */
	    glMaterialfv(GL_FRONT,GL_AMBIENT, ambient_coeffs);
	    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
	    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	    
	    /*Postavljanje kamere: */
	    gluLookAt(-1,4,-5,
                   -2,1,1,
                   0,1,0);
	  
	    
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	  
        //inicijalizacija teksture
          initializeTexture();

        /* pod  za teksture*/
        glPushMatrix();
              texture_pod(loptica_texture);   	
        glPopMatrix();	
	
        
	    /*Funkcija za crtanje veceg ostrva. */
	    nacrtaj_sigurno_ostrvo();
	    
	    /*Funkcija za crtanje lopte*/
	    nacrtaj_l();
	   
	    /*Funkcija za crtanje manjeg ostrva: */
 	    nacrtaj_manja_ostrva();
            
	    glutSwapBuffers();
	}
	
/*Funkcija za crtanje lopte. */
void nacrtaj_l(){
		
	    glPushMatrix();    
            /*Postavljanje difuznog materijala crvene boje*/
            GLfloat  diffuse_coeffs[]={0.9,0.1,0.1,1};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
            
            glTranslatef(lopta_x-1.5,lopta_y+0.2,lopta_z-1.5);
            glutSolidSphere(0.15,20,20);
	    glPopMatrix();
	    
	}

/* Funkcija za crtanje veceg ostrva. */
void nacrtaj_sigurno_ostrvo(){
	    
	    glPushMatrix();
            GLfloat  diffuse_coeffs[]={0.2,0.2,0.9,1};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);      
            glTranslatef(sigurno_x,sigurno_y,sigurno_z);
            glutSolidCube(1);
	    glPopMatrix();
	}

void nacrtaj_manja_ostrva(){
     int i,j;
     /*Popunjavamo matrica 6x6 jedinicama, kako bi mogli lepo da postavimo ostrva: */
     for(i=0;i<6;i++){
         for(j=0;j<6;j++){
            matrica_ostrva[i][j]=1;
         }
     }
     for(i=0;i<BR_OSTRVA;i++){
      for(j=0;j<BR_OSTRVA;j++){
          /*Postavljamo difuzni materijal za ostrva: */
            GLfloat diffuse_coeffs[]={0.4,0.4,0.2,1};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
            if(matrica_ostrva[i][j]==1){
            /*Na mestima gde smo postavili jedinice u matrici postavljamo ostrva. */
                glPushMatrix();
                /*Svaki drugi red ide u istu stranu: */
                    if(i%2==1){
                        /*cuvamo x i z koordinate */
                        matrica_ostrva_x[i][j]=-j+0.5+pomeranje_kocke-pomeranje_kocke2;
                        matrica_ostrva_y[i][j]=i-0.5;
                        glTranslatef(-j+0.5+pomeranje_kocke-pomeranje_kocke2,0,i-0.5);
                    }
                    else{
                        matrica_ostrva_x[i][j]=-j+0.5-pomeranje_kocke+pomeranje_kocke2;
                        matrica_ostrva_y[i][j]=i-0.5;
                        glTranslatef(-j+0.5-pomeranje_kocke+pomeranje_kocke2,0,i-0.5);
                    }
                    glutSolidCube(0.3);
                glPopMatrix();            
            }
      }
     }
}

void idi_desno(){
        lopta_x-=0.1;
	    if(pom==0){
            
            printf("%.2f\n",sigurno_y);
            if(sigurno_y<=-.49){
              printf("Potonuo");
              resetuj();
            }             
            if(lopta_x<=-.6){ 
                resetuj();
            }
	    }
	}
void idi_levo(){
        lopta_x+=0.1;   
	    if(pom==0){
            
            printf("%.2f\n",sigurno_y);
            if(sigurno_y<=-.49){
              printf("Potonuo");
             resetuj();
            }            
            if(lopta_x>=0.6){
                resetuj();
            }
	    }
}
void idi_napred(){
        lopta_z+=0.1;
	    if(pom==0){
            if(sigurno_y<=-.49){
              printf("Potonuo");
             resetuj();
            }
            if(lopta_z>=0.6){
                resetuj();
            }
	    }
	    
	}
void idi_nazad(){
        lopta_z-=0.1;
	    if(pom==0){
            
            printf("%.2f\n",sigurno_y);
            if(sigurno_y<=-.49){
              printf("Potonuo");
             resetuj();
            }
            if(lopta_z<-0.6){
                resetuj();
            }
	    }
	}
void skok_uvis(){
	    lopta_y+=.1;
//         printf("lopta_y : %f\n", lopta_y);
        if(lopta_y>=0.5){
         lopta_y=0.02;
         animation=0;
        }
        
	}
void skok_napred(){
        pom++;
        lopta_z+=0.1;
        lopta_y+=0.08;
        //pomocna koja nam pomaza da odredimo duzinu skoka, kada postane 10 treba da se prekine animacija skoka
        pom_duzina_skoka++;
        if(pom_duzina_skoka==10){
                lopta_y=0.02;
            //promenljiva koja nam pokazuje na kom smo i po redu od ostrva, broji od 1 i povecava se za jedan svaki put pri skoku napred 
            pom_linija2++;
            pom_duzina_skoka=0;
            //iskljucujemo animaciju za skok;
            animation=0;
            if(pom_linija2>=7){
             resetuj();   
            }
            int j; 
            int alive1=0;
            int alive=0;
//               printf("%d. \n", pom_linija2);   
            if(pom_linija2%2==1){
                for(j=0;j<BR_OSTRVA;j++){

                    if((matrica_ostrva_x[pom_linija2-1][j]-0.15-(lopta_x-1.5)>=-0.15 && matrica_ostrva_x[pom_linija2-1][j]-0.15-(lopta_x-1.5)<=0) || 
                        (matrica_ostrva_x[pom_linija2-1][j]+.15-(lopta_x-1.5)>=0 && matrica_ostrva_x[pom_linija2-1][j]+.15-(lopta_x-1.5)<=0.15)){
                        pomocna_j=j;
                        alive1++;   
                   }
                }
                if(alive1==0){
//                    printf("reset alive1\n");   
                  resetuj();
                }
                alive1=0;
            }
            else{
                for (j=0;j<BR_OSTRVA;j++){
         if((matrica_ostrva_x[pom_linija2-1][j]-0.15-(lopta_x-1.5)>=-0.15 && matrica_ostrva_x[pom_linija2-1][j]-0.15-(lopta_x-1.5)<=0) |(matrica_ostrva_x[pom_linija2-1][j]+.15-(lopta_x-1.5)>=0 && matrica_ostrva_x[pom_linija2-1][j]+.15-(lopta_x-1.5)<=0.15)){
                      pomocna_j=j;
                     alive++;   
                    }
                }
                if(alive==0){
                    resetuj();
                }
                alive=0;
            }
//             printf("pom_j %d \n", pomocna_j);
        }
}
void skok_levo(){
        lopta_y+=0.08;
        lopta_x+=0.1;
        pom_duzina_skoka++;
        //ako pokusamo da skocimo sa sigurnog ostrva ulevo 
        if(pom==0){ 
          resetuj();   
        }
        
        if(pom_duzina_skoka==10){
                lopta_y=0.02;
                pom_k2--;
                pom_duzina_skoka=0;
                animation=0;
                if(pom_k2<=0){
                    resetuj();   
                }
                            int ziv=0;;
            printf("pomocna_j levo: %d\n", pomocna_j-1);
            if(pomocna_j<=0){
             resetuj();   
            }
            else{
            if(pom_linija2%2==1){
                    pomocna_j=pomocna_j-1;
                if((matrica_ostrva_x[pom_linija2-1][pomocna_j]+znak*(lopta_x))>=-.5 /*&& (matrica_ostrva_x[pom_linija2-1][pomocna_j]- pomeranje_kocke*znak-(lopta_x))<=.4*/){
                    ziv=1;
                    printf("zivo");   
                }
                    printf("matr x: %.3f\n", matrica_ostrva_x[pom_linija2-1][pomocna_j]);
                    printf("matr l: %.3f\n", lopta_x);
                if(ziv==0){
                 printf("restart\n");   
                }
            }
            else {
                    pomocna_j=pomocna_j-1;
                if((matrica_ostrva_x[pom_linija2-1][pomocna_j]-znak*(lopta_x))<=.5 /*&& (matrica_ostrva_x[pom_linija2-1][pomocna_j]- pomeranje_kocke*znak-(lopta_x))<=.4*/){
                    ziv=1;
                    printf("zivo");   
                }
                    printf("matr x: %.3f\n", matrica_ostrva_x[pom_linija2-1][pomocna_j]);
                    printf("matr l: %.3f\n", lopta_x);
                if(ziv==0){
                     resetuj();
                 printf("restart\n");   
                }
                
            }
            }
            printf("ziv : %d\n" ,ziv);

                
        }
        
    }
void skok_desno(){
        lopta_y+=0.08;
        lopta_x-=0.1;
        int i,j;
        /* Pomocna promenljiva pomocu koje odredjujemo duzinu skoka, kada postane 10, prekidamo animaciju skoka */
        pom_duzina_skoka++;
        /*ako pokusamo da skocimo sa sigurnog ostrva udesno  */
        if(pom==0){
         resetuj();   
        }
        if(pom_duzina_skoka==10){
            /* Postavljamo y koordinatu lopte na 0.02*/
            lopta_y=0.02;
            pom_k2++;
            pom_duzina_skoka=0;
            animation=0;
            if(pom_k2>=7){
              resetuj();   
            }
            int ziv=0;;
            if(pom_linija2%2==1){
                if((matrica_ostrva_x[pom_linija2-1][pomocna_j]-znak*(lopta_x))>=-.5 /*&& (matrica_ostrva_x[pom_linija2-1][pomocna_j]- pomeranje_kocke*znak-(lopta_x))<=.4*/){
                    ziv=1;
                    printf("zivo");   
                    pomocna_j++;
                }
                    printf("matr x: %.3f\n", matrica_ostrva_x[pom_linija2-1][pomocna_j]);
                    printf("matr l: %.3f\n", lopta_x);
                if(ziv==0){
                 printf("restart\n");   
                }
            }
            else {
                if((matrica_ostrva_x[pom_linija2-1][pomocna_j]+znak*(lopta_x))<=.5 /*&& (matrica_ostrva_x[pom_linija2-1][pomocna_j]- pomeranje_kocke*znak-(lopta_x))<=.4*/){
                    ziv=1;
                    printf("zivo");   
                    pomocna_j++;
                }
                    printf("matr x: %.3f\n", matrica_ostrva_x[pom_linija2-1][pomocna_j]);
                    printf("matr l: %.3f\n", lopta_x);
                if(ziv==0){
                    resetuj();
                 printf("restart\n");   
                }
                
            }
            printf("ziv : %d\n" ,ziv);
        }
    }
    
void skok_nazad(){
        lopta_y+=0.08;
        lopta_z-=0.1;
        /* Pomocna promenljiva pomocu koje odredjujemo duzinu skoka, kada postane 10, prekidamo animaciju skoka */
        pom_duzina_skoka++;
        if(pom==0){
            resetuj();   
        }
        if(pom_duzina_skoka==10){
            lopta_y=0.02;
            //pomocna koja nam pomaze da odredimo na kom smo i od pocetnog- velikog ostrva, kada se vracamo unazad kad pocetnoj poziciji, ona se smanjuje za 1
            pom_linija2--;
            pom_duzina_skoka=0;
            animation=0;
            if(pom_linija2<=0){
                resetuj();   
            }
             int j; 
            int alive1=0;
            int alive=0;
//               printf("%d. \n", pom_linija2);   
            if(pom_linija2%2==1){
                for(j=0;j<BR_OSTRVA;j++){
                    if((matrica_ostrva_x[pom_linija2-1][j]-0.15-(lopta_x-1.5)>=-0.15 && matrica_ostrva_x[pom_linija2-1][j]-0.15-(lopta_x-1.5)<=0) || (matrica_ostrva_x[pom_linija2-1][j]+.15-(lopta_x-1.5)>=0 && matrica_ostrva_x[pom_linija2-1][j]+.15-(lopta_x-1.5)<=0.15)){
                     alive1++;   
                     pomocna_j=j;

                 }
                }
                if(alive1==0){
                    resetuj();
//                  printf(" alive1 reset\n");   
                }
                alive1=0;
            }
             else{
                for (j=0;j<5;j++){
                  if((matrica_ostrva_x[pom_linija2-1][j]-0.15-(lopta_x-1.5)>=-0.15 && matrica_ostrva_x[pom_linija2-1][j]-0.15-(lopta_x-1.5)<=0) || (matrica_ostrva_x[pom_linija2-1][j]+.15-(lopta_x-1.5)>=0 && matrica_ostrva_x[pom_linija2-1][j]+.15-(lopta_x-1.5)<=0.15)){
                     alive++;   
                     pomocna_j=j;
                    }
                }
                if(alive==0){
                    resetuj();
                }
                alive=0;
            }
        }
}
    
void resetuj(){
     lopta_x=0;
	 lopta_z=0;
     lopta_y=0;
     pom=0;
     sigurno_x=-1.5;
     sigurno_y=-.35;
     sigurno_z=-1.5;
     pom_duzina_skoka=0;
     pom_k2=3;
     pom_duzina_skoka=0;
     pom_linija2=0;
     pomocna_j=0;
     animation=0;
     animation2=0;
     pomeranje_kocke=0;
 	 printf("\nKraj, resetovano\n");
}

/*funkcija za inicijalizaciju teksture: */
void initializeTexture(void)
{
	/*kod sa casa*/
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	/* Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz fajla */
    Image *image = image_init(0, 0);
    /* Kreira se tekstura */
    image_read(image, L_TEXTURE);

    /* Generisu se identifikatori teksture i inicijalizuje tekstura*/
    glGenTextures(1, &loptica_texture);

    glBindTexture(GL_TEXTURE_2D, loptica_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);
}

void texture_pod(GLuint loptica_texture){
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, loptica_texture);
	
	//funkcija koja crta pod od okeana
	glPushMatrix();
		glBegin(GL_QUADS);
            glNormal3f(0, 1,0);
			glTexCoord2f(0, 0);	
			glVertex3f(-20, 0, -5); 
			glTexCoord2f(60, 0);
			glVertex3f(10, 0, -5); 
			glTexCoord2f(60,60);
			glVertex3f(30, 0, 22); 
			glTexCoord2f(0, 70);
			glVertex3f(-40, 0, 19);
		glEnd();
	
		glBindTexture(GL_TEXTURE_2D, 0);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPopMatrix();
}
