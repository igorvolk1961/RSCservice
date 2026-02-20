/******  mapsyst.h  ************  Belenkov O.V.    ***  08/04/22 ***
*******  mapsyst.h  ************  Derevyagina Zh.A.***  03/09/20 ***
*******  mapsyst.h  ************  Борзов А.Ю.      ***  17/07/15 ***
*******  mapsyst.h  ************  Shabakov D.A.    ***  06/07/18 ***
*******  mapsyst.h  ************  Kruzhkov A.E.    ***  16/11/21 ***
*******  mapsyst.h  ************  Tarakanova I.R.  ***  29/03/22 ***
*******  mapsyst.h  ************  Vitsko D.A.      ***  24/11/21 ***
*******  mapsyst.h  ************  Savelov A.S.     ***  16/09/21 ***
*******  mapsyst.h  ************  Zhelezniakov V.A.***  09/12/21 ***
*******  mapsyst.h  ************  Korjilov A.      ***  04/02/16 ***
*******  mapsyst.h  ************  Letov V.I.       ***  17/03/20 ***
*******  mapsyst.h  ************  Kruzhkova E.A.   ***  05/06/18 ***
*******  mapsyst.h  ************  Ganyushin K.A.   ***  27/04/22 ***
*******  mapsyst.h  ************  Sotnikov P.I.    ***  15/11/21 ***
*******  mapsyst.h  ************  Portnyagin E.V.  ***  22/04/22 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2022              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*                ОПИСАНИЕ СИСТЕМНЫХ ФУНКЦИЙ                        *
*                                                                  *
*******************************************************************/

#ifndef MAPSYST_H
#define MAPSYST_H

#if !defined(__linux__) && !defined(__QNX__)    // Объявляется компилятором для Linux и QNX
#ifndef WIN32API
  #define WIN32API      // Тип системного интерфейса WinApi
#endif
#else
#ifndef LINUXAPI
  #define  LINUXAPI     // Тип системного интерфейса Linux
#endif
#endif  // !__linux__ && !__QNX__

//#define CHECK_MEMORY  // Контроль new, delete, open, close, critical section

//#define NUMBERTURN    // Процессор типа SPARC/MIPS (разворот байт)

//#define QNX
//#define SPARC_SOLARIS

//#define LINUXSERVER   // Тип сборки для Linux-сервера
                        // (увеличение размеров области памяти
                        //  для отображения карты)

// Чтобы не было путаницы в момент исполнения между вариантами UnicodeToString(CP1251/KOI8-R),
// сделаем переобъявление функций с учетом CLASSPREFIX
#include "classmacro.h"

#ifdef WIN32API

#if defined(__GNUC__) || defined(__MINGW32__)
 #include <sys/types.h>  // __int64
#endif

 #define INLINEFUNC inline

#if defined(_M_X64) || defined(BUILD_DLL64)
  #pragma pack(8)
#endif

 #if !defined(STRICT)
   #define STRICT
 #endif
  #include <windows.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <psapi.h>       // функции запроса свойств процесса (память и пр.)

  typedef __int64  BIGINT; // Большой int
  typedef HANDLE   THREADHANDLE;
  typedef HANDLE   SEMPHHANDLE;
  typedef long int XYTYPE;

#endif

#ifdef LINUXAPI
 #define INLINEFUNC __attribute__((always_inline)) inline

 #include "semaphore.h"
 typedef pthread_t THREADHANDLE;
 typedef sem_t*    SEMPHHANDLE;

#endif

#ifdef SPARC_SOLARIS
  #define INADDR_NONE -1
  #define FNM_CASEFOLD (1 << 4)
  #define  HIDEX11
  #define  HIDE3D
  #define  HIDERSC3D
  #define  HIDE_DOMAIN_AUTH
  #define  HIDETRY
  #define  sinl(x)     sin(x)
  #define  cosl(x)     cos(x)
#endif
#ifdef QNX
  #define  FILESYSTEMUTF8
  #define  HIDEX11
  #define  HIDE3D
  #define  HIDERSC3D
  #define  HIDE_DOMAIN_AUTH
  #define  HIDETRY
  #define  CBAUD 15
  #define  CRTSCTS 020000000000
  #define  sinl(x)     sin(x)
  #define  cosl(x)     cos(x)
#endif

#ifndef MAPTRACE_H
  #include "maptrace.h"
#endif

#if !defined(WIN32API) || defined(__BORLANDC__)
 #define Snprintf snprintf
#else

 INLINEFUNC int Snprintf(char* buffer, size_t sizeOfBuffer, const char* format, ...)
 {
   if ((buffer == 0) || (format == 0))
     return 0;
   *buffer = 0;
   va_list args;
   va_start(args, format);
   int ret = 0;
   try
   {
#if defined(__BORLANDC__)
     ret = _vsnprintf_s(buffer, sizeOfBuffer, format, args);
#else
     ret = _vsnprintf_s(buffer, sizeOfBuffer, sizeOfBuffer - 1, format, args);
#endif
   }
   catch (...)
   {
     buffer[sizeOfBuffer] = 0;
   }
   va_end(args);
   return ret;
 }
#endif

#ifdef WIN32API
  #define HMESSAGE HWND
  #define WCHAR2   WCHAR
#else
  #define HMESSAGE MSGHANDLER
  #define WCHAR2   WCHAR
#endif

#ifdef WIN32API
  #define WTEXT(_x) L ## _x
  #define WTEXTM WTEXT
#else
  #if (__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__)
    #define WTEXT(_x) (const WCHAR*)u ## _x
    #define WTEXTM(_x) u ## _x
  #else
    #define WTEXT(_x) L ## _x
    #define WTEXTM WTEXT
  #endif
#endif

#ifdef WIN32API
  #define WS(X) L##X
  #define WSL(X) X
#else
  #ifndef UTILS_H
    #include "utils.h"
  #endif

  #define MYSTR(X) L##X

  #if (__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__)
    #define WS(X) WTEXT(X)
  #else
    #ifdef SHORT_WCHAR
      #define WS(X) L##X
    #else
      #define WS(X) PLocalString(MYSTR(X)).utf16()
      #define WSL(X) PLocalString(X).utf16()
    #endif
  #endif
#endif  // WIN32API

#ifdef LINUXAPI

 #ifdef LINUXSERVER
   #define HIDEMESS
 #endif

 #define QMF_BYCOMMAND 1
 #define QMF_CHECKED   2 // Константы для работы контекстного меню
 #define MAX_COMPUTERNAME_LENGTH 31

#ifdef NUMBERTURN
  #ifdef LINUXSERVER
    #define HIDERSC45
  #endif
#endif
//  #define RISCCPU       // Выравнивание переменных и структур
//  #define HIDEMTL
//  #define HIDEMTD
//  #define HIDETIN
  #define HIDEMETAFILE
  #define HIDEMTR3D
//  #define HIDE3D
  #define HIDEIMAGEDIT
//  #define HIDEUNION
  #define WINAPI
  #define TURNRASTERTEXT
  #ifdef LINUXSERVER
    #define HIDEX11    // Скрыть функции Xwindow
    #define HIDEGISSERV
    #define MULTIPLATFORM  // Одновременная работа с данными на платформах SPARC и INTEL
    #define TRANSTELECOM
  #else
//  #define HIDEGISSERV
//  #define HIDEX11          // Скрыть функции Xwindow (Объявляется через makefile)
//  #define MULTIPLATFORM  // Одновременная работа с данными на платформах SPARC и INTEL
//  #define TRANSTELECOM
  #endif
#ifdef HIDEX11
  #define HIDESCREENIMAGE
#endif
  #define CROSS_MIPS      // Поддержка кросскомпилятора под процессор MIPS
  #define HIDEPAINTSTEP   // Игнорировать шаг вывода DIB в окно
#if defined (__x86_64__) || (__e2k__) || (__aarch64__)        // Объявляется компилятором при 64-битной системе
  #define BUILD_DLL64
#endif
#define FILESYSTEMUTF8   // Локализация файловой системы UTF8 (по умолчанию KOI8-R) (Объявляется через makefile)
#ifndef QT_DRAW
  #define QT_DRAW          // Подключение функций внешнего рисования в DIB (Qt)
#endif

  #include <sys/types.h>
  #include <sys/stat.h>
  #include <stdio.h>
  #include <cstdio>   // BUFSIZ
  #include <unistd.h>
  #include <fcntl.h>
  #include <time.h>
  #include <stdlib.h>
  #include <ctype.h>
  #include <errno.h>
  #include <string.h>
  #include <dlfcn.h>
  #ifdef __QNX__
    #include <sys/statvfs.h>
    #include <utime.h>
    #include <limits.h>
    #include <inttypes.h>
    #define MAXLONG LONG_MAX
  #else
    #include <sys/vfs.h>
  #endif

#include <pthread.h>
#include <iconv.h>
#ifndef HIDEGISSERV
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#endif

#ifdef BUILD_DLL64
#pragma pack(push,1)
#endif

  typedef unsigned char BYTE;
  typedef struct PALETTEENTRY
  {
    BYTE peRed;
    BYTE peGreen;
    BYTE peBlue;
    BYTE peFlags;
  }
  PALETTEENTRY;
  typedef struct tagRGBTRIPLE
  {
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
  }
  RGBTRIPLE;

  typedef unsigned long int HANDLE;
  typedef unsigned int UINT;
  typedef unsigned long long UINT64;
  typedef unsigned int COLORREF;
  typedef unsigned short int WORD;
  typedef unsigned int DWORD;
  typedef int LONG;
  typedef HANDLE HBITMAP;
  typedef HANDLE HGLOBAL;
  typedef HANDLE HPALETTE;
  typedef HANDLE HDC;
  typedef HANDLE HPEN;
  typedef HANDLE HMODULE;
  typedef HANDLE HWIDGET;
  typedef int XYTYPE;
#ifndef HWND
  #define HWND unsigned long int
#endif
  typedef long int HINSTANCE;
  typedef long int LPARAM;
  typedef long int WPARAM;
  typedef long int LRESULT;
  typedef long int HMENU;
  typedef void *   LPVOID;
  typedef char *   LPTSTR;
  typedef const char* LPCSTR;
  typedef char *   LPSTR;
  typedef unsigned long long ULONGLONG;
#if defined(SHORT_WCHAR)
  // С BUILD_DLL транслируются только библиотеки ядра
  // вместе с опцией компилятору -fshort-wchar, что
  // означает размер wchar_t равен 2 байтам
  typedef wchar_t  WCHAR;
#else
  typedef WORD     WCHAR;
#endif

#ifndef TCHAR
  typedef char TCHAR;
  #define _TEXT(x)   x
  #define TEXT       _TEXT
  #define _tcsncpy   strncpy
  #define _tcsncat   strncat
  #define _tcslen    strlen
#endif

#ifdef __GNUC__
  typedef int64_t __int64;
#endif
  typedef __int64 BIGINT;             // Большой int
  typedef unsigned int SOCKET;
  #define MessageBoxA MessageBox
#if (!defined(WATCOM) && !defined(OC2000))
  #define strnicmp   strncasecmp
  #define stricmp    strcasecmp
  #define strcmpi    strcasecmp
  #define _tcscpy    strcpy
  #define _tcscat    strcat
#else
  #define stricmp    strcmpi
  #define sinl       sin
  #define cosl       cos
#endif

#ifndef FALSE
  #define FALSE      0
  #define TRUE       1
#endif

  #define _wcscmpi wcscmpi
  #define _stprintf sprintf
  #define _fastcall
  #define __fastcall
  #define PASCAL __stdcall
  #define WINAPI
  #define __stdcall
  #define _stdcall
  #define far
  #define _import
  #define near
  #define _export  __attribute__ ((visibility ("default")))
  #define __export __attribute__ ((visibility ("default")))
  #define __cdecl
  #define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
  #define PALETTERGB(r,g,b)   (0x02000000 | RGB(r,g,b))
  #define PALETTEINDEX(i)     ((COLORREF)(0x01000000 | (DWORD)(WORD)(i)))
  #define GetRValue(rgb)   ((BYTE) (rgb))
  #define GetGValue(rgb)   ((BYTE) (((WORD) (rgb)) >> 8))
  #define GetBValue(rgb)   ((BYTE) ((rgb) >> 16))

  #define ERROR_ALREADY_EXISTS 183L
  #define ERROR_FILE_NOT_FOUND 2L
  #define LF_FACESIZE 32

  #define FILE_SHARE_READ (S_IRUSR|S_IRGRP|S_IROTH)
  #define FILE_SHARE_WRITE (S_IWUSR|S_IWGRP|S_IWOTH)
  #define GENERIC_READ  0x100000
  #define GENERIC_WRITE O_WRONLY
  #define OPEN_ALWAYS O_CREAT
  #define CREATE_ALWAYS (O_CREAT|O_TRUNC)
  #define OPEN_EXISTING O_EXCL
  #define CREATE_NEW (O_CREAT|O_EXCL)
  #define TRUNCATE_EXISTING (O_CREAT|O_EXCL|O_TRUNC)
  #define FILE_BEGIN SEEK_SET
  #define FILE_CURRENT SEEK_CUR
  #define FILE_END SEEK_END
  #define MAX_PATH 260
  #define FILE_ATTRIBUTE_READONLY (S_IRUSR|S_IRGRP|S_IROTH)
  #define FILE_ATTRIBUTE_ARCHIVE (S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR|S_IRGRP|S_IROTH)
  #define FILE_ATTRIBUTE_HIDDEN (S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR)
  #define FILE_ATTRIBUTE_NORMAL (S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR|S_IRGRP|S_IROTH)
  #define FILE_ATTRIBUTE_OFFLINE (S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR|S_IRGRP|S_IROTH)
  #define FILE_ATTRIBUTE_SYSTEM (S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR)
  #define FILE_ATTRIBUTE_TEMPORARY (S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR|S_IRGRP|S_IROTH)

  // Управляющие флаги диалога выбора файлов
  #define OFN_SAVEMODE            0x00000001
  #define OFN_FILEMUSTEXIST       0x00000002
  #define OFN_PATHMUSTEXIST       0x00000004
  #define OFN_ALLOWMULTISELECT    0x00000008
  #define OFN_CREATEPROMPT        0x00000010
  #define OFN_OVERWRITEPROMPT     0x00000020
  #define OFN_HIDEREADONLY        0x00000040
  #define OFN_NOREADONLYRETURN    0x00000080

  // Маски клавиш (полное соотвествие WIN32)
  #define MK_LBUTTON                  0x0001
  #define MK_RBUTTON                  0x0002
  #define MK_SHIFT                    0x0004
  #define MK_CONTROL                  0x0008
  #define MK_MBUTTON                  0x0010
  // Коды клавиш (XWindow)
  #define VK_F1                       0xffbe
  #define VK_F2                       0xffbf
  #define VK_F3                       0xffc0
  #define VK_F4                       0xffc1
  #define VK_F5                       0xffc2
  #define VK_F6                       0xffc3
  #define VK_F7                       0xffc4
  #define VK_F8                       0xffc5
  #define VK_F9                       0xffc6
  #define VK_F10                      0xFFC7             // XK_F10
  #define VK_ESCAPE                   0xFF1B             // XK_Escape
  #define VK_TAB                      0xFF09             // XK_Tab
  #define VK_HOME                     0xFF50             // XK_Home
  #define VK_PRIOR                    0xFF55             // XK_Prior
  #define VK_NEXT                     0xFF56             // XK_Next
  #define VK_END                      0xFF57             // XK_End
  #define VK_LEFT                     0xFF51             // XK_Left
  #define VK_UP                       0xFF52             // XK_Up
  #define VK_RIGHT                    0xFF53             // XK_Right
  #define VK_DOWN                     0xFF54             // XK_Down
  #define VK_SHIFT                    0xFFE1             // XK_Shift_L
  #define VK_CONTROL                  0xFFE3             // XK_Control_L
  #define VK_RETURN                   0xFF0D             // XK_Return
  #define VK_LBUTTON                  0xFEE0             // XK_Pointer_Left
  #define VK_RBUTTON                  0xFEE1             // XK_Pointer_Right
  #define VK_SPACE                    0x020              //  XK_space
  #define VK_MENU                     0xFF67             //  XK_Menu
  #define VK_BACK                     0xFF08             // XK_BackSpace // back space, back char
  #define VK_INSERT                   0xFF63             // XK_Insert      Insert, insert here    WIN32 Ins 45
  #define VK_ADD                      0xFFAB             // XK_KP_Add
  #define VK_DELETE                   0xFFFF             // XK_Delete  Delete, rubout WIN32 Del  0x2e
                                                         // XK_KP_Delete 0xFF9F не добавлена
  #define VK_SUBTRACT                 0xFFAD             // XK_KP_Subtract   WIN32 109

  #define MB_OK                       0x00000000L
  #define MB_ICONHAND                 0x00000010L
  #define MB_OKCANCEL                 0x00000001L
  #define MB_YESNOCANCEL              0x00000003L
  #define MB_YESNO                    0x00000004L
  #define MB_ABORTRETRYIGNORE         0x00000002L
  #define MB_RETRYCANCEL              0x00000005L

  #define MB_APPLMODAL                0x00000000L
  #define MB_ICONQUESTION             0x00000020L
  #define MB_ICONEXCLAMATION          0x00000030L
  #define MB_ICONWARNING              MB_ICONEXCLAMATION
  #define MB_ICONERROR                MB_ICONHAND
  #define MB_ICONASTERISK             0x00000040L
  #define MB_ICONINFORMATION          MB_ICONASTERISK
  #define MB_ICONSTOP                 MB_ICONHAND
  #define MB_TASKMODAL                0x00002000L

  #define MB_SETFOREGROUND            0

  #define IDOK                        1
  #define IDCANCEL                    2
  #define IDABORT                     3
  #define IDRETRY                     4
  #define IDIGNORE                    5
  #define IDYES                       6
  #define IDNO                        7
  #define IDCLOSE                     8
  #define IDHELP                      9
  #define IDTRYAGAIN                  10
  #define IDCONTINUE                  11

  #define DT_PLOTTER          0   // Vector plotter
  #define DT_RASDISPLAY       1   // Raster display
  #define DT_RASPRINTER       2   // Raster printer

  #define R2_COPYPEN          0x3 // GXcopy   - src  для XWindow
  #define R2_NOT              0xa // GXinvert - (NOT dst) для XWindow
  #define R2_NOTXORPEN        0x9 // GXequiv    - (NOT src) XOR dst  для XWindow
  #define R2_XORPEN           0x6 // GXxor    - src XOR dst для XWindow

  #define OEM_CHARSET         255
  #define RUSSIAN_CHARSET     204
  #define DEFAULT_CHARSET     RUSSIAN_CHARSET
  #define BI_RGB              0L

  #define TA_LEFT             0
  #define TA_RIGHT            2
  #define TA_CENTER           6

  #define TA_TOP              0
  #define TA_BOTTOM           8
  #define TA_BASELINE         24

  // Стандартный вид курсора        // Коды XWindow:
  #define IDC_ARROW_INT    68L      // XC_left_ptr
  #define IDC_ICON_INT     IDC_ARROW_INT
  #define IDC_IBEAM_INT    152L     // XC_xterm
  #define IDC_WAIT_INT     150L     // XC_watch
  #define IDC_CROSS_INT    34L      // XC_crosshair
  #define IDC_UPARROW_INT  22L      // XC_center_ptr
  #define IDC_SIZEALL_INT  52L      // XC_fleur
  #define IDC_SIZE_INT     IDC_SIZEALL_INT
  #define IDC_SIZENWSE_INT 32L      // XC_cross_reverse
  #define IDC_SIZENESW_INT IDC_SIZENWSE_INT
  #define IDC_SIZEWE_INT   108L     // XC_sb_h_double_arrow
  #define IDC_SIZENS_INT   116L     // XC_sb_v_double_arrow

  #define IDC_ARROW    (char *)IDC_ARROW_INT
  #define IDC_ICON     (char *)IDC_ICON_INT
  #define IDC_IBEAM    (char *)IDC_IBEAM_INT
  #define IDC_WAIT     (char *)IDC_WAIT_INT
  #define IDC_CROSS    (char *)IDC_CROSS_INT
  #define IDC_UPARROW  (char *)IDC_UPARROW_INT
  #define IDC_SIZEALL  (char *)IDC_SIZEALL_INT
  #define IDC_SIZE     (char *)IDC_SIZE_INT
  #define IDC_SIZENWSE (char *)IDC_SIZENWSE_INT
  #define IDC_SIZENESW (char *)IDC_SIZENESW_INT
  #define IDC_SIZEWE   (char *)IDC_SIZEWE_INT
  #define IDC_SIZENS   (char *)IDC_SIZENS_INT

  #define INFINITE     600000
  #define MF_SEPARATOR 0x0800
  #define MF_BYCOMMAND 0x0000
  #define MF_CHECKED   0x0008
  #define MF_UNCHECKED 0x0000
  #define MF_DISABLED  0x0002
  #define MF_GRAYED    0x0001
  #define MF_STRING    0x0000
  #define _fastcall

typedef struct _FILETIME
{
   DWORD dwLowDateTime;
   DWORD dwHighDateTime;
}
   FILETIME;

typedef struct tagSIZE
{
   LONG cx;
   LONG cy;
}
   SIZE;

typedef struct tagLOGFONT
{
   LONG lfHeight;
   LONG lfWidth;
   LONG lfEscapement;
   LONG lfOrientation;
   LONG lfWeight;
   BYTE lfItalic;
   BYTE lfUnderline;
   BYTE lfStrikeOut;
   BYTE lfCharSet;
   BYTE lfOutPrecision;
   BYTE lfClipPrecision;
   BYTE lfQuality;
   BYTE lfPitchAndFamily;
   char lfFaceName[LF_FACESIZE];
}
   LOGFONT;
typedef LOGFONT LOGFONTA;

typedef struct tagLOGFONTW
{
   LONG lfHeight;
   LONG lfWidth;
   LONG lfEscapement;
   LONG lfOrientation;
   LONG lfWeight;
   BYTE lfItalic;
   BYTE lfUnderline;
   BYTE lfStrikeOut;
   BYTE lfCharSet;
   BYTE lfOutPrecision;
   BYTE lfClipPrecision;
   BYTE lfQuality;
   BYTE lfPitchAndFamily;
   WCHAR lfFaceName[LF_FACESIZE];
}
   LOGFONTW;

typedef struct tagRGBQUAD
{
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
}
    RGBQUAD;

typedef struct tagPOINT
{
    int x;
    int y;

    tagPOINT(int x) { this->x = x; this->y = x; }
    tagPOINT(int x, int y) { this->x = x; this->y = y; }
    tagPOINT() { }
}
    POINT;

typedef struct _RECT
{
    int left;
    int top;
    int right;
    int bottom;

    POINT GetLeftTop() const { return POINT(left, top); }
    POINT GetRightBottom() const { return POINT(right, bottom); }
}
    RECT;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
}
    BITMAPINFOHEADER;

typedef struct tagBITMAPINFO
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
}
    BITMAPINFO;

typedef struct _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
}
    SYSTEMTIME;

typedef struct tagMSG
{
    HWND hwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD time;
    POINT pt;
}
    MSG;

typedef struct TMessage
{
    WPARAM    WParam;
    LPARAM    LParam;
    long int  Result;
}
    TMessage;

typedef struct MENUSTRINGTABLE
{
    int       IdItem;             // Идентификатор строки
    WCHAR     NameItem[128];      // Текст
    int       Enable;             // Доступность
    int       Check;
}
    MENUSTRINGTABLE;

// Прототипы переопределенных функций (XWindow.cpp)
char * _fastcall strlwr(char * string);
void   _fastcall GetSystemTime(SYSTEMTIME * systemtime);
void   _fastcall GetLocalTime(SYSTEMTIME * systemtime);

// Получить имя компьютера
// compname - адрес строки для размещения результата
// size - адрес переменной для размещения размера результата.
// При вызове функции в size размещается размер строки compname.
// После выполнения функции в size размещается число символов, скопированных
// в compname (без учета завершающего нуля).
// В случае успешного выполнения возвращает true
extern "C"
{
bool GetComputerNameA(char* compname, DWORD *size);
bool GetComputerNameW(WCHAR* compname, DWORD *size);
bool GetUserNameA(char* username, DWORD *size);
bool GetUserNameW(WCHAR* username, DWORD *size);
}

unsigned long GetTickCountSec();
unsigned long GetTickCount();
unsigned long long GetTickCount64();

int _fastcall MessageBox(HWND hwnd, const TCHAR * message, const TCHAR * title, int flag);
int _fastcall MessageBoxW(HWND hwnd, const WCHAR * message, const WCHAR * title, int flag);

// Запросить длину строки UNICODE (datbase.cpp)
int _fastcall strlenun(const char * text, int limit = 4096);

// Прототипы функций, определенных только для Linux (xwindow.cpp)

// Конвертировать индекс палитры в цвет вида COLORREF
// index - индекс палитры XWindow
// возвращает COLORREF
COLORREF _fastcall ConvertIndexPaletteToCOLORREF(COLORREF index);

// Конвертировать цвет вида COLORREF в индекс палитры
// color - цвет вида COLORREF
// возвращает индекс палитры
COLORREF _fastcall ConvertCOLORREFToIndexPalette(COLORREF color);

typedef int(WINAPI *FARPROC) ();

#endif     // LINUX

// Обработчик сообщений
typedef long int (*MSGHANDLER)(long int hwnd, long int code, long int p1, long int p2, long int typemsg);

#ifdef __BORLANDC__ // Для Borland C++
 #define _strnicmp  strncmpi
 #define _strcmpi   strcmpi
 #define _stricmp   stricmp
#endif

#ifdef _MSC_VER // Для VC
 #define wcsncmpi   _wcsnicmp
 #define wcscmpi    _wcsicmp
 #define stricmp    _stricmp
 #define strcmpi    _stricmp
 #define strupr     _strupr
#endif

#ifdef LINUXAPI
 #define _strnicmp  strnicmp
 #define _strcmpi   strcmpi
 #define _stricmp   strcmpi
 #define _strlwr    strlwr
 #define wcsicmp    wcscmpi
 #define _atoi64    atoll
#endif

// Определяется экспорт/импорт классов и функций (_DLLCLASS)
#ifdef BUILD_DLL
  #ifdef BUILD_DLL64
    #ifdef WIN32API
      #define _DLLCLASS __declspec(dllexport)
      #define _MAPAPI   WINAPI
      #define _MAPIMP   __declspec(dllexport)
    #else
      #define _DLLCLASS __attribute__ ((visibility ("default")))
      #define _MAPAPI   WINAPI
      #define _MAPIMP __attribute__ ((visibility ("default")))
    #endif
  #else
    #ifdef __BORLANDC__ // Для Borland C++
      #define _DLLCLASS _export
      #define _MAPAPI   _export WINAPI
      #define _MAPIMP
    #else               // Для Visual C++
      #ifdef LINUXAPI
        #define _DLLCLASS __attribute__ ((visibility ("default")))
        #define _MAPAPI   WINAPI
        #define _MAPIMP __attribute__ ((visibility ("default")))
      #else
        #ifndef GISWEB
          #define _DLLCLASS __declspec(dllexport)
          #define _MAPAPI   WINAPI
          #define _MAPIMP   __declspec(dllexport)
        #else
          #define _DLLCLASS
          #define _MAPAPI   WINAPI
          #define _MAPIMP   __declspec(dllexport)
        #endif
      #endif // LINUXAPI
    #endif  //  __BORLANDC__
  #endif   // BUILD_DLL64

#else // BUILD_DLL

  #ifdef __BORLANDC__ // Для Borland C++
   #ifdef _GISSERVER
      #define _DLLCLASS
      #define _MAPAPI   WINAPI
      #define _MAPIMP
   #else
    #ifdef BUILD_DLL64
      #define _DLLCLASS
      #define _MAPAPI   WINAPI
      #define _MAPIMP   __declspec( dllimport )
    #else
      #define _DLLCLASS
      #define _MAPAPI   WINAPI
      #define _MAPIMP   __declspec( dllimport )
    #endif
   #endif
  #else                     // Для Visual C++
   #if defined(LINUXAPI)
     #define _DLLCLASS
     #define _MAPAPI   WINAPI
     #define _MAPIMP
   #else
     #define _DLLCLASS
     #define _MAPAPI   WINAPI
     #define _MAPIMP   __declspec(dllimport)
   #endif
  #endif
