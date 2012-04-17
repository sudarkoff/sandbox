#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void main(void) {

     int R = 20;   /* радиус окружности */
     int Cx = 100; /* абсцисса центра окружности */
     int Cy = 100; /* ордината центра окружности */

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

     /* выбрать белый цвет пера */
     setcolor(15);

     /* начертить окружность с начальными параметрами */
     circle(Cx,Cy,R);

     while (1) { /* вечный цикл приема пользовательских команд */
        char ch = getch();
        if (ch == 13) break; /* прервать цикл по клавише Enter */
        if (ch == 0)         /* если была нажата клавиша с расширенным */
           ch = getch();     /* кодом, то прочитать вторую его часть */

        setcolor(0);     /* выбрать черный цвет */
        circle(Cx,Cy,R); /* стереть старую окружность
                            (нарисовать черную поверх белой) */

        switch (ch) { /* проверить нажатую клавишу */
          case '<': case ',': R>0?R-=5:R; break;   /* уменьшить радиус */
          case '>': case '.': R<200?R+=5:R; break; /* увеличить радиус */
          case 72: Cy>0?Cy-=5:Cy; break;           /* вверх */
          case 75: Cx>0?Cx-=5:Cx; break;           /* влево */
          case 77: Cx<getmaxx()?Cx+=5:Cx; break;   /* вправо */
          case 80: Cy<getmaxy()?Cy+=5:Cy; break;   /* вниз */
        }
        setcolor(15);    /* выбрать белый цвет */
        circle(Cx,Cy,R); /* нарисовать новую окружность */
     }

     /* закрытие графики */
     closegraph();
}
                   