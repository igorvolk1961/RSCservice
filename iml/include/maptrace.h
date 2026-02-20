/***** maptrace.h ************** Belenkov O.V.   ****** 29/07/20 ***
****** maptrace.h ************** Vitsko   D.A.   ****** 03/07/20 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2020              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*        “–ј——»–ќ¬ ј ¬џѕќЋЌ≈Ќ»я ѕ–ќ√–јћћ ƒЋя ѕќ»— ј ќЎ»Ѕќ          *
*                                                                  *
*******************************************************************/

#ifndef MAPTRACE_H
#define MAPTRACE_H

// ƒл€ трассировки убрать символ комментари€
// TRACE_LOCK может включатьс€ отдельно, остальные только при включенном MAPTRACER

// #define MAPTRACER 1
// #define TRACE_LOCK 1
// #define TRACE_MEM 1
// #define TRACE_MAP 1

#ifndef MAPTRACER
  #define MAPTRACER 0
#endif

#ifndef TRACE_LOCK
  #define TRACE_LOCK 0
#endif

#ifndef TRACE_MEM
  #define TRACE_MEM 0
#endif

#ifndef TRACE_MAP
  #define TRACE_MAP 0
#endif

// ”правление выводом дополнительной информации
// (закомментировать лишнее)
#define TRACE_WRITE_THREAD_ID       // выводить идентификатор потока
#define TRACE_WRITE_CURRENT_TIME    // выводить текущее врем€ операции

// ¬ключена кака€ либо трассировка
#if ((MAPTRACER == 1) || (TRACE_LOCK == 1) || (TRACE_MEM == 1) || (TRACE_MAP == 1))

#include <stdio.h>

// ‘айл трассировки
#ifndef LOGFILENAME
  #ifndef LINUXAPI
    #define LOGFILENAME "c:\\ProgramData\\maptrace.log"
  #else
    #ifdef LINUXSERVER
      #define LOGFILENAME "/export/home/maptrace.log"
    #else
      #define LOGFILENAME "/maptrace.log"
    #endif
  #endif 
#endif

// идентификатор потока
#ifndef LINUXAPI 
  #define THREAD_ID_UINT (unsigned long int)GetCurrentThreadId()
#else
  #define THREAD_ID_UINT (unsigned long int)pthread_self()
#endif


// Ѕлоки операций вывода в лог трассировки
// начало
#define TRACE_BEGIN \
            FILE* hFile = fopen( LOGFILENAME, "a+" );  \
            if (hFile) { 
// трассировка идентификатора потока		
#ifdef TRACE_WRITE_THREAD_ID
#define TRACE_THREAD \
            char c_thredId[32];\
            sprintf(c_thredId, " tid=%lu ", THREAD_ID_UINT);\
            fprintf( hFile, c_thredId);
// трассировка идентификатора потока выключена
#else 
#define TRACE_THREAD
#endif		

// трассировка времени	
#ifdef TRACE_WRITE_CURRENT_TIME
#ifndef LINUXAPI			
#define TRACE_TIME \
            char timebuff[40]; timebuff[0] = 0; \
            GetTimeFormatA(NULL,0,NULL, "HH':'mm':'ss", timebuff, sizeof(timebuff)); \
            strcat(timebuff, "  ");\
            fputs(timebuff, hFile); 
#else 	
// дл€ Linux пока без времени
#define TRACE_TIME	
#endif		
// трассировка времени выкючена
#else 	
#define TRACE_TIME	
#endif		

// конец 	
#define TRACE_END \
            fclose( hFile );} 

// Ѕазова€ функци€ вывода в лог сообщени€ с 1 параметром
  #define BaseTracer1(str, mes) \
          { TRACE_BEGIN  \
            TRACE_THREAD \
            TRACE_TIME   \
            fprintf(hFile, str, mes); \
            TRACE_END }
	
// нет трассировки	
#else	
  #define BaseTracer1(str, mes) 	
#endif

// “рассировка локировок
// (можно включить отдельно от общей трассировки)
#if TRACE_LOCK == 1
  #define TracerLoc1(str, mes) BaseTracer1(str, mes)				
#else
  #define TracerLoc1(str, mes)
#endif


#if MAPTRACER == 1
//*******************************************************************
//  ѕример вызова трассировки :
//   Tracer("0x%04X|message = 0x%04X BEGIN.\n", hwnd, uMsg);
//*******************************************************************

  #define Tracer(message) \
          { TRACE_BEGIN  \
            TRACE_THREAD \
            TRACE_TIME   \
            fprintf( hFile, message);\
            TRACE_END }

  #define Tracer1(str, mes) BaseTracer1(str, mes)

  #define Tracer2(str, num, mes) \
          { TRACE_BEGIN  \
            TRACE_THREAD \
            TRACE_TIME   \
            fprintf(hFile, str, num, mes);\
            TRACE_END }

  #define Tracer3(str, num, mes, mes2) \
          { TRACE_BEGIN  \
            TRACE_THREAD \
            TRACE_TIME   \
            fprintf(hFile, str, num, mes, mes2);\
            TRACE_END }

  #define TraceCode(str, nwin, nmes) Tracer2(str, nwin, nmes)
  #define TraceMessage(message) Tracer(message)
  #define TraceMessage1(message, mes) Tracer1(message, mes)
  #define TraceMessage2(message, v1, v2) Tracer2(message, v1, v2)

// нет трассировки	
#else
  #define Tracer3(str, num, mes, mes2)
  #define Tracer2(str, num, mes)
  #define Tracer1(str, mes)
  #define Tracer(message)

  #define TraceCode(str, nwin, nmes)
  #define TraceMessage(message)
  #define TraceMessage1(message, v1)
  #define TraceMessage2(message, v1, v2)
#endif


#endif // MAPTRACE_H