#endif // BUILD_DLL

#ifdef WIN32API
typedef CRITICAL_SECTION MAPCRITICALSECTION;
#else
typedef pthread_mutex_t MAPCRITICALSECTION;
#endif

#ifdef CHECK_MEMORY

extern "C"
{
  _MAPIMP HANDLE _MAPAPI mapOpenFileUn(const WCHAR * name, int access, int mode, int attribute);
  _MAPIMP HANDLE _MAPAPI mapOpenTempFile(const WCHAR * name, int access, int mode, int attribute);
  _MAPIMP HANDLE _MAPAPI mapOpenFile(const char * name, int access, int mode, int attribute);
  _MAPIMP int    _MAPAPI mapCloseFile(HANDLE& hFile);

  _MAPIMP void _MAPAPI mapInitializeSection(MAPCRITICALSECTION * section);
  _MAPIMP void _MAPAPI mapDeleteSection(MAPCRITICALSECTION * section);
}

  INLINEFUNC HANDLE OpenTheFile(const char * name, int access = 0,
                                int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                                int attribute = OPEN_EXISTING)
  {
    return mapOpenFile(name, access, mode, attribute);
  }

  INLINEFUNC HANDLE OpenTheFileUn(const WCHAR * name, int access = 0,
                                  int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                                  int attribute = OPEN_EXISTING)
  {
    return mapOpenFileUn(name, access, mode, attribute);
  }

  INLINEFUNC HANDLE OpenTheTempFile(const WCHAR * name, int access = 0,
                                    int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                                    int attribute = OPEN_EXISTING)
  {
    return mapOpenTempFile(name, access, mode, attribute);
  }

  INLINEFUNC int CloseTheFile(HANDLE& hfile)
  {
    return mapCloseFile(hfile);
  }

  INLINEFUNC void InitializeTheCriticalSection(MAPCRITICALSECTION *section)
  {
    mapInitializeSection(section);
  }

  INLINEFUNC void DeleteTheCriticalSection(MAPCRITICALSECTION *section)
  {
    mapDeleteSection(section);
  }

#endif // CHECK_MEMORY

#ifdef WIN32API       // Windows GDI

#include <tchar.h>

#if defined(_M_X64) || defined(BUILD_DLL64)
typedef __int64 XDHANDLE;
#else
typedef unsigned long XDHANDLE;
#endif

typedef struct XDrawHandle
{
  XDHANDLE newhandle;
  XDHANDLE oldhandle;
}
  XDrawHandle;

typedef XDrawHandle  XHPEN;
typedef XDrawHandle  XHBRUSH;

#else

typedef long int   XHPEN;
typedef long int   XHBRUSH;

#endif


// MAPTRACER   - включение трассировки (1/0);
// TRACE_MEM   - трассировка работы с памятью;
// TRACE_MAP   - трассировка работы конструкторов,деструкторов,
//               доступа к району работ;
// TRACE_READ  - трассировка операций чтения;
// TRACE_WRITE - трассировка операций записи.

#if (MAPTRACER == 1)
#ifndef TRACE_MAP
 #define TRACE_MAP 1
#endif
#ifndef TRACE_MEM
 #define TRACE_MEM 1
#endif
#ifndef TRACE_READ
 #define TRACE_READ 0
#endif
#ifndef TRACE_WRITE
 #define TRACE_WRITE 0
#endif
#endif

#ifndef __BORLANDC__          // Кроме Borland C++
 #define strncmpi _strnicmp
#endif

 // преобразование символов к верхнему/нижнему регистрам
 // русские: Ё - 1025, А - 1040, Я - 1071, а - 1072, я - 1103, ё - 1105
 // английские: A - 65, Z - 90, a - 97, z - 122
#define _wdelta_rus 32 // а - А = 1072 - 1040
#define _wdelta_eng 32 // a - A = 97 - 65

#define _wtolower_rus(__c) ( ( (int)(__c) >= (int)1040 && (int)(__c) <= (int)1071 ) ? (WCHAR)((int)(__c) + (int)_wdelta_rus) : ( (int)(__c) == 1025 ? 1105 : (__c) ) )
#define _wtoupper_rus(__c) ( ( (int)(__c) >= (int)1072 && (int)(__c) <= (int)1103 ) ? (WCHAR)((int)(__c) - (int)_wdelta_rus) : ( (int)(__c) == 1105 ? 1025: (__c) ) )
#define _wtolower_eng(__c) ( ( (int)(__c) >= (int)65   && (int)(__c) <= (int)90   ) ? (WCHAR)((int)(__c) + (int)_wdelta_eng) : (__c) )
#define _wtoupper_eng(__c) ( ( (int)(__c) >= (int)97   && (int)(__c) <= (int)122  ) ? (WCHAR)((int)(__c) - (int)_wdelta_eng) : (__c) )
#define _wtolower(__c)     ( ( (int)(__c) >= (int)65   && (int)(__c) <= (int)90   ) ? _wtolower_eng(__c) : _wtolower_rus(__c) )
#define _wtoupper(__c)     ( ( (int)(__c) >= (int)97   && (int)(__c) <= (int)122  ) ? _wtoupper_eng(__c) : _wtoupper_rus(__c) )

// Перевести символ в верхний регистр
INLINEFUNC WCHAR ToUpperCase(WCHAR s)
{
  if (s < 'a') return s;

  // От 'a' до 'z'
  if (s >= 0x0061 && s <= 0x007A) return (WCHAR)(s-32);   // 0x0061  латиница
  if (s >= 0x0430 && s <= 0x044F) return (WCHAR)(s-32);   // 0x0430  русский
  if (s >= 0x0450 && s <= 0x045F) return (WCHAR)(s-80);   // 0x0450  кириллица (ё, є, ї, і)
  if (s >= 0x00E0 && s <= 0x00F6) return (WCHAR)(s-32);   // 0x00E0  латиница-1 (французский, ..., вьетнамский)
  if (s >= 0x00F8 && s <= 0x00FE) return (WCHAR)(s-32);   // 0x00F8  латиница-1 (французский, ..., вьетнамский)
  if (s >= 0x0561 && s <= 0x0586) return (WCHAR)(s-48);   // 0x0561  армянский
// В настоящее время используется только нижний регистр (0x10D0 - 0x10FC) - шрифт Sylfaen
//  if (s >= 0x10D0 && s <= 0x10F5) return (WCHAR)(s-48);   // 0x10D0  грузинский, абхазский

  if (s & 1)  // Нечетные строчные
  {
    if (s == 0x01A1 || s == 0x01F5) return (WCHAR)(s-1);  // 0x01A1  латиница-B (o' - вьетнамский, g')
    if (s >= 0x0101 && s <= 0x0137) return (WCHAR)(s-1);  // 0x0101  латиница-A (румынский, ..., вьетнамский)
    if (s >= 0x014B && s <= 0x0177) return (WCHAR)(s-1);  // 0x014B  латиница-A
    if (s >= 0x01DF && s <= 0x01EF) return (WCHAR)(s-1);  // 0x01DF  латиница-B
    if (s >= 0x01F9 && s <= 0x021F) return (WCHAR)(s-1);  // 0x01F9  латиница-B
    if (s >= 0x0223 && s <= 0x0233) return (WCHAR)(s-1);  // 0x0223  латиница-B
    if (s >= 0x0247 && s <= 0x024F) return (WCHAR)(s-1);  // 0x0247  латиница-B
    if (s >= 0x048B && s <= 0x04BF) return (WCHAR)(s-1);  // 0x048B  казахский, татарский
    if (s >= 0x1EA1 && s <= 0x1EF9) return (WCHAR)(s-1);  // 0x1EA1  вьетнамский
  }
  else
  {
    if (s == 0x01B0) return (WCHAR)(s-1);                 // 0x01B0  латиница-B (u' - вьетнамский)
    if (s >= 0x013A && s <= 0x0148) return (WCHAR)(s-1);  // 0x013A  латиница-A
    if (s >= 0x017A && s <= 0x017E) return (WCHAR)(s-1);  // 0x017A  латиница-A
    if (s >= 0x01CE && s <= 0x01DC) return (WCHAR)(s-1);  // 0x01CE  латиница-B
  }

  // Пока не обрабатываются:
  // 0x0180-0x024F  словенский, хорватский
  // 0x0270-0x03FF  греческий, коптский
  // 0x1100 ...     не проверялись (арабский, китайский, ...)
  return (WCHAR)s;
}

// Перевести символ в нижний регистр
INLINEFUNC WCHAR ToLowerCase(WCHAR s)
{
  if (s < 'A') return s;

  // От 'A' до 'Z'
  if (s >= 0x0041 && s <= 0x005A) return (WCHAR)(s+32);   // 0x0041  латиница
  if (s >= 0x0410 && s <= 0x042F) return (WCHAR)(s+32);   // 0x0410  русский
  if (s >= 0x0400 && s <= 0x040F) return (WCHAR)(s+80);   // 0x0400  кириллица (ё, є, ї, і)
  if (s >= 0x00C0 && s <= 0x00D6) return (WCHAR)(s+32);   // 0x00C0  латиница-1 (французский, ..., вьетнамский)
  if (s >= 0x00D8 && s <= 0x00DE) return (WCHAR)(s+32);   // 0x00D8  латиница-1 (французский, ..., вьетнамский)
  if (s >= 0x0531 && s <= 0x0556) return (WCHAR)(s+48);   // 0x0531  армянский
// В настоящее время используется только нижний регистр (0x10D0 - 0x10FC) - шрифт Sylfaen
//  if (s >= 0x10A0 && s <= 0x10C5) return (WCHAR)(s+48);   // 0x10A0  грузинский, абхазский

  if ((s & 1) == 0)  // Четные ПРОПИСНЫЕ
  {
    if (s == 0x01A0 || s == 0x01F4) return (WCHAR)(s+1);  // 0x01A0  латиница-B (o' - вьетнамский, g')
    if (s >= 0x0100 && s <= 0x0136) return (WCHAR)(s+1);  // 0x0100  латиница-A (румынский, ..., вьетнамский)
    if (s >= 0x014A && s <= 0x0176) return (WCHAR)(s+1);  // 0x014A  латиница-A
    if (s >= 0x01DE && s <= 0x01EE) return (WCHAR)(s+1);  // 0x01DE  латиница-B
    if (s >= 0x01F8 && s <= 0x021E) return (WCHAR)(s+1);  // 0x01F8  латиница-B
    if (s >= 0x0222 && s <= 0x0232) return (WCHAR)(s+1);  // 0x0222  латиница-B
    if (s >= 0x0246 && s <= 0x024E) return (WCHAR)(s+1);  // 0x0246  латиница-B
    if (s >= 0x048A && s <= 0x04BE) return (WCHAR)(s+1);  // 0x048A  казахский, татарский
    if (s >= 0x1EA0 && s <= 0x1EF8) return (WCHAR)(s+1);  // 0x1EA0  вьетнамский
  }
  else
  {
    if (s == 0x01AF) return (WCHAR)(s+1);                 // 0x01AF  латиница-B (u' - вьетнамский)
    if (s >= 0x0139 && s <= 0x0147) return (WCHAR)(s+1);  // 0x0139  латиница-A
    if (s >= 0x0179 && s <= 0x017D) return (WCHAR)(s+1);  // 0x0179  латиница-A
    if (s >= 0x01CD && s <= 0x01DB) return (WCHAR)(s+1);  // 0x01CD  латиница-B
  }

  // Пока не обрабатываются:
  // 0x0180-0x024F  словенский, хорватский
  // 0x0270-0x03FF  греческий, коптский
  // 0x1100 ...     не проверялись (арабский, китайский, ...)
  return (WCHAR)s;
}

// Перевести символ WCHAR в латинский
// (применять только для коррекции UnicodeToString)
INLINEFUNC char ToLatin(WCHAR w)
{
  if (w == 0) return 0;

  if (w < 0x0080) return (char)w;
  if (w > 0x1EF9) return '?';  // Символ для замены не найден
  if (w > 0x0233 && w < 0x1EA0) return '?';

  if (w < 0x00E0)
    {
      if (w >= 0x00C0 && w <= 0x00C6) return 'A';
      if (w == 0x00C7) return 'C';
      if (w >= 0x00C8 && w <= 0x00CB) return 'E';
      if (w >= 0x00CC && w <= 0x00CF) return 'I';
      if (w == 0x00D0) return 'D';
      if (w == 0x00D1) return 'N';
      if (w >= 0x00D2 && w <= 0x00D6) return 'O';
      if (w >= 0x00D9 && w <= 0x00DC) return 'U';
      if (w == 0x00DD) return 'Y';
      return (char)w;
    }

  if (w < 0x0100)
    {
      if (w >= 0x00E0 && w <= 0x00E6) return 'a';
      if (w == 0x00E7) return 'c';
      if (w >= 0x00E8 && w <= 0x00EB) return 'e';
      if (w >= 0x00EC && w <= 0x00EF) return 'i';
      if (w == 0x00F0) return 'd';
      if (w == 0x00F1) return 'n';
      if (w >= 0x00F2 && w <= 0x00F6) return 'o';
      if (w >= 0x00F9 && w <= 0x00FC) return 'u';
      if (w == 0x00FD) return 'y';
      return (char)w;
    }

  if (w < 0x0128)
    {
      if (w >= 0x0100 && w <= 0x0105) return (w & 1) ? 'a':'A';
      if (w >= 0x0106 && w <= 0x010D) return (w & 1) ? 'c':'C';
      if (w >= 0x010E && w <= 0x0111) return (w & 1) ? 'd':'D';
      if (w >= 0x0112 && w <= 0x011B) return (w & 1) ? 'e':'E';
      if (w >= 0x011C && w <= 0x0123) return (w & 1) ? 'g':'G';
      if (w == 0x0124 || w == 0x0126) return 'H';
      if (w == 0x0125 || w == 0x0127) return 'h';
      return '?';
    }

  if (w < 0x0168)
    {
      if (w >= 0x0128 && w <= 0x0131) return (w & 1) ? 'i':'I';
      if (w >= 0x0139 && w <= 0x0142) return (w & 1) ? 'L':'l';  // Заглавные - нечетные
      if (w >= 0x0143 && w <= 0x0148) return (w & 1) ? 'N':'n';  // Заглавные - нечетные
      if (w == 0x014A) return 'N';
      if (w == 0x014B) return 'n';
      if (w >= 0x014C && w <= 0x0153) return (w & 1) ? 'o':'O';
      if (w >= 0x0154 && w <= 0x0159) return (w & 1) ? 'r':'R';
      if (w >= 0x015A && w <= 0x0161) return (w & 1) ? 's':'S';
      if (w >= 0x0162 && w <= 0x0167) return (w & 1) ? 't':'T';
      return '?';
    }

  if (w < 0x01C0)
    {
      if (w >= 0x0168 && w <= 0x0173) return (w & 1) ? 'u':'U';
      if (w == 0x0174) return 'W';
      if (w == 0x0175) return 'w';
      if (w == 0x0176 || w == 0x01B3) return 'Y';
      if (w == 0x0177 || w == 0x01B4) return 'y';
      if (w >= 0x0179 && w <= 0x017E) return (w & 1) ? 'Z':'z';  // Заглавные - нечетные
      if (w == 0x01A0) return 'O';
      if (w == 0x01A1) return 'o';
      if (w >= 0x01AF && w <= 0x01B2) return (w & 1) ? 'U':'u';  // Заглавные - нечетные
      if (w == 0x01B5) return 'Z';
      if (w == 0x01B6) return 'z';
      return '?';
    }

  if (w < 0x01F8)
    {
      if (w == 0x01CD) return 'A';
      if (w == 0x01CE) return 'a';
      if (w == 0x01CF) return 'I';
      if (w == 0x01D0) return 'i';
      if (w == 0x01D1) return 'O';
      if (w == 0x01D2) return 'o';
      if (w >= 0x01D3 && w <= 0x01DC) return (w & 1) ? 'U':'u';  // Заглавные - нечетные
      if (w >= 0x01DE && w <= 0x01E3) return (w & 1) ? 'a':'A';
      if (w >= 0x01E4 && w <= 0x01F5) return (w & 1) ? 'g':'G';
      if (w == 0x01E8) return 'K';
      if (w == 0x01E9) return 'k';
      if (w >= 0x01EA && w <= 0x01ED) return (w & 1) ? 'o':'O';
      return '?';
    }

  if (w < 0x0218)
    {
      if (w == 0x01F8) return 'N';
      if (w == 0x01F9) return 'n';
      if (w >= 0x01FA && w <= 0x01FD) return (w & 1) ? 'a':'A';
      if (w >= 0x0200 && w <= 0x0203) return (w & 1) ? 'a':'A';
      if (w >= 0x0204 && w <= 0x0207) return (w & 1) ? 'e':'E';
      if (w >= 0x0208 && w <= 0x020B) return (w & 1) ? 'i':'I';
      if (w >= 0x020C && w <= 0x020F) return (w & 1) ? 'o':'O';
      if (w >= 0x0210 && w <= 0x0213) return (w & 1) ? 'r':'R';
      if (w >= 0x0214 && w <= 0x0217) return (w & 1) ? 'u':'U';
      return '?';
    }

  if (w < 0x1EA0)
    {
      if (w == 0x0218) return 'S';
      if (w == 0x0219) return 's';
      if (w == 0x021A) return 'T';
      if (w == 0x021B) return 't';
      if (w == 0x021E) return 'H';
      if (w == 0x021F) return 'h';
      if (w == 0x0224) return 'Z';
      if (w == 0x0225) return 'z';
      if (w == 0x0226) return 'A';
      if (w == 0x0227) return 'a';
      if (w == 0x0228) return 'E';
      if (w == 0x0229) return 'e';
      if (w >= 0x022A && w <= 0x0231) return (w & 1) ? 'o':'O';
      if (w == 0x0232) return 'Y';
      if (w == 0x0233) return 'y';
      return '?';
    }

  if (w >= 0x1EA0 && w <= 0x1EB7) return (w & 1) ? 'a':'A';
  if (w >= 0x1EB8 && w <= 0x1EC7) return (w & 1) ? 'e':'E';
  if (w >= 0x1EC8 && w <= 0x1ECB) return (w & 1) ? 'i':'I';
  if (w >= 0x1ECC && w <= 0x1EE3) return (w & 1) ? 'o':'O';
  if (w >= 0x1EE4 && w <= 0x1EF1) return (w & 1) ? 'u':'U';
  if (w >= 0x1EF2 && w <= 0x1EF9) return (w & 1) ? 'y':'Y';
  return '?';
}

// Создание и удаление с проверкой указателя и его последующим обнулением (и трассировкой)

// создание объекта (new) без объявления переменной
#define NEW_OBJ(_p, _T) try { _p = new _T; } catch(...) { _p=NULL; }
// создание массива (new[]) без объявления переменной
#define NEW_ARR(_p, _T, _s) try {_p = (_s) ? new _T[_s] : NULL; } catch(...){ _p=NULL; }

// удаление объекта (после new)
#define DELETE_OBJ(_x) if (_x != NULL) { delete _x; _x = NULL;  }
// удаление массива (после new[])
#define DELETE_ARR(_x) if (_x != NULL) { delete[] _x; _x = NULL;  }
//---------------------------------------------------------------------

#ifdef LINUXAPI

#define GetCharWidthA GetCharWidth

#ifdef SHORT_WCHAR
#define _wtoi    _wtoi_size2
#define wcslen   wcslen_size2
#define wcscmp   wcscmp_size2
#define wcscmpi  wcscmpi_size2
#define wcsncmp  wcsncmp_size2
#define wcsncmpi wcsncmpi_size2
#define wcsstr   wcsstr_size2
#define wcscpy   wcscpy_size2
#define wcsncpy  wcsncpy_size2
#define wcscat   wcscat_size2
#define wcsncat  wcsncat_size2
#define wcschr   wcschr_size2
#define wcstoul  wcstoul_size2
#define _itow    Itow
#endif

 // Аналоги системных функций для широких символов,
 // но wchar_t равно 2 байта
 // Только для внутреннего пользования!
 //------------------------------------------------------------------

#ifdef SHORT_WCHAR
 //-----------------------------------------------------------------
 // Конвертировать строку широких символов в длинное беззнаковое целое
 // В параметр endptr ничего не пишется обратно!!!
 //-----------------------------------------------------------------
 unsigned long int wcstoul(const WCHAR * inputPtr, WCHAR ** endptr, int base);

 // Функция  wcslen  является  эквивалентом  функции
 // strlen для широких символов. Она определяет
 // длину строки широких символов, на которую указывает s,
 // не учитывая конечный символ L'\0'
 int wcslen(const WCHAR *s);

 // Функция _wtoi  является  эквивалентом  функции
 // atoi для широких символов. Она преобразует строку
 // символов в целое число
 int _wtoi(const WCHAR *s);

 // Функция _wtof  является  эквивалентом  функции
 // atof для широких символов. Она преобразует строку
 // символов в число типа double
 double _wtof(const WCHAR *s);

 // Аналог функции _ltow для Windows
 // Возвращает в случае успеха - 1, иначе - 0
 int _ltow(long value, WCHAR *dest, int radix);

 // Функция  wcscmp  является  эквивалентом
 // функции strcmp для широких символов
 // Возвращает ноль, если строки, на которые указывают s1 и s2, равны
 // Иначе 1
 int wcscmp(const WCHAR *s1, const WCHAR *s2);

 // Функция  wcscmpi  является  эквивалентом
 // функции strcmpi для широких символов (без учета регистра)
 // Возвращает ноль, если строки, на которые указывают s1 и s2, равны
 // Иначе 1
 int wcscmpi(const WCHAR *s1, const WCHAR *s2);

 // Функция  wcsncmp  является  эквивалентом
 // функции strncmp для широких символов
 // Сравнивает только первые  n  символов  строки s1 с s2
 // Возвращает ноль, если равны
 // Иначе 1
 int wcsncmp(const WCHAR *s1, const WCHAR *s2, int n);

 // Функция  wcsncmpi  является  эквивалентом
 // функции strncmpi для широких символов
 // Сравнивает только первые  n  символов  строки s1 с s2 (без учета регистра)
 // Возвращает ноль, если равны
 // Иначе 1
 int wcsncmpi(const WCHAR *s1, const WCHAR *s2, int n);

 // Функция wcsstr является эквивалентом
 // функции strstr для широких символов.
 // Она ищет положение первой строки широких символов s2
 // (без завершающего символа L'\0')
 // в строке широких символов s1.
 // Возвращает указатель на первое появление подстроки s2 в s1.
 WCHAR *wcsstr(const WCHAR *s1, const WCHAR *s2);

 // Функция  wcscpy  является  эквивалентом
 // функции strcpy для широких символов.
 // Копирует  строку,  указанную как src
 // (включая завершающий символ '\0'), в
 // массив, указанный как dest.  Строки не могут
 // перекрываться, и в целевой строке  dest  должно
 // быть достаточно места для получения копии.
 // Возвращает указатель на целевую строку dest
 WCHAR *wcscpy(WCHAR *dest, const WCHAR *src);

 // Функция  wcsncpy  является  эквивалентом
 // функции strncpy для широких символов
 // Копируются только первые n символов строки src.
 // Таким образом, если нулевой байт отсутствует
 // в первых n байтах src, то результирующая строка
 // не будет завершена символом L'\0'.
 // Возвращает указатель на целевую строку dest
 WCHAR *wcsncpy(WCHAR *dest, const WCHAR *src, int n);

 // Функция  wcscat  является  эквивалентом  strcat  для  широких  символов
 // Она копирует строку широких символов, на которую указывает src, включая
 // конечный L'\0', в конец  строки  широких символов, на которую
 // указывает dest. Строки не должны перекрываться.
 // Программист   должен   убедиться  в  том,  что  в  dest  имеется
 // место, как минимум, для wcslen(dest)+wcslen(src)+1 широких символов.
 // Возвращает dest
 WCHAR *wcscat(WCHAR *dest, const WCHAR *src);

 // Функция  wcsncat  является  эквивалентом  strncat  для  широких  символов
 // Добавляются только первые n символов строки src
 // Таким образом, если нулевой байт отсутствует
 // в первых n байтах src, то результирующая строка
 // не будет завершена символом L'\0'. Строки не должны перекрываться.
 // Программист   должен   убедиться  в  том,  что  в  dest  имеется
 // место, как минимум, для wcslen(dest)+n+1 широких символов
 // Возвращает dest
 WCHAR *wcsncat(WCHAR *dest, const WCHAR *src, int n);

 // Функция wcschr возвращает указатель на первое вхождение
 // символа wc в строку ws
 // В случае неудачи возвращает 0
 WCHAR* wcschr(WCHAR *ws, WCHAR wc);
 const WCHAR* wcschr(const WCHAR *ws, WCHAR wc);

 // Функция swprintf является эквивалентом sprintf для широких символов
 // Поддерживаются все спецификации и модификаторы, кроме *.
 // Возвращает dest
 int swprintf(WCHAR *dest, const WCHAR *fmt, ...);

 // Функция _wfopen является эквивалентом fopen для широких символов
 FILE *_wfopen(const WCHAR *p1, const  WCHAR *p2);

 // Функция fwprintf является эквивалентом fprintf для широких символов
 int fwprintf(FILE *fp, const WCHAR *text);
#endif  // SHORT_WCHAR

#endif  // LINUXAPI

// --------------------------------------------------------------
// РЕАЛИЗАЦИЯ СИСТЕМНЫХ ФУНКЦИЙ
// --------------------------------------------------------------

// Копирует char строку в char строку
// в отличие от strncpy в буфер всегда будет записан завершающий 0
// Если буфер меньше строки, то строка обрезается
// src      - адрес входной строки
// dest     - адрес буфера для размещения выходной строки
// destsize - размер выходной строки в байтах
INLINEFUNC char* StrCopy(char *dest, const char *src, int destsize)
{
  if ((dest == 0) || (destsize <= 0)) return 0;
  if (src == 0) { *dest = 0; return 0; }
  int last = destsize - 1;
  dest[last] = 0;
#if defined(WIN32API) && !defined(__BORLANDC__)
  strncpy_s(dest, destsize, src, last);
  return dest;
#else
  return strncpy(dest, src, last);
#endif
}

// Копирует TCHAR строку в TCHAR строку
// Если буфер меньше строки, то строка обрезается
// src      - адрес входной строки
// dest     - адрес буфера для размещения выходной строки
// destsize - размер выходной строки в БАЙТАХ
#ifdef UNICODE
 #define TcsCopy WcsCopy
#else
 #define TcsCopy StrCopy
#endif

#define WSIZE2 2
#define WSIZE4 4

#ifdef LINUXAPI

