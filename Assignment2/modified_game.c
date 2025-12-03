#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define LANES 7
#define ROWS 10
#define OBST 5
#define FRAME 100

typedef struct{ int x,y,dir,act; } O; 
HANDLE con;

void setcur(int x,int y){ COORD p={(SHORT)x,(SHORT)y}; SetConsoleCursorPosition(con,p); }

int main(){
    con = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci; GetConsoleCursorInfo(con,&ci); ci.bVisible=FALSE; SetConsoleCursorInfo(con,&ci);
    srand((unsigned)time(NULL));

    O obs[OBST];
    for(int i=0;i<OBST;i++){ obs[i].act=0; obs[i].y=0; }

    int px = LANES/2;        
    int top = 1;            
    int frames=0;

    for(int i=0;i<ROWS+6;i++) puts("");

    while(1){
        if(_kbhit()){
            int c = getch();
            if(c==0||c==224){ int c2=getch(); if(c2==75 && px>0) px--; if(c2==77 && px<LANES-1) px++; }
            else{
                if(c=='a'||c=='A') if(px>0) px--;
                if(c=='d'||c=='D') if(px<LANES-1) px++;
                if(c==' ') top = !top; 
                if(c=='q'||c=='Q') break;
            }
        }
        if(rand()%10 < 3){
            for(int i=0;i<OBST;i++) if(!obs[i].act){
                obs[i].act = 1;
                obs[i].x = rand()%LANES;
                if(rand()%2){ obs[i].y = 0; obs[i].dir = 1; } 
                else        { obs[i].y = ROWS; obs[i].dir = -1; } 
                break;
            }
        }
        setcur(0,0);
        printf("Gravity Flip  (Space=Flip)   Top=%s\n\n", top? "YES":"NO");

        for(int r=0;r<=ROWS;r++){
            for(int lane=0; lane<LANES; lane++){
                int printed = 0;
                for(int j=0;j<OBST;j++) if(obs[j].act && obs[j].x==lane && obs[j].y==r){ printf(" * "); printed=1; break; }
                if(!printed) printf("   ");
            }
            printf("\n");
        }
        int prow = top? 0 : ROWS;
        setcur(0, 2 + prow); 
        for(int lane=0; lane<LANES; lane++){
            if(lane==px) printf(" # ");
            else {
                int occ=0; for(int j=0;j<OBST;j++) if(obs[j].act && obs[j].x==lane && obs[j].y==prow){ occ=1; break;}
                if(!occ) printf("   ");
            }
        }
        setcur(0, ROWS+4);
        printf("Press Q to quit. Frames: %d   \n", frames);
        for(int j=0;j<OBST;j++) if(obs[j].act && obs[j].x==px && obs[j].y==prow){
            setcur(0, ROWS+6);
            printf("\nGAME OVER! You were hit.\n");
            ci.bVisible=TRUE; SetConsoleCursorInfo(con,&ci);
            printf("Press any key to exit..."); getch(); return 0;
        }
        for(int j=0;j<OBST;j++) if(obs[j].act){
            obs[j].y += obs[j].dir;
            if(obs[j].y < 0 || obs[j].y > ROWS){ obs[j].act = 0; obs[j].y = 0; }
        }

        Sleep(FRAME);
        frames++;
    }

    ci.bVisible=TRUE; SetConsoleCursorInfo(con,&ci);

}
