/*
* By Poulkas
*/
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

using namespace std;

/* Rutas de imágenes */
#define RUTA_1 "ROBOT\\"
#define RUTA_2 "ENEMIGOS\\"
#define RUTA_3 "NUKE\\"
#define RUTA_4 "BALAS\\"
#define RUTA_5 "CARROS\\"
#define RUTA_6 "MAPAS\\"
#define RUTA_7 "MUSICA\\"
#define RUTA_8 "EXPLOSIONES\\"

/* Defines */
#define MAX_ENEMIGOS 7
#define CERO 0
#define UNO 1
#define DOS 2
#define TRES 3
#define CUATRO 4
#define CINCO 5
#define SEIS 6
#define FPS (1000/24)

/* Funciones de Random */
int random1(){
    return rand()%3;
}

int random2(){
    int ran = rand()%40;
    if(ran==13)
        return 3;
    if(ran==5)
        return 5;
    if(ran==10)
        return 10;
    return ran;
}

int random3(){
    int ran = rand()%28;
    if(ran==13)
        return 3;
    if(ran==5)
        return 5;
    if(ran==10)
        return 10;
    return ran;
}

/* Enumeración de imágenes */
enum{/** ROBOT */
	LINK_1,
	LINK_2,
	LINK_3,
	LINK_4,
	LINK_5,
    LINK_6,
	LINK_7,
	LINK_8,
	LINK_9,
	LINK_10,
	LINK_11,
	LINK_12,

	NUM_IMGS
};
enum{/** ENEMIGOS */
    zom_1,
    zom_2,
    zom_3,

    N_ENIM
};
enum{/** NUKE */
    nuke_1,
    nuke_2,
    nuke_3,
    nuke_4,

    n_n
};
enum{/** BALAS */
    bala_1,
    bala_2,

    n_b
};
enum{/** CARROS */
    car_1,
    car_2,
    car_3,
    car_4,

    n_c
};
enum{/** SONIDOS */
    DISPARO,
    CHOQUE,
    DEAD,
    FIN,
    CRASH,

    n_s
};
enum{/** EXPLOSIONES */
    EXPL_1,
    EXPL_2,
    EXPL_3,

    nex
};

/* Nombre de Imagenes y Sonidos */
const char *ENIM[N_ENIM]={
    RUTA_2 "zombie.png",
    RUTA_2 "zombie2.png",
    RUTA_2 "zombie3.png"
};
const char *imagenes[NUM_IMGS]={
	RUTA_1 "Robot_1.png",
	RUTA_1 "Robot_2.png",
	RUTA_1 "Robot_3.png",
	RUTA_1 "Robot_4.png",
	RUTA_1 "Robot_5.png",
	RUTA_1 "Robot_6.png",
	RUTA_1 "Robot_7.png",
	RUTA_1 "Robot_8.png",
	RUTA_1 "Robot_9.png",
	RUTA_1 "Robot_10.png",
	RUTA_1 "Robot_11.png",
	RUTA_1 "Robot_12.png"
};
const char *NUKE[n_n]={
    RUTA_3 "nuke.png",
    RUTA_3 "nuke2.png",
    RUTA_3 "nuke3.png",
    RUTA_3 "nuke4.png"
};
const char *BALAS[n_b]={
    RUTA_4 "bala.png",
    RUTA_4 "bala2.png",
};
const char *CARRO[n_c]={
    RUTA_5 "car1.png",
    RUTA_5 "car2.png",
    RUTA_5 "car3.png",
    RUTA_5 "car4.png"
};
const char *SONIDOS[n_s]={
    RUTA_7 "disparo.wav",
    RUTA_7 "choque.wav",
    RUTA_7"Zombie.wav",
    RUTA_7 "game_over.wav",
    RUTA_7 "choque2.wav"
};
const char *EXPL[nex]={
    RUTA_8 "d_explote.png",
    RUTA_8 "d_explote2.png",
    RUTA_8 "explosion.png"
};


void explosion(SDL_Rect pos);

/*  Variables Globales */
SDL_Window *ventana;
SDL_Surface *pantalla;
SDL_Surface *robot[NUM_IMGS];
SDL_Surface *enemigo[N_ENIM];
SDL_Surface *nuke[n_n];
SDL_Surface *bala[n_b];
SDL_Surface *car[n_c];
SDL_Surface *explote[nex];
Mix_Chunk *sonido[n_s];
Mix_Music *musica;
/* MAPA */
SDL_Surface *mapa;
SDL_Rect rect5;

