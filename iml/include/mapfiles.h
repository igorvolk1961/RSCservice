/****** mapfiles.h ************** Belenkov O.V.  ****** 16/12/21 ***
******* mapfiles.h ************** Shabakov  D.A. ****** 14/08/18 ***
******* mapfiles.h ************** Korjilov    A. ****** 13/02/15 ***
******* mapfiles.h ************** Letov V.I.     ****** 20/11/15 ***
******* mapfiles.h ************** Kruzhkov A.E.  ****** 16/08/18 ***
******* mapfiles.h ************** Savelov A.S.   ****** 29/12/18 ***
******* mapfiles.h ************** Zheleznyakov V.A.**** 22/08/19 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2021              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*                 ПОТОКОВАЯ ОБРАБОТКА ФАЙЛОВ                       *
*                                                                  *
*******************************************************************/

#ifndef MAPFILES_H
#define MAPFILES_H

#ifndef MAPTYPE_H
  #include "maptype.h"
#endif

#ifdef LINUXAPI
 #include <dirent.h>
 #include <stdio.h>
#endif

class TRunThread;

typedef struct RUNTHREADPARM
{
  public:

  RUNTHREADPARM(void * parm = 0)
  {
    ThreadParm = parm; State = 0; RunThread = 0; Number = 0; Ident = 0; Percent = 0; Items = 0; Zero = 0;
  }

  TRunThread *  RunThread;    // Класс управления потоками (общий, устанавливается при старте)
  void *        ThreadParm;   // Прикладные параметры потока (обычно свои для каждого потока или адрес класса данных)
  THREADHANDLE  State;        // Идентификатор (HANDLE) потока (определяется при старте)
  BIGINT        Items;        // Число обработанных элементов или 0

  unsigned int  Ident;        // Номер потока с нуля (определяется при старте потока)
  unsigned int  Number;       // Номер записи входных параметров (устанавливается при старте в Run)
  unsigned int  Percent;      // Процент выполнения обработки
  unsigned int  Zero;         
}
  RUNTHREADPARM;

// Функция потока
typedef int (WINAPI * CALLTHREAD)(void * lparm, RUNTHREADPARM * threadparm);

