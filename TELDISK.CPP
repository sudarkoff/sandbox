#include <dos.h>
#include <ctype.h>
#include <math.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int r=100,   /* радиус телефонного диска */
    cx,      /* центра окружности диска */
    cy,      
    dr=75,   /* расстояние от центра диска до центра отверстия с цифрой */
    rm=15;   /* радиус отверстия с цифрой */

const double Pi=3.1415926;
double eps;  /* угол между сосежними отверстиями */

void drawdig(int dig){
     clearviewport(); /* очистить экран */

     circle(cx,cy,r); /* нарисовать окружность диска */
     line(cx+r*cos(2*eps),cy+r*sin(2*eps),	/* нарисовать ограничитель */
          cx+(r/2)*cos(2*eps),cy+(r/2)*sin(2*eps));

     double A;	    /* угол текущего отверстия */
     int dx,dy;     /* смещение центра отверстия от центра диска */
     char *D="0\0"; /* цифра */

     A=dig*eps;     /* угол отверстия с цифрой 1 */
     for (int i=1;i<=10;i++) {
        dx=dr*cos(A);     
        dy=dr*sin(A);
        A-=eps;           /* угол следующего отверстия */
        circle(cx+dx,cy+dy,rm); /* рисуем отверстие */
        outtextxy(cx+dx-5,cy+dy-5,itoa(i%10,D,10)); /* выводим цифру */
     }
}

void main(void) {

     /* автоматическое определение типа графической системы */
     int gdriver = DETECT, gmode, errorcode;

     /* инициализация графики */  /* путь к драйверу графики */
     initgraph(&gdriver, &gmode, "e:\\progs\\tc\\bgi");

     /* проверка результата инициализации графики */
     errorcode = graphresult();

     if (errorcode != grOk)  /* произошла ошибка */
     {
        printf("Ошибка инициализации графики: %s\n",
           grapherrormsg(errorcode));
        printf("Нажмите любую клавишу для останова программы.");
        getch();
        exit(1);      /* выход из программы с кодом ошибки 1 */
     }

     cx = getmaxx()/2; /* центр окружности телефонного диска */
     cy = getmaxy()/2;
     eps= 2*Pi/12;     /* угол между соседними отверстиями */

     /* рисуем начальное положение диска */ 
     drawdig(0);

     char ch = 0; /* символ введенный пользователем */
     while (ch != 27) { /* выполняем пока не Esc */
        ch = getch();
        if (!isdigit(ch)) continue; /* пропустить все не цифры */
        ch-=48; /* преобразовать код символа в число */
        if (ch==0) ch=10; /* если нажали 0, то сделать 10 */
        for (int i=ch;i>=0;i--) {  /* для каждой цифры от ch до 1 */
            drawdig(i);             /* нарисовать положение диска */
            delay(200);				/* задержка 200 мс */
        }                              
     }

     /* закрытие графики */
     closegraph();
}