/*-----main-----*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char *argv[]){
    /* ROBOT */
    int i=0, rep=0, py=0, px=0, n_ima=0, arriba=0, abajo=0, izquierda=0, derecha=0,
    racum=0, ronda=0, choqueBala1=0, choqueBala2=0;
    SDL_Rect rect;
    SDL_Rect choque;

    /* NUKE/VIDAS */
    int n=0, j=0, vida=1000, n_nukes=3;
    SDL_Rect rect4[TRES];
    rect4[CERO].x=600;
    rect4[CERO].y=-7;
    rect4[UNO].x=566;
    rect4[UNO].y=-7;
    rect4[DOS].x=532;
    rect4[DOS].y=-7;

    /* ENEMIGO */
    int z[MAX_ENEMIGOS]={0,0,0,0,0,0}, c[MAX_ENEMIGOS]={0,0,0,0,0,0}, enpy[MAX_ENEMIGOS]={0,0,0,0,0,0},
    nenemigo=0, mas[MAX_ENEMIGOS]={0,0,0,0,0,0}, n_vida[MAX_ENEMIGOS]={400,400,400,400,400,400};
    SDL_Rect rect2[MAX_ENEMIGOS];

    /* BALAS */
    int bpy=0, bstop=0;
    SDL_Rect rect3;

    /* CARRO */
    int cy[CUATRO]={0,0,0,0}, carro=0, carro_2=0, repc[CUATRO]={0,0,0},c_vida[CUATRO]={300,300,300,300};
    SDL_Rect rect6[CUATRO];

    /* SALIDA DEL JUEGO */
    int SALIR=0;

    SDL_Event event;
    uint32_t now_time, last_time;

    /* SDL INIT SUBSYSTEM/VIDEO/AUDIO */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    ventana = SDL_CreateWindow("Road Robot PROTOTYPE",200,200,640,480,0);
    pantalla = SDL_GetWindowSurface(ventana);
    Mix_OpenAudio(22050,AUDIO_S16,2,4096);
    Mix_AllocateChannels(5);

    /* Cargar Imágenes/Audio/Sonido*/
    for(int i=0;i<NUM_IMGS;i++){
        robot[i]=IMG_Load(imagenes[i]);
    }
    for(int i=0;i<N_ENIM;i++){
        enemigo[i]=IMG_Load(ENIM[i]);
    }
    for(int i=0;i<n_n;i++){
        nuke[i]=IMG_Load(NUKE[i]);
    }
    for(int i=0;i<n_b;i++){
        bala[i]=IMG_Load(BALAS[i]);
    }
    for(int i=0;i<n_c;i++){
        car[i]=IMG_Load(CARRO[i]);
    }
    for(int i=0;i<nex;i++){
        explote[i]=IMG_Load(EXPL[i]);
    }
    for(int i=0;i<n_s;i++){
        sonido[i]=Mix_LoadWAV(SONIDOS[i]);
    }
    musica = Mix_LoadMUS(RUTA_7 "Temple of Shroom.wav");
    mapa = IMG_Load(RUTA_6 "road3.png");

    Mix_PlayMusic(musica, 0);
    rect5.x=0;
    rect5.y=0;