#define THREADCOUNTLIMIT 64

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++ Класс для работы с потоками +++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef class TRunThread
{
 public:

  // Конструктор класса управления потоками по обработке чего-либо
  // call  - прикладаная многопоточная функция
  // threadparm - указатель на массив параметров потоков в количестве limit
  // limit - предельное число потоков
  // total - число обрабатываемых ресурсов для расчета процента выполнения
  TRunThread(CALLTHREAD call = 0, void * threadparm = 0, int parmsize = 0,
             unsigned int limit = 8, unsigned int total = 0, int ismainhandle = 0);
  ~TRunThread();

  // Установить новые параметры для выполнения
  void _fastcall Init(CALLTHREAD call, void * threadparm, int parmsize,
                      unsigned int limit, unsigned int total = 0);

  // Запросить/Установить код прерывания потоков
  int GetBreakCode() { return RetCode; }
  void SetBreakCode() { RetCode = -1; }

  // Запросить число активных потоков
  unsigned int GetThreadCount() { return MultiCount; }

  // Запустить прикладной поток
  // number - номер потока с 0 до предельного числа запускаемых потоков - 1
  // Номеру потока соответствует номер записи в списке параметров потока
  // При ошибке запуска потока возвращает ноль
  int _fastcall Run(unsigned int number);

  // Ждать завершение какого-либо потока заданное время (0,4 сек)
  int _fastcall WaitTime(int check = 0, int period = 400);

  // Отметить завершение потока
  void _fastcall End(THREADHANDLE& state, unsigned int ident);

  // Запросить состояние потока
  THREADHANDLE GetThreadState(int index)
  {
    if ((index > 0) && (index <= (int)Limit))
      return Threadparm[index-1].State;
    return 0;
  }

  // Отправить сообщение об ошибке
  int SendError();

  // Запросить число ошибок и сбросить счетчик
  unsigned int GetErrorCountAndClear();

  // Отправить сообщение о проценте выполнения потока
  int SendPercent(unsigned int ident, unsigned int percent, BIGINT items = 0, const WCHAR * message = 0);

  // Запросить текущий процент выполнения всех потоков
  unsigned int GetPercent(BIGINT* items);
  unsigned int GetPercent(unsigned int * items = 0);

  // Запросить число обработанных элементов
  unsigned int GetReadyCount() { return ReadyCount; }

  // Увеличить число обработанных элементов
  // Вызывается, когда отдельный элемент обработкан без потока
  unsigned int UpReadyCount();
  unsigned int UpReadyCount(unsigned int ident);

  // Запросить общее число обработываемых элементов
  unsigned int GetTotalCount() { return TotalCount; }
  unsigned int SetTotalCount(int count) { return (TotalCount = count); }

  // Запросить/Установить контрольное время
  UINT64 GetLastTick64() { return LastTick; }
  unsigned int GetLastTick() { return (unsigned int)LastTick; }
  UINT64 UpdateLastTick64() { return (LastTick = GetTheTickCount64()); }
  unsigned int UpdateLastTick() { return (unsigned int)(LastTick = GetTheTickCount64()); }

  // Запросить текст сообщения
  void GetMessage(WCHAR* message, int size)
  {
    MultiLock();
    WcsCopy(message, Message, size);
    MultiUnLock();
  }

  // Запросить главный идентификатор
  HANDLE GetMainHandle() { return MainHandle; }

  // Установить главный идентификатор
  void _fastcall SetMainHandle(HANDLE handle);

  // Ожидать установления главного идентификатора
  // time - число мск ожидания
  // Возвращает >0 - объект освобожден, 0 - ошибка, -1 - таймаут (WAIT_TIMEOUT)
  int WaitMainHandle(int time)
  {
#ifdef WIN32API
    return WaitTheObject(hEvent, 200);
#else
    // Приостановить процесс на 0.2 с
    struct timespec tv;
    tv.tv_sec = 0;
    tv.tv_nsec = time* 1000000;
    nanosleep(&tv, &tv);
    return -1;                       // WAIT_TIMEOUT
#endif
  }

  // Запросить номер обрабатываемого ресурса в потоке
  // Возвращает значение от 1 до TotalCount
  // Если все ресурсы обработаны - возвращает ноль
  unsigned int GetPrepareItem()
  {
    unsigned int ret = 0;
    MultiLock();
    if (LastItem < TotalCount)
      ret = ++LastItem;
    MultiUnLock();
    return ret;
  }

  // Запросить адрес функции
  CALLTHREAD GetThreadFunction() { return CallThread; }

 protected:

  // Занять переменные класса
  void MultiLock() { EnterTheCriticalSection(&MultiSection);  }

  // Освободить переменные класса
  void MultiUnLock() { LeaveTheCriticalSection(&MultiSection);  }

 protected:

  CALLTHREAD         CallThread;            // Функция потока
  MAPCRITICALSECTION MultiSection;          // Секция доступа к переменным
  SEMPHHANDLE        hSemaphore;            // Семафор доступности каналов для обработки запроса
#ifndef WIN32API
  sem_t              hSemaphoreLinux;       // Истинный семафор для Linux
#endif

  HANDLE             MainHandle;
#ifdef WIN32API
  HANDLE             hEvent;                // Флаг ожидания назначения MainHandle
#endif

  RUNTHREADPARM      Threadparm[THREADCOUNTLIMIT];

  UINT64             LastTick;
  BIGINT             ReadyItems;

  unsigned int       TotalCount;
  unsigned int       LastItem;
  unsigned int       ReadyCount;
  unsigned int       Limit;

  unsigned int       ErrorCount;
  unsigned int       MultiCount;
  int                RetCode;
  int                Zero;

  WCHAR              Message[256];
}
  TRunThread;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++ ПОТОКОВАЯ ОБРАБОТКА ФАЙЛОВ +++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Предназначен для обработки данных за один проход
// (конвертирование, кодирование, копирование и т.п.)

#define PF_BUFFERSIZE  (512*1024L)         // Максимальный размер буфера