#ifndef SHORT_WCHAR

  INLINEFUNC double Wtof(const wchar_t *s)
  {
    return _Wtof((char*)s, WSIZE4);
  }

  INLINEFUNC int Itow(int value, wchar_t *dest, int radix)
  {
    return _Itow(value, (char*)dest, 32, radix, WSIZE4);
  }

  INLINEFUNC int Itow(int value, wchar_t *dest, int destsize, int radix)
  {
    return _Itow(value, (char *)dest, destsize / WSIZE4, radix, WSIZE4);
  }

  INLINEFUNC int I64tow(int64_t value, wchar_t *dest, int radix)
  {
    return _I64tow(value, (char*)dest, 32, radix, WSIZE4);
  }

  INLINEFUNC int I64tow(int64_t value, wchar_t *dest, int destsize, int radix)
  {
    return _I64tow(value, (char *)dest, destsize / WSIZE4, radix, WSIZE4);
  }

  INLINEFUNC int Wtoi(const wchar_t *s)
  {
    return _Wtoi((char*)s, WSIZE4);
  }

  INLINEFUNC int Wcslen(const wchar_t* src)
  {
    return _Wcslen((const char*) src, WSIZE4);
  }

  INLINEFUNC int Wcscmp(const WCHAR *s1, const wchar_t *s2)
  {
    return _Wcscmp((char*)s1, (char*)s2, WSIZE2, WSIZE4);
  }

  INLINEFUNC int Wcscmp(const wchar_t *s1, const WCHAR *s2)
  {
    return _Wcscmp((char*)s1, (char*)s2, WSIZE4, WSIZE2);
  }

  INLINEFUNC int Wcsicmp(const WCHAR *s1, const wchar_t *s2)
  {
    return _Wcsicmp((char*)s1, (char*)s2, WSIZE2, WSIZE4);
  }

  INLINEFUNC int Wcsicmp(const wchar_t *s1, const WCHAR *s2)
  {
    return _Wcsicmp((char*)s1, (char*)s2, WSIZE4, WSIZE2);
  }

  INLINEFUNC int Wcsncmp(const WCHAR *s1, const wchar_t *s2, int n)
  {
    return _Wcsncmp((char*)s1, (char*)s2, n, WSIZE2, WSIZE4);
  }

  INLINEFUNC int Wcsncmp(const wchar_t *s1, const WCHAR *s2, int n)
  {
    return _Wcsncmp((char*)s1, (char*)s2, n, WSIZE4, WSIZE2);
  }

  INLINEFUNC int Wcsnicmp(const WCHAR *s1, const wchar_t *s2, int n)
  {
    return _Wcsnicmp((char*)s1, (char*)s2, n, WSIZE2, WSIZE4);
  }

  INLINEFUNC int Wcsnicmp(const wchar_t *s1, const WCHAR *s2, int n)
  {
    return _Wcsnicmp((char*)s1, (char*)s2, n, WSIZE4, WSIZE2);
  }

  INLINEFUNC WCHAR* Wcsncat(WCHAR *s1, const wchar_t *s2, int n)
  {
    return (WCHAR*)_Wcsncat((char*)s1, (char*)s2, n, WSIZE2, WSIZE4);
  }

  INLINEFUNC const WCHAR* Wcsstr(const WCHAR *s1, const wchar_t *s2)
  {
    return (const WCHAR*)_Wcsstr((char*)s1, (char*)s2, WSIZE2, WSIZE4);
  }

  INLINEFUNC WCHAR* Wcsstr(WCHAR *s1, const wchar_t *s2)
  {
    return (WCHAR*)_Wcsstr((char*)s1, (char*)s2, WSIZE2, WSIZE4);
  }

  INLINEFUNC const wchar_t *Wcschr(const wchar_t *s1, WCHAR s2)
  {
    return (const wchar_t *)_Wcschr((char *) s1, (char *)&s2, WSIZE4);
  }

  INLINEFUNC wchar_t *Wcschr(wchar_t *s1, WCHAR s2)
  {
    return (wchar_t *)_Wcschr((char *) s1, (char *)&s2, WSIZE4);
  }

  INLINEFUNC const wchar_t *Wcsrchr(const wchar_t *s1, WCHAR s2)
  {
    return (const wchar_t *)_Wcsrchr((char *) s1, (char *)&s2, WSIZE4);
  }

  INLINEFUNC wchar_t *Wcsrchr(wchar_t *s1, WCHAR s2)
  {
    return (wchar_t *) _Wcsrchr((char *) s1, (char *)&s2, WSIZE4);
  }

  INLINEFUNC FILE* Wfopen(const wchar_t *path, const WCHAR *mode)
  {
    return _Wfopen((char*)path, (char*)mode, WSIZE4, WSIZE2);
  }

  INLINEFUNC FILE* Wfopen(const WCHAR *path, const wchar_t *mode)
  {
    return _Wfopen((char*)path, (char*)mode, WSIZE2, WSIZE4);
  }

  INLINEFUNC FILE* Wfopen(const wchar_t *path, const wchar_t *mode)
  {
    return _Wfopen((char*)path, (char*)mode, WSIZE4, WSIZE4);
  }

  INLINEFUNC int MessageBoxWS(HWND hwnd, const WCHAR * message, const wchar_t * title, int flag)
  {
    return _MessageBoxW(hwnd, (char*)message, (char*)title, flag, WSIZE2, WSIZE4);
  }

  INLINEFUNC int MessageBoxWS(HWND hwnd, const wchar_t* message, const WCHAR * title, int flag)
  {
    return _MessageBoxW(hwnd, (char*)message, (char*)title, flag, WSIZE4, WSIZE2);
  }

  INLINEFUNC int MessageBoxWS(HWND hwnd, const wchar_t* message, const wchar_t * title, int flag)
  {
    return _MessageBoxW(hwnd, (char*)message, (char*)title, flag, WSIZE4, WSIZE4);
  }

  INLINEFUNC int Wcslwr(wchar_t * str)
  {
    return _Wcslwr((char*)str, WSIZE4);
  }

  INLINEFUNC int Wcsupr(wchar_t * str)
  {
    return _Wcsupr((char*)str, WSIZE4);
  }

#endif  // !SHORT_WCHAR

  INLINEFUNC double Wtof(const WCHAR *s)
  {
    return _Wtof((char*)s, WSIZE2);
  }

  INLINEFUNC int Itow(int value, WCHAR *dest, int radix)
  {
    return _Itow(value, (char*)dest, 32, radix, WSIZE2);
  }

  INLINEFUNC int Itow(int value, WCHAR *dest, int destsize, int radix)
  {
    return _Itow(value, (char *)dest, destsize / WSIZE2, radix, WSIZE2);
  }

  INLINEFUNC int I64tow(int64_t value, WCHAR *dest, int radix)
  {
    return _I64tow(value, (char*)dest, 32, radix, WSIZE2);
  }

  INLINEFUNC int I64tow(int64_t value, WCHAR *dest, int destsize, int radix)
  {
    return _I64tow(value, (char *)dest, destsize / WSIZE2, radix, WSIZE2);
  }

  INLINEFUNC int Wtoi(const WCHAR *s)
  {
    return _Wtoi((char*)s, WSIZE2);
  }

  INLINEFUNC int Wcslen(WCHAR* src)
  {
    return _Wcslen((char*) src, WSIZE2);
  }

  INLINEFUNC int Wcslen(const WCHAR* src)
  {
    return _Wcslen((char*) src, WSIZE2);
  }

  INLINEFUNC int Wcscmp(const WCHAR *s1, const WCHAR *s2)
  {
    return _Wcscmp((char*)s1, (char*)s2, WSIZE2, WSIZE2);
  }

  INLINEFUNC int Wcsicmp(const WCHAR *s1, const WCHAR *s2)
  {
    return _Wcsicmp((char*)s1, (char*)s2, WSIZE2, WSIZE2);
  }

  INLINEFUNC int Wcsncmp(const WCHAR *s1, const WCHAR *s2, int n)
  {
    return _Wcsncmp((char*)s1, (char*)s2, n, WSIZE2, WSIZE2);
  }

  INLINEFUNC int Wcsnicmp(const WCHAR *s1, const WCHAR *s2, int n)
  {
    return _Wcsnicmp((char*)s1, (char*)s2, n, WSIZE2, WSIZE2);
  }

  INLINEFUNC WCHAR* Wcsncat(WCHAR *s1, const WCHAR *s2, int n)
  {
    return (WCHAR*)_Wcsncat((char*)s1, (char*)s2, n, WSIZE2, WSIZE2);
  }

  INLINEFUNC const WCHAR* Wcsstr(const WCHAR *s1, const WCHAR *s2)
  {
    return (const WCHAR*)_Wcsstr((char*)s1, (char*)s2, WSIZE2, WSIZE2);
  }

  INLINEFUNC WCHAR* Wcsstr(WCHAR *s1, const WCHAR *s2)
  {
    return (WCHAR*)_Wcsstr((char*)s1, (char*)s2, WSIZE2, WSIZE2);
  }

  INLINEFUNC const WCHAR* Wcschr(const WCHAR *s1, WCHAR s2)
  {
    return (const WCHAR*)_Wcschr((char*)s1, (char*)&s2, WSIZE2);
  }

  INLINEFUNC WCHAR* Wcschr(WCHAR *s1, WCHAR s2)
  {
    return (WCHAR*)_Wcschr((char*)s1, (char*)&s2, WSIZE2);
  }

  INLINEFUNC const WCHAR* Wcsrchr(const WCHAR *s1, WCHAR s2)
  {
    return (const WCHAR*)_Wcsrchr((char*)s1, (char*)&s2, WSIZE2);
  }

  INLINEFUNC WCHAR* Wcsrchr(WCHAR *s1, WCHAR s2)
  {
    return (WCHAR*)_Wcsrchr((char*)s1, (char*)&s2, WSIZE2);
  }

  INLINEFUNC FILE* Wfopen(const WCHAR *path, const WCHAR *mode)
  {
    return _Wfopen((char*)path, (char*)mode, WSIZE2, WSIZE2);
  }

  INLINEFUNC int MessageBoxWS(HWND hwnd, const WCHAR * message, const WCHAR * title, int flag)
  {
    return _MessageBoxW(hwnd, (char*)message, (char*)title, flag, WSIZE2, WSIZE2);
  }

  INLINEFUNC int Wcslwr(WCHAR * str)
  {
    return _Wcslwr((char*)str, WSIZE2);
  }

  INLINEFUNC int Wcsupr(WCHAR * str)
  {
    return _Wcsupr((char*)str, WSIZE2);
  }

#define Wcscmpi Wcsicmp
#define WcsiCmp Wcsicmp
#define Wcsncmpi Wcsnicmp
#define WcsniCmp Wcsnicmp

#else

#define Wtof _wtof
#define Itow _itow
#define Wtoi _wtoi
#define Wcscmp wcscmp
#define Wcsncmp wcsncmp
#define Wcsstr wcsstr
#define Wfopen _wfopen
#define MessageBoxWS MessageBoxW
#define Wcsupr wcsupr
#define Wcslwr _wcslwr
#define Wcsncat wcsncat
#define Wcschr wcschr
#define Wcsrchr wcsrchr

// Сравнение строк без учета различий регистров
// Возвращает: value == 0 - строки равны
//             value  < 0 - строка s1 < s2
//             value  > 0 - строка s1 > s2
// При ошибке возвращает 1
INLINEFUNC int WcsiCmp(const WCHAR *s1, const WCHAR *s2)
{
  if ((s1 == 0) || (s2 == 0))
    return 1;

  // Сравнивать посимвольно (пока не найдены разные символы)
  while(1)
  {
    WCHAR c1 = ToUpperCase(*s1);  s1++;
    WCHAR c2 = ToUpperCase(*s2);  s2++;

    int delta = c1 - c2;
    if (delta != 0)
      return delta;

    // Выше не переставлять (одна строка может быть длиннее другой)
    if (c1 == 0)
      break;   // Обе строки закончились
  }

  return 0;
}

INLINEFUNC int WcsniCmp(const WCHAR *s1, const WCHAR *s2, int count)
{
  if ((s1 == 0) || (s2 == 0) || (count < 0))
    return 1;

  // Сравнивать посимвольно (пока не найдены разные символы)
  while(count-- > 0)
  {
    WCHAR c1 = ToUpperCase(*s1);  s1++;
    WCHAR c2 = ToUpperCase(*s2);  s2++;

    int delta = c1 - c2;
    if (delta != 0)
      return delta;

    // Выше не переставлять (одна строка может быть длиннее другой)
    if (c1 == 0)
      break;   // Обе строки закончились
  }

  return 0;
}

INLINEFUNC int Wcslen(const WCHAR* wstr)
{
  if (wstr == 0)
    return 0;
  return (int)wcslen(wstr);
}

#define Wcsicmp WcsiCmp
#define Wcscmpi WcsiCmp
#define Wcsnicmp WcsniCmp
#define Wcsncmpi WcsniCmp

#endif // LINUXAPI

// Копирует UNICODE строку в UNICODE строку
// Если буфер меньше строки, то строка обрезается
// src      - адрес входной строки UTF-16
// dest     - адрес буфера для размещения выходной строки UTF-16
// destsize - размер выходной строки в БАЙТАХ

INLINEFUNC WCHAR* WcsCopy(WCHAR *dest, const WCHAR *src, int destsize)
{
  // Если входная строка меньше 2-х байт, то даже некуда писать завершающий 0
  if ((dest == 0) || (destsize < sizeof(WCHAR)))
    return 0;
  if (src == 0) { *dest = 0; return 0; }
  if (dest == src)
    return dest;
  int last = destsize/sizeof(WCHAR) - 1;
  dest[last] = 0;
#ifdef WIN32API
#ifndef __BORLANDC__
  wcsncpy_s(dest, destsize/sizeof(WCHAR), src, last);
  return dest;
#else
  return wcsncpy(dest, src, last);
#endif
#else
  return (WCHAR*)_Wcsncpy((char*)dest, (const char*)src, last, WSIZE2, WSIZE2);
#endif
}

// Присоединяет WCHAR строку src к WCHAR строке dest
// Если свободная часть буфера dest меньше строки src, то строка обрезается
// src      - адрес входной строки UTF-16
// dest     - адрес буфера для размещения выходной строки UTF-16
// destsize - размер выходной строки в БАЙТАХ
INLINEFUNC WCHAR* WcsCat(WCHAR *dest, const WCHAR *src, int destsize)
{
  // Если входная строка меньше 2-х байт, то даже некуда писать завершающий 0
  if ((src == 0) || (dest == 0) || (destsize < sizeof(WCHAR)))
    return 0;
  int last = destsize/sizeof(WCHAR) - 1;
  dest[last] = 0;
#ifndef LINUXAPI
#ifndef __BORLANDC__
  wcsncat_s(dest, destsize/sizeof(WCHAR), src, last-wcslen(dest));
  return dest;
#else
  return wcsncat(dest, src, last-wcslen(dest));
#endif
#else
  return (WCHAR*)_Wcsncat((char*)dest, (const char*)src, last-Wcslen(dest), WSIZE2, WSIZE2);
#endif
}

#if defined(LINUXAPI) && !defined(SHORT_WCHAR)
INLINEFUNC WCHAR* WcsCopy(WCHAR *dest, const wchar_t *src, int destsize)
{
  // Если входная строка меньше 2-х байт, то даже некуда писать завершающий 0
  if ((dest == 0) || (destsize < 2)) return 0;
  if (src == 0) { *dest = 0; return 0; }
  int last = destsize/2 - 1;
  dest[last] = 0;
  return (WCHAR*)_Wcsncpy((char*)dest, (const char*)src, last, WSIZE2, WSIZE4);
}

INLINEFUNC WCHAR* WcsCat(WCHAR *dest, const wchar_t *src, int destsize)
{
  // Если входная строка меньше 2-х байт, то даже некуда писать завершающий 0
  if ((src == 0) || (dest == 0) || (destsize < 2))
    return 0;
  int last = destsize/2 - 1;
  dest[last] = 0;
  return (WCHAR*)_Wcsncat((char*)dest, (const char*)src, last - Wcslen(dest), WSIZE2, WSIZE4);
}
#endif

#define _WcsCat WcsCat

// Присоединяет char строку src к сhar строке dest
// Если свободная часть буфера dest меньше строки src, то строка обрезается
// src      - адрес входной строки
// dest     - адрес буфера для размещения выходной строки
// destsize - размер выходной строки в байтах
INLINEFUNC char* StrCat(char *dest, const char *src, int destsize)
{
  if (src == 0 || dest == 0 || destsize <= 0) return 0;
  dest[destsize - 1] = 0;
#if defined(WIN32API) && !defined(__BORLANDC__)
  strncat_s(dest, destsize, src, (size_t)destsize - 1 - strlen(dest));
  return dest;
#else
  return strncat(dest, src, destsize - 1 - strlen(dest));
#endif
}

// Присоединяет char строку src к сhar строке dest
// Если свободная часть буфера dest меньше строки src, то строка обрезается
// src      - адрес входной строки
// dest     - адрес буфера для размещения выходной строки
// destsize - размер выходной строки в байтах
INLINEFUNC char* _StrCat(char *dest, const char *src, int destsize)
{
  return StrCat(dest, src, destsize);
}


#ifdef UNICODE
 #define TcsCat WcsCat
#else
 #define TcsCat _StrCat
#endif

#ifdef SPARC_SOLARIS
INLINEFUNC size_t iconv(iconv_t cd, char **inbuf,  size_t *inbytes,
                        char **outbuf, size_t *outbytes)
{
  return iconv(cd, (const char **) inbuf, inbytes, outbuf, outbytes);
}
#endif // SPARC_SOLARIS

#ifdef LINUXAPI

enum CharEncoding
{
  CE_MIN,
  CE_CP1251,
  CE_CP1252,
  CE_CP1256,
  CE_KOI8,
  CE_UTF8,
  CE_UTF16LE,
  CE_UTF16BE,
  CE_UTF32LE,
  CE_UTF32BE,
  CE_MAX
};

#ifdef CONVERTERLIB
  #include "codec.h"
#endif

#ifdef _GISSERVER
  #define STRINGENCODING  CE_CP1251
#else
  #define STRINGENCODING  CE_KOI8
#endif

#ifndef NUMBERTURN
  #define UNICODEENCODING   CE_UTF16LE
  #define UNICODE32ENCODING  CE_UTF32LE
#else
  #define UNICODEENCODING   CE_UTF16BE
  #define UNICODE32ENCODING  CE_UTF32BE
#endif

#ifdef FILESYSTEMUTF8
  #define FSENCODING      CE_UTF8
#else
  #define FSENCODING      CE_KOI8
#endif

// Запрос кодировки в текстовом виде по перечислению
INLINEFUNC const char * GetEncodingText(int codecType)
{
  const char * retval = NULL;
  if (codecType > CE_MIN && codecType < CE_MAX)
  {
    switch(codecType)
    {
      case CE_KOI8:
        retval = "KOI8-R";
        break;
      case CE_CP1251:
        retval = "CP1251";
        break;
      case CE_CP1252:
        retval = "CP1252";
        break;
      case CE_CP1256:
        retval = "CP1256";
        break;
      case CE_UTF8:
        retval = "UTF-8";
        break;
      case CE_UTF16LE:
        retval = "UTF-16LE";
        break;
      case CE_UTF16BE:
        retval = "UTF-16BE";
        break;
      case CE_UTF32LE:
        retval = "UTF-32LE";
        break;
      case CE_UTF32BE:
        retval = "UTF-32BE";
        break;
      default:
        retval = NULL;
        break;
    }
  }
  return retval;
}

// Перекодировать строку с помощью iconv
// src      - исходная строка
// dest     - адрес буфера для размещения выходной строки
// inBytes  - указатель на переменную, содержащую размер входной строки в БАЙТАХ
//            После работы функции содержит число необработанных байт входного буфера
// outBytes - указатель на переменную, содержащую размер выходной строки в БАЙТАХ
//            После работы функции содержит число неиспользованных байт в выходном буфере
// fromCode - исходная кодировка
// toCode   - выходная кодировка
// При ошибке возвращает -1
INLINEFUNC long ConvertString(CharEncoding toCode, CharEncoding fromCode, char **src, size_t *inBytes, char **dest, size_t *outBytes )   // 07/09/18 Ганюшин
{
  long ret = -1;
  const char *destEncoding = GetEncodingText(toCode);
  const char *srcEncoding = GetEncodingText(fromCode);
  if (!destEncoding || !srcEncoding)
    return ret;

  iconv_t myCodec = iconv_open(destEncoding, srcEncoding);

  if ((long int)myCodec != -1)
  {
#ifdef ICONV_CONST_CHAR_INTERFACE
    const char **src1  = (const char **)src;
#else
    char **src1  = src;
#endif
    char *tempDest = *dest;
    char **dest1 = &tempDest;

    ret = iconv(myCodec, src1, inBytes, dest1, outBytes);
    iconv_close(myCodec);
  }

  return ret;
}

// Перекодировать строку c помощью
// toCode    - выходная кодировка
// fromCode  - исходная кодировка
// src       - исходная строка
// dest      - адрес буфера для размещения выходной строки
// srcBytes  - размер входной строки в БАЙТАХ
// destBytes - размер выходной строки в БАЙТАХ
INLINEFUNC long ConvertStringWrapper(CharEncoding toCode, CharEncoding fromCode, char *src, char *dest, size_t srcBytes, int *destBytes)   // 07/09/18 Ганюшин
{
  long ret = 0;
  size_t inBytes = srcBytes;
  size_t outBytes = *destBytes;

  if ((long int)src != (long int)dest)
    dest[0]= 0;

  char **inBuf  = &src;
  char **outBuf = &dest;

#ifdef CONVERTERLIB
  ret = panorama::converter::ConvertString(toCode, fromCode, inBuf, &inBytes, outBuf, &outBytes);
#else
  ret = ConvertString(toCode, fromCode, inBuf, &inBytes, outBuf, &outBytes);
#endif
  *destBytes = outBytes;
  if (ret == -1)
    ret = 0;
  else
    ret = 1;
  return ret;
}

#endif // LINUXAPI

// Преобразовать строку "char" в UNICODE
// src   - адрес входной строки ANSI
// dest  - адрес буфера для размещения выходной строки UTF-16
// bytes - размер выходной строки в БАЙТАХ
INLINEFUNC int StringToUnicode(const char * src, WCHAR * dest, int bytes)
{
  if ((dest == 0) || (bytes < 2)) return 0;
  if (src == 0) { *dest = 0; return 0; }
#ifdef WIN32API
#ifdef PANFR
  unsigned int code = 1252;        // ANSI Latin 1; Western European (Windows)
#else
  unsigned int code = CP_ACP;      // The current system Windows ANSI code page.
#endif
  int ret = MultiByteToWideChar(code, 0, src, -1, dest, bytes/2);
  dest[bytes/2-1] = 0;
  return ret;
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlen(src)+1;
  return ConvertStringWrapper(UNICODEENCODING, STRINGENCODING, inBuf, outBuf, srcBytes, &bytes);    // 07/09/18 Ганюшин
#endif  // WIN32API
}

// Преобразовать строку "char" в UNICODE
// src      - адрес входной строки ANSI
// srcsize  - размер входной строки в БАЙТАХ
// dest     - адрес буфера для размещения выходной строки UTF-16
// destsize - размер выходной строки в БАЙТАХ
// В случае успеха возвращает количество СИМВОЛОВ, записанных в выходной буфер,
// иначе - 0
INLINEFUNC int StringToUnicode(const char* src, int srcsize, WCHAR* dest, int destsize)
{
  if ((dest == 0) || (destsize < sizeof(WCHAR))) return 0;
  if (src == 0 || srcsize <= 0) { *dest = 0; return 0; }
  int last = destsize / sizeof(WCHAR) - 1;
  if (last == 0) { *dest = 0; return 1; }
  // Количество символов, записанных в выходной буфер
  int wcharCount = 0;
#ifdef WIN32API
#ifdef PANFR
  unsigned int code = 1252;        // ANSI Latin 1; Western European (Windows)
#else
  unsigned int code = CP_ACP;      // The current system Windows ANSI code page.
#endif
  wcharCount = MultiByteToWideChar(code, 0, src, srcsize, dest, last);
#else
  char* inBuf  = (char*)src;
  char* outBuf = (char*)dest;
  int   bytes  = destsize;
  if (ConvertStringWrapper(UNICODEENCODING, STRINGENCODING, inBuf, outBuf, srcsize, &bytes))
    wcharCount = (destsize - bytes) / sizeof(WCHAR);
#endif  // WIN32API
  // Добавляем терминальный символ в конец строки
  if (wcharCount > 0)
    dest[wcharCount] = 0;
  else
    dest[last] = 0;
  return wcharCount;
}

// Преобразовать строку "char" в кодировке CP1251
// в UNICODE
// src   - адрес входной строки ANSI
// dest  - адрес буфера для размещения выходной строки UTF-16
// bytes - размер выходной строки в БАЙТАХ
INLINEFUNC int AnsiStringToUnicode(const char * src, WCHAR * dest, int bytes)
{
#ifdef WIN32API
  return StringToUnicode(src, dest, bytes);
#else
  if ((dest == 0) || (bytes < 2)) return 0;
  if (src == 0) { *dest = 0; return 0; }

  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlen(src)+1;
  return ConvertStringWrapper(UNICODEENCODING, CE_CP1251, inBuf, outBuf, srcBytes, &bytes);  // 07/09/18 Ганюшин
#endif
}

// Преобразовать символы UNICODE в строку "char"
INLINEFUNC int UnicodeToString(const WCHAR * src, char * dest, int bytes)
{
  if ((dest == 0) || (bytes <= 0)) return 0;
  if (src == 0) { *dest = 0; return 0; }
#ifdef WIN32API
#ifdef PANFR
  unsigned int code = 1252;        // ANSI Latin 1; Western European (Windows)
#else
  unsigned int code = CP_ACP;      // The current system Windows ANSI code page.
#endif
  int ret = WideCharToMultiByte(code, 0, src, -1, dest, bytes, 0, 0);
  dest[bytes-1] = 0;

  for (int i=0; i < bytes; i++)
  {
    // Найден конец строки
    if (dest[i] == 0) return ret;

    // Найден ошибочный символ
    if (dest[i] == '?')
      dest[i] = ToLatin(src[i]);  // Коррекция символа (Европейские, Вьетнамские)
  }

  return ret;
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlenun((const char *)src, bytes * 2)+2;

  return ConvertStringWrapper(STRINGENCODING, UNICODEENCODING, inBuf, outBuf, srcBytes, &bytes);
#endif  // WIN32API
}

INLINEFUNC int UnicodeToString(const WCHAR * src, unsigned char * dest, int bytes)
{
  return UnicodeToString(src, (char *)dest, bytes);
}

INLINEFUNC int Unicode8ToString(const char * src, char * dest, int bytes)
{
  if ((dest == 0) || (bytes <= 0)) return 0;
  if (src == 0) { *dest = 0; return 0; }
#ifdef WIN32API
  WCHAR temp[4096];
  if (bytes > (int)(sizeof(temp) / 2))
    bytes = sizeof(temp)/2 - 1;
  MultiByteToWideChar(CP_UTF8, 0, src, -1, temp, bytes);
  return WideCharToMultiByte(CP_ACP, 0, temp, -1, dest, bytes, 0, 0);
#else
#ifndef LINUXAPI
  return (int)(StrCopy(dest, src, bytes) != 0);
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlen(src)+1;
  return ConvertStringWrapper(STRINGENCODING, CE_UTF8, inBuf, outBuf, srcBytes, &bytes);
#endif
#endif  // WIN32API
}

// Latin-1
INLINEFUNC int StringCP1252ToUnicode(const char * src, WCHAR * dest, int bytes)
{
  if ((dest == 0) || (bytes < 2)) return 0;
  if (src == 0) { *dest = 0; return 0; }
#ifdef WIN32API
  unsigned int code = 1252;        // ANSI Latin 1; Western European (Windows)
  int ret = MultiByteToWideChar(code, 0, src, -1, dest, bytes/2);
  dest[bytes/2-1] = 0;
  return ret;
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlen(src)+1;
  return ConvertStringWrapper(UNICODEENCODING, CE_CP1252, inBuf, outBuf, srcBytes, &bytes);
#endif  // WIN32API
}

// Arabic
INLINEFUNC int StringCP1256ToUnicode(const char * src, WCHAR * dest, int bytes)
{
  if ((dest == 0) || (bytes < 2)) return 0;
  if (src == 0) { *dest = 0; return 0; }
#ifdef WIN32API
  unsigned int code = 1256;        // ANSI Arabic
  int ret = MultiByteToWideChar(code, 0, src, -1, dest, bytes/2);
  dest[bytes/2-1] = 0;
  return ret;
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlen(src)+1;
  return ConvertStringWrapper(UNICODEENCODING, CE_CP1256, inBuf, outBuf, srcBytes, &bytes);
#endif
}

INLINEFUNC int Unicode8ToUnicode(const char * src, WCHAR * dest, int bytes)
{
  if ((dest == 0) || (bytes < sizeof(WCHAR))) return 0;
  if (src == 0) { *dest = 0; return 0; }
#ifdef WIN32API
  int last = bytes/sizeof(WCHAR) - 1;
  if (last == 0) { *dest = 0; return 1; }
  int ret = MultiByteToWideChar(CP_UTF8, 0, src, -1, dest, last);
  if (ret > 0) ret++;
  dest[last] = 0;
  return ret;
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlen(src)+1;
  return ConvertStringWrapper(UNICODEENCODING, CE_UTF8, inBuf, outBuf, srcBytes, &bytes);
#endif
}