/* Game Loop */
    do{
        while(!SALIR){
            last_time = SDL_GetTicks();
            /* DETECCIÓN DE TECLAS*/
            while(SDL_PollEvent(&event)>0){
                switch(event.type){
                    case SDL_KEYDOWN:
                        /* MOVER ROBOT */
                        if(event.key.keysym.sym==SDLK_RIGHT){ derecha=1; }
                        if(event.key.keysym.sym==SDLK_UP){ arriba=1; }
                        if(event.key.keysym.sym==SDLK_DOWN){ abajo=1; }
                        if(event.key.keysym.sym==SDLK_LEFT){ izquierda=1; }

                        /* DIRECCIÓN DEL DISPARO */
                        if(event.key.keysym.sym==SDLK_d){
                            n_ima=6;
                            bstop=1;
                            Mix_PlayChannel(-1,sonido[DISPARO],0);
                        }
                        if(event.key.keysym.sym==SDLK_a){
                            n_ima=9;
                            bstop=2;
                            Mix_PlayChannel(-1,sonido[DISPARO],0);
                        }
                        if(event.key.keysym.sym==SDLK_w){
                            n_ima=0;
                            bstop=3;
                            Mix_PlayChannel(-1,sonido[DISPARO],0);
                        }
                        if(event.key.keysym.sym==SDLK_s){
                            n_ima=3;
                            bstop=4;
                            Mix_PlayChannel(-1,sonido[DISPARO],0);
                        }
                        break;

                    case SDL_KEYUP:
                        if(event.key.keysym.sym==SDLK_UP){ n_ima=0; i=0; arriba=0; }
                        if(event.key.keysym.sym==SDLK_LEFT){ n_ima=9; i=0; izquierda=0; }
                        if(event.key.keysym.sym==SDLK_DOWN){ n_ima=3; i=0; abajo=0; }
                        if(event.key.keysym.sym==SDLK_RIGHT){ n_ima=6; i=0; derecha=0; }
                }
            }

            if(racum>=30 && racum<60){ carro=1; mas[UNO]=1; ronda=1; nenemigo=1; carro_2=1; }
            if(racum>=60 && racum<90){ carro=1; mas[DOS]=2; ronda=2; nenemigo=2; }
            if(racum>=90 && racum<200){ carro=1; mas[TRES]=3; ronda=3; nenemigo=3; }
            if(racum>=200 && racum<250){ carro=3; mas[CUATRO]=4; ronda=4; nenemigo=4; carro_2=3; }
            if(racum>=250 && racum<300){ carro=3; mas[CINCO]=5; ronda=5; nenemigo=5; }
            if(racum>350){ carro=4; mas[SEIS]=6; ronda=6; nenemigo=6; carro_2=4; }

            /* ROBOT */
            rect.x = 290+px;
            rect.y = 430+py;

            /* NIVLES DE CARROS */
            /* CARROS NIVEL UNO */
            if(carro==1){
                if(random2()==10 && repc[CERO]!=1){ repc[CERO]=1; }
                if(repc[CERO]==1){
                    rect6[CERO].y=-100+cy[CERO];
                    if(rect6[CERO].y==-100){
                        if(random1()==0){ rect6[CERO].x=370; }
                        if(random1()==1){ rect6[CERO].x=116; }
                        if(random1()==2){ rect6[CERO].x=370; }
                    }
                    cy[CERO]=cy[CERO]+8;
                    if(c_vida[CERO]==0){
                        repc[CERO]=0; cy[CERO]=0; rect6[CERO].y=-100; c_vida[CERO]=300; racum=racum+5;
                        Mix_PlayChannel(-1, sonido[CHOQUE],0 ); cout<<"PUNTAJE: "<< racum <<endl;
                    }
                    if(cy[CERO]>=500){
                        repc[CERO]=0; rect6[CERO].y=-70; cy[CERO]=0;
                    }
                }
            }

            /* CARROS NIVEL 2 */
            if(carro==3){
                if(random2()==10 && repc[UNO]!=1){ repc[UNO]=1; }
                if(repc[UNO]==1){
                    rect6[UNO].y=-70+cy[UNO];
                    if(rect6[UNO].y==-70){
                        if(random1()==0){ rect6[UNO].x=116; }
                        if(random1()==1){ rect6[UNO].x=116; }
                        if(random1()==2){ rect6[UNO].x=370; }
                    }
                    cy[UNO]=cy[UNO]+13;
                    if(c_vida[UNO]==0){
                        repc[UNO]=0; cy[UNO]=0; rect6[UNO].y=-70; c_vida[UNO]=300; racum=racum+5;
                        Mix_PlayChannel(-1,  sonido[CHOQUE], 0); cout<<"PUNTAJE: "<< racum <<endl;
                    }
                    if(cy[UNO]>=500){
                        repc[UNO]=0; rect6[UNO].y=-70; cy[UNO]=0;
                    }
                    cout<<"Carro 2 x = "<< rect6[UNO].x <<endl;
                }
                if(random2()==5 && repc[CERO]!=1){ repc[CERO]=1; }
                if(repc[CERO]==1){
                    rect6[CERO].y=-70+cy[CERO];
                    if(rect6[CERO].y==-70){
                        if(random1()==0){ rect6[CERO].x=370; }
                        if(random1()==1){ rect6[CERO].x=370; }
                        if(random1()==2){ rect6[CERO].x=116; }
                    }
                    cy[CERO]=cy[CERO]+8;
                    if(c_vida[CERO]==0){
                        repc[CERO]=0; cy[CERO]=0; rect6[CERO].y=-70; c_vida[CERO]=300; racum=racum+5;
                        Mix_PlayChannel(-1, sonido[CHOQUE], 0); cout<<"PUNTAJE: "<< racum <<endl;
                    }
                    if(cy[CERO]>=500){
                        repc[CERO]=0; rect6[CERO].y=-70; cy[CERO]=0;
                    }
                }
                if(random3()==10 && repc[DOS]!=1){ repc[DOS]=1; }
                if(repc[DOS]==1){
                    rect6[DOS].y=520-cy[DOS];
                    if(rect6[DOS].y==520){
                        if(random1()==0){ rect6[DOS].x=220; }
                        if(random1()==1){ rect6[DOS].x=466; }
                        if(random1()==2){ rect6[DOS].x=466; }
                    }
                    cy[DOS]=cy[DOS]+8;
                    if(c_vida[DOS]==0){
                        repc[DOS]=0; cy[DOS]=0; rect6[DOS].y=520; c_vida[DOS]=300; racum=racum+5;
                        Mix_PlayChannel(-1,sonido[CHOQUE],0); cout<<"PUNTAJE: "<< racum <<endl;
                    }
                    if((rect6[DOS].y-cy[DOS])<=(-590)){
                        repc[DOS]=0; cy[DOS]=0;
                    }
                }
            }

            /* CARROS NIVEL 3 */
            if(carro==4){
                if(random3()==10 && repc[UNO]!=1){ repc[UNO]=1; }
                if(repc[UNO]==1){
                    rect6[UNO].y=-70+cy[UNO];
                    if(rect6[UNO].y==-70){
                        if(random1()==0){ rect6[UNO].x=116; }
                        if(random1()==1){ rect6[UNO].x=116; }
                        if(random1()==2){ rect6[UNO].x=370; }
                    }
                    cy[UNO]=cy[UNO]+13;
                    if(c_vida[UNO]==0){
                        repc[UNO]=0; cy[UNO]=0; rect6[UNO].y=-70; c_vida[UNO]=300; racum=racum+5;
                        Mix_PlayChannel(-1,sonido[CHOQUE],0); cout<<"PUNTAJE: "<< racum <<endl;
                    }
                    if(cy[UNO]>=500){
                        repc[UNO]=0; rect6[UNO].y=-70; cy[UNO]=0;
                    }
                }
                if(random3()==5 && repc[CERO]!=1){repc[CERO]=1; }
                if(repc[CERO]==1){
                    rect6[CERO].y=-70+cy[CERO];
                    if(rect6[CERO].y==-70){
                        if(random1()==0){ rect6[CERO].x=370; }
                        if(random1()==1){ rect6[CERO].x=370; }
                        if(random1()==2){ rect6[CERO].x=116; }
                    }
                    cy[CERO]=cy[CERO]+8;
                    if(c_vida[CERO]==0){
                        repc[CERO]=0; cy[CERO]=0; rect6[CERO].y=-70; c_vida[CERO]=300; racum=racum+5;
                        Mix_PlayChannel(-1,sonido[CHOQUE],0); cout<<"PUNTAJE: "<< racum <<endl;
                    }
                    if(cy[CERO]>=500){
                        repc[CERO]=0; rect6[CERO].y=-70; cy[CERO]=0;
                    }
                }
                if(random3()==10 && repc[DOS]!=1){ repc[DOS]=1; }
                if(repc[DOS]==1){
                    rect6[DOS].y=520-cy[DOS];
                    if(rect6[DOS].y==520){
                        if(random1()==0){ rect6[DOS].x=220; }
                        if(random1()==1){ rect6[DOS].x=466; }
                        if(random1()==2){ rect6[DOS].x=466; }
                    }
                    cy[DOS]=cy[DOS]+8;
                    if(c_vida[DOS]==0){
                        repc[DOS]=0; cy[DOS]=0; rect6[DOS].y=520; c_vida[DOS]=300; racum=racum+5;
                        Mix_PlayChannel(-1,sonido[CHOQUE],0); cout<<"PUNTAJE: "<< racum <<endl;
                    }
                    if((rect6[DOS].y-cy[DOS])<=(-590)){
                        repc[DOS]=0; cy[DOS]=0;
                    }
                }
                if(random3()==10 && repc[TRES]!=1){ repc[TRES]=1; }
                if(repc[TRES]==1){
                    rect6[TRES].y=520-cy[TRES];
                    if(rect6[TRES].y==520){
                        if(random1()==0){ rect6[TRES].x=466; }
                        if(random1()==1){ rect6[TRES].x=220; }
                        if(random1()==2){ rect6[TRES].x=220; }
                    }
                    cy[TRES]=cy[TRES]+13;
                    if(c_vida[TRES]==0){
                        repc[TRES]=0; cy[TRES]=0; rect6[TRES].y=520; c_vida[TRES]=300; racum=racum+5;
                        Mix_PlayChannel(-1,sonido[CHOQUE],0); cout<<"PUNTAJE: "<< racum <<endl;
                    }
                    if((rect6[TRES].y-cy[TRES])<=(-590)){
                        repc[TRES]=0; cy[TRES]=0;
                    }
                }
            }


            /* ENEMIGOS */
            /* ENEMIGO #0 */
            rect2[CERO].y = 0+enpy[CERO];
            if(rect2[CERO].y==0){
                if(random1()==0){rect2[CERO].x = 200;}
                if(random1()==1){rect2[CERO].x = 300;}
                if(random1()==2){rect2[CERO].x = 134;}
            }
            if(z[CERO]>=10){
                z[CERO]=0;
            }
            enpy[CERO]=enpy[CERO]+6;
            if(enpy[CERO]>465){
                enpy[CERO]=0;
            }
            z[CERO]++;
            if(z[CERO]>=10){
                c[CERO]++;
            }
            if(c[CERO]>=3){
                c[CERO]=1;
            }
            if(n_vida[CERO]==0){
                enpy[CERO]=0; n_vida[CERO]=500; racum=racum+10;
                Mix_PlayChannel(-1, sonido[DEAD], 0); cout<<"PUNTAJE: "<< racum <<endl;
            }

            /* ENEMIGO #1 */
            if(mas[UNO]==UNO){
                rect2[mas[UNO]].y = 0+enpy[mas[UNO]];
                if(rect2[mas[UNO]].y==0){
                    if(random1()==0){rect2[mas[UNO]].x = 400;}
                    if(random1()==1){rect2[mas[UNO]].x = 350;}
                    if(random1()==2){rect2[mas[UNO]].x = 0;}
                }
                if(z[mas[UNO]]>=10){
                    z[mas[UNO]]=0;
                }
                enpy[mas[UNO]]=enpy[mas[UNO]]+9;
                if(enpy[mas[UNO]]>=465){
                    enpy[mas[UNO]]=0;
                }
                z[mas[UNO]]++;
                if(z[mas[UNO]]>=10){
                    c[mas[UNO]]++;
                }
                if(c[mas[UNO]]>=3){
                    c[mas[UNO]]=1;
                }
                if(n_vida[mas[UNO]]==0){
                    enpy[mas[UNO]]=0; n_vida[mas[UNO]]=500; racum=racum+10;
                    Mix_PlayChannel(-1, sonido[DEAD], 0); cout<<"PUNTAJE: "<< racum <<endl;
                }
            }

            /* ENEMIGO #2 */
            if(mas[DOS]==DOS){
                rect2[mas[DOS]].y = 0+enpy[mas[DOS]];
                if(rect2[mas[DOS]].y==0){
                    if(random1()==0){rect2[mas[DOS]].x = 250;}
                    if(random1()==1){rect2[mas[DOS]].x = 100;}
                    if(random1()==2){rect2[mas[DOS]].x = 420;}
                }
                if(z[mas[DOS]]>=10){
                    z[mas[DOS]]=0;
                }
                enpy[mas[DOS]]=enpy[mas[DOS]]+8;
                if(enpy[mas[DOS]]>=465){
                    enpy[mas[DOS]]=0;
                }
                z[mas[DOS]]++;
                if(z[mas[DOS]]>=10){
                    c[mas[DOS]]++;
                }
                if(c[mas[DOS]]>=3){
                    c[mas[DOS]]=1;
                }
                if(n_vida[mas[DOS]]==0){
                    enpy[mas[DOS]]=0; n_vida[mas[DOS]]=500; racum=racum+10;
                    Mix_PlayChannel(-1, sonido[DEAD], 0); cout<<"PUNTAJE: "<< racum <<endl;
                }
            }

            /* ENEMIGO #3 */
            if(mas[TRES]==TRES){
                rect2[mas[TRES]].y = 0+enpy[mas[TRES]];
                if(rect2[mas[TRES]].y==0){
                    if(random1()==0){rect2[mas[TRES]].x = 480;}
                    if(random1()==1){rect2[mas[TRES]].x = 500;}
                    if(random1()==2){rect2[mas[TRES]].x = 190;}
                }
                if(z[mas[TRES]]>=10){
                    z[mas[TRES]]=0;
                }
                enpy[mas[TRES]]=enpy[mas[TRES]]+7;
                if(enpy[mas[TRES]]>=465){
                    enpy[mas[TRES]]=0;
                }
                z[mas[TRES]]++;
                if(z[mas[TRES]]>=10){
                    c[mas[TRES]]++;
                }
                if(c[mas[TRES]]>=3){
                    c[mas[TRES]]=1;
                }
                if(n_vida[mas[TRES]]==0){
                    enpy[mas[TRES]]=0; n_vida[mas[TRES]]=500; racum=racum+10;
                    Mix_PlayChannel(-1, sonido[DEAD], 0); cout<<"PUNTAJE: "<< racum <<endl;
                }
            }

            /* ENEMIGO #4 */
            if(mas[CUATRO]==CUATRO){
                rect2[mas[CUATRO]].y = 0+enpy[mas[CUATRO]];
                if(rect2[mas[CUATRO]].y==0){
                    if(random1()==0){rect2[mas[CUATRO]].x = 400;}
                    if(random1()==1){rect2[mas[CUATRO]].x = 350;}
                    if(random1()==2){rect2[mas[CUATRO]].x = 50;}
                }
                if(z[mas[CUATRO]]>=10){
                    z[mas[CUATRO]]=0;
                }
                enpy[mas[CUATRO]]=enpy[mas[CUATRO]]+8;
                if(enpy[mas[CUATRO]]>=465){
                    enpy[mas[CUATRO]]=0;
                }
                z[mas[CUATRO]]++;
                if(z[mas[CUATRO]]>=10){
                    c[mas[CUATRO]]++;
                }
                if(c[mas[CUATRO]]>=3){
                    c[mas[CUATRO]]=1;
                }
                if(n_vida[CUATRO]==0){
                    enpy[mas[CUATRO]]=0; n_vida[mas[CUATRO]]=500; racum=racum+10;
                    Mix_PlayChannel(-1,sonido[DEAD],0); cout<<"PUNTAJE: "<< racum <<endl;
                }
            }

            /* ENEMIGO #5*/
            if(mas[CINCO]==CINCO){
                rect2[mas[CINCO]].y = 0+enpy[mas[CINCO]];
                if(rect2[mas[CINCO]].y==0){
                    if(random1()==0){rect2[mas[CINCO]].x = 590;}
                    if(random1()==1){rect2[mas[CINCO]].x = 480;}
                    if(random1()==2){rect2[mas[CINCO]].x = 220;}
                }
                if(z[mas[CINCO]]>=10){
                    z[mas[CINCO]]=0;
                }
                enpy[mas[CINCO]]=enpy[mas[CINCO]]+11;
                if(enpy[mas[CINCO]]>=465){
                    enpy[mas[CINCO]]=0;
                }
                z[mas[CINCO]]++;
                if(z[mas[CINCO]]>=10){
                    c[mas[CINCO]]++;
                }
                if(c[mas[CINCO]]>=3){
                    c[mas[CINCO]]=1;
                }
                if(n_vida[CINCO]==0){
                    enpy[mas[CINCO]]=0; n_vida[mas[CINCO]]=500; racum=racum+10;
                    Mix_PlayChannel(-1,sonido[DEAD],0); cout<<"PUNTAJE: "<< racum <<endl;
                }
            }

            /* ENEMIGO #6 */
            if(mas[SEIS]==SEIS){
                rect2[mas[SEIS]].y = 0+enpy[mas[SEIS]];
                if(rect2[mas[SEIS]].y==0){
                    if(random1()==0){rect2[mas[SEIS]].x = 570;}
                    if(random1()==1){rect2[mas[SEIS]].x = 525;}
                    if(random1()==2){rect2[mas[SEIS]].x = 505;}
                }
                if(z[mas[SEIS]]>=10){
                    z[mas[SEIS]]=0;
                }
                enpy[mas[SEIS]]=enpy[mas[SEIS]]+11;
                if(enpy[mas[SEIS]]>=465){
                    enpy[mas[SEIS]]=0;
                }
                z[mas[SEIS]]++;
                if(z[mas[SEIS]]>=10){
                    c[mas[SEIS]]++;
                }
                if(c[mas[SEIS]]>=3){
                    c[mas[SEIS]]=1;
                }
                if(n_vida[SEIS]==0){
                    enpy[mas[SEIS]]=0; n_vida[mas[SEIS]]=500; racum=racum+10;
                    Mix_PlayChannel(-1,sonido[DEAD],0); cout<<"PUNTAJE: "<< racum <<endl;
                }
            }

            /* NUKE\VIDAS */
            if(vida<=600 && vida>300){
                n_nukes=2;
            }
            if(vida<=300 && vida>0){
                n_nukes=1;
            }
            if(vida<=0){
                n_nukes = 0;
                Mix_PlayChannel(-1, sonido[FIN], 0);
                cout<<"FIN del juego\nSe acabo la vida"<<endl;
                SALIR=1;
            }
            if(n_nukes==3){
                if(j>=7){
                    j=0;
                }
                j++;
                if(j>=7){
                    n++;
                }
                if(n>=4){
                    n=0;
                }
            }
            if(n_nukes==2){
                if(j>=7){
                    j=0;
                }
                j++;
                if(j>=7){
                    n++;
                }
                if(n>=4){
                    n=0;
                }
            }
            if(n_nukes==1){
                if(j>=7){
                    j=0;
                }
                j++;
                if(j>=7){
                    n++;
                }
                if(n>=4){
                    n=0;
                }
            }

            /* DISPAROS */
            /*DERECHA*/
            if(bstop==1){
                rect3.y=rect.y;
                rect3.x=rect.x+15;
                rect3.x=rect3.x+bpy;
                bpy=bpy+65;
                if(rect3.x>=645){
                    bpy=0, bstop=0;
                }
                /* COLISIÓN ZOMBIES */
                for(int g=0; g<=ronda; g++){
                    if(SDL_IntersectRect(&rect3,&rect2[g],&choque)==SDL_TRUE){
                        n_vida[g]=n_vida[g]-100; bpy=0;
                        choqueBala1=1;
                        break;
                    }
                }
                /* COLISIÓN CARROS */
                if(choqueBala1==0){
                    for(int w=0; w<carro_2; w++){
                        if(SDL_IntersectRect(&rect3,&rect6[w],&choque)==SDL_TRUE){
                            c_vida[w]=c_vida[w]-100; bpy=0;
                            choqueBala1=1;
                            break;
                        }
                    }
                }
            }
            /* IZQUIERDA */
            if(bstop==2){
                rect3.y=rect.y;
                rect3.x=rect.x+15;
                rect3.x=rect3.x-bpy;
                bpy=bpy+65;
                if(rect3.x<=0){
                    bpy=0, bstop=0;
                }
                /* COLISIÓN ZOMBIES */
                for(int g=0; g<=ronda; g++){
                    if(SDL_IntersectRect(&rect3,&rect2[g],&choque)==SDL_TRUE){
                        n_vida[g]=n_vida[g]-100; bpy=0;
                        choqueBala1=1;
                        break;
                    }
                }
                /* COLISIÓN CARROS */
                if(choqueBala1==0){
                    for(int w=0; w<carro_2; w++){
                        if(SDL_IntersectRect(&rect3,&rect6[w],&choque)==SDL_TRUE){
                            c_vida[w]=c_vida[w]-100; bpy=0;
                            choqueBala1=1;
                            break;
                        }
                    }
                }
            }
            /* ARRIBA */
            if(bstop==3){
                rect3.x=rect.x;
                rect3.y=rect.y+15;
                rect3.y=rect3.y-bpy;
                bpy=bpy+65;
                if(rect3.y<=0){
                    bpy=0, bstop=0;
                }
                /* COLISIÓN ZOMBIES */
                for(int g=0; g<=ronda; g++){
                    if(SDL_IntersectRect(&rect3,&rect2[g],&choque)==SDL_TRUE){
                        n_vida[g]=n_vida[g]-100; bpy=0;
                        choqueBala2=1;
                        break;
                    }
                }
                /* COLISIÓN CARROS */
                if(choqueBala2==0){
                    for(int w=0; w<carro_2; w++){
                        if(SDL_IntersectRect(&rect3,&rect6[w],&choque)==SDL_TRUE){
                            c_vida[w]=c_vida[w]-100; bpy=0;
                            choqueBala2=1;
                            break;
                        }
                    }
                }
            }
            /* ABAJO */
            if(bstop==4){
                rect3.x=rect.x;
                rect3.y=rect.y+15;
                rect3.y=rect3.y+bpy;
                bpy=bpy+65;
                if(rect3.y>=535){
                    bpy=0, bstop=0;
                }
                /* COLISIÓN ZOMBIES */
                for(int g=0; g<=ronda; g++){
                    if(SDL_IntersectRect(&rect3,&rect2[g],&choque)==SDL_TRUE){
                        n_vida[g]=n_vida[g]-100; bpy=0;
                        choqueBala2=1;
                        break;
                    }
                }
                /* COLISIÓN CARROS */
                if(choqueBala2==0){
                    for(int w=0; w<carro_2; w++){
                        if(SDL_IntersectRect(&rect3,&rect6[w],&choque)==SDL_TRUE){
                            c_vida[w]=c_vida[w]-100; bpy=0;
                            choqueBala2=1;
                            break;
                        }
                    }
                }
            }

            /* COLISIONES ROBOT */
            for(int g=0; g<=nenemigo; g++){
                if(SDL_IntersectRect(&rect,&rect2[g],&choque)==SDL_TRUE){
                    vida=vida-8; cout<<"Cantidad de vida: "<< vida <<endl;
                }
            }
            for(int e=0; e<carro_2; e++){
                if(SDL_IntersectRect(&rect,&rect6[e],&choque)==SDL_TRUE){
                    vida=vida-4; Mix_PlayChannel(-1,sonido[CRASH],0); cout<<"Cantidad de vida: "<< vida <<endl;
                }
            }

            /* BANDERAS DE MOVIMIENTO */
            if(derecha==1){
                if(rect.x<590){
                    px=px+7;
                }
                n_ima=7;
                if(rep>=6){
                    rep=0;
                }
                rep++;
                if(rep>=6){
                    i++;
                }
                if(i>=2){
                    i=0;
                }
            }
            if(arriba==1){
                if(rect.y>0){
                    py=py-7;
                }
                n_ima=1;
                if(rep>=6){
                    rep=0;
                }
                rep++;
                if(rep>=6){
                    i++;
                }
                if(i>=2){
                    i=0;
                }
            }
            if(abajo==1){
                if(rect.y<430){
                    py=py+7;
                }
                n_ima=4;
                if(rep>=6){
                    rep=0;
                }
                rep++;
                if(rep>=6){
                    i++;
                }
                if(i>=2){
                    i=0;
                }
            }
            if(izquierda==1){
                if(rect.x>0){
                    px=px-7;
                }
                n_ima=10;
                if(rep>=6){
                    rep=0;
                }
                rep++;
                if(rep>=6){
                    i++;
                }
                if(i>=2){
                    i=0;
                }
            }

            /* ACTUALIZACIÓN DEL MAPA */
            /* DIBUJAR IMÁGENES */
            SDL_BlitSurface(mapa,NULL,pantalla,&rect5);
            for(int g=0; g<=nenemigo; g++){
                SDL_BlitSurface(enemigo[zom_1+c[mas[g]]],NULL,pantalla,&rect2[mas[g]]);
            }
            for(int g=0; g<carro_2; g++){
                if(repc[g]==1)
                    SDL_BlitSurface(car[g],NULL,pantalla,&rect6[g]);
            }
            SDL_BlitSurface(robot[n_ima+i],NULL,pantalla,&rect);
            if(bstop==1 || bstop==2){
                if(choqueBala1==1){
                    SDL_BlitSurface(explote[UNO],NULL,pantalla,&rect3);
                    choqueBala1 = 0;
                    bstop = 0;
                }
                else SDL_BlitSurface(bala[bala_2],NULL,pantalla,&rect3);
            }
            if(bstop==3 || bstop==4){
                if(choqueBala2==1){
                    SDL_BlitSurface(explote[CERO],NULL,pantalla,&rect3);
                    choqueBala2 = 0;
                    bstop = 0;
                }
                else SDL_BlitSurface(bala[bala_1],NULL,pantalla,&rect3);
            }
            for(int g=0; g<n_nukes; g++){
                SDL_BlitSurface(nuke[n],NULL,pantalla,&rect4[g]);
            }
            if(SALIR==1){
                SDL_BlitSurface(nuke[CERO], NULL, pantalla, &rect);
                SDL_UpdateWindowSurface(ventana);
                SDL_Delay(9500);
            }/* ACTUALIZAR VENTANA */
            else SDL_UpdateWindowSurface(ventana);
            /* BORRADO DE IMÁGENES */
            for(int g=0; g<=nenemigo; g++){
                SDL_FillRect(pantalla,&rect2[g],0);
            }
            for(int g=0; g<carro_2; g++){
                SDL_FillRect(pantalla,&rect6[g],0);
            }
            for(int g=0; g<n_nukes; g++){
                SDL_FillRect(pantalla,&rect4[g],0);
            }
            SDL_FillRect(pantalla,&rect,0);
            SDL_FillRect(pantalla,&rect3,0);
            SDL_FillRect(pantalla,&rect5,0);

            now_time = SDL_GetTicks();
            if(now_time<last_time+FPS){ SDL_Delay(last_time+FPS-now_time); }
        }

        /* EXLOSIÓN FINAL */
        explosion(rect);
        event.type = SDL_QUIT;

    }while(event.type!=SDL_QUIT);

    SDL_Quit();

    return 0;
}
#pragma GCC diagnostic pop

