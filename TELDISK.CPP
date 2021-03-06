#include <dos.h>
#include <ctype.h>
#include <math.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int r=100,   /* ࠤ��� ⥫�䮭���� ��᪠ */
    cx,      /* 業�� ���㦭��� ��᪠ */
    cy,      
    dr=75,   /* ����ﭨ� �� 業�� ��᪠ �� 業�� �⢥���� � ��ன */
    rm=15;   /* ࠤ��� �⢥���� � ��ன */

const double Pi=3.1415926;
double eps;  /* 㣮� ����� �ᥦ���� �⢥���ﬨ */

void drawdig(int dig){
     clearviewport(); /* ������ ��࠭ */

     circle(cx,cy,r); /* ���ᮢ��� ���㦭���� ��᪠ */
     line(cx+r*cos(2*eps),cy+r*sin(2*eps),	/* ���ᮢ��� ��࠭��⥫� */
          cx+(r/2)*cos(2*eps),cy+(r/2)*sin(2*eps));

     double A;	    /* 㣮� ⥪�饣� �⢥���� */
     int dx,dy;     /* ᬥ饭�� 業�� �⢥���� �� 業�� ��᪠ */
     char *D="0\0"; /* ��� */

     A=dig*eps;     /* 㣮� �⢥���� � ��ன 1 */
     for (int i=1;i<=10;i++) {
        dx=dr*cos(A);     
        dy=dr*sin(A);
        A-=eps;           /* 㣮� ᫥���饣� �⢥���� */
        circle(cx+dx,cy+dy,rm); /* ��㥬 �⢥��⨥ */
        outtextxy(cx+dx-5,cy+dy-5,itoa(i%10,D,10)); /* �뢮��� ���� */
     }
}

void main(void) {

     /* ��⮬���᪮� ��।������ ⨯� ����᪮� ��⥬� */
     int gdriver = DETECT, gmode, errorcode;

     /* ���樠������ ��䨪� */  /* ���� � �ࠩ���� ��䨪� */
     initgraph(&gdriver, &gmode, "e:\\progs\\tc\\bgi");

     /* �஢�ઠ १���� ���樠����樨 ��䨪� */
     errorcode = graphresult();

     if (errorcode != grOk)  /* �ந��諠 �訡�� */
     {
        printf("�訡�� ���樠����樨 ��䨪�: %s\n",
           grapherrormsg(errorcode));
        printf("������ ���� ������� ��� ��⠭��� �ணࠬ��.");
        getch();
        exit(1);      /* ��室 �� �ணࠬ�� � ����� �訡�� 1 */
     }

     cx = getmaxx()/2; /* 業�� ���㦭��� ⥫�䮭���� ��᪠ */
     cy = getmaxy()/2;
     eps= 2*Pi/12;     /* 㣮� ����� �ᥤ���� �⢥���ﬨ */

     /* ��㥬 ��砫쭮� ��������� ��᪠ */ 
     drawdig(0);

     char ch = 0; /* ᨬ��� �������� ���짮��⥫�� */
     while (ch != 27) { /* �믮��塞 ���� �� Esc */
        ch = getch();
        if (!isdigit(ch)) continue; /* �ய����� �� �� ���� */
        ch-=48; /* �८�ࠧ����� ��� ᨬ���� � �᫮ */
        if (ch==0) ch=10; /* �᫨ ������ 0, � ᤥ���� 10 */
        for (int i=ch;i>=0;i--) {  /* ��� ������ ���� �� ch �� 1 */
            drawdig(i);             /* ���ᮢ��� ��������� ��᪠ */
            delay(200);				/* ����প� 200 �� */
        }                              
     }

     /* �����⨥ ��䨪� */
     closegraph();
}