// Преобразовать строку UTF-8 в UTF-16
// src      - адрес входной строки ANSI
// srcsize  - размер входной строки в БАЙТАХ
// dest     - адрес буфера для размещения выходной строки UTF-16
// destsize - размер выходной строки в БАЙТАХ
// В случае успеха возвращает количество СИМВОЛОВ, записанных в выходной буфер,
// иначе - 0
INLINEFUNC int Unicode8ToUnicode(const char* src, int srcsize, WCHAR* dest, int destsize)
{
  if ((dest == 0) || (destsize < sizeof(WCHAR))) return 0;
  if (src == 0 || srcsize <= 0)  { *dest = 0; return 0; }
  int last = destsize / sizeof(WCHAR) - 1;
  if (last == 0) { *dest = 0; return 1; }
  // Количество символов, записанных в выходной буфер
  int wcharCount = 0;
#ifdef WIN32API
  wcharCount = MultiByteToWideChar(CP_UTF8, 0, src, srcsize, dest, last);
#else
  char* inBuf  = (char*)src;
  char* outBuf = (char*)dest;
  int   bytes  = destsize;
  if(ConvertStringWrapper(UNICODEENCODING, CE_UTF8, inBuf, outBuf, srcsize, &bytes))
    wcharCount = (destsize - bytes) / sizeof(WCHAR);
#endif
  // Добавляем терминальный символ в конец строки
  if (wcharCount > 0)
    dest[wcharCount] = 0;
  else
    dest[last] = 0;
  return wcharCount;
}

INLINEFUNC int UnicodeToUnicode8(const WCHAR * src, int srcbytes, char * dest, int bytes)
{
  if ((dest == 0) || (bytes <= 0)) return 0;
  if (src == 0) { *dest = 0; return 0; }
#ifdef WIN32API
  return WideCharToMultiByte(CP_UTF8, 0, src, srcbytes, dest, bytes, 0, 0);
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes = srcbytes;
  return ConvertStringWrapper(CE_UTF8, UNICODEENCODING, inBuf, outBuf, srcBytes, &bytes);
#endif
}

INLINEFUNC int UnicodeToUnicode8(const WCHAR * src, char * dest, int bytes)
{
  if ((dest == 0) || (bytes <= 0)) return 0;
  if (src == 0) { *dest = 0; return 0; }
#ifdef WIN32API
  return WideCharToMultiByte(CP_UTF8, 0, src, -1, dest, bytes, 0, 0);
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlenun((const char *)src, bytes * 2)+2;
  return ConvertStringWrapper(CE_UTF8, UNICODEENCODING, inBuf, outBuf, srcBytes, &bytes);
#endif
}

// Преобразовать входную строку в UTF-8 и дописать к выходной строке
INLINEFUNC int UnicodeToUnicode8AndCat(const WCHAR * src, char * dest, int bytes)
{
  if ((src == 0) || (dest == 0) || (bytes <= 0)) return 0;
  int size = (int)strlen(dest);
  int tail = bytes - size;
  if (tail < 2) return 0;

  return UnicodeToUnicode8(src, dest+size, tail);
}

#ifdef LINUXAPI
#ifdef USE_UNICODE_COPYING_CONVERTER
// Преобразование входной строки UTF-32 в UTF-16 путем копирования кода символа (2 байта)
// src   - адрес входной строки UTF-32
// dest  - адрес буфера для размещения выходной строки UTF-16
// destSize - размер выходной строки в байтах
// При ошибке возвращает 0
INLINEFUNC int Unicode32ToUnicode16ByCopying(const wchar_t *src, WCHAR *dest, int destSize)
{
  if (!src || !dest || destSize < WSIZE2)
    return 0;

  int handledCount = 0;
  while (handledCount < (destSize / WSIZE2))
  {
    *dest = *src;
    handledCount++;

    if (*src == 0)
      break;

    src++;
    dest++;
  }

  return 1;
}

// Преобразовать входную строку UTF-16 в UTF-32 путем копирования кода символа (2 байта)
// src   - адрес входной строки UTF-16
// dest  - адрес буфера для размещения выходной строки UTF-32
// destSize - размер выходной строки в байтах
// При ошибке возвращает 0
INLINEFUNC int Unicode16ToUnicode32ByCopying(const WCHAR *src, wchar_t *dest, int destSize)
{
  if (!src || !dest || destSize < WSIZE4)
    return 0;

  int handledCount = 0;
  while (handledCount < (destSize / WSIZE4))
  {
    *dest = *src;
    handledCount++;

    if (*src == 0)
      break;

    src++;
    dest++;
  }

  return 1;
}

// Преобразовать входную строку UTF-32 в String через UTF-16 путем копирования кода символа (2 байта)
// src   - адрес входной строки UTF-32
// dest  - адрес буфера для размещения выходной строки String
// destSize - размер выходной строки в байтах
// При ошибке возвращает 0
INLINEFUNC int Unicode32ToStringByCopying(const wchar_t *src, char *dest, int destSize)
{
  if (!src || !dest || destSize < 1)
    return 0;

  WCHAR *unicode16String = NULL;
  NEW_ARR(unicode16String, WCHAR, destSize * WSIZE2);

  int isConverted = Unicode32ToUnicode16ByCopying(src, unicode16String, destSize * WSIZE2);
  if (isConverted)
    isConverted = UnicodeToString(unicode16String, dest, destSize);

  DELETE_ARR(unicode16String);

  return isConverted;
}
#endif  // USE_UNICODE_COPYING_CONVERTER

INLINEFUNC int Unicode16ToUnicode32(const WCHAR * src, wchar_t * dest, int bytes)
{
  if ((src == 0) || (dest == 0) || (bytes < 4))
    return 0;
#ifdef USE_UNICODE_COPYING_CONVERTER
  return Unicode16ToUnicode32ByCopying(src, dest, bytes);
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t inbytes  = (_Wcslen((const char *)src, WSIZE2) * 2)+2;

  return ConvertStringWrapper(UNICODE32ENCODING, UNICODEENCODING, inBuf, outBuf, inbytes, &bytes);
#endif
}

INLINEFUNC int Unicode32ToUnicode16(const wchar_t * src, WCHAR * dest, int bytes)
{
  if ((src == 0) || (dest == 0) || (bytes < 2))
    return 0;

#ifdef USE_UNICODE_COPYING_CONVERTER
  return Unicode32ToUnicode16ByCopying(src, dest, bytes);
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t inbytes  = (_Wcslen((const char *)src, WSIZE4) * 4)+4;

  return ConvertStringWrapper(UNICODEENCODING, UNICODE32ENCODING, inBuf, outBuf, inbytes, &bytes);
#endif
}

INLINEFUNC int Unicode32ToString(const wchar_t * src, char * dest, int bytes)
{
  if ((src == 0) || (dest == 0) || (bytes < 1))
    return 0;
#ifdef USE_UNICODE_COPYING_CONVERTER
  return Unicode32ToStringByCopying(src, dest, bytes);
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t inbytes  = (_Wcslen((const char *)src, WSIZE4) * 4)+4;

  return ConvertStringWrapper(STRINGENCODING, UNICODE32ENCODING, inBuf, outBuf, inbytes, &bytes);
#endif
}
#endif  // LINUXAPI

INLINEFUNC int StringToUnicode8(const char * src, char * dest, int bytes)
{
  if ((src == 0) || (dest == 0) || (bytes <= 0)) return 0;

#ifdef WIN32API
  WCHAR temp[4096];
  if (bytes > (int)sizeof(temp))
    bytes = sizeof(temp);

  StringToUnicode(src, temp, bytes);

  return WideCharToMultiByte(CP_UTF8, 0, temp, -1, dest, bytes, 0, 0);
#else
#ifndef LINUXAPI
  return (int)StrCopy(dest,src,bytes);
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlen(src)+1;

  return ConvertStringWrapper(CE_UTF8, STRINGENCODING, inBuf, outBuf, srcBytes, &bytes);
#endif
#endif  // WIN32API
}

INLINEFUNC int StringCP1251ToUnicode8(const char * src, char * dest, int bytes)
{
  if ((src == 0) || (dest == 0) || (bytes <= 0)) return 0;

#ifdef WIN32API
  WCHAR temp[4096];
  if (bytes > (int)sizeof(temp))
    bytes = sizeof(temp);

  StringToUnicode(src, temp, bytes);

  return WideCharToMultiByte(CP_UTF8, 0, temp, -1, dest, bytes, 0, 0);
#else
#ifndef LINUXAPI
  return (int)strncpy(dest,src,bytes);
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlen(src)+1;

  return ConvertStringWrapper(CE_UTF8, CE_CP1251, inBuf, outBuf, srcBytes, &bytes);
#endif
#endif  // WIN32API
}

INLINEFUNC int StringCP1251ToUnicode(const char * src, WCHAR * dest, int bytes)
{
  if ((dest == 0) || (bytes < 2)) return 0;
  if (src == 0) { *dest = 0; return 0; }
#ifdef WIN32API
  unsigned int code = 1251;
  int ret = MultiByteToWideChar(code, 0, src, -1, dest, bytes/2);
  dest[bytes/2-1] = 0;
  return ret;
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlen(src)+1;
  return ConvertStringWrapper(UNICODEENCODING, CE_CP1251, inBuf, outBuf, srcBytes, &bytes);
#endif  // WIN32API
}

INLINEFUNC int UnicodeToAnsiString(const WCHAR * src, char * dest, int bytes)
{
  if ((dest == 0) || (bytes <= 0)) return 0;
  if (src == 0) { *dest = 0; return 0; }
#ifdef WIN32API
  return UnicodeToString(src, dest, bytes);
#else
  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlenun((const char *)src, bytes * 2)+2;
  return ConvertStringWrapper(CE_CP1251, UNICODEENCODING, inBuf, outBuf, srcBytes, &bytes);
#endif
}

// Преобразовать входную строку UTF-16  в строку "char" в кодировке CP1251
// c заменой не кириллических символов на точку
// src   - адрес входной строки UTF-16
// dest  - адрес буфера для размещения выходной строки
// bytes - размер выходной строки в байтах
// При ошибке возвращает 0
INLINEFUNC int UnicodeToAnsiStringRu(const WCHAR * src, char * dest, int bytes)
{
  if ((dest == 0) || (bytes <= 0)) return 0;
  if (src == 0) { *dest = 0; return 0; }

#ifdef WIN32API
  return UnicodeToString(src, dest, bytes);
#else
  WCHAR processingString[1024]; processingString[0] = 0;
  WcsCopy(processingString, src, sizeof(processingString));

  int symbolCount = Wcslen(processingString);

  for (int symbolNumber = 0; symbolNumber <= symbolCount; symbolNumber++)
  {
    WCHAR currentSymbol = processingString[symbolNumber];

    // Заменить неконвертируемые символы пробелами
    if ((currentSymbol > 0x7f) && (currentSymbol < 0x0400 || currentSymbol > 0x045F))
      processingString[symbolNumber] = '.';
  }

  return UnicodeToAnsiString(processingString, dest, bytes);
#endif
}

INLINEFUNC int UnicodeToFSEncoding(const WCHAR * src, char * dest, int bytes)
{
#ifdef LINUXAPI
  if ((src == 0) || (dest == 0) || (bytes <= 0)) return 0;

  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlenun((const char *)src, bytes * 2)+2;

  return ConvertStringWrapper(FSENCODING, UNICODEENCODING, inBuf, outBuf, srcBytes, &bytes);
#else
  return UnicodeToUnicode8(src, dest, bytes);
#endif
}

INLINEFUNC int FSEncodingToUnicode(const char * src, WCHAR * dest, int bytes)
{
#ifdef LINUXAPI
  if ((src == 0) || (dest == 0) || (bytes < 2)) return 0;

  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlen(src)+1;

  return ConvertStringWrapper(UNICODEENCODING, FSENCODING, inBuf, outBuf, srcBytes, &bytes);
#else
  return Unicode8ToUnicode(src, dest, bytes);
#endif
}


#ifdef LINUXAPI

// Перевод UNICODE в CP1251
INLINEFUNC int Unicode8ToStringWindows(const char * src, char * dest, int bytes, int *outlength)
{
  if ((src == 0) || (dest == 0) || (bytes <= 0)) return 0;

  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlen(src)+1;
  int ret = ConvertStringWrapper(CE_CP1251, CE_UTF8, inBuf, outBuf, srcBytes, &bytes);
  outlength = &bytes;
  return ret;
}

INLINEFUNC int Unicode8ToStringCP1251(const char * src, char * dest, int bytes)
{
 return Unicode8ToStringWindows(src, dest, bytes, 0);
}

// Перевод CP1251 в KOI8-R
INLINEFUNC int StringWindowsToStringKoi8(const char * src, char * dest, int bytes)
{
  if ((src == 0) || (dest == 0) || (bytes <= 0)) return 0;

  char *inBuf  = (char *)src;
  char *outBuf = (char *)dest;
  size_t srcBytes  = strlen(src)+1;

  return ConvertStringWrapper(CE_KOI8, CE_CP1251, inBuf, outBuf, srcBytes, &bytes);
}

#ifndef HIDEMAXMIN
#ifndef max
  #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
  #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#endif

// Процедура strupr для LINUX
 INLINEFUNC char * strupr(char * string)
 {
   char * point;
   if ((point = string) == 0) return 0;
   while(*point)
   {
     *point = toupper(*point);
     point++;
   }
   return string;
 }

 // Процедура strupr для LINUX
 INLINEFUNC WCHAR * _wcsupr(WCHAR * string)
 {
   WCHAR * point;
   if ((point = string) == 0) return 0;
   while(*point)
   {
     *point = _wtoupper(*point);
     point++;
   }
   return string;
 }


#define _wcsnicmp wcsncmpi
#define _wcsicmp  wcscmpi
#endif  // LINUXAPI


// ПРЕОБРАЗОВАНИЕ ДЕСЯТИЧНОГО ЧИСЛА В СТРОКУ
INLINEFUNC int IntToStr(int value, char * buffer, int size)
{
  if (buffer == 0) return 0;
#ifdef WIN32API
 #ifndef __BORLANDC__
   return (_itoa_s(value, buffer, size, 10) == 0);
 #else
   return (itoa(value, buffer, 10) != 0);
 #endif
#else
  return (snprintf(buffer, size, "%i", value) > 0);
#endif
}

INLINEFUNC int IntToStrCat(int value, char* buffer, int size)
{
  if (buffer == 0) return 0;
  int asize = (int)strlen(buffer);
  int tail = size - asize;
  if (tail < 2)
    return 0;
#ifdef WIN32API
#ifndef __BORLANDC__
  return (_itoa_s(value, buffer + asize, tail, 10) == 0);
#else
  if (tail < 16)
    return 0;
  return (itoa(value, buffer + asize, 10) != 0);
#endif
#else
  return (snprintf(buffer+asize, tail, "%i", value) > 0);
#endif
}

INLINEFUNC int IntToStr16(int value, char * buffer, int size)
{
  if (buffer == 0) return 0;
#ifdef WIN32API
 #ifndef __BORLANDC__
   return (_itoa_s(value, buffer, size, 16) == 0);
 #else
   return (itoa(value, buffer, 16) != 0);
 #endif
#else
  return (snprintf(buffer, size, "%x", value) > 0);
#endif
}

INLINEFUNC int IntToStrCat16(int value, char* buffer, int size)
{
  if (buffer == 0) return 0;
  int asize = (int)strlen(buffer);
  int tail = size - asize;
  if (tail < 4)
    return 0;
#ifdef WIN32API
#ifndef __BORLANDC__
  return (_itoa_s(value, buffer + asize, tail, 16) == 0);
#else
  if (tail < 16)
    return 0;
  return (itoa(value, buffer, 16) != 0);
#endif
#else
  return (snprintf(buffer + asize, tail, "%x", value) > 0);
#endif
}

INLINEFUNC int IntToStr(int value, WCHAR * buffer, int size)
{
  if (buffer == 0) return 0;
#ifdef WIN32API
 #ifndef __BORLANDC__
  return (_itow_s(value, buffer, size / sizeof(WCHAR), 10) == 0);
 #else
   return (_itow(value, buffer, 10) != 0);
 #endif
#else
  return (Itow(value, buffer, size, 10) != 0);
#endif
}

INLINEFUNC int IntToStrCat(int value, WCHAR* buffer, int size)
{
  if (buffer == 0) return 0;
  int wsize = Wcslen(buffer);
  int tail = size / sizeof(WCHAR) - wsize;
  if (tail < 2)
    return 0;
#ifdef WIN32API
#ifndef __BORLANDC__
  return (_itow_s(value, buffer + wsize, tail, 10) == 0);
#else
  if (tail < 16)
    return 0;
  return (_itow(value, buffer + wsize, 10) != 0);
#endif
#else
  return (Itow(value, buffer + wsize, tail * sizeof(WCHAR), 10) != 0);
#endif
}

INLINEFUNC int IntToStr16(int value, WCHAR * buffer, int size)
{
  if (buffer == 0) return 0;
#ifdef WIN32API
 #ifndef __BORLANDC__
   return (_itow_s(value, buffer, size / sizeof(WCHAR), 16) == 0);
 #else
   return (_itow(value, buffer, 16) != 0);
 #endif
#else
  return (Itow(value, buffer, size, 16) != 0);
#endif
}

INLINEFUNC int IntToStrCat16(int value, WCHAR* buffer, int size)
{
  if (buffer == 0) return 0;
  int wsize = Wcslen(buffer);
  int tail = size / sizeof(WCHAR) - wsize;
  if (tail < 2)
    return 0;
#ifdef WIN32API
#ifndef __BORLANDC__
  return (_itow_s(value, buffer + wsize, tail, 16) == 0);
#else
  if (tail < 16)
    return 0;
  return (_itow(value, buffer, 16) != 0);
#endif
#else
  return (Itow(value, buffer + wsize, tail * sizeof(WCHAR), 16) != 0);
#endif
}

INLINEFUNC int Int64ToStr(__int64 value, char * buffer, int size)
{
  if (buffer == 0) return 0;
#ifdef WIN32API
 #ifndef __BORLANDC__
   return (_i64toa_s(value, buffer, size, 10) == 0);
 #else
   return (_i64toa(value, buffer, 10) != 0);
 #endif
#else
  #ifdef __QNX__
    return (snprintf(buffer, size, "%" PRIi64, value) > 0);
  #else
    return (snprintf(buffer, size, "%li", value) > 0);
  #endif
#endif
}

INLINEFUNC int Int64ToStrCat(__int64 value, char* buffer, int size)
{
  if (buffer == 0) return 0;
  int asize = (int)strlen(buffer);
  int tail = size - asize;
  if (tail <= 16)
    return 0;
#ifdef WIN32API
#ifndef __BORLANDC__
  return (_i64toa_s(value, buffer + asize, tail, 10) == 0);
#else
  return (_i64toa(value, buffer + asize, 10) != 0);
#endif
#else
  #ifdef __QNX__
    return (snprintf(buffer + asize, tail, "%" PRIi64, value) > 0);
  #else
    return (snprintf(buffer + asize, tail, "%li", value) > 0);
  #endif
#endif
}

INLINEFUNC int Int64ToStr16(__int64 value, char * buffer, int size)
{
  if (buffer == 0) return 0;
#ifdef WIN32API
 #ifndef __BORLANDC__
   return (_i64toa_s(value, buffer, size, 16) == 0);
 #else
   return (_i64toa(value, buffer, 16) != 0);
 #endif
#else
  #ifdef __QNX__
    return (snprintf(buffer, size, "%" PRIx64, value) > 0);
  #else
    return (snprintf(buffer, size, "%lx", value) > 0);
  #endif
#endif
}

INLINEFUNC int Int64ToStrCat16(__int64 value, char* buffer, int size)
{
  if (buffer == 0) return 0;
  int asize = (int)strlen(buffer);
  int tail = size - asize;
  if (tail <= 16)
    return 0;
#ifdef WIN32API
#ifndef __BORLANDC__
  return (_i64toa_s(value, buffer + asize, tail, 16) == 0);
#else
  return (_i64toa(value, buffer + asize, 16) != 0);
#endif
#else
  #ifdef __QNX__
    return (snprintf(buffer + asize, tail, "%" PRIx64, value) > 0);
  #else
    return (snprintf(buffer + asize, tail, "%lx", value) > 0);
  #endif
#endif
}

INLINEFUNC int Int64ToStr(__int64 value, WCHAR * buffer, int size)
{
  if (buffer == 0) return 0;
#ifdef WIN32API
 #ifndef __BORLANDC__
   return (_i64tow_s(value, buffer, size / sizeof(WCHAR), 10) == 0);
 #else
   return (_i64tow(value, buffer, 10) != 0);
 #endif
#else
  return I64tow(value, buffer, size, 10);
#endif
}

INLINEFUNC int Int64ToStrCat(__int64 value, WCHAR* buffer, int size)
{
  if (buffer == 0) return 0;
  int wsize = Wcslen(buffer);
  int tail = size / sizeof(WCHAR) - wsize;
  if (tail < 16)
    return 0;
#ifdef WIN32API
#ifndef __BORLANDC__
  return (_i64tow_s(value, buffer + wsize, tail, 10) == 0);
#else
  return (_i64tow(value, buffer + wsize, 10) != 0);
#endif
#else
  return I64tow(value, buffer + wsize, tail * sizeof(WCHAR), 10);
#endif
}

INLINEFUNC int Int64ToStr16(__int64 value, WCHAR * buffer, int size)
{
  if (buffer == 0) return 0;
#ifdef WIN32API
 #ifndef __BORLANDC__
   return (_i64tow_s(value, buffer, size / sizeof(WCHAR), 16) == 0);
 #else
   return (_i64tow(value, buffer, 16) != 0);
 #endif
#else
  return I64tow(value, buffer, size, 16);
#endif
}

INLINEFUNC int Int64ToStrCat16(__int64 value, WCHAR* buffer, int size)
{
  if (buffer == 0) return 0;
  int wsize = Wcslen(buffer);
  int tail = size / sizeof(WCHAR) - wsize;
  if (tail < 16)
    return 0;
#ifdef WIN32API
#ifndef __BORLANDC__
  return (_i64tow_s(value, buffer + wsize, tail, 16) == 0);
#else
  return (_i64tow(value, buffer + wsize, 16) != 0);
#endif
#else
  return I64tow(value, buffer + wsize, tail * sizeof(WCHAR), 16);
#endif
}

INLINEFUNC WCHAR * DoubleToStr(double value, unsigned int precision, WCHAR* buffer, int size)
{
  if (buffer == 0) return 0;

  char format[32];
  Snprintf(format, sizeof(format), "%%.%uf", precision);
  char temp[128];
  Snprintf(temp, sizeof(temp), format, value);
  StringToUnicode(temp, buffer, size);
  return buffer;
}

INLINEFUNC WCHAR* DoubleToStrCat(double value, unsigned int precision, WCHAR* buffer, int size)
{
  if (buffer == 0) return 0;

  char format[32];
  Snprintf(format, sizeof(format), "%%.%uf", precision);
  char temp[128];
  Snprintf(temp, sizeof(temp), format, value);
  WCHAR wtemp[128];
  StringToUnicode(temp, wtemp, sizeof(wtemp));
  return WcsCat(buffer, wtemp, size);
}

INLINEFUNC char* DoubleToStr(double value, unsigned int precision, char* buffer, int size)
{
  if (buffer == 0) return 0;

  char format[32];
  Snprintf(format, sizeof(format), "%%.%uf", precision);
  Snprintf(buffer, size, format, value);
  return buffer;
}

INLINEFUNC char* DoubleToStrCat(double value, unsigned int precision, char* buffer, int size)
{
  if (buffer == 0) return 0;

  char format[32];
  Snprintf(format, sizeof(format), "%%.%uf", precision);
  char temp[128];
  Snprintf(temp, sizeof(temp), format, value);
  return StrCat(buffer, temp, size);
}

// Запрос текущего времени в миллисекундах
// Аналог системной функции Windows
INLINEFUNC unsigned int GetTheTickCount()
{
  return GetTickCount();
}

// Запрос текущего времени в миллисекундах
// Аналог системной функции Windows
INLINEFUNC unsigned long long GetTheTickCount64()
{
#ifdef BUILD_DLL64
  return GetTickCount64();
#else
  return GetTickCount();
#endif
}

// РАБОТА С ПАМЯТЬЮ
// ----------------------

// Выделить физическую память
INLINEFUNC char * AllocateTheMemory(int size)
{
  char * memory;

  try
  {
    memory = new char[size];
  }
  catch (...)
  {
    memory = 0;
  }

#if TRACE_MEM == 1
  Tracer2("\n+++++ %p - Allocate memory block, size - %u\n", memory,size);
#endif

  return memory;
}

// Освободить физическую память
INLINEFUNC void FreeTheMemory(char * address)
{
#if TRACE_MEM == 1
  Tracer1("\n----- %p - Free memory block\n",address);
#endif

  if (address == 0) return;
  delete [] address;
}

INLINEFUNC int GetTheLastError()
{
#ifdef WIN32API
  int ret = ::GetLastError();
#else
  int ret = errno;
#endif

  return ret;
}

// РАБОТА С ФАЙЛАМИ
// ----------------------

// Добавить в конец пути слэш
INLINEFUNC void CompleteThePath(WCHAR * path, int size)
{
  if (path == 0)
    return;
  int length;
  if ((length = (int)Wcslen(path) - 1) < 0)
    return;
#ifdef WIN32API
  if (path[length] != '\\')
    WcsCat(path, WTEXT("\\"), size);
#else
  if (path[length] != '/')
    WcsCat(path, WTEXT("/"), size);
#endif
}

INLINEFUNC void CompleteThePath(char * path, int size)
{
  if (path == 0)
    return;
  int length;
  if ((length = (int)strlen(path) - 1) < 0)
    return;
#ifdef WIN32API
  if (path[length] != '\\')
    StrCat(path, "\\", size);
#else
  if (path[length] != '/')
    StrCat(path, "/", size);
#endif
}

INLINEFUNC void SplitThePath(const WCHAR *path, WCHAR *drive, WCHAR *dir, WCHAR *name, WCHAR *ext)
{
  if (path == 0)
    return;

#ifdef WIN32API
  _wsplitpath(path, drive, dir, name, ext);
#else
  // DRIVE
  if (drive)
    drive[0] = 0;

  if (dir)
    dir[0] = 0;

  if (name)
    name[0] = 0;

  if (ext)
    ext[0]  = 0;

  if (*path == 0)
    return;

  int first = 0;
  int begin = 0;
  int size = Wcslen(path);
  int end  = size;

  while((path[end] != '/') && (begin < end))
  {
    end--;
  }

  if ((begin < end) || ((begin == end) && (path[end] == '/')))
  {
    end++;

    if (dir)
    {
      _Wcsncpy((char*)dir, (char*)path, end, WSIZE2, WSIZE2);
      dir[end] = 0;
    }

    begin = end;
  }

  if (begin < (end = size))
  {
    //EXT
    while((path[end] != '.') && (begin < end))
    {
      end--;
    }

    if (begin < end)
    {
      if (name)
      {
        _Wcsncpy((char*)name, (char*)(path + begin), end - begin, WSIZE2, WSIZE2);
        name[end - begin] = 0;
      }

      if (ext && (end < size))
      {
        _Wcsncpy((char*)ext, (char*)(path + end), size - end, WSIZE2, WSIZE2);
        ext[size - end] = 0;
      }
    }
    else
    {
      if (name)
      {
        _Wcsncpy((char*)name, (char*)(path + end), size - end, WSIZE2, WSIZE2);
        name[size - end] = 0;
      }
    }
  }
#endif  // !WIN32API
}

