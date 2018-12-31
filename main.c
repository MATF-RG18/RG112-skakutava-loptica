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


/*Animacijski parametri za pokretanje animacija: */
static int animation=0;
static int animation2=0;


/* promenljiva koja nam pokazuje da li treba da prekinemo i resetujemo igru.*/
static int resetovanje=0;

/* prikazuje nam koji skok zelimo u on_keyboard()  */
int pomocna_animation=0;

/* interval za on_timer2 */
int timer_interval2=50;
    
/* pocetne pozicije lopte */
static float lopta_x=0;
static float lopta_z=0;
static float lopta_y=0;

/*promenljive za ispitivanje da li je loptica ziva. */	
static int alive=0;
static int alive2=0;

/* Pocetne pozicije sigurnog ostrva*/
float sigurno_x=-1.5;
float sigurno_y=-.35;
float sigurno_z=-1.5;

//promenljiva koja nam pokazuje da li smo na pocetnom ostrvu, kada skocimo sa njega, ona se uveca.
static int pomocna_velikoOst=0;
    
/*promenljiva pomocu koje odredjujemo duzinu skoka. */
int pom_duzina_skoka=0;

/* pomocna koja nam prikazuje broj trenutne linije manjeg ostrva (i) */
int pomocna_i=0;

/* pomocna koja nam prikazuje broj trenutne linije manjeg ostrva (j), kada pronajemo poziciju j za ostrvo gde je skocila loptica dodeljujemo je pomocnoj_j */
int pomocna_j=0;
    
/* promenljiva za pomeranje manjih ostrva */
float pomeranje_kocke=0;  
float pomeranje_kocke2=0; //za vracanje unazad.

/*promenljiva koja nam potapa manje ostrvo(transliramo y koordinatu ostva postepeno), ukoliko je loptica dugo na njemu.*/
float potapanje_manjeg_ostrva=0;

/* promenljive za racunanje sekundi */
int broj_milisec=0;
int broj_sekundi=0;

/* promenljive za racunanje osvojenih poena i broj nivo(na osnovu poena). */
float brojac_poena=0;
int broj_nivoa=1;

/* matrica pomocu koje iscrtavamo manja ostrva */
int matrica_ostrva[BR_OSTRVA][BR_OSTRVA];

/* matrice pomocu kojih cuvamo x i z koordinate*/
float matrica_ostrva_x[BR_OSTRVA][BR_OSTRVA];    
float matrica_ostrva_z[BR_OSTRVA][BR_OSTRVA];

/*promenljiva i funkcije za texture */
GLuint loptica_texture;
static void initializeTexture(void);
void texture_pod(GLuint loptica_texture);

/* za ispis teksta na ekranu. */
char tekst_poeni[15];
void poeni_tekst(const char* tekst_poeni) ;

/*Funkcije za iscrtavanje: */

//funckija za crtanje lopte
void nacrtaj_l();
//funkcija za crtanje veceg ostrva
void nacrtaj_sigurno_ostrvo();
//funkcija za crtanje manjih ostrva
void nacrtaj_manja_ostrva();

/*funkcije za kretanje levo/desno/napred/nazad na vecem ostrvu*/
void idi_desno();
void idi_levo();
void idi_napred();
void idi_nazad();
    
/*funkcije za skokove */
void skok_desno();
// void skok_uvis();
void skok_napred();
void skok_levo();
void skok_nazad();

/*funckije pomocu kojih proveravamo da li je loptica dobro skocila.*/
int proveri_skok_napred_nazad(int pomocna_i);
int proveri_skok_levo_desno(int pomocna_i, int pomocna_j);
       
/*funkcija za resetovanje. */
void resetuj(float lopta_y);

int odredjivanje_nivoa(float brojac_poena);
    