class TPrepareFile
{
 public:

  TPrepareFile()
  {
    File    = 0;      // Идентификатор файла
    Buffer  = 0;      // Адрес буфера
    Length  = 0;      // Размер файла
    Offset  = 0;      // Текущее смещение в файле
    Size    = 0;      // Размер буфера
    Zero    = 0;
  }

  ~TPrepareFile()
  {
    Close();
  }

  // Обработать файл данных
  // name - путь к файлу
  // Если считан не весь файл - возвращает ноль
  int _fastcall Run(const char * name, int mode = GENERIC_READ, int access = FILE_SHARE_READ|FILE_SHARE_WRITE);
  int _fastcall Run(const WCHAR * name, int mode = GENERIC_READ, int access = FILE_SHARE_READ|FILE_SHARE_WRITE);

 protected :

  // Обработать блок данных из файла
  // Если возвращает ноль - обработка завершается
  virtual int Prepare(char * /*buffer*/, int /*size*/) { return 1; }

  // Открыть файл данных
  // При ошибке возвращает ноль
  int _fastcall Open(const WCHAR * name, int mode = GENERIC_READ, int access = FILE_SHARE_READ|FILE_SHARE_WRITE);

  // Закрыть файл
  void Close();

 protected:

  HANDLE       File;      // Идентификатор файла
  char *       Buffer;    // Адрес буфера

  BIGINT       Length;    // Размер файла
  BIGINT       Offset;    // Текущее смещение в файле