INLINEFUNC void SplitThePath(const char *path, char *drive, char *dir, char *name, char *ext)
{
  if (path == 0)
    return;
#ifdef LINUXAPI
  int first = 0;
  int size, begin;
  int end;
  int drivesize;

  size = strlen(path);

  // DRIVE
  if (drive)
  {
    drive[0] = 0;
    drivesize = strcspn(path, ":");
    if (drivesize !=  size)  // строка содержит ':'
    {
      strncpy(drive, path, drivesize+1);
      drive[drivesize+1] = 0;
      first = drivesize+1;
    }
  }

  begin = first;
  end = size;

  if (dir)  dir[0] = 0;

  while((path[end] != '/') &&  (end > begin))
  {
    end--;
  }

  if (begin < end || ((begin == end) && (path[end] == '/')))
  {
    end++;

    if (dir)
    {
      strncpy(dir, path+begin, end-begin);
      dir[end-begin] = 0;
    }
    begin = end;
  }
  else
  {
    begin = first;
  }

  if (name)
    name[0] = 0;
  if (ext)
    ext[0]  = 0;

  if (begin < (end = size))
  {
    //EXT
    while((path[end] != '.') && (begin < end))
    {
      end--;
    }

    if (begin < end)
    {
      if (name)
      {
        strncpy(name, path+begin, end-begin);
        name[end-begin] = 0;
      }

      if(ext && (end < size))
      {
        strncpy(ext, path+end, size-end);
        ext[size-end] = 0;
      }
    }
    else
    {
      if (name)
      {
        strncpy(name, path + end, size - end);
        name[size - end] = 0;
      }
    }
  }

#else // if WIN32API
  _splitpath(path, drive, dir, name, ext);
#endif
}

// Сформировать путь по входным параметром
// path - выходной путь
// pathSize - размер пути
// drive - диск
// dir - путь к файлу
// name - имя файля
// ext - расширение файла
// В случае успеха возвращает 0, иначе код ошибки
INLINEFUNC int MakeThePathEx(WCHAR *path, int pathSize, const WCHAR *drive,
                             const WCHAR *dir, const WCHAR *name, const WCHAR *ext)
{
  if (path == 0)
    return -1;
  *path = 0;
#if defined(__BORLANDC__) || defined(LINUXAPI)
  if (drive != 0)
    WcsCat(path, drive, pathSize);
  if (dir != 0)
    WcsCat(path, dir, pathSize);
  if (name != 0)
    WcsCat(path, name, pathSize);
  if (ext != 0)
    WcsCat(path, ext, pathSize);
  return 0;
#else
  try { return _wmakepath_s(path, pathSize / sizeof(WCHAR), drive, dir, name, ext); }
  catch (...) { return -1; }
#endif
}

#if !defined(_GISSERVER) && !defined(BUILD_DLL)
INLINEFUNC void MakeThePath(WCHAR *path, const WCHAR *drive, const WCHAR *dir,
                            const WCHAR *name, const WCHAR *ext)
{
  if (path == 0)
    return;
  *path = 0;
#ifdef WIN32API
  _wmakepath(path, drive, dir, name, ext);
#else
  if (drive != 0)
    _Wcsncat((char*)path, (char*)drive, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE2);

  if (dir != 0)
    _Wcsncat((char*)path, (char*)dir, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE2);

  if (name != 0)
  {
    _Wcsncat((char*)path, (char*)name, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE2);

    if (ext != 0)
      _Wcsncat((char*)path, (char*)ext, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE2);
  }
#endif
}
#endif

#ifdef LINUXAPI
 #ifndef SHORT_WCHAR

INLINEFUNC int MakeThePathEx4(WCHAR *path, int pathSize, const WCHAR *drive,
                              const WCHAR *dir, const wchar_t *name, WCHAR *ext)
{
  if (path == 0)
    return -1;
  *path = 0;
#if defined(__BORLANDC__) || defined(LINUXAPI)
  *path = 0;
  if (drive != 0)
    WcsCat(path, drive, pathSize);
  if (dir != 0)
    WcsCat(path, dir, pathSize);
  if (name != 0)
    WcsCat(path, name, pathSize);
  if (ext != 0)
    WcsCat(path, ext, pathSize);
  return 0;
#else
  try { return _wmakepath_s(path, pathSize / sizeof(WCHAR), drive, dir, name, ext); }
  catch (...) { return -1; }
#endif
}

INLINEFUNC int MakeThePathEx4(WCHAR *path, int pathSize, const WCHAR *drive,
                              const WCHAR *dir, const WCHAR *name, wchar_t *ext)
{
  if (path == 0)
    return -1;
  *path = 0;
#if defined(__BORLANDC__) || defined(LINUXAPI)
  if (drive != 0)
    WcsCat(path, drive, pathSize);
  if (dir != 0)
    WcsCat(path, dir, pathSize);
  if (name != 0)
    WcsCat(path, name, pathSize);
  if (ext != 0)
    WcsCat(path, ext, pathSize);
  return 0;
#else
  try { return _wmakepath_s(path, pathSize / sizeof(WCHAR), drive, dir, name, ext); }
  catch (...) { return -1; }
#endif
}

INLINEFUNC int MakeThePathEx4(WCHAR *path, int pathSize, const WCHAR *drive,
                              const WCHAR *dir, const wchar_t *name, wchar_t *ext)
{
  if (path == 0)
    return -1;
  *path = 0;
#if defined(__BORLANDC__) || defined(LINUXAPI)
  if (drive != 0)
    WcsCat(path, drive, pathSize);
  if (dir != 0)
    WcsCat(path, dir, pathSize);
  if (name != 0)
    WcsCat(path, name, pathSize);
  if (ext != 0)
    WcsCat(path, ext, pathSize);
  return 0;
#else
  try { return _wmakepath_s(path, pathSize / sizeof(WCHAR), drive, dir, name, ext); }
  catch (...) { return -1; }
#endif
}

INLINEFUNC void MakeThePath4(WCHAR *path, const WCHAR *drive, const WCHAR *dir,
                            const wchar_t *name, const WCHAR *ext)
{
  if (path == 0)
    return;
  *path = 0;

  if (drive != 0)
    _Wcsncat((char*)path, (char*)drive, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE2);

  if (dir != 0)
    _Wcsncat((char*)path, (char*)dir, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE2);

  if(name != 0)
  {
    _Wcsncat((char*)path, (char*)name, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE4);

    if (ext != 0)
      _Wcsncat((char*)path, (char*)ext, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE2);
  }
}

INLINEFUNC void MakeThePath4(WCHAR *path, const WCHAR *drive, const WCHAR *dir,
                             const WCHAR *name, const wchar_t *ext)
{
  if (path == 0)
    return;
  *path = 0;

  if (drive != 0)
    _Wcsncat((char*)path, (char*)drive, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE2);

  if (dir != 0)
    _Wcsncat((char*)path, (char*)dir, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE2);

  if(name != 0)
  {
    _Wcsncat((char*)path, (char*)name, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE2);

    if (ext != 0)
      _Wcsncat((char*)path, (char*)ext, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE4);
  }
}

INLINEFUNC void MakeThePath4(WCHAR *path, const WCHAR *drive, const WCHAR *dir,
                            const wchar_t *name, const wchar_t *ext)
{
  if (path == 0)
    return;
  *path = 0;

  if (drive != 0)
    _Wcsncat((char*)path, (char*)drive, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE2);

  if (dir != 0)
    _Wcsncat((char*)path, (char*)dir, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE2);

  if(name != 0)
  {
    _Wcsncat((char*)path, (char*)name, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE4);

    if (ext != 0)
      _Wcsncat((char*)path, (char*)ext, MAX_PATH - Wcslen(path) - 1, WSIZE2, WSIZE4);
  }
}

 #endif //SHORT_WCHAR
#endif //LINUXAPI

#if !defined(_GISSERVER) && !defined(BUILD_DLL)
INLINEFUNC void MakeThePath(char *path, const char *drive, const char *dir,
                            const char *name, const char *ext)
{
#if defined(LINUXAPI)

  if (path == 0)
    return;
  *path = 0;

  if (drive)
    strncat(path, drive, MAX_PATH-strlen(path)-1);

  if (dir)
    strncat(path, dir, MAX_PATH-strlen(path)-1);

  if (name)
  {
    strncat(path, name, MAX_PATH-strlen(path)-1);

    if (ext)
      strncat(path, ext, MAX_PATH-strlen(path)-1);
  }

#else // if WIN32API
  _makepath(path,drive,dir,name,ext);
#endif
}
#endif

#ifndef CHECK_MEMORY
INLINEFUNC HANDLE OpenTheTempFile(const WCHAR * name,int access = 0, int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                                  int attribute = 0)
#else
INLINEFUNC HANDLE OpenTheTempFileCheck(const WCHAR * name,int access = 0,
                                       int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                                       int attribute = 0)
#endif
{
  HANDLE file;

#ifdef WIN32API
  if (name == 0)
    name = WTEXT("");
#else
  if (name == 0)
    return 0;
#endif

  if (attribute == 0)
    attribute = OPEN_ALWAYS;

#ifdef LINUXAPI
  if (attribute & O_CREAT)
    mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

  if (access == 0) access = O_RDWR;
  else
  {
    if (access & GENERIC_READ)
    {
      // Уберем GENERIC_READ(это наш флаг)
      access = access & 0xFFEFFFFF;
      if (access & GENERIC_WRITE)
      {
        // Уберем GENERIC_WRITE
        access = access & 0xFFFFFFFE;
        // Зададим аттрибуты файла на запись и чтение
        access = access | O_RDWR;
      }
    }
  }

  access=access|attribute;

  if ((access & O_CREAT) == 0) mode = 0;
#endif

#ifdef WIN32API
  if (access == 0)
    access = GENERIC_READ|GENERIC_WRITE;

  if ((file = ::CreateFileW(name, access, mode, NULL, attribute, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE,
                            0)) == INVALID_HANDLE_VALUE)
  {
    file = 0;
  }
#else

  #ifdef LINUXAPI
   char cname[1024]; cname[0] = 0;

   UnicodeToFSEncoding(name, cname, sizeof(cname));

   if ((int)(file = open(cname,access,mode)) == -1)
     file = 0;

   if ((access & O_CREAT) && file)
   {
     char        dir[MAX_PATH];
     struct stat statv;
     int         flag = (S_IXUSR|S_IXGRP|S_IXOTH);

     memset(dir, 0, MAX_PATH);
     SplitThePath(cname, 0, dir, 0, 0);
     if (dir[0])
     {
       // Узнаем права каталога на запись и чтение
       stat(dir, &statv);
     }
     else
       statv.st_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

     // Установим права созданного файла на запись и чтение
     chmod(cname, (statv.st_mode & ~flag));
   }
  #else
    file = 0;
  #endif

#endif

#if TRACE_MAP == 1
  char aname[MAX_PATH];
  UnicodeToString(name, aname, sizeof(aname));
  Tracer2("\n->>>> Open file : %s,"
          "\n      Handle    = 0x%04X\n", aname, (int)file);
#endif

  return file;
}

// Открыть файл для ассинхронного чтения и записи
INLINEFUNC HANDLE OpenTheOverlapFile(const char * name,int access = 0,
                                     int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                                     int attribute = 0)
{
  HANDLE file;

  if (name == 0)
    name = "";

  if (attribute == 0)
    attribute = OPEN_ALWAYS;

#ifdef LINUXAPI
  if (attribute & O_CREAT)
    mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

  if (access == 0) access = O_RDWR;
  else
  {
    if (access & GENERIC_READ)
    {
      // Уберем GENERIC_READ(это наш флаг)
      access = access & 0xFFEFFFFF;
      if (access & GENERIC_WRITE)
      {
        // Уберем GENERIC_WRITE
        access = access & 0xFFFFFFFE;
        // Зададим аттрибуты файла на запись и чтение
        access = access | O_RDWR;
      }
    }
  }

  access=access|attribute;

  if ((access & O_CREAT) == 0) mode = 0;
#endif

#ifdef WIN32API

  if (access == 0) access = GENERIC_READ|GENERIC_WRITE;

#ifdef UNICODE
  WCHAR tname[MAX_PATH];
  StringToUnicode(name,tname,sizeof(tname));
#endif

  if ((file = ::CreateFile(
#ifndef UNICODE
                            name,
#else
                            tname,
#endif
                            access,mode,NULL,
                            attribute,
                            FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
                            0)) == INVALID_HANDLE_VALUE)
    {
      file = 0;
    }
#else

  #ifdef LINUXAPI

  char cname[1028] = {0};
#ifdef FILESYSTEMUTF8
   StringToUnicode8(name, cname, sizeof(cname) - 1);
#else
#ifdef _GISSERVER
   // Кодировку CP1251 ГИС Сервера переводим
   // в кодировку KOI8-R файловой системы
   StringWindowsToStringKoi8(name, cname, sizeof(cname));
#else
   strcpy(cname, name);
#endif
#endif

   if ((int)(file = open(cname,access,mode)) == -1)
     file = 0;

   if ((access & O_CREAT) && file)
   {
     char        dir[MAX_PATH];
     struct stat statv;
     int         flag = (S_IXUSR|S_IXGRP|S_IXOTH);

     memset(dir, 0, MAX_PATH);
     SplitThePath(cname, 0, dir, 0, 0);
     if (dir[0])
     {
       // Узнаем права каталога на запись и чтение
       stat(dir, &statv);
     }
     else
       statv.st_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

     // Установим права созданного файла на запись и чтение
     chmod(cname, (statv.st_mode & ~flag));
   }
  #else
    file = 0;
  #endif

#endif

#if TRACE_MAP == 1
  Tracer2("\n->>>> Open file : %s,"
          "\n      Handle    = 0x%04X\n", name, (int)file);
#endif

  return file;
}

INLINEFUNC HANDLE OpenTheOverlapFile(const WCHAR * name,int access = 0,
                                 int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                                 int attribute = 0)
{
  HANDLE file;

#ifdef WIN32API
  if (name == 0) name = WTEXT("");
#else
  if (name == 0) return 0;
#endif

  if (attribute == 0)
    attribute = OPEN_ALWAYS;

#ifdef LINUXAPI
  if (attribute & O_CREAT)
    mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

  if (access == 0) access = O_RDWR;
  else
  {
    if (access & GENERIC_READ)
    {
      // Уберем GENERIC_READ(это наш флаг)
      access = access & 0xFFEFFFFF;
      if (access & GENERIC_WRITE)
      {
        // Уберем GENERIC_WRITE
        access = access & 0xFFFFFFFE;
        // Зададим аттрибуты файла на запись и чтение
        access = access | O_RDWR;
      }
    }
  }

  access=access|attribute;

  if ((access & O_CREAT) == 0)
    mode = 0;
#endif

#ifdef WIN32API

  if (access == 0)
    access = GENERIC_READ|GENERIC_WRITE;

  if ((file = ::CreateFileW(name, access,mode,NULL, attribute, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
                            0)) == INVALID_HANDLE_VALUE)
    {
      file = 0;
    }
#else

  #ifdef LINUXAPI
   char cname[1024]; cname[0] = 0;

   UnicodeToFSEncoding(name, cname, sizeof(cname));

   if ((int)(file = open(cname,access,mode)) == -1)
   {
     file = 0;
   }

   if ((access & O_CREAT) && file)
   {
     char        dir[MAX_PATH];
     struct stat statv;
     int         flag = (S_IXUSR|S_IXGRP|S_IXOTH);

     memset(dir, 0, MAX_PATH);
     SplitThePath(cname, 0, dir, 0, 0);
     if (dir[0])
     {
       // Узнаем права каталога на запись и чтение
       stat(dir, &statv);
     }
     else
       statv.st_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

     // Установим права созданного файла на запись и чтение
     chmod(cname, (statv.st_mode & ~flag));
   }
  #else
    file = 0;
  #endif

#endif

#if TRACE_MAP == 1
  char tname[1024];

  memset(tname, 0, sizeof(tname));
  UnicodeToString(name, tname, sizeof(tname)-1);
  tname[sizeof(tname)-1] = 0;

  Tracer2("\n->>>> Open file : %s,"
          "\n      Handle    = 0x%04X\n", tname, (int)file);
#endif

  return file;
}

// Открыть файл
#ifndef CHECK_MEMORY
INLINEFUNC HANDLE OpenTheFile(const char * name, int access = 0,
                              int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                              int attribute = OPEN_EXISTING)
#else
INLINEFUNC HANDLE OpenTheFileCheck(const char * name, int access = 0,
                                   int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                                   int attribute = OPEN_EXISTING)
#endif
{
  HANDLE file;

  if (name == 0) return 0;

  if (attribute == 0) attribute = OPEN_ALWAYS;

#ifdef LINUXAPI
  if (attribute & O_CREAT)
    mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

  if (access == 0) access = O_RDWR;
  else
  {
    if (access & GENERIC_READ)
    {
      // Уберем GENERIC_READ(это наш флаг)
      access = access & 0xFFEFFFFF;
      if (access & GENERIC_WRITE)
      {
        // Уберем GENERIC_WRITE
        access = access & 0xFFFFFFFE;
        // Зададим аттрибуты файла на запись и чтение
        access = access | O_RDWR;
      }
    }
  }

  access=access|attribute;

  if ((access & O_CREAT) == 0) mode = 0;
#endif

#ifdef WIN32API

  if (access == 0) access = GENERIC_READ|GENERIC_WRITE;

#ifdef UNICODE
  WCHAR tname[MAX_PATH];
  StringToUnicode(name,tname,sizeof(tname));
#endif

  if ((file = ::CreateFile(
#ifndef UNICODE
                            name,
#else
                            tname,
#endif
                            access,mode,NULL,
                            attribute,FILE_ATTRIBUTE_NORMAL,
                            0)) == INVALID_HANDLE_VALUE)
    {
      file = 0;
    }
#else

  #ifdef LINUXAPI
   char cname[1028];

   memset(cname, 0, sizeof(cname));
#ifdef FILESYSTEMUTF8
   StringToUnicode8(name, cname, sizeof(cname) - 1);
#else
#ifdef _GISSERVER
   // Кодировку CP1251 ГИС Сервера переводим
   // в кодировку KOI8-R файловой системы
   StringWindowsToStringKoi8(name, cname, sizeof(cname) - 1);
#else
   strcpy(cname, name);
#endif
#endif

   if ((int)(file = open(cname,access,mode)) == -1)
   {
     file = 0;
   }

   if ((access & O_CREAT) && file)
   {
     char        dir[MAX_PATH];
     struct stat statv;
     int         flag = (S_IXUSR|S_IXGRP|S_IXOTH);

     memset(dir, 0, MAX_PATH);
     SplitThePath(cname, 0, dir, 0, 0);
     if (dir[0])
     {
       // Узнаем права каталога на запись и чтение
       stat(dir, &statv);
     }
     else statv.st_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

     // Установим права созданного файла на запись и чтение
     chmod(cname, (statv.st_mode & ~flag));
   }
  #else
    file = 0;
  #endif

#endif

#if TRACE_MAP == 1
  Tracer2("\n->>>> Open file : %s,"
          "\n      Handle    = 0x%04X\n", name, (int)file);
#endif

  return file;
}

#ifndef CHECK_MEMORY
INLINEFUNC HANDLE OpenTheFileUn(const WCHAR * name, int access = GENERIC_READ|GENERIC_WRITE,
                                int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                                int attribute = OPEN_EXISTING)
#else
INLINEFUNC HANDLE OpenTheFileCheckUn(const WCHAR * name, int access = GENERIC_READ|GENERIC_WRITE,
                                     int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                                     int attribute = OPEN_EXISTING)
#endif
{
  HANDLE file;

  if (name == 0)
    return 0;

  if (attribute == 0) attribute = OPEN_ALWAYS;

#ifdef LINUXAPI
  if (attribute & O_CREAT)
    mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

  if (access == 0) access = O_RDWR;
  else
  {
    if (access & GENERIC_READ)
    {
      // Уберем GENERIC_READ(это наш флаг)
      access = access & 0xFFEFFFFF;
      if (access & GENERIC_WRITE)
      {
        // Уберем GENERIC_WRITE
        access = access & 0xFFFFFFFE;
        // Зададим аттрибуты файла на запись и чтение
        access = access | O_RDWR;
      }
    }
  }

  access=access|attribute;

  if ((access & O_CREAT) == 0) mode = 0;
#endif

#ifdef WIN32API
  if (access == 0) access = GENERIC_READ|GENERIC_WRITE;

  WCHAR longname[1024];
  int namelen = (int)wcslen(name);
  if (namelen >= 256)
    {
      WcsCopy(longname, WTEXT("\\\\?\\"), sizeof(longname));
      WcsCat(longname, name, sizeof(longname));
      name = longname;
    }

  if ((file = ::CreateFileW(name, access,mode,NULL,
                            attribute,FILE_ATTRIBUTE_NORMAL,
                            0)) == INVALID_HANDLE_VALUE)
    {
      file = 0;
    }
#else
   char cname[1024]; cname[0] = 0;
   UnicodeToFSEncoding(name, cname, sizeof(cname));

   if ((int)(file = open(cname,access,mode)) == -1)
     file = 0;

   if ((access & O_CREAT) && file)
   {
     char        dir[MAX_PATH];
     struct stat statv;
     int         flag = (S_IXUSR|S_IXGRP|S_IXOTH);

     memset(dir, 0, MAX_PATH);
     SplitThePath(cname, 0, dir, 0, 0);
     if (dir[0])
     {
       // Узнаем права каталога на запись и чтение
       stat(dir, &statv);
     }
     else statv.st_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

     // Установим права созданного файла на запись и чтение
     chmod(cname, (statv.st_mode & ~flag));
   }
#endif

#if TRACE_MAP == 1
   char tname[1024];
   memset(tname, 0, sizeof(tname));
   UnicodeToString(name, tname, sizeof(tname)-1);

   Tracer2("\n->>>> Open file : %s,"
          "\n      Handle    = 0x%04X\n", tname, (int)file);
#endif

  return file;
}

INLINEFUNC HANDLE OpenTheFile(const WCHAR * name, int access = GENERIC_READ|GENERIC_WRITE,
                              int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                              int attribute = OPEN_EXISTING)
{
#ifdef CHECK_MEMORY
  return mapOpenFileUn(name, access, mode, attribute);
#else
  return OpenTheFileUn(name, access, mode, attribute);
#endif
}

// Открыть файл на чтение
INLINEFUNC HANDLE OpenTheFileForRead(const char * name)
{
  return OpenTheFile(name, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING);
}

// Открыть файл на чтение
INLINEFUNC HANDLE OpenTheFileForRead(const WCHAR * name)
{
  return OpenTheFile(name, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING);
}

// Открыть файл в режиме разделения (Share mode)
// Именем файла должна быть строка в латинице в любой однобайтовой кодировке
INLINEFUNC HANDLE OpenTheShareFileLatin(const char * name,int access = 0,
                                        int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                                        int attribute = 0)
{
#ifdef CHECK_MEMORY
  return mapOpenFile(name, access, mode, attribute);
#else
  HANDLE file;

  if (name == 0) name = "";

#ifdef LINUXAPI
  if (attribute & O_CREAT)
    mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
#endif

  if ((mode & FILE_SHARE_WRITE) == 0)
    {
      // Доступ на запись монопольный
      return OpenTheFile(name,access,mode,attribute);
    }

  if (attribute == 0) attribute = OPEN_ALWAYS;

#ifdef LINUXAPI
  int fileLinux = 0;
  if (access == 0) access = O_RDWR;
  else
  {
    if (access & GENERIC_READ)
    {
      // Уберем GENERIC_READ(это наш флаг)
      access = access & 0xFFEFFFFF;
      if (access & GENERIC_WRITE)
      {
        // Уберем GENERIC_WRITE
        access = access & 0xFFFFFFFE;
        // Зададим аттрибуты файла на запись и чтение
        access = access | O_RDWR;
      }
    }
  }

  access = access|attribute;

  if ((access & O_CREAT) == 0) mode = 0;

  if ((fileLinux = open(name,access,mode)) == -1)
    fileLinux = 0;
  file = fileLinux;

  if ((access & O_CREAT) && file)
  {
     char        dir[MAX_PATH];
     struct stat statv;
     int         flag = (S_IXUSR|S_IXGRP|S_IXOTH);

     memset(dir, 0, MAX_PATH);
     SplitThePath(name, 0, dir, 0, 0);
     if (dir[0] != 0)
     {
       // Узнаем права каталога на запись и чтение
       stat(dir, &statv);
     }
     else statv.st_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

     // Установим права созданного файла на запись и чтение
     chmod(name, (statv.st_mode & ~flag));
  }
#else

  if (access == 0) access = GENERIC_READ|GENERIC_WRITE;

#ifdef UNICODE
  WCHAR tname[MAX_PATH];
  StringToUnicode(name,tname,sizeof(tname));
#endif

  if ((file = ::CreateFile(
#ifndef UNICODE
                           name,
#else
                           tname,
#endif
                           access, mode, NULL, attribute, FILE_FLAG_WRITE_THROUGH,
                           0)) == INVALID_HANDLE_VALUE)
    {
      file = 0;
    }
#endif // LINUXAPI

#if TRACE_MAP == 1
  Tracer2("\n->>>> Open file : %s,"
          "\n      Handle    = 0x%04X\n", name, (int)file);
#endif

  return file;
#endif
}

INLINEFUNC HANDLE OpenTheShareFile(const WCHAR * name,int access = 0,
                                   int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                                   int attribute = 0)
{
#ifdef CHECK_MEMORY
  return mapOpenFileUn(name, access, mode, attribute);
#else
  HANDLE file;

#ifdef WIN32API
  if (name == 0) name = WTEXT("");
#else
  if (name == 0) return 0;
#endif

#ifdef LINUXAPI
  if (attribute & O_CREAT)
    mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
#endif

  if ((mode & FILE_SHARE_WRITE) == 0)
    {
      // Доступ на запись монопольный
      return OpenTheFile(name,access,mode,attribute);
    }

  if (attribute == 0) attribute = OPEN_ALWAYS;

#ifdef LINUXAPI
  int fileLinux = 0;
  if (access == 0) access = O_RDWR;
  else
  {
    if (access & GENERIC_READ)
    {
      // Уберем GENERIC_READ(это наш флаг)
      access = access & 0xFFEFFFFF;
      if (access & GENERIC_WRITE)
      {
        // Уберем GENERIC_WRITE
        access = access & 0xFFFFFFFE;
        // Зададим аттрибуты файла на запись и чтение
        access = access | O_RDWR;
      }
    }
  }

  access = access|attribute;

  if ((access & O_CREAT) == 0) mode = 0;

  char cname[1024]; cname[0] = 0;

  UnicodeToFSEncoding(name, cname, sizeof(cname));

  if ((fileLinux = open(cname,access,mode)) == -1)
    fileLinux = 0;
  file = fileLinux;

  if ((access & O_CREAT) && file)
  {
     char        dir[MAX_PATH];
     struct stat statv;
     int         flag = (S_IXUSR|S_IXGRP|S_IXOTH);

     memset(dir, 0, MAX_PATH);
     SplitThePath(cname, 0, dir, 0, 0);
     if (dir[0])
     {
       // Узнаем права каталога на запись и чтение
       stat(dir, &statv);
     }
     else statv.st_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

     // Установим права созданного файла на запись и чтение
     chmod(cname, (statv.st_mode & ~flag));
  }
#else
  if (access == 0) access = GENERIC_READ|GENERIC_WRITE;

  WCHAR longname[1024];
  int namelen = (int)wcslen(name);
  if (namelen >= 256)
    {
      WcsCopy(longname, WTEXT("\\\\?\\"), sizeof(longname));
      WcsCat(longname, name, sizeof(longname));
      name = longname;
    }

  if ((file = ::CreateFileW(name,  access,mode, NULL,
                            attribute,FILE_FLAG_WRITE_THROUGH,
                            0)) == INVALID_HANDLE_VALUE)
    {
      file = 0;
    }
#endif // LINUXAPI

#if TRACE_MAP == 1
   char tname[1024];
   memset(tname, 0, sizeof(tname));
   UnicodeToString(name, tname, sizeof(tname)-1);

   Tracer2("\n->>>> Open file : %s,"
           "\n      Handle    = 0x%04X\n", tname, (int)file);
#endif

  return file;
#endif
}