void explosion(SDL_Rect pos){
    int exp = 0, v = 0;
    bool explot_active = true;
    SDL_Rect rect7, rect8;

    rect8.x = pos.x-73;
    rect8.y = pos.y-22;

    while(explot_active){
        if(exp==0){
            rect7.x=0;
            rect7.y=0;
            rect7.w=180;
            rect7.h=100;
        }
        if(exp==1){
            rect7.x=210;
            rect7.y=0;
            rect7.w=180;
            rect7.h=100;
        }
        if(exp==2){
            rect7.x=420;
            rect7.y=0;
            rect7.w=180;
            rect7.h=100;
        }
        if(exp==3){
            rect7.x=0;
            rect7.y=102;
            rect7.w=180;
            rect7.h=100;
        }
        if(exp==4){
            rect7.x=210;
            rect7.y=102;
            rect7.w=180;
            rect7.h=100;
        }
        if(exp==5){
            rect7.x=420;
            rect7.y=103;
            rect7.w=190;
            rect7.h=100;
        }
        if(exp==6){
            rect7.x=0;
            rect7.y=199;
            rect7.w=190;
            rect7.h=100;
        }
        if(exp==7){
            rect7.x=210;
            rect7.y=199;
            rect7.w=188;
            rect7.h=100;
        }
        if(exp==8){
            rect7.x=420;
            rect7.y=199;
            rect7.w=188;
            rect7.h=100;
        }
        if(exp==9){
            rect7.x=0;
            rect7.y=302;
            rect7.w=188;
            rect7.h=100;
        }
        if(exp==10){
            rect7.x=210;
            rect7.y=302;
            rect7.w=188;
            rect7.h=100;
        }
        if(exp==11){
            rect7.x=420;
            rect7.y=302;
            rect7.w=188;
            rect7.h=100;
        }
        v++;
        if(v==10){
            v=0;
            exp++;
        }
        if(exp==45){
            explot_active = false;
        }
        SDL_UpdateWindowSurface(ventana);
        SDL_FillRect(pantalla,&rect8,0);
        SDL_FillRect(pantalla,&rect5,0);
        SDL_BlitSurface(mapa,NULL,pantalla,&rect5);
        SDL_BlitSurface(explote[EXPL_3],&rect7,pantalla,&rect8);
    }
}

