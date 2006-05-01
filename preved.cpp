//
// Превед.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <locale.h>

//------------------------------------------------------------------
// The PREVED Language Kernel area. Do not touch! :)

#if !defined (_MSC_VER) || _MSC_VER < 1400
#error Wrong compiler! Use MSVS 8.0
#endif

#define НАЧЕЛ      {
#define КОНЧЕЛ     ;}
#define ТИПА       int
#define ВДРУГ      if (
#define ТАДА       )
#define НИХРИНА    else
#define ВЗАД       return
#define КАГДИЛА    (
#define ЙО         ;
#define ЖЖОШ(p,n)  for (; (p) <= (n); (p)++)

#define БАЗАР      std::cout << 
#define СЛЫШЬ      << 
#define СТОЙ       system ("echo. & pause");
#define БЛИН       _wsetlocale (LC_ALL, L"Russian_Russia.ACP");

#define ВРОДЕ      try 
#define ИБАНУЦЦО   throw
#define АПСТЕНУ    catch (const char* __чё__)

#define ПРЕВЕД     ТИПА main КАГДИЛА ТАДА
#define МЕДВЕД     ВЗАД 0; КОНЧЕЛ

// End of area
//------------------------------------------------------------------

ТИПА КРУТО КАГДИЛА ТИПА фигня ТАДА
    НАЧЕЛ
    БАЗАР "ВАЩЕ " ЙО
    ВДРУГ фигня == 8 ТАДА ИБАНУЦЦО "мля! " ЙО
    ВЗАД 0
    КОНЧЕЛ

ПРЕВЕД
    НАЧЕЛ
    БЛИН

    ВРОДЕ НАЧЕЛ

    ТИПА фишка = 0 ЙО

    ЖЖОШ (фишка, 10)
        НАЧЕЛ
        БАЗАР фишка СЛЫШЬ " "; 
        ВДРУГ фишка >= 5 ТАДА КРУТО (фишка) ЙО
        КОНЧЕЛ

    КОНЧЕЛ АПСТЕНУ НАЧЕЛ
        БАЗАР "ИБАНУЦЦО invoked: " СЛЫШЬ __чё__;
    КОНЧЕЛ

    СТОЙ
    МЕДВЕД