static void on_keyboard(unsigned char key,int x,int y);
static void on_display(void);
static void on_timer(int value);
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

    /*inicijalizacija teksture */
    initializeTexture();    
    
	
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
                    glutTimerFunc(timer_interval2,on_timer,TIMER_ID1);
                }
                break;
            //PAUZA!!!!!!!!!!!
            case 'p':
                if(animation2==1){
                 animation2=0;   
                }
                break;
            //kretanje udesno na velikom ostrvu --> idi_desno
            case 'l':
                if(animation2==1){
                    idi_desno();
                }
                break;
            
            //kretanje ulevo na velikom ostrvu --> idi_levo
            case 'j':
                if(animation2==1){
                    idi_levo();
                }
                break;
            /* skok_napred */
            case 'w':
                potapanje_manjeg_ostrva=0;
                 alive=0;
                 if(animation2==1){
                    if(animation==0){
                        pomocna_animation=2;
                        animation=1;
                        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                    }
                 }
                
                break;
            /* skok_levo */
            case 'a':
                potapanje_manjeg_ostrva=0;
                 alive2=0;
                 if(animation2==1){
                    if(!animation){
                        pomocna_animation=3;
                        animation=1;
                        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                    }
                 }
                break;
            /* skok_desno */
            case 'd':
                potapanje_manjeg_ostrva=0;
                 alive2=0;
                 if(animation2==1){
                    if(!animation){
                        pomocna_animation=4;
                        animation=1;
                        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                    
                    }
                 }
                break;
            /* skok_nazad */
            case 'x':
                alive=0;
                potapanje_manjeg_ostrva=0;
                 if(animation2==1){
                    if(!animation){
                        pomocna_animation=1;   
                        animation=1;
                        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                    }
                 }
                break;  
                
        }
	    
}
static void on_timer(int value){
 	    if(value!=TIMER_ID && value!=TIMER_ID1){
             return;
 	    }
	    if(value==1){
            sigurno_y-=0.002;
            /*ukoliko loptica nije skocila na vreme, gubi zivot.  */
            if(pomocna_velikoOst==0){
                if(sigurno_y<=-.505){
                   printf("Loptica je potonula, nije skocila na vreme sa velikog ostrva.\n");
                   resetuj(lopta_y);
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
            
            //ako je proslo 10 sekundi smanjujemo timer_interval2 za 2 i ponovo pocinje brojanje sekundi
            if(broj_sekundi>10){
                broj_sekundi=0;
                timer_interval2-=2;
                if(timer_interval2==30){
                    timer_interval2+=30;   
                }
            }
                
            /*parametri za pomeranje manjih ostrva. Kada pomeranje_kocke dostigne vrednost >=1 potrebno je ostrva vratiti na suprotnu stranu.*/
            pomeranje_kocke+=0.01;
            if(pomeranje_kocke>=1){
                pomeranje_kocke=1;
                /*pomeranje_kocke2 za  vracanje */
                pomeranje_kocke2+=0.01;
                if(pomeranje_kocke2>=1){
                    pomeranje_kocke=0;
                    pomeranje_kocke2=0;
                }
            }
            /*ukoliko je loptica prezivela skok, potrebno je da manje ostrvo krene da tone, pa ukoliko loptica nije stigla na vreme da skoci gubi se zivot. */
            if(alive2==1){
                /*postepeno povecamo parametar za potapanje ostrva */
                potapanje_manjeg_ostrva+=0.002;
                lopta_x=1.5+matrica_ostrva_x[pomocna_i-1][pomocna_j]; 
                if(potapanje_manjeg_ostrva>=0.12){
                    printf("Loptica je potonula, nije skocila na vreme sa ostrva.\n");
                    potapanje_manjeg_ostrva=1;   
                    resetuj(lopta_y);
                    
                }
            }
            /*ako je potrebno resetovati, prvo omogucavamo da nam se lopta_y spusti, kako bi imali stvarni utisak pada. */
            if(resetovanje==1){
             lopta_y-=0.01;
             if(lopta_y<=0){
                 lopta_y=0;
                 resetuj(lopta_y);   
             }
            }
            
            glutPostRedisplay();
            if(animation2==1){
                glutTimerFunc(timer_interval2,on_timer,TIMER_ID1);   
            }   
            
        }
        /*za kretanje levo/desno/napred/nazad na dugme.*/
	    if(value==0){
            //Zavisno od slova koje smo pritisnuli je pomocna_animation postavljena na odredjenu vrednost. Pomocu nje mozemo izvrsiti zeljenu funkciju.
            switch(pomocna_animation){
                case 1:
                    skok_nazad();
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
               default:
                    glutPostRedisplay();
                    if(animation==1){
                        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);   
                    }
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
	    GLfloat light_position[]={-2,5,-7,1};
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
 	    
        //ispisujemo poene na ekran.
        sprintf(tekst_poeni, "Poeni: %.f", brojac_poena);
        poeni_tekst(tekst_poeni);
    
        
	    glutSwapBuffers();
	}
	
/*Funkcija za crtanje lopte. */
void nacrtaj_l(){
		
	    glPushMatrix();    
            /*Postavljanje difuznog materijala crvene boje*/
            GLfloat  diffuse_coeffs[]={0.9,0.1,0.1,1};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
            /*postavimo sencenje lopte, kako bi lepse izgledala. */
            glShadeModel(GL_FLAT);
            glTranslatef(lopta_x-1.5,lopta_y+0.2,lopta_z-1.5);
            glutSolidSphere(0.15,20,20);
	    glPopMatrix();
	    
	}

/* Funkcija za crtanje veceg ostrva. */
void nacrtaj_sigurno_ostrvo(){
	    glPushMatrix();
            /*Postavljamo difuzni materijal, kako bi dobili zeljenu boju.*/
            GLfloat  diffuse_coeffs[]={0.2,0.2,0.9,1};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);      
            glTranslatef(sigurno_x,sigurno_y,sigurno_z);
            glutSolidCube(1);
	    glPopMatrix();
}

void nacrtaj_manja_ostrva(){
     int i,j;
     /*Popunjavamo matrica BR_OSTRVA x BR_OSTRVA jedinicama, kako bi mogli lepo da postavimo ostrva: */
     for(i=0;i<BR_OSTRVA;i++){
         for(j=0;j<BR_OSTRVA;j++){
            matrica_ostrva[i][j]=1;
         }
     }
     /*iscrtavamo ostrva */
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
                        
                        /*Postavljamo difuzni materijal, kako bi dobili zeljenu boju.*/
                        GLfloat diffuse_coeffs2[]={.5,.5,.5,0};
                        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs2);
                        /*cuvamo x i z koordinate */
                        matrica_ostrva_x[i][j]=-j+0.5+pomeranje_kocke-pomeranje_kocke2;
                        matrica_ostrva_z[i][j]=i-0.5;
                        /*transliramo kako ne bi dobili zalepljena ostrva */
                        glTranslatef(-j+0.5+pomeranje_kocke-pomeranje_kocke2,0,i-0.5);
                        /*ukoliko je loptica duze na jednom ostrvu, to ostrvo pocinje da se potapa.*/
                        if(i==(pomocna_i-1) && j==pomocna_j){
                            glTranslatef(0,-potapanje_manjeg_ostrva,0);
                        }
                    }
                    else{
                        /*cuvamo x i z koordinate */
                        matrica_ostrva_x[i][j]=-j+0.5-pomeranje_kocke+pomeranje_kocke2;
                        matrica_ostrva_z[i][j]=i-0.5;
                        /*transliramo kako ne bi dobili zalepljena ostrva */
                        glTranslatef(-j+0.5-pomeranje_kocke+pomeranje_kocke2,0,i-0.5);
                        /*ukoliko je loptica duze na jednom ostrvu, to ostrvo pocinje da se potapa.*/
                        if(i==(pomocna_i-1) && j==pomocna_j){
                            glTranslatef(0,-potapanje_manjeg_ostrva,0);
                            
                        }
                    }
                    
                    glutSolidCube(0.3);
                glPopMatrix();            
            }
      }
    }
}
void idi_desno(){
        lopta_x-=0.1;
	    /* ukoliko se nalazi na velikom ostrvu i ide ulevo, proveravamo da li je ostrvo potonulo i da li je otisao previse desno. */
        if(pomocna_velikoOst==0){            
//             printf("%.2f\n",sigurno_y);
            if(sigurno_y<=-.49){
              resetuj(lopta_y);
            }             
            if(lopta_x<=-.6){ 
                resetuj(lopta_y);
            }
	    }/*
	     else{ 
            if((matrica_ostrva_x[pomocna_i-1][pomocna_j]-(lopta_x-1.5))>0.15){
                resetuj(lopta_y);
            }
            
            else{
                brojac_poena+=1;

            }
        }*/
	}