// Закрыть файл
#ifndef CHECK_MEMORY
INLINEFUNC int CloseTheFile(HANDLE& file)
#else
INLINEFUNC int CloseTheFileCheck(HANDLE& file)
#endif
{
  int ret = 0;

#ifdef WIN32API

  if (file != 0)
    ret = ::CloseHandle(file);

#if TRACE_MAP == 1
  Tracer1("\n-<<<< Close for : 0x%04X\n", (int)file);
#endif


#else

 #ifdef LINUXAPI
  if (file > 0)
    if (close(file) == 0)
      ret = 1;
#if TRACE_MAP == 1
  Tracer1("\n-<<<< Close for : 0x%04X\n", (int)file);
#endif

  #endif

#endif

  file = 0;
  return ret;
}

// Создать файл
INLINEFUNC HANDLE CreateTheFile(const WCHAR * name)
{
  HANDLE hfile;

  if ((hfile = OpenTheFile(name, GENERIC_READ|GENERIC_WRITE,
                                 FILE_SHARE_READ, CREATE_ALWAYS)) == 0)
  {
    hfile = OpenTheFile(name, GENERIC_READ|GENERIC_WRITE,
                              FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_ALWAYS);
  }

  return hfile;
}

// Запросить, является ли заданный путь папкой
INLINEFUNC int IsTheDirectory(const WCHAR * name)
{
  int isDir = 0;
#ifdef WIN32API
  DWORD value = ::GetFileAttributesW(name);
  if (value != INVALID_FILE_ATTRIBUTES)
    isDir = value & FILE_ATTRIBUTE_DIRECTORY;
#else
  char filePathLocal[1024];  filePathLocal[0] = 0;
  UnicodeToFSEncoding(name, filePathLocal, sizeof(filePathLocal));
  struct stat st;
  if (lstat(filePathLocal, &st) == 0)
  {
    if (S_ISDIR(st.st_mode))
      isDir = 1;
  }
#endif
  return isDir;
}

// Проверить наличие файла
INLINEFUNC int ExistTheFileUn(const WCHAR * name)
{
  int ret;
  HANDLE handle;

  if (name == 0) return 0;

  handle = OpenTheFileUn(name, GENERIC_READ,
                         FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING);

#ifdef LINUXAPI
  if (((int)handle) <= 0)
  {
    ret = 0;
  }
  else
  {
    if(IsTheDirectory(name) != 0)
      ret = 0;
    else
      ret = 1;
    CloseTheFile(handle);
  }
#else
  ret = handle != 0;

  CloseTheFile(handle);
#endif

  return ret;
}

INLINEFUNC int ExistTheFile(const WCHAR * name)
{
  return ExistTheFileUn(name);
}

// Проверить наличие файла
INLINEFUNC int ExistTheFile(const char * name)
{
  int ret;
  HANDLE handle;

  if (name == 0) return 0;

  handle = OpenTheFile(name, GENERIC_READ,
                             FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING);

#ifdef LINUXAPI

  if (((int)handle) <= 0)
  {
    ret = 0;
  }
  else
  {
    ret = 1;
    CloseTheFile(handle);
  }

#else
  ret = handle != 0;

  CloseTheFile(handle);
#endif

  return ret;
}

// Проверить наличие файла, доступного на запись
INLINEFUNC int ExistTheFileForWrite(const char * name)
{
  if (name == 0) return 0;

  HANDLE handle = OpenTheFile(name, GENERIC_WRITE,
                              FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING);
  if (handle)
    {
      CloseTheFile(handle);
      return 1;
    }

  return 0;
}

// Проверить наличие файла, доступного на запись
INLINEFUNC int ExistTheFileForWriteUn(const WCHAR * name)
{
  if (name == 0) return 0;

  HANDLE handle = OpenTheFileUn(name, GENERIC_WRITE,
                                FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING);
  if (handle)
    {
      CloseTheFile(handle);
      return 1;
    }

  return 0;
}

// Открыть файл в режиме разделения (Share mode)
// Именем файла должна быть строка в латинице в любой однобайтовой кодировке
INLINEFUNC FILE * OpenTheTextFile(const WCHAR * name, const WCHAR * mode = NULL)
{
  if (!name)
    return 0;

#ifdef WIN32API
#ifndef __BORLANDC__
  FILE* pfile = 0;
  if (!mode)
  {
    if (_wfopen_s(&pfile, name, WTEXT("r")) == 0)
      return pfile;
  }
  else
  {
    if (_wfopen_s(&pfile, name, mode) == 0)
      return pfile;
  }

  return 0;
#else
  if (!mode)
    return _wfopen(name, WTEXT("r"));
  else
    return _wfopen(name, mode);
#endif
#else
  char cname[1024]; cname[0] = 0;
  UnicodeToFSEncoding(name, cname, sizeof(cname));
  char cmode[32]; cmode[0] = 0;
  if (!mode)
    *cmode = 'r';
  else
    UnicodeToString(mode, cmode, sizeof(cmode));

  return fopen(cname, cmode);
#endif
}

INLINEFUNC void CloseTheTextFile(FILE * handle)
{
  if (handle)
    fclose(handle);
}

// Считать блок данных из файла
INLINEFUNC int ReadTheFile(HANDLE file,char * address,long int length, long int * result = 0)
{
  int ret;

  if (file == 0)
    return 0;

#ifdef WIN32API
  DWORD dwret = 0;
  ret = ::ReadFile(file,address,length,&dwret,NULL);
  if (ret)
  {
    if (result)
      *result = dwret;
  }
#else
  ret = read(file,address,length);
  if (ret == -1)
  {
    ret = 0;
    if (result)
      *result = 0;
  }
  else
  {
    if (result)
      *result = ret;
    ret = 1;
  }
#endif

#if TRACE_READ == 1
  if (result)
    Tracer2("\n--->> Read for : %p, total = %u\n", file, *result);
#endif

  return ret;
}

#ifdef WIN32API
// Считать блок данных из файла асинхронно
INLINEFUNC int ReadTheOverlapFile(HANDLE file,char * address,long int length,
                                  long int * result, OVERLAPPED * overlap)
{
#ifdef WIN32API
  int ret = ::ReadFile(file,address,length,(LPDWORD)result,overlap);
#else
  int ret = read(file,address,length);
  if (ret == -1)
    {
      ret = 0;
      *result = ret;
    }
  else
  if (ret == 0)
    {
      *result = ret;
      ret = 1;
    }
  else
    {
      *result = ret;
      ret = 1;
    }
#endif

#if TRACE_READ == 1
  Tracer2("\n--->> Read for : 0x%04X, total = %u\n", (int)file,*ret);
#endif

  return ret;
}
#endif

// Установить смещение в файле перед чтением/записью
INLINEFUNC __int64 OffsetTheLongFile(HANDLE file, __int64 offset, int type)
{
  if (file == 0) return 0;

#ifdef WIN32API
  long int top64 = (long int)(offset >> 32);
  __int64 ret    = ::SetFilePointer(file, (long int)(offset & 0x0FFFFFFFF), &top64, type);
  if (ret == (unsigned int)0x0FFFFFFFF)
    {
      ret   = 0;
      top64 = 0;
    }

  ret += (((__int64)top64) << 32);
#else

#ifdef __USE_LARGEFILE64
  __int64 ret = lseek64(file,offset,type);
#else
  __int64 ret = lseek(file,offset,type);
#endif
  if (ret == -1) ret = 0;
#endif

#if TRACE_READ == 1
  Tracer2("\n===== Offset for: %p, offset = %p\n", file, ret);
#endif

  return ret;
}

// Установить смещение в файле перед чтением/записью
INLINEFUNC BIGINT OffsetTheFile(HANDLE file, BIGINT offset,int type)
{
  return OffsetTheLongFile(file, offset, type);
}

// Запросить длину файла
INLINEFUNC __int64 LengthTheLongFile(HANDLE file)
{
#ifdef WIN32API
  __int64 ret = OffsetTheLongFile(file, 0, FILE_END);
#else
  __int64 ret;

  if (file != 0)
  {
#ifdef __USE_LARGEFILE64
    ret = lseek64(file,0,SEEK_END);
#else
    ret = lseek(file,0,SEEK_END);
#endif
    if (ret == -1) ret = 0;
  }
  else
  {
    ret = 0;
  }
#endif

  return ret;
}

INLINEFUNC __int64 LengthTheLongFile(const WCHAR * name)
{
  if (name == 0)
    return 0;

  HANDLE hfile = OpenTheFileForRead(name);
  if (hfile == 0)
    return 0;
  __int64 ret = LengthTheLongFile(hfile);
  CloseTheFile(hfile);
  return ret;
}

// Запросить длину файла
INLINEFUNC BIGINT LengthTheFile(HANDLE file)
{
  return LengthTheLongFile(file);
}

INLINEFUNC __int64 LengthTheFile(const WCHAR * name)
{
  return LengthTheLongFile(name);
}

INLINEFUNC double LengthTheBigFile(HANDLE file)
{
  double ret = 0;

#ifdef WIN32API
  if (file != 0)
    {
      LARGE_INTEGER filesize;

      if (GetFileSizeEx(file, &filesize) != 0)
        ret = (double)filesize.QuadPart;
    }
#else
  if (file != 0)
    {
#ifdef __USE_LARGEFILE64
      ret = lseek64(file,0,SEEK_END);
#else
      ret = lseek(file,0,SEEK_END);
#endif
      if (ret == -1) ret = 0;
    }
#endif

  return ret;
}

// Установить новую длину файла
INLINEFUNC int EndTheFile(HANDLE file)
{
  int ret;
#ifdef WIN32API
  if (file != 0)
    {
      ret = ::SetEndOfFile(file);
    }
  else
    {
      ret = 0;
    }
#else
  if (file != 0)
    {
#ifdef __USE_LARGEFILE64
      ret = lseek64(file,0,SEEK_CUR);
#else
      ret = lseek(file,0,SEEK_CUR);
#endif
#ifdef WATCOM
      ret = ltrunc(file,ret,0);
#else
      ret = ftruncate(file,ret);
#endif
      if (ret == -1) ret = 0;
      else ret = 1;
    }
    else
    {
      ret = 0;
    }
#endif

#if TRACE_WRITE == 1
  Tracer1("\n----< End   for : 0x%04X\n", file);
#endif

  return ret;
}

// Записать блок данных в файл
INLINEFUNC int WriteTheFile(HANDLE file, const char * address, long int length, long int * result)
{
  if ((file == 0) || (address == 0) || (length <= 0))
    return 0;

#ifdef WIN32API
  DWORD dwret = 0;
  int ret = ::WriteFile(file,address,length,&dwret,NULL);
  if (ret)
  {
    if (result)
      *result = dwret;
  }
#else
  int ret = write(file, address, length);
  if (ret == -1)
  {
    ret = 0;
    if (result)
      *result = 0;
  }
  else
  if (ret == 0)
  {
    ret = 1;
    if (result)
      *result = 0;
  }
  else
  {
    if (result)
      *result = ret;
    ret = 1;
  }
#endif  // WIN32API

#if TRACE_WRITE == 1
  if (result)
    Tracer2("\n---<< Write for : 0x%04X, total = %lu\n", file, result);
#endif

  return ret;
}

// Записать блок данных в файл
INLINEFUNC int WriteTheFile(HANDLE file, const char * address, long int length)
{
  long int result;
  return (WriteTheFile(file, address, length, &result) != 0) && (result == length);
}

// Записать блок данных в файл (возможна запись больше 4ГБ)
INLINEFUNC int WriteTheBigFile(HANDLE file, const char * address, BIGINT length)
{
  if ((file == 0) || (address == 0) || (length <= 0))
    return 0;

  while(length > 0)
  {
    unsigned int block;
    if (length > 0x01000000)
      block = 0x01000000;
    else
      block = (unsigned int)length;

    if (WriteTheFile(file, address, block) == 0)
      return 0;

    address += block;
    length  -= block;
  }

  return 1;
}

#ifdef WIN32API
// Записать блок данных в файл асинхронно
INLINEFUNC int WriteTheOverlapFile(HANDLE file,const char * address,long int length,
                                   long int * result, OVERLAPPED * overlap)
{
  int ret;

#ifdef WIN32API
  ret = ::WriteFile(file,address,length,(LPDWORD)result,overlap);
#else
  ret = write(file,address,length);
  if (ret == -1)
  {
    ret = 0;
    *result = ret;
  }
  else
  if (ret == 0)
  {
    ret = 1;
    *result = 0;
  }
  else
  {
    *result = ret;
    ret = 1;
  }
#endif  // WIN32API

#if TRACE_WRITE == 1
  Tracer2("\n---<< Write for : 0x%04X, total = %lu\n", file,result);
#endif

 return ret;
}
#endif

// Сохранить буферы записи данных в файл на диск
INLINEFUNC void FlushTheFileBuffers(HANDLE file)
{
  if (file == 0) return;
#ifdef WIN32API
  FlushFileBuffers(file);
#else
  fsync(file);
#endif
}

INLINEFUNC int SetTheFileAttributes(const char * name, long int attributes)
{
#ifdef WIN32API
  return ::SetFileAttributesA(name,(DWORD)attributes);
#else
  int ret = chmod(name,attributes);
  if (ret == 0) ret = 1;
  else ret = 0;

  return ret;
#endif
}

INLINEFUNC int SetTheFileAttributes(const WCHAR * name, long int attributes)
{
#ifdef WIN32API
  return ::SetFileAttributesW(name,(DWORD)attributes);
#else
  char cname[1024];
  memset(cname, 0, sizeof(cname));
  UnicodeToFSEncoding(name, cname, sizeof(cname));

  return SetTheFileAttributes(cname, attributes);
#endif
}