  unsigned int Size;      // Размер буфера
  unsigned int Zero; 
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++   ВСТАВКА ЗАГОЛОВКА В НАЧАЛО ФАЙЛА   +++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class TCopyFile : public TPrepareFile
{
 public :

  TCopyFile() : TPrepareFile()
  {
    OutFile = 0;
    OutName = 0;
  }

 ~TCopyFile()
  {
    if (OutFile) ::CloseTheFile(OutFile);
  }

  // Копировать файл данных
  // in - путь к исходному файлу,
  // out - путь к выходному файлу
  // Если функция Prepare не переопределена - выполняет
  // копирование файлов
  // Если считан не весь файл - возвращает ноль
  int _fastcall Copy(const char * in, const char * out);
  int _fastcall Copy(const WCHAR * in, const WCHAR * out);
  int _fastcall Copy(const WCHAR * in, const WCHAR * out, int isappend);

 protected:

  // Обработать блок данных из файла (копирование)
  // Если возвращает ноль - обработка завершается
  int Prepare(char * buffer, int size);

 protected:

  const WCHAR * OutName;
  HANDLE        OutFile;
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++   ВСТАВКА ЗАГОЛОВКА В НАЧАЛО ФАЙЛА   +++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class TPrepareFileHeader : public TCopyFile
{
 public:

  TPrepareFileHeader() : TCopyFile() {}

 ~TPrepareFileHeader() {}

  // Вставить заголовок в начало файла
  //  in     - путь к входному файлу
  //  out    - путь к выходному файлу
  //  record - запись
  //  size   - размер записи
  // При ошибке возвращает ноль
  int Insert(const WCHAR * name,char * record, int size)
  {
    return InsertAndRun(name,name,record,size);
  }

  int InsertAndRun(const WCHAR * in, const WCHAR * out, char * record, int size);
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++   СРАВНЕНИЕ ДВУХ ФАЙЛОВ   +++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef class TCompareFile : public TPrepareFile
{
 public :

  TCompareFile() : TPrepareFile()
  {
    SecondFile = 0; SecondBuffer = 0;
    SecondSize = 0; CZero = 0;
  }

 ~TCompareFile()
  {
    if (SecondFile) ::CloseTheFile(SecondFile);
    if (SecondBuffer) delete [] SecondBuffer;
  }

  // Сравнить содержимое файлов
  // Дата и время обновления файлов игнорируются
  // При несовпадении возвращает ноль, иначе - ненулевое значение
  int _fastcall Compare(const WCHAR * first, const WCHAR * second);

 protected:

  // Обработать блок данных из файла (копирование)
  // Если возвращает ноль - обработка завершается
  int Prepare(char * buffer, int size);

 protected:

  HANDLE       SecondFile;
  char *       SecondBuffer;          // Адрес буфера

  unsigned int SecondSize;            // Размер буфера
  unsigned int CZero;
}
  TCompareFile;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++ TFindControl/TFindData ++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Класс TFindControl не поддерживает многопоточность (в каждом потоке должен быть свой экземпляр класса)
//

// Функция уведомления о процессе обработки файлов в папке
// error - системный код ошибки операции (GetLastError()/errno) или ноль
typedef long int (WINAPI * EVENTFOLDER)(void * parm, const WCHAR * filename, int error);


typedef class TFindControl
{
 public:

  TFindControl()  
  { 
    hFind = 0;
#ifdef WIN32API
    memset(&FData, 0, sizeof(FData));
#else
    FormatChar[0] = 0;
#endif
  }
  ~TFindControl()  { Close(); }

  // Закрыть результаты поиска
  void Close();

#ifdef WIN32API
  // Запросить идентификатор поиска
  HANDLE GetHandle() { return hFind; }

  // Установить идентификатор поиска
  int FindFirst(const WCHAR * path)
  {
    hFind = FindFirstFileW(path, &FData);
    if (hFind == INVALID_HANDLE_VALUE)
      hFind = 0;
    return (hFind != 0);
  }

  // Найти следующий элемент
  int FindNext()
  {
    return ::FindNextFileW(hFind, &FData);
  }

  // Является ли текущий элемент директорией?
  int IsDirectory()
  {
    // Сначала могут идти относительные пути (\. и \..)
    if (FData.cFileName[0] == '.')
      return -1;

    if ((FData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
      return 1;

    return 0;
  }

  // Запросить найденное имя
  const WCHAR * GetName() { return FData.cFileName; }

  // Запросить размер файла
  BIGINT GetSize() { return (((BIGINT)FData.nFileSizeHigh)<<32) + ((BIGINT)FData.nFileSizeLow); }
#else
  // Запросить идентификатор поиска
  DIR * GetHandle() { return hFind; }

  // Установить идентификатор поиска
  int OpenFind(const char * path)
  {
    hFind = opendir(path);
    return (hFind != 0);
  }
#endif

 protected:

#ifdef WIN32API
  HANDLE            hFind;
  WIN32_FIND_DATAW  FData;
#else
  DIR *             hFind;

 public:

  char              FormatChar[128];   // Фильтр поиска файлов
#endif
}
  TFindControl;

typedef class TFindData
{
 public:

  TFindData()
  { 
    FindHandle    = 0; 
    EventHandler  = 0; 
    EventParm     = 0; 
    *Format       = 0; 
    *SearchPath   = 0; 
#ifdef WIN32API
    memset(&FoundData, 0, sizeof(FoundData));
    IsFirstLaunch = 0;
    Zero          = 0;
#else
    FoundData     = 0;
#endif
  }

  ~TFindData() { CloseFolder(); }

  // Копировать содержимое папки и саму папку
  // При ошибке возвращает ноль
  int _fastcall CopyFolder(const WCHAR * path, const WCHAR * target, int iscopyfolder = 1, int filesonly = 0);

  // Удалить папку и все ее содержимое
  // path - полный путь к папке
  // insideonly - удалить только содержимое папки, папку не удалять
  // filesonly  - удалить только файлы в корневой папке, директории не удалять
  // При ошибке возвращает ноль
#if !defined(_GISSERVER) || defined(HIDEGISSERV)
  int _fastcall DeleteFolder(const WCHAR * path, int force = 0, int insideonly = 0, int filesonly = 0);
#else
  int _fastcall DeleteFolder(const WCHAR * path, void * client = 0, void * reply = 0, int force = 0, int insideonly = 0, int filesonly = 0);
#endif

  // Переместить папку и все ее содержимое с удалением
  // path - полный путь к папке
  // При ошибке возвращает ноль
  int _fastcall MoveFolder(const WCHAR * path, const WCHAR * target, int force = 1, int passtopfolder = 0);

  // Перебрать все файлы с вызовом функции обратного вызова
  // path - полный путь к папке
  // format - шаблон поиска ("*", "*.*", "*.sxf")
  // eventhandler - адрес функции обратного вызова
  // eventparm    - первый параметр функции обратного вызова
  // infolders - признак обработки вложенных папок
  // При ошибке возвращает ноль
  int _fastcall PrepareFolder(const WCHAR * path, const WCHAR * format, EVENTFOLDER eventhandler, void * eventparm, int infolders = 0);

  // ----------------------------------------------------------------
  // Функции поиска
  // ----------------------------------------------------------------

  // Открыть папку для поиска
  // path - полный путь к папке
  // format - шаблон поиска ("*", "*.*", "*.sxf")
  // При ошибке возвращает ноль
  int _fastcall OpenFolder(const WCHAR *path, const WCHAR *format);

  // Закрыть папку для поиска
  void _fastcall CloseFolder();

  // Установить адрес обработчика для обработки событий на каждый файл при обработке папки
  // eventhandler - адрес функции обратного вызова
  // eventparm    - первый параметр функции обратного вызова
  void _fastcall SetEventHandler(EVENTFOLDER eventhandler = 0, void * eventparm = 0)
  {
    EventHandler = eventhandler;
    EventParm    = eventparm;
  }

  // Найти файл в папке
  // relativeFileName - переменная для размещения найденного файла (относительный путь)
  int _fastcall FindNextFile(WCHAR *relativeFileName, int relativeFileNameSize);

  // Является ли файл папкой
  // filePath - полный путь к файлу
  static int _fastcall IsFolder(const WCHAR *filePath);

  // Является ли файл папкой
  // filePath - относительный путь к файлу
  int _fastcall IsFolderRelative(const WCHAR *filePath);

  // Сформировать шаблон поиска (служебная)
  // inputFormat - входной шаблон
  // outputFormat - модифицированный шаблон
  void _fastcall FormFormatString(const WCHAR *inputFormat, WCHAR *outputFormat, int outputFormatSize);

  // Удовлетворяет ли найденное содержимое формату (служебная)
  // filePath - относительный путь к файлу
  // fileFormat - шаблон поиска ("*", "*.*", "*.sxf")
  int _fastcall IsFileFormatSatisfied(const WCHAR *filePath, const WCHAR *fileFormat) const;

  // Проверить что имя файла является системным (. или ..)
  // filePath - относительный путь к файлу
  int _fastcall IsSystemPath(const WCHAR *filePath) const;

  // Найти набор данных заданного формата
  // format - шаблон поиска ("*", "*.*", "*.sxf")
  // path   - путь к папке для поиска
  // name   - строка для записи результата поиска
  // size   - размер строки для записи результата поиска
  // При ошибке возвращает ноль
  int _fastcall FindFile(TFindControl& control, const WCHAR * format, const WCHAR * path, WCHAR * name, int size, int flag = 0)
  {
    if (flag == 0)
      return Find(control, format, path, name, size, 0);
    else
      return FindNext(control, path, name, size, 0);
  }

  // Перебрать папки в заданной папке
  // path   - путь к папке для поиска
  // name   - строка для записи результата поиска
  // size   - размер строки для записи результата поиска
  // При ошибке возвращает ноль
  int _fastcall FindFolder(TFindControl& control, const WCHAR * path, WCHAR * name, int size, int flag = 0)
  {
    if (flag == 0)
      return Find(control, 0, path, name, size, 1);
    else
      return FindNext(control, path, name, size, 1);
  }

  // Найти набор данных заданного формата
  // format - шаблон поиска ("*", "*.*", "*.sxf")
  // path   - путь к папке для поиска
  // name   - строка для записи результата поиска
  // size   - размер строки для записи результата поиска
  // onlyfolder - признак выбора только папок в заданной папке
  // При ошибке возвращает ноль
  int _fastcall Find(TFindControl& control, const WCHAR * format, const WCHAR * path, WCHAR * name, int size, int onlyfolder);

  // Запросить следующий набор данных в открытой папке
  // Если данных нет - возвращает ноль
  int _fastcall FindNext(TFindControl& control, const WCHAR * path, WCHAR * name, int size, int onlyfolder);

  // Найти заданный файл в папке, включая вложенные папки
  // sfile - путь к файлу, с содержанием которого выполняется сравнение
  // path   - путь к папке для поиска
  // При ошибке возвращает ноль
  int _fastcall FindFile(const WCHAR * sfile, const WCHAR * path, WCHAR * name, int size);

  // Найти папку с заданным именем внутри другой папки, включая вложенные папки (директории)
  // seekname - имя папки, которую нужно найти, без пути
  // path   - путь к папке для поиска
  // name   - строка для записи результата поиска
  // size   - размер строки для записи результата поиска
  // infolders - признак обработки вложенных папок
  // При ошибке возвращает ноль
  int _fastcall FindFolderByName(const WCHAR * seekname, const WCHAR * path, WCHAR * name, int size, int infolders = 0);

  // Найти файл заданного формата в папке, включая вложенные папки
  // format - шаблон поиска ("*", "*.*", "*.sxf")
  // path   - путь к папке для поиска
  // name   - строка для записи результата поиска
  // size   - размер строки для записи результата поиска
  // infolders - признак обработки вложенных папок
  // При ошибке возвращает ноль
  int _fastcall FindFileFormat(const WCHAR * format, const WCHAR * path, WCHAR * name, int size, int infolders = 1);

  // Найти файл одного из заданных форматов в папке
  // format - список форматов файлов (WTEXT("sit"), WTEXT("sitx"), WTEXT("sitz") ...)
  // count  - число форматов в списке
  // path   - путь к папке для поиска
  // name   - строка для записи результата поиска
  // size   - размер строки для записи результата поиска
  // infolders - признак обработки вложенных папок
  // При ошибке возвращает ноль, иначе - номер найденного формата в списке с 1
  int _fastcall FindFileFormatList(TFindControl& control, const WCHAR ** formatlist, int count,
                                   const WCHAR * path, WCHAR * name, int size, int flag);

  // Найти файл в папке и поддиректориях, исключая заданные форматы 
  // format - список форматов файлов (WTEXT("sit"), WTEXT("sitx"), WTEXT("sitz") ...)
  // count  - число форматов в списке
  // path   - путь к папке для поиска
  // name   - строка для записи результата поиска
  // size   - размер строки для записи результата поиска
  // Если в папке только файлы, которые заданы в списке, то возвращает ноль
  // При ошибке возвращает ноль
  int _fastcall FindFileExcludingFormatList(const WCHAR** formatlist, int count, const WCHAR* path, WCHAR* name, int size);

  // Найти файл в папке, исключая заданные форматы 
  // format - список форматов файлов (WTEXT("sit"), WTEXT("sitx"), WTEXT("sitz") ...)
  // count  - число форматов в списке
  // path   - путь к папке для поиска
  // name   - строка для записи результата поиска
  // size   - размер строки для записи результата поиска
  // flag   - признак поиска первого (0) или следующих наборов (!= 0)
  // Если в папке только файлы, которые заданы в списке, то возвращает ноль
  // При ошибке возвращает ноль
  int _fastcall FindFileExcludingFormatList(TFindControl& control, const WCHAR** formatlist, int count,
                                            const WCHAR* path, WCHAR* name, int size, int flag);

  // Запросить число файлов и их размер для заданного формата в заданной папке
  // format - шаблон поиска ("*", "*.*", "*.sxf")
  // path   - путь к папке для поиска
  // infolders - признак обработки вложенных папок
  // При ошибке возвращает ноль
  int _fastcall GetFileCountAndSize(const WCHAR * format, const WCHAR * path, BIGINT& size, int infolders = 0);

  protected:

  // Удалить папку и все ее содержимое
  // При ошибке возвращает ноль
#if !defined(_GISSERVER) || defined(HIDEGISSERV)
  int _fastcall DeleteFolder(TFindControl& control, const WCHAR * folder, int force = 0, int insideonly = 0);
#else
  int _fastcall DeleteFolder(TFindControl& control, const WCHAR * folder, void * client = 0, void * reply = 0, int force = 0, int insideonly = 0);
#endif

  // Переместить папку и все ее содержимое с удалением
  // При ошибке возвращает ноль
  int _fastcall MoveFolder(TFindControl& control, const WCHAR * path, const WCHAR * target, int force);

  // Копировать содержимое папки и саму папку
  // При ошибке возвращает ноль
  int _fastcall CopyFolder(TFindControl& control, const WCHAR * path, const WCHAR * target);

 // Перебрать все файлы с вызовом функции обратного вызова
 // path - полный путь к папке
 // format - шаблон поиска ("*", "*.*", "*.sxf")
 // infolders - признак обработки вложенных папок
 // При ошибке возвращает ноль
 int _fastcall PrepareFolder(TFindControl& control, const WCHAR * path, const WCHAR * format, int infolders);

  protected:

#ifdef WIN32API
  HANDLE  FindHandle;
  WIN32_FIND_DATAW FoundData;
  int     IsFirstLaunch;
  int     Zero;
#else
  DIR*    FindHandle;
  struct dirent* FoundData;
#endif

  EVENTFOLDER EventHandler;
  void *      EventParm;
  WCHAR       Format[128];
  WCHAR       SearchPath[1024];
}
  TFindData;


inline int CopyFile(const char * in, const char * out)
{
  TCopyFile File;
  return File.Copy(in, out);
}

inline int CopyFile(const WCHAR * in, const WCHAR * out)
{
  TCopyFile File;
  return File.Copy(in, out);
}

 // Копировать файлы данных векторной карты
 // inname  - исходный файл паспорта векторной карты,
 // outname - выходной файл паспорта векторной карты,
 // Выполняет копирование файлов.
 // В случае ошибки возвращает 0
 long int cfCopyMapFiles(const char * inname, const char * outname);

 // Выделить имя крайней папки из пути
 long int GetFolderFromPath(const WCHAR * spath, WCHAR * folder, long int size);

 // Удалить имя крайней папки из пути
 long int CutFolderFromPath(WCHAR * folder, long int size);

 // Сформировать временный файл
 // Создается файл нулевой длины с уникальным именем и префиксом в папке Temp
 // По окончании использования файл нужно удалить
 // ext - расширение файла (например, L".sitx") - если задано расширение,
 //       то файл не создается на диске (удаляется при создании)
 // При ошибке возвращает ноль

 long int GetTempFile(const WCHAR * label, WCHAR * tempname, long int size, const WCHAR * ext = 0);

 // Заменить расширение файла на заданное
 // name - путь к файлу
 // size - длина строки в байтах, содержащей путь к файлу (не менее MAX_PATH)
 // ext  - требуемое расширение, например: L".sitx"
 // Если расширение уже такое, как задано, то возвращает -1, иначе меняет расширение и возвращает 1
 // При ошибке возвращает ноль

 long int ChangeFileExt(WCHAR * filename, int size, const WCHAR * newext);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++ LoadLibrary/FreeLibrary +++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 // Загрузить библиотеку DLL
 HINSTANCE xwLoadLibrary(const WCHAR * dllname);

 // Загрузить функцию библиотеки DLL
 FARPROC xwGetProcAddress(HINSTANCE libinst, const char * funcname);

 // Выгрузить библиотеку DLL
 int xwFreeLibrary(HINSTANCE libinst);

 // Загрузить библиотеку DLL
 // При поиске DLL проверяется и директория приложения
 // При ошибке возвращает ноль и выдает сообщение на экран
 FARPROC LoadMapLibrary(const char * dllname, HINSTANCE  & libinst,
                        const char * funcname, int message);

 FARPROC LoadMapLibrary(const WCHAR * dllname, HINSTANCE  & libinst,
                        const WCHAR * funcname, int message);

 FARPROC LoadMapLibraryEx(WCHAR * dllname, int size, HINSTANCE& libinst,
                          const WCHAR * funcname, int message);


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++ Create GUID +++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Сформировать GUID в виде строки шестнадцатеричных цифр длиной 32 байта
int CreateTheGUID(INTQUAD * quad);                                 
int CreateTheGUID(char * ident, int size);                         

// Преобразовать указатель на GUID (16 байт) в строку (33 байта)
// При ошибке возвращает ноль
int GUIDToString(const unsigned char * guid, char * point);

// Преобразовать строку (32/36 байт) в GUID (16 байт, без замыкающего нуля)
// При ошибке возвращает ноль
int StringToGUID(const char * point, char * guid);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++ TBigFile ++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TBigFile
{
  public:

  TBigFile();
  ~TBigFile();

  // Запросить - открыт ли файл
  int isopen() { return (File != 0); }

  // Открыть файл
  // При ошибке возвращает ноль
  int _fastcall flopen(const WCHAR * path);

  // Закрыть файл
  void flclose();

  // Запросить длину файла
  // При ошибке возвращает ноль
  BIGINT fllength();

  // Позиционировать на заданное смещение
  // При ошибке возвращает ноль
  int flseek(BIGINT offset) { return flsetpos(offset); }

  // Считать запись из файла
  // При ошибке возвращает ноль
  int _fastcall flgets(char * buffer, int size);

  // Запросить не закончился ли файл
  int fleof() { return IsEof; }

  // Запросить текущую позицию в файле
  // При ошибке возвращает ноль
  BIGINT flgetpos() { if (File) return Offset; return 0; }
  BIGINT fltell() { return flgetpos(); }

  // Установить текущую позицию в файле
  // При ошибке возвращает ноль
  int _fastcall flsetpos(BIGINT offset);

  protected:

  BIGINT BufferOffset;  // Текущая позиция буфера
  BIGINT Offset;        // Текущая позиция записи
  BIGINT Length;

  char * Buffer;
  HANDLE File;          // Идентификатор файла

  int    BufferSize;
  int    DataSize;
  int    IsEof;
  int    Zero;
}
  TBigFile;

#if (MAPAPIVERSION >= 0x0120602)
#if !defined(BUILD_DLL_GISSERVER) && !defined(HIDEWMS) && !defined(HIDEMAPAPI)
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++ TUrlFileReader ++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Класс не поддерживает многопоточность (в каждом потоке должен быть свой экземпляр класса)
//
typedef class TUrlFileReader
{
 public:

  TUrlFileReader() { Control = 0; WorkPath[0] = 0; }
  ~TUrlFileReader() { Close(); }

  // Запросить рабочую папку для размещения считанных файлов
  const WCHAR * GetWorkPath() { return WorkPath; }

  // Установить рабочую папку для размещения считанных файлов
  const WCHAR * SetWorkPath(const WCHAR * workpath)
  { return WcsCopy(WorkPath, workpath, sizeof(WorkPath)); }

  // Считать файл из web в память и вернуть указатель на буфер
  // url    - путь к файлу в web
  // size   - размер буфера, устанавливается после чтения файла
  // Возвращает адрес буфера со считанным файлом
  // При ошибке возвращает ноль
  const char * _fastcall ReadFileToMemory(const WCHAR* url, int& size);

  // Считать файл из web в заданный файл
  // url  - путь к файлу в web
  // path - путь к файлу, в который нужно поместить результат считывания
  // При ошибке возвращает ноль
  int _fastcall ReadFileToFile(const WCHAR* url, const WCHAR * path);

  // Считать файл из web в рабочую папку и вернуть путь к файлу
  // url  - путь к файлу в web
  // path - буфер для записи пути к локальному файлу, в котором содержится копия считанного файла из web
  //        (расширение файла не меняется, имя формируется с учетом URL)
  // size - размер буфера для записи пути к локальному файлу
  // При ошибке возвращает ноль
  int _fastcall GetFile(const WCHAR* url, WCHAR * path, int size);

  // Освободить ресурсы соединений
  void Close();

 protected:

  HANDLE    Control;
  WCHAR     WorkPath[MAX_PATH];
}
  TUrlFileReader;

#endif  // !_GISSERVER !HIDEWMS !HIDEMAPAPI
#endif  // MAPAPIVERSION >= 0x0120602

#endif  // MAPFILES_H