void idi_levo(){
        lopta_x+=0.1;   
        /* ukoliko se nalazi na velikom ostrvu i ide ulevo, proveravamo da li je ostrvo potonulo i da li je otisao previse levo. */
	    if(pomocna_velikoOst==0){
            if(sigurno_y<=-.49){
             resetuj(lopta_y);
            }            
            if(lopta_x>=0.6){
                resetuj(lopta_y);
            }
	    }
}
void skok_napred(){
        //nismo vise na velikom ostrvu,pa povecamo promenlju cim skocimo sa nje
        pomocna_velikoOst++;
        lopta_z+=0.1;
        lopta_y+=0.08;
        //pomocna koja nam pomaza da odredimo duzinu skoka, kada postane 10 treba da se prekine animacija skoka
        pom_duzina_skoka++;
        if(pom_duzina_skoka==10){
            lopta_y=0;
            //promenljiva koja nam pokazuje na kom smo i po redu od ostrva, broji od 1 i povecava se za jedan svaki put pri skoku napred 
            pomocna_i++;
            /*duzina skoka nam ponovo postaje 0, zbog sledeceg. */
            pom_duzina_skoka=0;
            //iskljucujemo animaciju za skok;
            animation=0;
            /*proveravamo da li je skocila sa poslednjeg reda napred, ako jeste, resetujemo.*/
            if(pomocna_i>=BR_OSTRVA+1){
                resetovanje=1;
            }
            //alive da proverimo da li je loptica pala van ostrva.Ukoliko je na ostrvo pala alive  ce biti povecane pri    pozivu funckije, dok u suprotnom ostaju 0 i samim tim loptica nije pala na ostrvo.
            
             alive=proveri_skok_napred_nazad(pomocna_i);
             if(alive==0){
                resetovanje=1;
             }
             /*zavisno u kom smo redu(paran-neparan) dobijamo razlicit broj poena */
             if(pomocna_i%2==1){
                brojac_poena+=pomocna_i+1;
             }
             else{
                brojac_poena+=pomocna_i+2;
             }
         }
         /* resetujemo na 0 promenljivu za proveru da li je loptica prezivela.*/
         alive=0;
}
void skok_levo(){
        /*menjamo koordinate tokom skoka. */
        lopta_y+=0.08;
        lopta_x+=0.1;
        pom_duzina_skoka++;
        /*ako pokusamo da skocimo sa sigurnog ostrva ulevo  */
        if(pomocna_velikoOst==0){ 
          resetuj(lopta_y);   
        }
        
        //pomocna koja nam pomaza da odredimo duzinu skoka, kada postane 10 treba da se prekine animacija skoka
        if(pom_duzina_skoka==10){
                //postavljamo lopta_y na 0, kako bi lepo stala na ostrvo(da ne lebi).
                lopta_y=0;
                /*resetovanje promenljive za sledece odredjivanje duzine skoka */
                pom_duzina_skoka=0;
                /*prekidamo animaciju za skok. */
                animation=0;
                if(pomocna_j<0){
                    resetuj(lopta_y);   
                }
                else{
                    /*umanjujemo je za 1, posto su nam za proveru potrebne koordinate prethodnog ostrva*/
                    pomocna_j=pomocna_j-1;
                        //ukoliko je alive==0 i posle skoka, to znaci da nismo skocili na ostrvo, vec da smo izgubili zivot.
                    alive=proveri_skok_levo_desno(pomocna_i,pomocna_j);
                    if(alive==0){
                        resetovanje=1;
                    }   
                    /*zavisno da li je paran ili neparan red dodeljujemo poene. */
                    if(pomocna_i%2==1){
                        brojac_poena+=pomocna_i;
                    }
                    else {
                            brojac_poena+=pomocna_i+1;
                    }
                }

        }
        /*nakon skoka ponovo je postavljamo na 0, kako bi imali mogucnost da ponovo ispitamo skok. */ 
        alive=0;
}
void skok_desno(){
        /*menjamo koordinate tokom skoka. */
        lopta_y+=0.08;
        lopta_x-=0.1;
        /* Pomocna promenljiva pomocu koje odredjujemo duzinu skoka, kada postane 10, prekidamo animaciju skoka */
        pom_duzina_skoka++;
        
        /*ako pokusamo da skocimo sa sigurnog ostrva udesno  */
        if(pomocna_velikoOst==0){
         resetuj(lopta_y);   
        }
        /*pomocna koja nam pomaza da odredimo duzinu skoka, kada postane 10 treba da se prekine animacija skoka */
        if(pom_duzina_skoka==10){

            /* Postavljamo y koordinatu lopte na 0 */
            lopta_y=0;
            /*resetovanje promenljive za skok. */
            pom_duzina_skoka=0;
            /*zaustavljamo animaciju.*/
            animation=0;
            /*ako pokusavamo da odemo van ostrva, gubi */
            if(pomocna_j>BR_OSTRVA){
             resetuj(lopta_y);
            }
            else{
                /*uvecavamo je za 1, posto su nam za proveru potrebne koordinate narednog ostrva*/
                pomocna_j=pomocna_j+1;
                /*proveravamo da li je loptica prezivela pri skoku, ako nije resetujemo. */
                alive=proveri_skok_levo_desno(pomocna_i,pomocna_j);
                if(alive==0){
                    resetovanje=1;
                }
                /*zavisno od parnosti reda dodeljujemo poene. */
                if(pomocna_i%2==1){
                    brojac_poena+=pomocna_i+1;
                }
                else {
                      brojac_poena+=pomocna_i;
                }
        }
    }
        /*ponovo se postavlja na 0, kako bi mogli da izracunamo sledeci skok. */
        alive=0;
}
    