INLINEFUNC int SetTheFileWriteAttributes(const char* name, int flag)
{
#ifdef WIN32API
  DWORD value = ::GetFileAttributesA(name);
  if (flag == 0)
    value |= FILE_ATTRIBUTE_READONLY;
  else
    value &= ~FILE_ATTRIBUTE_READONLY;

  return SetTheFileAttributes(name, value);
#else
  int isModeChanged;
  if (flag)
    isModeChanged = chmod(name, (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
  else
    isModeChanged = chmod(name, (S_IRUSR | S_IRGRP | S_IROTH));

  return (isModeChanged == 0);
#endif
}

INLINEFUNC int SetTheFileWriteAttributes(const WCHAR * name, int flag)
{
#ifdef WIN32API
  DWORD value = ::GetFileAttributesW(name);
  if (flag == 0)
    value |= FILE_ATTRIBUTE_READONLY;
  else
    value &= ~FILE_ATTRIBUTE_READONLY;

  return SetTheFileAttributes(name, value);
#else
  // Атрибуты файла
  char cname[MAX_PATH]; cname[0] = 0;
  UnicodeToFSEncoding(name, cname, sizeof(cname));

  int isModeChanged;
  if (flag)
    isModeChanged = chmod(cname, (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
  else
    isModeChanged = chmod(cname, (S_IRUSR | S_IRGRP | S_IROTH));

  return (isModeChanged == 0);
#endif
}

INLINEFUNC int GetTheFileWriteAttributes(const WCHAR * name)
{
#ifdef WIN32API
  DWORD value = ::GetFileAttributesW(name);
  return ((value & FILE_ATTRIBUTE_READONLY) == 0);
#else
  //  Атрибуты файла
  char cname[MAX_PATH]; cname[0] = 0;
  struct stat stbuf;
  FILE *fp;
  memset(&stbuf, 0, sizeof(stbuf));

  UnicodeToFSEncoding(name, cname, sizeof(cname));
  if ((fp=fopen(cname, "rb")) != NULL)
  {
    if (fstat(fileno(fp), &stbuf) == -1)
    {
      fclose(fp);
      return 0;
    }
    fclose(fp);
    return (stbuf.st_mode & (S_IWGRP | S_IWRITE | S_IWOTH));
  }
  return 0;
#endif
}

INLINEFUNC int DeleteTheFile(const WCHAR * name)
{
  int ret = 0;
  if ((name == 0) || (*name == 0))
    return 0;

#ifdef WIN32API
  WCHAR longname[1024];
  if (wcslen(name) > MAX_PATH)
    {
      WcsCopy(longname, WTEXT("\\\\?\\"), sizeof(longname));
      WcsCat(longname, name, sizeof(longname));
      name = longname;
    }
  ret = ::DeleteFileW(name);
  if (ret == 0)
  {
    DWORD error = ::GetLastError();
    if (error == ERROR_FILE_NOT_FOUND)
      return -1;
  }
#else
  char dest[MAX_PATH]; dest[0] = 0;

  UnicodeToFSEncoding(name, dest, sizeof(dest));

  if (unlink(dest) == 0)
  {
    ret = 1;
  }
  else
  {
    if (errno == ENOENT)
      ret = -1;
  }
#endif

#if TRACE_MAP == 1
  char aname[MAX_PATH];
  UnicodeToString(name, aname, sizeof(aname));
#endif

  return ret;
}

INLINEFUNC int DeleteTheFileUn(const WCHAR * name)
{
  return DeleteTheFile(name);
}

INLINEFUNC int DeleteTheFileEx(const WCHAR* name, int force = 0)
{
  if (force)
    SetTheFileWriteAttributes(name, 1);
  return DeleteTheFile(name);
}

INLINEFUNC int DeleteTheFile(const char * name)
{
#ifdef WIN32API
  int ret = ::DeleteFileA(name);
  if (ret == 0)
  {
    DWORD error = ::GetLastError();
    if (error == ERROR_FILE_NOT_FOUND)
      return -1;
  }
#else
  char dest[MAX_PATH];
  memset(dest, 0, sizeof(dest));
#ifdef FILESYSTEMUTF8
  // Перевод в кодировку UTF-8 файловой системы
  StringToUnicode8(name, dest, sizeof(dest));
#else
  strncpy(dest, name, sizeof(dest) - 1);
#endif

  int ret = unlink(dest);
  if (unlink(dest) == 0)
  {
    ret = 1;
  }
  else
  {
    if (errno == ENOENT)
      ret = -1;
    else
      ret = 0;
  }
#endif

#if TRACE_MAP == 1
  Tracer2("\n===== Delete for : %s, ret = %u\n", name,ret);
#endif

  return ret;
}

INLINEFUNC int CopyTheFileUn(const WCHAR * oldname, const WCHAR * newname, int exist = 0)
{
#ifdef WIN32API
  int ret = ::CopyFileW(oldname, newname, exist);

#else
  if (!ExistTheFile(oldname))
    return 0;

  (void)exist;               // warning unused
  char toldname[MAX_PATH]; toldname[0] = 0;
  UnicodeToFSEncoding(oldname, toldname, sizeof(toldname));

  char tnewname[MAX_PATH]; tnewname[0] = 0;
  UnicodeToFSEncoding(newname, tnewname, sizeof(tnewname));

  char * copyBuffer = (char *)AllocateTheMemory(BUFSIZ);
  size_t readSize = 0;

  int ret = 0;

  if (copyBuffer != 0)
  {
    int sourceFile = open(toldname, O_RDONLY, 0);
    if (sourceFile > 0)
    {
      int destFile = open(tnewname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (destFile > 0)
      {
        while ((readSize = read(sourceFile, copyBuffer, BUFSIZ)) > 0)
        {
          if (write(destFile, copyBuffer, readSize) > 0)
          {
            ret = 1;
          }
          else
          {
            ret = 0;
            break;
          }
        }

        if (ret)
        {
#ifndef __QNX__
          struct stat statbuf;
          if (stat(toldname, &statbuf) == 0)
          {
            struct timespec times[2];
            times[0] = statbuf.st_atim;
            times[1] = statbuf.st_mtim;

            futimens(destFile, times);

            fchmod(destFile, statbuf.st_mode);
          }
#else
  #ifdef __e2k__
          struct stat64 statbuf;
          if (stat64(toldname, &statbuf) == 0)
  #else
          struct stat statbuf;
          if (stat(toldname, &statbuf) == 0)
  #endif
          {
            struct utimbuf timebuf;
            timebuf.actime = statbuf.st_atime;
            timebuf.modtime = statbuf.st_mtime;

            futime(destFile, &timebuf);

            fchmod(destFile, statbuf.st_mode);
          }
#endif
        }

        close(destFile);
      }

      close(sourceFile);
    }

    FreeTheMemory(copyBuffer);
  }
#endif
#if TRACE_MAP == 1
   char coldname[1024];
   char cnewname[1024];

   UnicodeToString(oldname, coldname, sizeof(coldname));
   UnicodeToString(newname, cnewname, sizeof(cnewname));

   Tracer2("\n===== Copy from %s to %s", coldname, cnewname);
   Tracer1("\n      Copy ret : %u\n", ret);
#endif

  return ret;
}

INLINEFUNC int CopyTheFile(const WCHAR * oldname, const WCHAR * newname, int exist = 0)
{
  return CopyTheFileUn(oldname, newname, exist);
}

INLINEFUNC int CopyTheFile(const char * oldname, const char * newname, int exist = 0)
{
  int ret;

#ifdef WIN32API

#ifdef UNICODE
  TCHAR toldname[MAX_PATH];
  StringToUnicode(oldname,toldname,sizeof(toldname));

  TCHAR tnewname[MAX_PATH];
  StringToUnicode(newname,tnewname,sizeof(tnewname));

  ret = ::CopyFile(toldname,tnewname,exist);
#else
  ret = ::CopyFile(oldname,newname,exist);
#endif
#else  // WIN32API
#ifdef LINUXAPI
  if (!ExistTheFile(oldname))
    return 0;

  (void)exist;       // warning unused
  WCHAR woldname[MAX_PATH]; woldname[0] = 0;
  StringToUnicode(oldname, woldname, sizeof(woldname));

  WCHAR wnewname[MAX_PATH]; wnewname[0] = 0;
  StringToUnicode(newname, wnewname, sizeof(wnewname));

  ret = CopyTheFileUn(woldname, wnewname);
#else
  ret = 0;
#endif // LINUXAPI
#endif // else WIN32API

  return ret;
}

INLINEFUNC int MoveTheFile(const WCHAR * oldname,const WCHAR * newname)
{
  if ((oldname == 0) || (*oldname == 0) || (newname == 0) || (*newname == 0))
    return 0;

#ifdef WIN32API
  WCHAR olongname[1024];
  if (wcslen(oldname) > MAX_PATH)
    {
      WcsCopy(olongname, WTEXT("\\\\?\\"), sizeof(olongname));
      WcsCat(olongname, oldname, sizeof(olongname));
      oldname = olongname;
    }

  WCHAR nlongname[1024];
  if (wcslen(newname) > MAX_PATH)
    {
      WcsCopy(nlongname, WTEXT("\\\\?\\"), sizeof(nlongname));
      WcsCat(nlongname, newname, sizeof(nlongname));
      newname = nlongname;
    }

  int isMoved = ::MoveFileExW(oldname,newname,
                              MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH);
#else
  if (!ExistTheFile(oldname))
    return 0;

  char toldname[MAX_PATH];
  memset(toldname, 0, sizeof(toldname));
  UnicodeToFSEncoding(oldname,toldname,sizeof(toldname));

  char tnewname[MAX_PATH];
  memset(tnewname, 0, sizeof(tnewname));
  UnicodeToFSEncoding(newname,tnewname,sizeof(tnewname));

  int isMoved = 0;
  if (rename(toldname, tnewname) == 0)
    isMoved = 1;
  else
  {
    SetTheFileWriteAttributes(newname, 1);
    DeleteTheFile(newname);
    isMoved = CopyTheFile(oldname, newname);
    if (isMoved)
      DeleteTheFile(oldname);
  }
#endif

#if TRACE_MAP == 1
  char coldname[1024];
  char cnewname[1024];

  UnicodeToString(oldname, coldname, sizeof(coldname)-1);
  UnicodeToString(newname, cnewname, sizeof(cnewname)-1);

  Tracer2("\n===== Move from %s to %s", coldname, cnewname);
  Tracer1("\n      Move ret : %u\n", isMoved);
#endif

  return isMoved;
}

INLINEFUNC int MoveTheFile(const char * oldname,const char * newname)
{
  int isMoved;

#ifdef WIN32API
  isMoved = ::MoveFileExA(oldname, newname,
                          MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH);
#else
  if (!ExistTheFile(oldname))
    return 0;

  char toldname[MAX_PATH];
  memset(toldname, 0, sizeof(toldname));

#ifdef FILESYSTEMUTF8
  // Перевод в кодировку UTF-8 файловой системы
  StringToUnicode8(oldname,toldname,sizeof(toldname));
#else
  strncpy(toldname, oldname, sizeof(toldname));
#endif

  char tnewname[MAX_PATH];
  memset(tnewname, 0, sizeof(tnewname));
#ifdef FILESYSTEMUTF8
  // Перевод в кодировку UTF-8 файловой системы
  StringToUnicode8(newname,tnewname,sizeof(tnewname));
#else
  strncpy(tnewname, newname, sizeof(tnewname));
#endif

  if (rename(toldname,tnewname) == 0)
    isMoved = 1;
  else
  {
    SetTheFileWriteAttributes(newname, 1);
    DeleteTheFile(newname);
    isMoved = CopyTheFile(oldname, newname);
    if (isMoved)
      DeleteTheFile(oldname);
  }
#endif

#if TRACE_MAP == 1
  Tracer2("\n===== Move from %s to %s", oldname,newname);
  Tracer1("\n      Move ret : %u\n", isMoved);
#endif

  return isMoved;
}

typedef struct TEMPHFILE
{
  TEMPHFILE(HANDLE value) { hFile = value; }
  TEMPHFILE() { hFile = 0; }
  ~TEMPHFILE() { Close(); }

  void Close() { CloseTheFile(hFile); }

  HANDLE operator = (HANDLE value) { Close(); return (hFile = value); }
  bool operator == (HANDLE value) { return (hFile == value); }
  bool operator != (HANDLE value) { return (hFile != value); }
  bool operator ! () { return (hFile == 0); }
  operator HANDLE() { return hFile; }

  HANDLE hFile;
}
TEMPHFILE;

#ifdef LINUXAPI
INLINEFUNC int SetModeOwnerAsParentFSEncoding(const char* name)
{
  int ret = 0;

  char currentFile[1024];
  currentFile[0] = 0;
  char parentFolder[1024];
  parentFolder[0] = 0;
  struct stat parentStat;
  memset(&parentStat, 0, sizeof(parentStat));

  // Узнаем имя каталога, в котором создается текущий каталог
  StrCopy(currentFile, name, sizeof(currentFile));
  int size = strlen(currentFile);
  if ((currentFile[size - 1] == '/') || (currentFile[size - 1] == '\\'))
    currentFile[size - 1] = 0;

  SplitThePath(currentFile, 0, parentFolder, 0, 0);

  // Узнаем права родительского каталога
  if (parentFolder[0] && (stat(parentFolder, &parentStat) == 0))
  {
    mode_t folderMode = parentStat.st_mode;

    // Установим права созданного каталога как у родительского
    chmod(name, folderMode);
    chown(name, parentStat.st_uid, parentStat.st_gid);
    ret = 1;
  }

  return ret;
}

INLINEFUNC int SetModeOwnerAsParent(const WCHAR* name)
{
  char tname[1024]; tname[0] = 0;
  UnicodeToFSEncoding(name, tname, sizeof(tname));

  int size = strlen(tname);
  if (!size)
    return 0;

  if ((tname[size - 1] == '/') || (tname[size - 1] == '\\'))
    tname[size - 1] = 0;

  return SetModeOwnerAsParentFSEncoding(tname);
}
#endif

INLINEFUNC int CreateTheDirectoryFSEncoding(const char *name)
{
#ifdef WIN32API
  int ret = ::CreateDirectoryA(name, NULL);
  if (ret == 0)
    {
      DWORD error = ::GetLastError();
      if (error == ERROR_ALREADY_EXISTS)
        return -1;
      return 0;
    }

  return 1;
#else
  int ret = mkdir(name, S_IRWXU|S_IRWXG|S_IRWXO);
  if (ret == -1)
    {
      if (errno == EEXIST)
        ret = -1;
      else
        ret = 0;
    }
  else
      ret = 1;

  if (ret > 0)
    {
      char currentdir[1024];
      currentdir[0] = 0;
      char folder[1024];
      folder[0] = 0;
      struct stat statv;
      memset(&statv, 0, sizeof(statv));

      // Узнаем имя каталога, в котором создается текущий каталог
      StrCopy(currentdir, name, sizeof(currentdir));
      int size = strlen(currentdir);
      if ((currentdir[size - 1] == '/') || (currentdir[size - 1] == '\\'))
        currentdir[size - 1] = 0;

      SplitThePath(currentdir, 0, folder, 0, 0);

      mode_t folderMode = S_IRWXU|S_IRWXG|S_IRWXO;
      // Узнаем права родительского каталога
      if (folder[0])
        if (stat(folder, &statv) == 0)
          folderMode = statv.st_mode;

      // Установим права созданного каталога как у родительского
      chmod(name, folderMode);
    }

  return ret;
#endif
}

INLINEFUNC int CreateTheDirectory(const char * name)
{
#ifdef WIN32API
  int ret = ::CreateDirectoryA(name, NULL);
  if (ret == 0)
    {
      DWORD error = ::GetLastError();
      if (error == ERROR_ALREADY_EXISTS)
        return -1;
      return 0;
    }

  return 1;
#else
  char tname[1024];  tname[0] = 0;
#ifdef FILESYSTEMUTF8
  // Перевод в кодировку UTF-8 файловой системы
  StringToUnicode8(name, tname, sizeof(tname));
#else
  strcpy(tname, name);
#endif

  int size = strlen(tname);
  if (!size)
    return 0;

  if ((tname[size - 1] == '/') || (tname[size - 1] == '\\'))
    tname[size - 1] = 0;

  return CreateTheDirectoryFSEncoding(tname);
#endif
}

INLINEFUNC int CreateTheDirectoryUn(const WCHAR * name)
{
#ifdef WIN32API
  int ret = ::CreateDirectoryW(name, NULL);
  if (ret == 0)
    {
      DWORD error = ::GetTheLastError();
      if (error == ERROR_ALREADY_EXISTS)
        return -1;
      return 0;
    }

  return 1;
#else
  char tname[1024]; tname[0] = 0;
  UnicodeToFSEncoding(name, tname, sizeof(tname));

  int size = strlen(tname);
  if (!size)
    return 0;

  if ((tname[size - 1] == '/') || (tname[size - 1] == '\\'))
    tname[size - 1] = 0;

  return CreateTheDirectoryFSEncoding(tname);
#endif
}

INLINEFUNC int CreateTheDirectory(const WCHAR * name)
{
  return CreateTheDirectoryUn(name);
}

INLINEFUNC int DeleteTheDirectory(const WCHAR * name)
{
#ifdef WIN32API
  int ret = ::RemoveDirectoryW(name);
  if (ret == 0)
  {
    DWORD error = GetLastError();
    if ((error == ERROR_PATH_NOT_FOUND) || (error == ERROR_FILE_NOT_FOUND))
    {
      ret = -1;
    }
    else
    if (error == ERROR_CURRENT_DIRECTORY)
    {
      // Сменить текущую директорию, чтобы освободить папку в системе
      ::SetCurrentDirectoryA("..\\");
      ret = ::RemoveDirectoryW(name);
      if (ret != 0)
        ret = 1;
    }
  }
  else
   ret = 1;
#else
  char cname[1024];  cname[0] = 0;
  UnicodeToFSEncoding(name, cname, sizeof(cname));

  int ret = rmdir(cname);
  // Если каталог не существует, считаем успехом
  if (ret == 0)
    ret = 1;
  else
  if (ret == -1 && errno == ENOENT)
    ret = -1;
  else
    ret = 0;
#endif
  return ret;
}

INLINEFUNC int DeleteTheDirectory(const char * name)
{
  WCHAR wname[1024];  wname[0] = 0;
  StringToUnicode(name, wname, sizeof(wname));

  return DeleteTheDirectory(wname);
}

INLINEFUNC int GetTheCurrentDirectory(char * name, long int size)
{
#ifdef WIN32API
  return ::GetCurrentDirectoryA(size, name);
#else
  return (long int) getcwd(name,size);
#endif
}

INLINEFUNC int GetTheCurrentDirectory(WCHAR * name, long int size)
{
#ifdef WIN32API
  return ::GetCurrentDirectoryW(size/2, name);
#else
  char cname[1024]; cname[0] = 0;

  long int ret = (long int) getcwd(cname, sizeof(cname));
  FSEncodingToUnicode(cname, name, size);
  return ret;
#endif
}

INLINEFUNC int SetTheCurrentDirectory(const WCHAR * name)
{
#ifdef WIN32API
  return ::SetCurrentDirectoryW(name);
#else
  char curDir[1024];
  UnicodeToFSEncoding(name, curDir, sizeof(curDir));
  if (chdir(curDir) < 0)
    return 0;
  return 1;
#endif
}

INLINEFUNC int GetTheTempPath(int bytes, char * buffer)
{
  if ((bytes <= 0) || (buffer == 0)) return 0;

#ifdef WIN32API
  return ::GetTempPathA(bytes,buffer);
#else
  const char * selectedTempPath = getenv("TMPDIR");
  if (selectedTempPath == NULL)
      selectedTempPath = P_tmpdir;
  StrCopy(buffer, selectedTempPath, bytes);
  StrCat(buffer, "/", bytes);
  return 1;
#endif
}

INLINEFUNC int GetTheTempPath(int bytes,WCHAR * buffer)
{
  if ((bytes <= 0) || (buffer == 0)) return 0;

#ifdef WIN32API
  return ::GetTempPathW(bytes/2,buffer);
#else
  const char * selectedTempPath = getenv("TMPDIR");
  if (selectedTempPath == NULL)
      selectedTempPath = P_tmpdir;
  StringToUnicode(selectedTempPath, buffer, bytes);
  WcsCat(buffer, WTEXT("/"), bytes);
  return 1;
#endif
}

INLINEFUNC int GetTheTempPathUn(int bytes,WCHAR * buffer)
{
  return GetTheTempPath(bytes, buffer);
}

// Создать временный файл:
//   - формат файла <временный_каталог>/<префикс><число>
//   - после создания файл не открывается
//   - после окончания работы файл нужно удалить отдельно
// tempPath - временный каталог для размещения файла
// prefixName - префикс имени файла
//   - при нулевом значении функция возьмет префикс tmp
// outName - выходной буфер для размещения имени файла
// outNameSize - размер выходного буфера:
//   - должен быть не меньше MAX_PATH
// При ошибке возвращает ноль
INLINEFUNC int CreateTheTempFile(const char *tempPath, const char *prefixName, char *outName, int outNameSize)
{
  if (tempPath)
  {
    if (outName && outNameSize >= MAX_PATH)
    {
      if (prefixName == 0 || prefixName[0] == 0)
        prefixName = "tmp";

#ifdef WIN32API
      if (GetTempFileNameA(tempPath, prefixName, 0, outName))
        return 1;
#else
      // Заполнить выходной буфер шаблоном имени временного файла
      if (outName != tempPath)
        StrCopy(outName, tempPath, outNameSize);
      CompleteThePath(outName, outNameSize);
      StrCat(outName, prefixName, outNameSize);
      StrCat(outName, "XXXXXX", outNameSize);

      int fd = mkstemp(outName);
      if (fd != -1)
      {
        close(fd);
        return 1;
      }
#endif
    }
  }

  return 0;
}

INLINEFUNC int CreateTheTempFileByTickCount(const WCHAR *tempPath, const WCHAR *prefixName, WCHAR *outName, int outNameSize)
{
  if ((outName == 0) || (outNameSize < (int)(MAX_PATH * sizeof(WCHAR))))
    return 0;

  if (tempPath == 0)
  {
    GetTheTempPathUn(outNameSize, outName);
  }
  else
  {
    // Заполнить выходной буфер шаблоном имени временного файла
    WcsCopy(outName, tempPath, outNameSize);
    CompleteThePath(outName, outNameSize);
  }

  int ispassext = 0;
  if ((prefixName == 0) || (*prefixName == 0))
  {
    WcsCat(outName, WTEXT("tmp"), outNameSize);
    ispassext = 1;
  }
  else
    WcsCat(outName, prefixName, outNameSize);

  WCHAR tail[64]; tail[0] = 0;
  IntToStr(GetTheTickCount(), tail, sizeof(tail));
  WcsCat(outName, tail, outNameSize);

  HANDLE hfile = OpenTheFile(outName, GENERIC_READ|GENERIC_WRITE, 0, CREATE_ALWAYS);
  if (hfile != 0)
  {
    CloseTheFile(hfile);
    return 1;
  }

  DeleteTheFile(outName);
  if (ispassext == 0)
  {
    WcsCat(outName, WTEXT("."), outNameSize);
    WcsCat(outName, prefixName, outNameSize);
  }

  return 0;
}

INLINEFUNC int CreateTheTempFile(const WCHAR *tempPath, const WCHAR *prefixName, WCHAR *outName, int outNameSize)
{
  if ((outName == 0) || (outNameSize < (int)(MAX_PATH * sizeof(WCHAR))))
    return 0;

  WCHAR internalName[MAX_PATH]; internalName[0] = 0;
  if ((tempPath == 0) || (*tempPath == 0))
    GetTheTempPathUn(sizeof(internalName), internalName);
  else
    WcsCopy(internalName, tempPath, sizeof(internalName));

  CompleteThePath(internalName, sizeof(internalName));
  tempPath = internalName;

  int ispassext = 0;
  if ((prefixName == 0) || (*prefixName == 0))
  {
#ifdef WIN32API
    prefixName = WTEXT("tmp");
#endif
    ispassext = 1;
  }

#ifdef WIN32API
  if (GetTempFileNameW(tempPath, prefixName, 0, outName))
  {
    DeleteTheFile(outName);
    if (ispassext == 0)
    {
      WcsCat(outName, WTEXT("."), outNameSize);
      WcsCat(outName, prefixName, outNameSize);
    }
    return 1;
  }
#else
  if (!ispassext)
    WcsCat(internalName, prefixName, sizeof(internalName));
  else
    WcsCat(internalName, WTEXT("tmp"), sizeof(internalName));
  WcsCat(internalName, WTEXT("XXXXXX"), sizeof(internalName));

  char internalNameFS[MAX_PATH]; internalNameFS[0] = 0;
  UnicodeToFSEncoding(internalName, internalNameFS, sizeof(internalNameFS));
  int fd = mkstemp(internalNameFS);
  if (fd != -1)
  {
    close(fd);
    FSEncodingToUnicode(internalNameFS, outName, outNameSize);
    DeleteTheFile(outName);
    if (ispassext == 0)
    {
      WcsCat(outName, WTEXT("."), outNameSize);
      WcsCat(outName, prefixName, outNameSize);
    }
    return 1;
  }
#endif

  if (ispassext)
    prefixName = 0;

  return CreateTheTempFileByTickCount(tempPath, prefixName, outName, outNameSize);
}

INLINEFUNC int CreateTheTempFile(const WCHAR *prefixName, WCHAR *outName, int outNameSize)
{
  return CreateTheTempFile(0, prefixName, outName, outNameSize);
}

// Создать временную папку в папке для временных файлов
//   - формат файла <временный_каталог>/<префикс><число>
// prefix - префикс имени файла
//   - при нулевом значении функция возьмет префикс .tmp
// name - выходной буфер для размещения имени файла
// size - размер выходного буфера в байтах
// При ошибке возвращает ноль
INLINEFUNC int CreateTheTempDirectory(const WCHAR * prefix, WCHAR * name, int size)
{
  if ((name == 0) || (size < MAX_PATH))
    return 0;

  WCHAR temppath[MAX_PATH]; temppath[0] = 0;

  if (GetTheTempPath(sizeof(temppath), temppath) == 0)
    return 0;

  if (CreateTheTempFile(temppath, prefix, name, size) == 0)
    return 0;

  DeleteTheFile(name);
  if (CreateTheDirectory(name) == 0)
    return 0;

  CompleteThePath(name, size);
  return 1;
}

INLINEFUNC int GetTheTempFileName(const WCHAR *tempPath, const WCHAR *prefixName, WCHAR *outName, int outNameSize)
{
  return CreateTheTempFile(tempPath, prefixName, outName, outNameSize);
}

// Запросить объем свободной памяти в Кб
INLINEFUNC BIGINT AvailableTheMemory()
{
#ifdef WIN32API
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof (statex);

  GlobalMemoryStatusEx (&statex);
  return (statex.ullAvailPhys/1024L);
#else
  HANDLE file = OpenTheFile("/proc/meminfo", GENERIC_READ, FILE_SHARE_READ, OPEN_ALWAYS);
  if (!file)
    return 0;
  int buflen = 2048;
  char buffer[2048];
  char avmemory[256]; *avmemory = 0;
  const char* memory = "MemAvailable:";
  int memlen = strlen(memory);
  BIGINT availlable = 0;
  while (ReadTheFile(file, buffer, sizeof(buffer)))
  {
    const char* curstr = strstr(buffer, memory);
    if (curstr)
    {
      curstr += memlen ;
      const char* curin = curstr;
      buflen = 2048;
      int isfirst = 0;
      while (buflen > 0)
      {
        if (!isfirst)
        {
          while (*curin == ' ')
          {
            curin++;
            buflen--;
            if (buflen == 0)
              break;
          }

          curstr = curin;
          isfirst = 1;
        }

        if (*curin == ' ')
          break;

        curin++;
        buflen--;
      }

      StrCopy(avmemory, curstr, curin - curstr + 1);
      sscanf(avmemory, "%ld", &availlable);
      break;
    }
  }
  CloseTheFile(file);
  return availlable;
#endif
}

// Запросить объем памяти в Кб, доступной приложению
INLINEFUNC BIGINT TotalTheMemory()
{
#ifdef WIN32API
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof (statex);

  GlobalMemoryStatusEx (&statex);
#ifdef BUILD_DLL64
  return (statex.ullTotalPhys/1024L);
#else
  return (statex.ullTotalVirtual/1024L);
#endif

#else
  // Общее число физических страниц памяти
  long physPages = sysconf(_SC_PHYS_PAGES);
  // Размер страницы памяти в байтах
  long pageSize  = sysconf(_SC_PAGESIZE);

  if (physPages == -1 || pageSize == -1)
    return 0;
  else
    return physPages * pageSize / 1024;

#endif  // WIN32API
}

// Запросить процент использования оперативной памяти
INLINEFUNC int GetTheUsedMemoryPercent()
{
#ifdef WIN32API
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof (statex);
  GlobalMemoryStatusEx (&statex);
  return statex.dwMemoryLoad;
#else
  return ((TotalTheMemory() - AvailableTheMemory()) * 100 / TotalTheMemory());
#endif  // WIN32API
}

// Запросить размер памяти, используемой данным процессом
INLINEFUNC BIGINT GetTheProcessMemoryUsage()
{
#ifdef WIN32API
#ifndef __BORLANDC__
  HANDLE hProcess = GetCurrentProcess();
  PROCESS_MEMORY_COUNTERS_EX psmemCounters;
  if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&psmemCounters, sizeof(psmemCounters)))
    return psmemCounters.PrivateUsage/1024L;
#endif
#else
  // TODO: дописать для Linux

#endif  // WIN32API
  return 0;
}

// Запросить число ядер (процессоров) в системе
INLINEFUNC unsigned int GetTheProcessorNumber()
{
#ifndef LINUXAPI
  SYSTEM_INFO syst;
#ifdef BUILD_DLL64
  GetNativeSystemInfo(&syst);
#else
  GetSystemInfo(&syst);
#endif
  return syst.dwNumberOfProcessors;
#else
  return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

// Запросить системное время в формате "YYYYMMDD"
INLINEFUNC int GetTheSystemDate(char * dest,int size)
{
  SYSTEMTIME data;
  int form = 0;
  int delta;

  if (dest == 0) return 0;
  if (size < 10) return 0;

  GetSystemTime(&data);

  delta = data.wYear * 10000;               // ГОД
  if (delta > 0) form += delta;
  delta     = data.wMonth * 100;            // МЕСЯЦ
  if (delta > 0) form += delta;
  delta     = data.wDay;                    // ДЕНЬ
  if (delta > 0) form += delta;

  return IntToStr(form, dest, size);
}

// Запросить локальную дату в формате "DD/MM/YYYY"
INLINEFUNC int GetTheSystemDateNormal(char * dest, int size)
{
  SYSTEMTIME data;

  if (dest == 0) return 0;
  if (size < 10) return 0;

  GetSystemTime(&data);

  Snprintf(dest, size, "%02d/%02d/%04d", data.wDay, data.wMonth, data.wYear);
  return 1;
}

// Запросить локальную дату в формате "YYYYMMDD"
INLINEFUNC int GetTheLocalDate(char * dest,int size)
{
  SYSTEMTIME data;
  int form = 0;
  int delta;

  if (dest == 0) return 0;
  if (size < 10) return 0;

  GetLocalTime(&data);

  delta = data.wYear * 10000;               // ГОД
  if (delta > 0) form += delta;
  delta     = data.wMonth * 100;            // МЕСЯЦ
  if (delta > 0) form += delta;
  delta     = data.wDay;                    // ДЕНЬ
  if (delta > 0) form += delta;

  return IntToStr(form, dest, size);
}

// Запросить локальную дату в формате "YYYY-MM-DD"
INLINEFUNC int GetTheLocalDateFormat(char * dest, int size)
{
  SYSTEMTIME data;

  if (dest == 0) return 0;
  if (size < 12) return 0;

  GetLocalTime(&data);

  IntToStr(data.wYear, dest, size);               // ГОД
  StrCat(dest, "-", size);
  int delta = (int)strlen(dest);
  IntToStr(data.wMonth, dest+delta, size-delta); // МЕСЯЦ
  StrCat(dest, "-", size);
  delta = (int)strlen(dest);
  return IntToStr(data.wDay, dest+delta, size-delta);   // ДЕНЬ
}

// Запросить локальную дату в формате "DD/MM/YYYY"
INLINEFUNC int GetTheLocalDateNormal(char * dest, int size)
{
  SYSTEMTIME data;

  if (dest == 0) return 0;
  if (size < 12) return 0;

  GetLocalTime(&data);

  Snprintf(dest, size, "%02d/%02d/%04d", data.wDay, data.wMonth, data.wYear);
  return 1;
}

INLINEFUNC int GetTheLocalDateNormal(WCHAR * dest, int size)
{
  char date[64]; date[0] = 0;
  GetTheLocalDateNormal(date, sizeof(date));
  StringToUnicode(date, dest, size);
  return 1;
}

INLINEFUNC int GetTheLocalDateNormalDot(char * dest, int size)
{
  SYSTEMTIME data;

  if (dest == 0) return 0;
  if (size < 12) return 0;

  GetLocalTime(&data);

  Snprintf(dest, size, "%02d.%02d.%04d", data.wDay, data.wMonth, data.wYear);
  return 1;
}

INLINEFUNC int GetTheLocalDateNormalDot(WCHAR * dest, int size)
{
  char date[64]; date[0] = 0;
  GetTheLocalDateNormalDot(date, sizeof(date));
  StringToUnicode(date, dest, size);
  return 1;
}

// Запросить локальное время в формате "ЧЧ:ММ:CC"
INLINEFUNC int GetTheLocalTimeFormat(char * dest, int size)
{
  SYSTEMTIME data;

  if (dest == 0) return 0;
  if (size < 10) return 0;

  GetLocalTime(&data);

  Snprintf(dest, size, "%02d:%02d:%02d", data.wHour, data.wMinute, data.wSecond);
  return 1;
}

// Запросить системное время в формате "ЧЧ:ММ:CC"
INLINEFUNC int GetTheSystemTimeFormat(char * dest, int size)
{
  SYSTEMTIME data;

  if (dest == 0) return 0;
  if (size < 10) return 0;

  GetSystemTime(&data);

  Snprintf(dest, size, "%02d:%02d:%02d", data.wHour, data.wMinute, data.wSecond);
  return 1;
}

// Запросить системное время в формате "ЧЧММCC" (HHMMSS)
INLINEFUNC int GetTheSystemTimeString(char * dest, int size)
{
  SYSTEMTIME data;

  if (dest == 0) return 0;
  if (size < 10) return 0;

  GetSystemTime(&data);

  Snprintf(dest, size, "%02d%02d%02d", data.wHour, data.wMinute, data.wSecond);
  return 1;
}

// Запросить системное время в формате "ЧЧММCC" (HHMMSS)
INLINEFUNC int GetTheLocalTimeString(char* dest, int size)
{
  SYSTEMTIME data;

  if (dest == 0) return 0;
  if (size < 10) return 0;

  GetLocalTime(&data);

  Snprintf(dest, size, "%02d%02d%02d", data.wHour, data.wMinute, data.wSecond);
  return 1;
}

// Преобразование структуры SYSTEMTIME в дату в формате "YYYYMMDD"
// и число секунд от 00:00:00
INLINEFUNC void SysTimeToValue(SYSTEMTIME &systime, unsigned int *outdate, unsigned int *outtime)
{
  if (outdate)
    {
      *outdate = 0;
       int delta = systime.wYear * 10000;       // ГОД
       if (delta > 0) *outdate += delta;
       delta = systime.wMonth * 100;            // МЕСЯЦ
       if (delta > 0) *outdate += delta;
       delta = systime.wDay;                    // ДЕНЬ
       if (delta > 0) *outdate += delta;
    }
  if (outtime)
    *outtime = (long) systime.wSecond + (long) systime.wMinute * 60L + (long) systime.wHour * 3600L;
}

// Запросить нулевой день года "YYYY0100"
INLINEFUNC unsigned int GetTheFirstDay(unsigned int date)
{
  return ((date / 10000) * 10000) + 100;
}

// Запрос даты и времени в формате "YYYYMMDD" и число секунд от 00:00:00
INLINEFUNC void GetTheSystemDateAndTime(unsigned int *outdate, unsigned int *outtime)
{
  SYSTEMTIME systime;
  GetSystemTime(&systime);
  SysTimeToValue(systime, outdate, outtime);
}

// Запрос даты и времени в формате "YYYYMMDD" и число секунд от 00:00:00
INLINEFUNC void GetTheLocalDateAndTime(unsigned int *outdate, unsigned int *outtime)
{
  SYSTEMTIME systime;
  GetLocalTime(&systime);
  SysTimeToValue(systime, outdate, outtime);
}

// Запрос даты и времени обновления файла в UTC
INLINEFUNC int GetTheFileTime(HANDLE hfile, SYSTEMTIME& time)
{
#ifdef LINUXAPI
  struct stat stbuf;
  memset(&stbuf, 0, sizeof(stbuf));
  if (fstat(hfile, &stbuf) == -1)
    return 0;

  time_t ft = stbuf.st_mtime;
  struct tm gtime;
  memset(&gtime, 0, sizeof(gtime));
  gmtime_r(&ft, &gtime);                         // Время по UTC 28/05/19 Савелов

  time.wDay         = gtime.tm_mday;
  time.wDayOfWeek   = gtime.tm_wday;
  time.wHour        = gtime.tm_hour;
  time.wMinute      = gtime.tm_min;
  time.wMonth       = gtime.tm_mon + 1;     // в Линуксе январь = 0 в Виндовс январь = 1. Делаем пока как в Виндовс.
  time.wSecond      = gtime.tm_sec;
  time.wYear        = gtime.tm_year + 1900;
  time.wMilliseconds = 0;

  return stbuf.st_mtime;
#else
  FILETIME mtime;
  int ret = 1;
  if (::GetFileTime(hfile,NULL,NULL,&mtime) != 0)
    return FileTimeToSystemTime(&mtime, &time);
  return 0;
#endif
}

// Запрос даты и времени обновления файла в UTC
INLINEFUNC int GetTheFileTime(const WCHAR *name, SYSTEMTIME& systime)
{
  if (name == 0) return 0;

#ifdef WIN32API
  // Считываем атрибуты файла
  WIN32_FIND_DATAW finddata;
  HANDLE findhandle = FindFirstFileW(name, &finddata);
  if (findhandle != INVALID_HANDLE_VALUE)
    {
      FindClose(findhandle);

      // Расшифровываем время последней записи
      if (FileTimeToSystemTime(&finddata.ftLastWriteTime, &systime) != 0)
        return 1;
    }
#else
  HANDLE hfile = OpenTheFile(name, GENERIC_READ);
  if (hfile != 0)
    {
      int ret = GetTheFileTime(hfile, systime);
      CloseTheFile(hfile);
      return ret;
    }
#endif

  return 0;
}

// Запрос даты и времени обновления файла в UTC
INLINEFUNC int GetTheFileTime(const char *name, SYSTEMTIME& systime)
{
  if (name == 0) return 0;

#ifdef WIN32API
  // Считываем атрибуты файла
  WIN32_FIND_DATAA finddata;
  HANDLE findhandle = FindFirstFileA(name, &finddata);
  if (findhandle != INVALID_HANDLE_VALUE)
    {
      FindClose(findhandle);

      // Расшифровываем время последней записи
      if (FileTimeToSystemTime(&finddata.ftLastWriteTime, &systime) != 0)
        return 1;
    }
#else
  HANDLE hfile = OpenTheFile(name, GENERIC_READ);
  if (hfile != 0)
    {
      int ret = GetTheFileTime(hfile, systime);
      CloseTheFile(hfile);
      return ret;
    }
#endif

  return 0;
}

// Установка даты и времени обновления файла
INLINEFUNC int SetTheFileTime(HANDLE hfile, SYSTEMTIME& time)
{
  if (hfile == 0) return 0;

#ifdef WIN32API
  FILETIME ftime;
  if (SystemTimeToFileTime(&time, &ftime) != 0)
    return SetFileTime(hfile, &ftime, &ftime, &ftime);
#else
  struct tm local;
  memset(&local, 0, sizeof(local));

  local.tm_mday   = time.wDay;
  local.tm_hour   = time.wHour;
  local.tm_min    = time.wMinute;
  local.tm_mon    = time.wMonth - 1;     // в Линуксе январь = 0 в Виндовс январь = 1. Делаем пока как в Виндовс.
  local.tm_sec    = time.wSecond;
  local.tm_year   = time.wYear - 1900;
  local.tm_isdst  = -1;

  char *tz = getenv("TZ");
  setenv("TZ", "", 1);
  tzset();

  time_t ft = mktime(&local);

  if (tz)
    setenv("TZ", tz, 1);
  else
    unsetenv("TZ");
  tzset();

#ifndef __QNX__
  struct timeval ftimes[2];
  ftimes[0].tv_sec = ftimes[1].tv_sec = ft;
  ftimes[0].tv_usec = ftimes[1].tv_usec = 0;

  if (futimes((int)hfile, ftimes) == 0)
    return ft;
#else
  struct utimbuf ftimes;
  ftimes.actime = ft;
  ftimes.modtime = ft;

  if (futime((int)hfile, &ftimes) == 0)
    return ft;
#endif // QNX
#endif

  return 0;
}

// Установка даты и времени обновления файла
INLINEFUNC int SetTheFileTime(HANDLE hfile, unsigned int *date, unsigned int * ftime)
{
  if (date == 0) return 0;

  SYSTEMTIME data;
  memset((char*)&data, 0, sizeof(data));

  data.wYear = (WORD)(*date / 10000);          // ГОД
  int delta = *date - data.wYear * 10000;
  data.wMonth = (WORD)(delta / 100);           // МЕСЯЦ
  delta = delta - data.wMonth * 100;
  data.wDay = (WORD)delta;                     // ДЕНЬ

  if (ftime != 0)
    {
      data.wHour = (WORD)(*ftime/3600);          // Часы
      delta = *ftime - data.wHour * 3600;
      data.wMinute = (WORD)(delta / 60);         // Минуты
      delta = delta - data.wMinute * 60;
      data.wSecond = (WORD)delta;                // Секунды
    }

  return SetTheFileTime(hfile, data);
}

// Запрос даты и времени в формате "YYYYMMDD" и число секунд от 00:00:00
INLINEFUNC int GetTheFileSystemDateAndTime(HANDLE hfile, unsigned int *outdate, unsigned int *outtime)
{
  SYSTEMTIME systime;
  if (GetTheFileTime(hfile, systime) == 0) return 0;
  SysTimeToValue(systime, outdate, outtime);
  return 1;
}

// Запрос по имени файла/папки даты и времени в формате "YYYYMMDD" и число секунд от 00:00:00
INLINEFUNC int GetTheFolderSystemDateAndTime(const WCHAR *name, unsigned int *outdate, unsigned int *outtime)
{
  if (name == 0) return 0;
  int ExitCode = 0;
  HANDLE hfile = OpenTheFile(name, GENERIC_READ);
  if (hfile != 0)
    {
      ExitCode = GetTheFileSystemDateAndTime(hfile, outdate, outtime);
      CloseTheFile(hfile);
    }
  if (ExitCode == 0)
    {
       if (outdate) *outdate = 0;
       if (outtime) *outtime = 0;
    }
  return ExitCode;
}

// Запрос по имени файла/папки даты и времени в формате "YYYYMMDD" и число секунд от 00:00:00
INLINEFUNC int GetTheFileSystemDateAndTime(const WCHAR *name, unsigned int *outdate, unsigned int *outtime)
{
  if (name == 0) return 0;
  int ExitCode = 0;

#ifdef WIN32API
  // Считываем атрибуты файла
  WIN32_FIND_DATAW finddata;
  HANDLE findhandle = FindFirstFileW(name, &finddata);
  if (findhandle != INVALID_HANDLE_VALUE)
    {
      FindClose(findhandle);

      // Расшифровываем время последней записи
      SYSTEMTIME systime;
      if (FileTimeToSystemTime(&finddata.ftLastWriteTime, &systime) != 0)
        {
          // Преобразуем расшифрованное время в дату, время
          SysTimeToValue(systime, outdate, outtime);
          ExitCode = 1;
        }
    }
#else
  HANDLE hfile = OpenTheFile(name, GENERIC_READ);
  if (hfile != 0)
    {
      ExitCode = GetTheFileSystemDateAndTime(hfile, outdate, outtime);
      CloseTheFile(hfile);
    }
#endif
  if (ExitCode == 0)
    {
       if (outdate) *outdate = 0;
       if (outtime) *outtime = 0;
    }
  return ExitCode;
}

INLINEFUNC int GetTheFileSystemDateAndTime(const char *name, unsigned int *outdate,
                                                             unsigned int *outtime)
{
  if (name == 0) return 0;
  int ExitCode = 0;

#ifdef WIN32API
  // Считываем атрибуты файла
  WIN32_FIND_DATAA finddata;
  HANDLE findhandle = FindFirstFileA(name, &finddata);
  if (findhandle != INVALID_HANDLE_VALUE)
    {
      FindClose(findhandle);

      // Расшифровываем время последней записи
      SYSTEMTIME systime;
      if (FileTimeToSystemTime(&finddata.ftLastWriteTime, &systime) != 0)
        {
          // Преобразуем расшифрованное время в дату, время
          SysTimeToValue(systime, outdate, outtime);
          ExitCode = 1;
        }
    }
#else
  HANDLE hfile = OpenTheFile(name, GENERIC_READ);
  if (hfile != 0)
    {
      ExitCode = GetTheFileSystemDateAndTime(hfile, outdate, outtime);
      CloseTheFile(hfile);
    }
#endif
  if (ExitCode == 0)
    {
      if (outdate) *outdate = 0;
      if (outtime) *outtime = 0;
    }
  return ExitCode;
}

// Запросить имя компьютера/хоста
INLINEFUNC int GetTheComputerName(char * compname, int size)
{
  if ((compname == 0) || (size <= 0))
    return 0;

  *compname = 0;

#ifndef LINUXAPI
  DWORD asize = size;
  return ::GetComputerNameA(compname, &asize);
#else
  int ret = gethostname(compname, size);
  if (ret != 0)
    return 0;
  return strlen(compname);
#endif
}

INLINEFUNC int GetTheComputerName(WCHAR * compname, int size)
{
  if ((compname == 0) || (size <= 0))
    return 0;

  char acompname[128]; acompname[0] = 0;
  int asize = sizeof(acompname);
  if (GetTheComputerName(acompname, asize) == 0)
    return 0;
  return (StringToUnicode(acompname, compname, size) != 0);
}

// Оставил для Windows и Linux один прототип
// Для Linux: Handle - указатель на функцию обратного вызова обработки сообщений (MSGHANDLER)
INLINEFUNC long int SendTheMessage(HMESSAGE Handle,long int messagenumber,WPARAM code, LPARAM parm)
{
  if (Handle == 0) return 0;
#ifndef LINUXAPI
  return (long int)::SendMessage(Handle,messagenumber,code,parm);
#else
  return (long int)Handle(0, messagenumber, code, parm, 0);
#endif
}

INLINEFUNC long int PostTheMessage(HMESSAGE Handle,long int messagenumber,WPARAM code, LPARAM parm)
{
  if (Handle == 0) return 0;
#ifndef LINUXAPI
  return (long int)::PostMessage(Handle,messagenumber,code,parm);
#else
  return (long int)Handle(0, messagenumber, code, parm, 1);
#endif
}

INLINEFUNC int LockTheFile(HANDLE file,long int offset, long int size)
{
#ifdef WIN32API
  return LockFile(file,offset,0,size,0);
#else
  struct flock fileLock;
  memset(&fileLock, 0, sizeof(fileLock));

  fileLock.l_pid = 0;
  fileLock.l_type = F_WRLCK;
  fileLock.l_whence = SEEK_SET; // смещение будет задаваться от начала файла
  fileLock.l_start = offset;    // смещенеи
  fileLock.l_len = size;        // если длина = 0, то локируется весь файл

  if (fcntl(file, F_SETLK, &fileLock) == -1)
    return 0;

  return 1;
#endif
}

INLINEFUNC int UnlockTheFile(HANDLE file,long int offset,
                         long int size)
{
#ifdef WIN32API
  return UnlockFile(file,offset,0,size,0);
#else
  struct flock fileLock;
  memset(&fileLock, 0, sizeof(fileLock));

  fileLock.l_pid = 0;
  fileLock.l_type = F_UNLCK;
  fileLock.l_whence = SEEK_SET; // смещение будет задаваться от начала файла
  fileLock.l_start = offset;    // смещенеи
  fileLock.l_len = size;        // если длина = 0, то локируется весь файл

  if (fcntl(file, F_SETLK, &fileLock) == -1)
    return 0;

  return 1;
#endif
}

//-----------------------------------------------------------------
// Перевести поток в режим ожидания на заданное число мск
//-----------------------------------------------------------------
INLINEFUNC void SleepTheThread(unsigned int milliseconds = 1)
{
#ifdef WIN32API
  Sleep(milliseconds);
#else
  usleep(milliseconds*1000);
#endif
}

//-----------------------------------------------------------------
// Проверить наличие свободного места на диске
// При ошибке возвращает ноль
//-----------------------------------------------------------------
INLINEFUNC int CheckTheDiskFreeSpace(const WCHAR *name, BIGINT length)
{
#ifdef WIN32API
  DWORD    SectorsPerCluster;
  DWORD    BytesPerSector;
  DWORD    NumberOfFreeClusters;
  DWORD    TotalNumberOfClusters;
  WCHAR    rootDisk[MAX_PATH];

  if (name == 0)
    return 0;

  ::SplitThePath(name, rootDisk, NULL, NULL, NULL);
  WcsCat(rootDisk, WTEXT("\\"), sizeof(rootDisk));

  if (::GetDiskFreeSpaceW(rootDisk, &SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters) == 0)
    {
      // Не могу определить свободное место на диске
      return 1;
    }

  if (NumberOfFreeClusters == 0 || ((BIGINT)SectorsPerCluster * (BIGINT)BytesPerSector) < (length/NumberOfFreeClusters))
    {
      // Отсутствует свободное место на диске
      return 0;
    }

#else
  char  tname[1024]; tname[0] = 0;

  WCHAR dirName[1024]; dirName[0] = 0;
  SplitThePath(name,0,dirName,0,0);

  UnicodeToFSEncoding(dirName,tname,sizeof(tname));

#ifndef __QNX__
  struct statfs stbuf;
  if (statfs(tname,&stbuf) == -1) return 0;
#else
  struct statvfs stbuf;
  if (statvfs(tname,&stbuf) == -1) return 0;
#endif

  BIGINT freebytecount = (BIGINT)stbuf.f_bsize*(BIGINT)stbuf.f_bfree;
  if ((freebytecount) < length)
    return 0;
#endif

  return 1;
}

INLINEFUNC int CheckTheDiskFreeSpace(const char *name, BIGINT length)
{
  WCHAR wcName[1024]; wcName[0] = 0;
  StringToUnicode(name, wcName, sizeof(wcName));
  return CheckTheDiskFreeSpace(wcName, length);
}

INLINEFUNC int CheckTheDiskFreeSpaceUn(const WCHAR *name, BIGINT length)
{
  return CheckTheDiskFreeSpace(name, length);
}

//-----------------------------------------------------------------
// Получить короткое имя файла
// При ошибке возвращает ноль
//-----------------------------------------------------------------
INLINEFUNC int GetShortFileName(char * filename,char * shortname, long int size)
{
  if ((filename == 0) || (shortname == 0) || (size <= 0))
    return 0;

#ifdef WIN32API
 HANDLE handle = ::OpenTheFile(filename,GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING);
 if (handle)
 {
   ::GetShortPathNameA(filename, shortname, size);
   ::CloseTheFile(handle);
   return 1;
 }
#endif

 StrCopy(shortname,filename,size);
 return 0;
}

//-----------------------------------------------------------------
// Получить длинное имя файла
// При ошибке возвращает ноль
//-----------------------------------------------------------------
INLINEFUNC int GetLongFileName(const char * filename,char * longname, long int size)
{
 if ((filename == 0) || (longname == 0) || (size <= 0))
   return 0;

#ifdef WIN32API
 HANDLE handle = ::OpenTheFile(filename,GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING);
 if (handle)
 {
   WIN32_FIND_DATAA name;
   HANDLE findhandle = ::FindFirstFileA(filename, &name);
   if (findhandle != INVALID_HANDLE_VALUE)
   {
     FindClose(findhandle);
     ::CloseTheFile(handle);
     StrCopy(longname, name.cFileName, size);
     return 1;
   }

   ::CloseTheFile(handle);
 }
#endif

 StrCopy(longname, filename, size);
 return -1;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++ РАЗГРАНИЧЕНИЕ ДОСТУПА НА ЗАПИСЬ ПОТОКОВ++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//-----------------------------------------------------------------
// Инициализация критической секции
//-----------------------------------------------------------------
#ifndef CHECK_MEMORY
INLINEFUNC void InitializeTheCriticalSection(MAPCRITICALSECTION *section)
#else
INLINEFUNC void InitializeTheCriticalSectionCheck(MAPCRITICALSECTION *section)
#endif
{
  if (section == 0)
    return;

#ifdef WIN32API
  InitializeCriticalSection(section);
#else
  pthread_mutexattr_t attr;

  pthread_mutexattr_init(&attr);
#if !defined (__QNX__)   // 01/08/18 Ганюшин
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
  // Для МСВС раскомментировать и закомментировать строчку выше
//  attr.__mutexkind = PTHREAD_MUTEX_RECURSIVE_NP;
#else
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#endif
  pthread_mutex_init(section, &attr);

  pthread_mutexattr_destroy(&attr);
#endif
}

//-----------------------------------------------------------------
// Удаление критической секции
//-----------------------------------------------------------------
#ifndef CHECK_MEMORY
INLINEFUNC void DeleteTheCriticalSection(MAPCRITICALSECTION *section)
#else
INLINEFUNC void DeleteTheCriticalSectionCheck(MAPCRITICALSECTION *section)
#endif
{
  if (section == 0)
    return;

#ifdef WIN32API
  DeleteCriticalSection(section);
#else
  pthread_mutex_destroy(section);
#endif
}

//-----------------------------------------------------------------
// Занять критическую секцию
//-----------------------------------------------------------------
INLINEFUNC void EnterTheCriticalSection(MAPCRITICALSECTION *section)
{
#ifdef TRACE_LOCK
  TracerLoc1("-----> Lock section = %p\n", section);
#endif

  if (section)
  {
#ifdef WIN32API
    EnterCriticalSection(section);
#else
    pthread_mutex_lock(section);
#endif
  }
}

//-----------------------------------------------------------------
// Освободить критическую секцию
//-----------------------------------------------------------------
INLINEFUNC void LeaveTheCriticalSection(MAPCRITICALSECTION *section)
{
#ifdef TRACE_LOCK
  TracerLoc1("<----- UnLock section = %p\n", section);
#endif

  if (section)
  {
#ifdef WIN32API
     LeaveCriticalSection(section);
#else
     pthread_mutex_unlock(section);
#endif
  }
}

// -----------------------------------------------------------------------------
// Класс занятия и освобождения критической секции
// Данный класс нельзя создавать динамически (через new)
// -----------------------------------------------------------------------------
typedef class TLocker
{
 private:

  MAPCRITICALSECTION * FSection;
  int                  IsLocked;
  int                  Reserve;

 public:

  // Конструктор (сразу локирует секцию)
  TLocker(MAPCRITICALSECTION &Section) { IsLocked = 0; FSection = &Section; Lock(); }

  // Деструктор (разлокирует секцию)
  ~TLocker()  { while (IsLocked > 0) UnLock(); }

  // Войти в критическую секцию
  void __fastcall Lock()  { if (FSection) { EnterTheCriticalSection(FSection); IsLocked++; } }

  // Выйти из критической секции
  void __fastcall UnLock() { if (IsLocked > 0) { if (FSection) LeaveTheCriticalSection(FSection); IsLocked--; } }
}
  TLocker;

// Объявление локировки (объявление объекта класса TLocker)
#define LOCK(_locker, _section) TLocker _locker(_section);
// Принудительный вызов разлокировки (до выхода и освобождения объекта класса TLocker)
#define DO_UNLOCK(_locker) _locker.UnLock();
// Повторный вызов локировки
#define DO_LOCK(_locker) _locker.Lock();

#ifdef WIN32API
typedef HANDLE HMUTEX;
#else
typedef pthread_mutex_t HMUTEX;
#endif

//-----------------------------------------------------------------
// Инициализация mutex
//-----------------------------------------------------------------
INLINEFUNC void CreateTheMutex(HMUTEX& mutex)
{
#ifdef WIN32API
  mutex = CreateMutex(NULL, FALSE, NULL);
#else
  pthread_mutexattr_t attr;

  pthread_mutexattr_init(&attr);
#if !defined (__QNX__)    // 01/08/18  Ганюшин
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
  // Для МСВС раскомментировать и закомментировать строчку выше
  // attr.__mutexkind = PTHREAD_MUTEX_RECURSIVE_NP;
#else
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#endif
  pthread_mutex_init(&mutex, &attr);

  pthread_mutexattr_destroy(&attr);
#endif
}

//-----------------------------------------------------------------
// Удаление критической секции
//-----------------------------------------------------------------
INLINEFUNC void DeleteTheMutex(HMUTEX& mutex)
{
#ifdef WIN32API
  if (mutex != 0)
    {
      CloseHandle(mutex);
      mutex = 0;
    }
#else
  pthread_mutex_destroy(&mutex);
#endif
}

//-----------------------------------------------------------------
// Инициализация EVENT
//-----------------------------------------------------------------
INLINEFUNC HANDLE CreateTheEvent(int ismanual, int isset)
{
#ifdef WIN32API
 return CreateEvent(0, ismanual, isset, 0);
#else
 (void)ismanual;         // warning unused
 (void)isset;
 return 0;
#endif
}

//-----------------------------------------------------------------
// Удаление события
//-----------------------------------------------------------------
INLINEFUNC void DeleteTheEvent(HANDLE& hevent)
{
#ifdef WIN32API
  if (hevent != 0)
  {
    CloseHandle(hevent);
    hevent = 0;
  }
#else
  (void)hevent;              // warning unused
#endif
}

//-----------------------------------------------------------------
// Ожидание освобождения объекта
//-----------------------------------------------------------------
INLINEFUNC int WaitTheObject(HANDLE object, DWORD dwMilliseconds = INFINITE)
{
#ifdef WIN32API
   if (object == 0)
     return 0;
   DWORD ret = WaitForSingleObject(object, dwMilliseconds);
   if (ret == WAIT_OBJECT_0)               // успешно
     return 1;
   if (ret == WAIT_ABANDONED)              // поток-владелец был закрыт без освобождения mutex
     return 2;
   if (ret == WAIT_TIMEOUT)                // Время истекло
     return -1;
   return 0;                               // Ошибка
#else
   (void)object;               // warning unused
   (void)dwMilliseconds;
   return 1;
#endif
}

#ifdef WIN32API
//-----------------------------------------------------------------
// Ожидание освобождения объекта и запрос результата
//-----------------------------------------------------------------
INLINEFUNC unsigned int GetTheOverlapResult(HANDLE object, OVERLAPPED * overlap)
{
#ifdef WIN32API
   DWORD result = 0;
   if ((object != 0) && (GetOverlappedResult(object, overlap, &result, TRUE) != 0))
     return result;

   return 0;
#else
//    pthread_mutex_lock(&mutex);
   return 1;
#endif
}
#endif

//-----------------------------------------------------------------
// Занять критическую секцию на неопределенное время
//-----------------------------------------------------------------
INLINEFUNC int WaitTheMutex(HMUTEX& mutex)
{
#ifdef WIN32API
   return WaitTheObject(mutex);
#else
   pthread_mutex_lock(&mutex);
   return 1;
#endif
}

//-----------------------------------------------------------------
// Занять критическую секцию в пределах заданного времени
//-----------------------------------------------------------------
INLINEFUNC int WaitTheMutexByTime(HMUTEX& mutex, int msec)
{
#ifdef WIN32API
   return WaitTheObject(mutex, msec);
#else
   struct timeval now;
   struct timespec timeout;
   int ret = 0;

   gettimeofday(&now, NULL);
   timeout.tv_sec = now.tv_sec + msec / 1000;
   timeout.tv_nsec = (now.tv_usec + (msec % 1000) * 1000) * 1000;           // 16/10/18 Савелов
   ret = pthread_mutex_timedlock(&mutex, &timeout);   // 14/09/11
   if (ret == 0)
     ret = 1;
   else
   // ETIMEDOUT, EAGAIN, EDEADLK, EINVAL
     ret = -1;

// Для МСВС раскомментировать и закомментировать весь код выше
/*   ret = 1;
   pthread_mutex_lock(&mutex);*/

   return ret;
#endif
}

//-----------------------------------------------------------------
// Освободить критическую секцию
// При успешном освобождении мьютекса возвращает ненулевое значение
//-----------------------------------------------------------------
INLINEFUNC int ReleaseTheMutex(HMUTEX& mutex)
{
#ifdef WIN32API
   if (mutex != 0)
     return ReleaseMutex(mutex);
   return 0;
#else
   // If successful, return zero; otherwise, an error number
   return (pthread_mutex_unlock(&mutex) == 0) ? 1 : 0;
#endif
}

//-----------------------------------------------------------------
// Удаление события
//-----------------------------------------------------------------
INLINEFUNC void SetTheEvent(HANDLE hevent)
{
#ifdef WIN32API
  if (hevent != 0)
    SetEvent(hevent);
#else
  (void)hevent;   // warning unused
#endif
}

INLINEFUNC void ResetTheEvent(HANDLE hevent)
{
#ifdef WIN32API
  if (hevent != 0)
    ResetEvent(hevent);
#else
  (void)hevent;   // warning unused
#endif
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++ СОЗДАНИЕ И УПРВАЛЕНИЕ ПОТОКАМИ ++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// Создать поток
// threadpoint - адрес функции потока
// threadparam - параметры потока
// В Linux при использовании функции CreateTheThreadEx, CreateTheThread обязателен вызов
// WaitTheThreadByTime, для освобождения ресурсов
// При ошибке возвращает 0
//------------------------------------------------------------------------------
#ifdef WIN32API
INLINEFUNC int CreateTheThreadEx(THREADHANDLE& htread, LPTHREAD_START_ROUTINE threadpoint, void* threadparam, int* errorcode)
{
  DWORD dwThreadID;
  htread = CreateThread(NULL, 1024*1024L, threadpoint, threadparam, 0, &dwThreadID);
  if (htread == 0)
  {
    if (errorcode != 0)
      *errorcode = (int)GetLastError();
    return 0;
  }
  return 1;
}

INLINEFUNC int CreateTheThreadDetached(THREADHANDLE& htread, LPTHREAD_START_ROUTINE threadpoint, void* threadparam, int* errorcode)
{
  return CreateTheThreadEx(htread, threadpoint, threadparam, errorcode);
}

INLINEFUNC int CreateTheThread(THREADHANDLE& htread, LPTHREAD_START_ROUTINE threadpoint, void* threadparam)
{
  return CreateTheThreadEx(htread, threadpoint, threadparam, 0);
}
#else
INLINEFUNC int CreateTheThreadDetached(THREADHANDLE& htread, void *(*threadpoint)(void *), void* threadparam, int* errorcode)
{
  pthread_attr_t attr;
  pthread_attr_init(&attr);
#ifdef __QNX__
  pthread_attr_setstacksize(&attr, 2048 * 1024L);
#endif
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  int ret = pthread_create(&htread, &attr, threadpoint, threadparam);
  if ((ret != 0) && (errorcode != 0))
  {
    *errorcode = ret;
    htread = 0;
  }

  pthread_attr_destroy(&attr);
  return (ret == 0);
}

INLINEFUNC int CreateTheThreadEx(THREADHANDLE& htread, void *(*threadpoint)(void *), void* threadparam, int* errorcode)
{
  pthread_attr_t attr;
  pthread_attr_init(&attr);
#ifdef __QNX__
  pthread_attr_setstacksize(&attr, 2048 * 1024L);
#endif
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  int ret = pthread_create(&htread, &attr, threadpoint, threadparam);
  if ((ret != 0) && (errorcode != 0))
  {
    *errorcode = ret;
    htread = 0;
  }

  pthread_attr_destroy(&attr);
  return (ret == 0);
}
INLINEFUNC int CreateTheThread(THREADHANDLE& htread, void *(*threadpoint)(void *), void* threadparam)
{
  return CreateTheThreadEx(htread, threadpoint, threadparam, 0);
}
#endif

// --------------------------------------------------------------
// Запросить идентификатор текущего потока
// --------------------------------------------------------------
INLINEFUNC unsigned long int GetTheCurrentThreadId()
{
#ifndef LINUXAPI
  return (unsigned long int)GetCurrentThreadId();
#else
  return (unsigned long int)pthread_self();
#endif
}

// --------------------------------------------------------------
// Запросить идентификатор текущего процесса
// --------------------------------------------------------------
#ifndef LINUXAPI
  typedef DWORD PROCESSIDENT;
#else
  typedef pid_t PROCESSIDENT;
#endif
INLINEFUNC PROCESSIDENT GetTheCurrentProcessId()
{
#ifndef LINUXAPI
  return (PROCESSIDENT)GetCurrentProcessId();
#else
  return (PROCESSIDENT)getpid();
#endif
}

// --------------------------------------------------------------
// Запросить идентификатор потока по handle
// (в Linux handle=ThreadID, в Windows это разные идентификаторы)
// --------------------------------------------------------------
INLINEFUNC unsigned long int GetTheThreadId(THREADHANDLE handle)
{
#if !defined(LINUXAPI) && !defined(__BORLANDC__)
  return (unsigned long int)GetThreadId(handle);
#else
  return (unsigned long int)handle;
#endif
}


//------------------------------------------------------------------------------
// Подождать поток по времени time
// tHandle - указатель на поток
// time - время ожидания в милисекундах
// В случае, если поток осовбодился возвращает 0, если достигнут интервал времени -1
//------------------------------------------------------------------------------
INLINEFUNC int WaitTheThreadByTime(THREADHANDLE& tHandle, unsigned int time)
{
#ifdef WIN32API
   return WaitTheObject(tHandle, time);
#else
  if (tHandle == 0)   // 20/02/19 Ганюшин
    return 0;

  unsigned int cantry = time / 10;
  while((cantry--)>0)
  {
#ifndef __QNX__      // 12/09/18 Ганюшин
    int errcode = pthread_tryjoin_np(tHandle, NULL);
#else
    int errcode = pthread_join(tHandle, NULL);
#endif
    if (errcode == EBUSY)
    {
      SleepTheThread(10);
      continue;
    }
    else
    {
      tHandle = 0;
      return 0;
    }
  }

  return -1;
#endif
}

//------------------------------------------------------------------------------
// Уничтожить процесс
//------------------------------------------------------------------------------
INLINEFUNC int TerminateTheThread(THREADHANDLE tHandle, DWORD& dwExitCode)
#ifdef LINUXAPI
{
  dwExitCode = pthread_cancel(tHandle);
  return dwExitCode;
}
#else
{
  return TerminateThread(tHandle, dwExitCode);
}
#endif

//------------------------------------------------------------------------------
// Закрыть поток
//------------------------------------------------------------------------------
INLINEFUNC void CloseTheThread(THREADHANDLE& tHandle)
{
  if (tHandle != 0)
  {
#ifdef WIN32API
    THREADHANDLE copy = tHandle;
    tHandle = 0;
    CloseHandle(copy);
#else
#ifndef __QNX__
    pthread_attr_t attr;
    int detached = 0;

    if (!pthread_getattr_np(tHandle, &attr))
    {
      pthread_attr_getdetachstate(&attr, &detached);
      pthread_attr_destroy(&attr);
    }

    if (detached != PTHREAD_CREATE_DETACHED)
#endif
      pthread_join(tHandle, NULL);
    tHandle = 0;
#endif
  }
}

//-----------------------------------------------------------------
// Занять светофор в пределах заданного времени в мсек
//-----------------------------------------------------------------
INLINEFUNC int WaitTheSemaphore(SEMPHHANDLE handle, int msec)
{
  if (handle == 0)
    return 0;

#ifdef WIN32API
  int ret = WaitForSingleObject(handle, msec);

  if ((ret == (int)WAIT_OBJECT_0) || (ret == (int)WAIT_ABANDONED))
    return 1;

  if (ret == WAIT_TIMEOUT)                // Время истекло
    return -1;
#else
  struct timespec ts;
  ts.tv_nsec = msec * 1000000;            // нсек
  int ret = sem_timedwait(handle, &ts);
  if (ret == 0) return 1;
  if (ret == -1)
    {
      if (errno == ETIMEDOUT)
        return -1;
    }
#endif
  return 0;
}

//-----------------------------------------------------------------
// Освободить светофор
//-----------------------------------------------------------------
INLINEFUNC void ReleaseTheSemaphore(SEMPHHANDLE handle)
{
  if (handle != 0)
#ifdef WIN32API
    ReleaseSemaphore(handle, 1, NULL);
#else
    sem_post(handle);
#endif
}

#if defined(BUILD_DLL64) && defined(LINUXAPI)
#pragma pack(pop)
#endif

#ifdef LINUXAPI
class StringTransformer
{
 public:

  static WCHAR* TransformWCharToUnsignedShort(const wchar_t *src, WCHAR *dest, size_t destLength)
  {
#ifndef SHORT_WCHAR
    Unicode32ToUnicode16(src, dest, destLength);
#else
    WcsCopy(dest, src, destLength);
#endif
    return dest;
  }
};

class PLocalString
{
 public:

  PLocalString(const wchar_t *wideString)
  {
    StringTransformer::TransformWCharToUnsignedShort(wideString, StringImpl, sizeof(StringImpl));
  }

  WCHAR* utf16() { return StringImpl; }

 private:

  WCHAR StringImpl[1024];
};

#endif // LINUXAPI

//-----------------------------------------------------------------
// Сравнить 2 числа
// Если равны возвращает ноль
//-----------------------------------------------------------------
INLINEFUNC bool Compare(double value1, double value2, double precision = 0.000001)
{
  value2 -= value1;
  if (value2 < 0)
    value2 = -value2;
  return (value2 > precision);
}

#endif  // MAPSYST_H