void skok_nazad(){
        /*postavljamo nove koordinate pri skoku.*/
        lopta_y+=0.08;
        lopta_z-=0.1;
        /* Pomocna promenljiva pomocu koje odredjujemo duzinu skoka, kada postane 10, prekidamo animaciju skoka */
        pom_duzina_skoka++;
        /*ako pokusamo da skocimo sa sigurnog ostrva unazad.  */
        if(pomocna_velikoOst==0){
            resetuj(lopta_y);   
        }
        /*ako smo skocili koliko je potrebno, postavljamo lopta_y na 0 i resetujemo promenljivu za duzinu skoka.*/
        if(pom_duzina_skoka==10){
            lopta_y=0;
            pom_duzina_skoka=0;
           
            /* pomocna koja nam pomaze da odredimo na kom smo   i   od pocetnog(velikog ostrva), kada se vracamo unazad ka pocetnoj poziciji, ona se smanjuje za 1 */
            pomocna_i--;
            /*zaustavljamo animaciju */
            animation=0;
            /*ako pokusavamo da skocimo unazad gde nema ostrva.*/
            if(pomocna_i<=0){
                resetuj(lopta_y);   
            }
            
            
            /* proveravamo da li je loptica prezivela. */
            alive=proveri_skok_napred_nazad(pomocna_i);
            /*ukoliko je alive==0, resetujemo --> nije prezivela.*/
            if(alive==0){
                resetovanje=1;
            }
        /*dodajemo zeljene poene. */
            brojac_poena+=20;
        }
               
        alive=0;
}
 /*funckija koja proverava da li je loptica skocila dobro napred i nazad. */
int proveri_skok_napred_nazad(int pomocna_i){
    int j;
    for(j=0;j<BR_OSTRVA;j++){
       if((matrica_ostrva_x[pomocna_i-1][j]-0.15-(lopta_x-1.5)>=-0.17 && matrica_ostrva_x[pomocna_i-1][j]-0.15-(lopta_x-1.5)<=0) || (matrica_ostrva_x[pomocna_i-1][j]+.15-(lopta_x-1.5)>=0 && matrica_ostrva_x[pomocna_i-1][j]+.15-(lopta_x-1.5)<=0.17)){
           /*postavljamo alive na 1, kako bi pokazali da je loptica prezivela */
            alive=1;
            /*postavljamo alive2 na 1, kako bi ostrvo moglo da se pocne da se spusta. */
            alive2=1;
            /*dodeljujemo pomocna_j na j, kako bi pamtilli gde je loptica skocia zbog drugih provera. */
            pomocna_j=j;
        }
    }
    /*vracamo da li je prezivela. */
    return alive;
}
 /*funckija koja proverava da li je loptica skocila dobro napred i nazad. */
int proveri_skok_levo_desno(int pomocna_i,int pomocna_j){
    if((matrica_ostrva_x[pomocna_i-1][pomocna_j]-0.15-(lopta_x-1.5)>=-0.17 && matrica_ostrva_x[pomocna_i-1][pomocna_j]-0.15-(lopta_x-1.5)<=0)            
                    ||(matrica_ostrva_x[pomocna_i-1][pomocna_j]+.15-(lopta_x-1.5)>=0 && matrica_ostrva_x[pomocna_i-1][pomocna_j]+.15-(lopta_x-1.5)<=0.17)){
                        alive=1;
                        alive2=1;
    
    }
    /*vracamo da li je prezivela. */
    return alive;
}

/*funckija za resetovanje. */
void resetuj(float lopta_y){
     if(lopta_y==0){
         /*ispis u terminalu. */   
         printf("Kraj, OSVOJENO : %.2f POENA\n", brojac_poena);
         broj_nivoa=odredjivanje_nivoa(brojac_poena);
         printf("Broj nivoa: %d\n", broj_nivoa);
     }
     exit(1);
     return;
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
int odredjivanje_nivoa(float brojac_poena){
    while(brojac_poena>=100){
          brojac_poena-=100;
          broj_nivoa++;
    }
    return broj_nivoa;
}

/*Funkcija za ispis teksta na ekranu. */ 
void poeni_tekst(const char* tekst_poeni) {
    /*iskljucujemo osvetljenje */
	glDisable(GL_LIGHTING);
	/*boja teksta.*/	
    glEnable(GL_COLOR_MATERIAL);
	glColor3f(.1,.6,.9);
    
	//pozicija
	glRasterPos3f(-15,0,35);
	int duzina,i;
    duzina=(int)strlen(tekst_poeni);
    for(i=0;i<duzina;i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, tekst_poeni[i]);
    }
    /*iskljucujemo GL_COLOR_MATERIAL i Ukljucujemo opet svetlo. */
    glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
}
