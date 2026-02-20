/****** mapprof.cpp  ************ Belenkov O.V.   ***** 09/09/21 ***
******* mapprof.cpp  ************ Shabakov D.A.   ***** 22/05/18 ***
******* mapprof.cpp  ************ Derevyagina Zh.A.**** 23/05/17 ***
******* mapprof.cpp  ************ Kruzhkov A.E.   ***** 13/12/17 ***
******* mapprof.cpp  ************ Korjilov A.     ***** 29/12/14 ***
******* mapprof.cpp  ************ Savelov A.S.    ***** 25/02/19 ***
******* mapprof.cpp  ************ Vitsko D.A.     ***** 31/10/19 ***
******* mapprof.cpp  ************ Letov V.I.      ***** 14/12/17 ***
******* mapprof.cpp  ************ Zheleznyakov V.A. *** 18/12/18 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2021              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*                    ЧТЕНИЕ/ЗАПИСЬ INI-ФАЙЛА                       *
*                                                                  *
*******************************************************************/
#ifdef _AFXDLL 
  #include "stdafx.h"
#endif

#include "mapprof.h"

#ifndef HIDE_ERRORMESSAGE
extern "C"
{
_MAPIMP void _MAPAPI mapErrorMessageUn(long int code, const WCHAR *filename);
}
#endif

// ---------------------------------------------------------------------------
// Use system profile for filename==0. Use system to search in normal places
// if no path is included in filename.
// ---------------------------------------------------------------------------
TMapProfile::TMapProfile(const WCHAR * section, const WCHAR * filename, int create, int mode,
                         __int64 zposition, int filesize) : KeyList(), Buffer()
{
  Init();

  if (filename == 0 || filename[0] == 0)
    return;

  if (Open(filename, create, mode) != 0)
    Init(section);
}

TMapProfile::TMapProfile(HANDLE hfile, const char * section)
{
  Init();

  if (hfile)
  {
    hFile   = hfile;
    IsAlias = 1;

    Init(section);
  }
}

TMapProfile::TMapProfile(const WCHAR * filename, int mode) : KeyList(), Buffer()
{
  Init();

  if (filename == 0 || filename[0] == 0)
    return;

  Open(0, filename, mode);
}

TMapProfile::TMapProfile(const char * section, const WCHAR * filename, int create,
                         __int64 zposition, int filesize) : KeyList(), Buffer()
{
  Init();

  if (Open(filename, create, 0, zposition, filesize) != 0)
    Init(section);
}

// ---------------------------------------------------------------------------
// Use system profile for filename==0. Use system to search in normal places
// if no path is included in filename.
// ---------------------------------------------------------------------------
TMapProfile::TMapProfile(const char * section, const char * filename, int create) : KeyList(), Buffer()
{
  Init();

  if (filename == 0 || filename[0] == 0)
    return;

  WCHAR wfilename[MAX_PATH_LONG];
  StringToUnicode(filename, wfilename, sizeof(wfilename));

  if (Open(wfilename, create, 0) != 0)
    Init(section);
}

TMapProfile::TMapProfile(const char * filename) : KeyList(), Buffer()
{
  Init();

  if (filename == 0 || filename[0] == 0)
    return;

  WCHAR wfilename[MAX_PATH_LONG];
  StringToUnicode(filename, wfilename, sizeof(wfilename));

  Open(0, wfilename);
}

// ---------------------------------------------------------------------------
// Очистить переменные
// ---------------------------------------------------------------------------
void TMapProfile::Init()
{
  hFile      = 0;
  IsDirty    = 0;
  IsWrite    = 0;

  Offset     = 0;
  Zposition  = 0;
  FileSize   = 0;

  Input      = 0;
  Current    = 0;
  Next       = 0;
  Limit      = 0;

  StrW       = 0;
  StrWSize   = 0;
  InputSize  = 0;

  IsCreate   = 0;
  IsAlias    = 0;

  SectionName[0] = 0;
}

// ---------------------------------------------------------------------------
// ~TMapProfile
// ---------------------------------------------------------------------------
TMapProfile::~TMapProfile()
{
  Close();

  if (StrW)
  {
    delete [] StrW;
    StrW = 0;
    StrWSize = 0;
  }

  if (Input)
  {
    delete [] Input;
    Input = 0;
    InputSize = 0;
  }
}

// ------------------------------------------------------------------
// Закрыть доступ к файлу
// ------------------------------------------------------------------
void _fastcall TMapProfile::Close()
{
  CloseSection();

  if (hFile)
  {
    if (IsAlias == 0)
      CloseTheFile(hFile);
    hFile   = 0;
    IsAlias = 0;
  }

  IsCreate = 0;
  IsWrite  = 0;
}

// ------------------------------------------------------------------
// Закрыть секцию
// ------------------------------------------------------------------
void _fastcall TMapProfile::CloseSection()
{
  if (IsDirty)
  {
    // Обновить содержание секции
    Update(0);
    IsDirty = 0;
  }

  KeyList.Clear();
  Buffer.Clear();

  SectionName[0] = 0;
}

// ---------------------------------------------------------------------------
// Открыть INI-файл
// ---------------------------------------------------------------------------
int _fastcall TMapProfile::Open(const WCHAR * section, const WCHAR * filename, int mode, __int64 zposition, int filesize)
{
  if ((filename == 0) || (Open(filename, (int)0, mode, zposition, filesize) != 0))
    return Init(section);

  return 0;
}

int _fastcall TMapProfile::Open(const WCHAR * filename, int create, int mode, __int64 zposition, int filesize)
{
  Close();

  if (filename == 0 || filename[0] == 0)
    return 0;

  if (zposition != 0)
    mode = GENERIC_READ;
  else
  {
    mode &= (GENERIC_READ|GENERIC_WRITE);
    if (mode == 0)
      mode = (GENERIC_READ|GENERIC_WRITE);
  }

  int attribute;
  if (mode == GENERIC_READ)
    attribute = OPEN_EXISTING;
  else
    attribute = OPEN_ALWAYS;

  // Открыть INI-файл
  hFile = OpenTheFile(filename, mode, FILE_SHARE_READ|FILE_SHARE_WRITE, attribute);
  if (hFile == 0)
  {
    if (mode == GENERIC_READ)
      return 0;

    hFile = OpenTheFile(filename, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING);
    if (hFile == 0)
      return 0;
  }
  else
  {
    if (mode == (GENERIC_READ|GENERIC_WRITE))
      IsWrite = 1;
  }

  IsCreate = create;

  Zposition = zposition;
  FileSize  = filesize;

  return 1;
}

// ---------------------------------------------------------------------------
// Стать на начало файла
// ---------------------------------------------------------------------------
int TMapProfile::BeginRead()
{
  if (hFile == 0)
    return 0;

  if (Zposition == 0)
    FileSize = (int)LengthTheFile(hFile);

  OffsetTheFile(hFile, Zposition, FILE_BEGIN);
  Offset = 0;

  return ReadBlock();
}

// ---------------------------------------------------------------------------
// Чтение очередного блока из файла
// ---------------------------------------------------------------------------
int TMapProfile::ReadBlock()
{
  Current    = 0;
  Next       = 0;
  Limit      = 0;

  int readsize = InputSize;
  if (Zposition)
  {
    // Чтение INI-файла, записанного внутри хранилища (другого файла) 
    int limit = (int)(FileSize - Offset);
    if (limit <= 0)
      return 0;

    if (readsize > limit)
      readsize = limit;
  }

  long res;
  if ((ReadTheFile(hFile, Input, readsize, &res) == 0) || (res == 0))
    return 0;

  Offset += res;

  Current = Input;
  Next    = Input;
  Limit   = Input + res - 1;

  const unsigned char utf8[] = { 0xEF, 0xBB, 0xBF };

  // Пропустить признак файла в кодировке UTF8
  if ((Next+3 <= Limit) && memcmp(Next, utf8, 3) == 0)  // "п»ї"
    Next += 3;

  return 1;
}

// ---------------------------------------------------------------------------
// Чтение очередной строки из файла
// isxml - признак чтения многострочного значения (не обнулять концы строк)
// ---------------------------------------------------------------------------
char * TMapProfile::ReadString()
{
  if ((hFile == 0) || (Next == 0))
    return 0;

  Current = Next;
  if (Current > Limit)
  {
    if (Limit < (Input + (InputSize - 1)))
    {
      // Конец файла
      return 0;
    }

    if (ReadBlock() == 0)
      return 0;
  }

  while(1)
  {
    // Найти конец строки
    while(Next <= Limit)
    {
      if ((*Next == '\n') || (*Next == '\r'))
      {
        *Next = 0;
        break;
      }

      Next++;
    }

    if (Next > Limit)
    {
      if ((Current == Input) || (Limit < (Input + (InputSize - 1))))
      {
        // Какой-то мусор - оборвем запись принудительно
        // или конец файла
        *Next = 0;
        return Current;
      }

      int delta = (int)(Next - Current);
      if (delta > 0)
        memmove(Input, Current, delta);

      Current = Input;
      Next    = Input + delta;
      *Next   = 0;
      Limit   = Next;

      long res;
      if ((ReadTheFile(hFile, Next, InputSize-delta, &res) == 0) || (res == 0))
      {
        if (*Current == 0)
          return 0;
        return Current;
      }

      Limit = Next + res - 1;
    }
    else
      break;
  }

  // Убрать в начале строки все, что меньше пробела и пробелы
  while((*Next == 0) && (Next <= Limit))
  {
    Next++;
    if (*((unsigned char*)Next) <= ' ')
      *Next = 0;
    else
      break;
  }

  return Current;
}

// ---------------------------------------------------------------------------
// Запись в файл
// ---------------------------------------------------------------------------
int _fastcall TMapProfile::WriteString(HANDLE hfile, const char * buffer, int size)
{
  if ((hfile == 0) || (buffer == 0) || (IsWrite == 0))
    return 0;

  if (size <= 0)
    size = (int)strlen(buffer);
  if (size == 0)
    return 0;

  long res;
  if ((WriteTheFile(hfile, buffer, size, &res) == 0) || (res != size))
    return 0;

  return size;
}

// ---------------------------------------------------------------------------
// Инициализировать переменные класса для текущей секции
// Если секция не найдена возвращает -1
// ---------------------------------------------------------------------------
int _fastcall TMapProfile::Init(const char * section)
{
  if ((section == 0) || (*section == 0) || (hFile == 0))
    return 0;

  // Если секция является текущей
  if (_stricmp(section, SectionName) == 0)
    return 1;

  if (StrW == 0)
  {
    // Выделим память
    try
    {
      StrW = new WCHAR[MAXSECSTRINGSIZE];
    }
    catch(...)
    {
      StrW = 0;
    }

    if (StrW == 0)
      return 0;

    StrWSize = MAXSECSTRINGSIZE*sizeof(WCHAR);
  }

  StrW[0] = 0;

  // Выделить память под входную переменную Input
  if (AllocateInputParm() == 0)
    return 0;

  // При смене секции
  if (SectionName[0] != 0)
  {
    // Обновить содержание секции
    // Если флаг reopen == 0, то при смене секции ini-файл закроется (hFile == 0)
    // и больше записываться в него ничего не будет
    Update(1);
  }

  KeyList.Clear();
  Buffer.Clear();

  // Установить имя текущей секции
  StrCopy(SectionName, section, sizeof(SectionName));

  int seekflag = 0;

  // Стать на начало файла - файл может быть пустым
  if (BeginRead() == 0)
    return -1;

  // Найти секцию
  while(seekflag == 0)
  {
    char * value = ReadString();
    if (value == 0)
    {
      // Считаем, что секция не найдена
      break;
    }

    // Это начало секции ?
    if (value[0] == '[')
    {
      char * current = value + 1;
      int size = (int)strlen(value);
      for (int i = 0; i < size; i++)
      {
        if (*current == ']')
        {
          *current = 0;
          size = i;
          break;
        }

        current++;
      }

      if (_stricmp(&value[1], SectionName) == 0)
      {
        // Секция найдена
        seekflag = 1;
        break;
      }
    }
  }

  if (seekflag == 0)
    return -1;

  if (IsCreate)                                                                   // 10/05/20
    return 1;

  if (_strnicmp(SectionName, "#xml#", 5) == 0)                                    // 28/10/20
  {
    const char * xmlkey = SectionName + 5;

    TLongString buffer;

    // Секция найдена - заполним описание
    while(1)
    {
      char * value = ReadString();
      if (value == 0)
        break;

      if (*value == '[')
        // Начало новой секции - конец данных
        break;

      buffer.Append(value);
    }

    KeyList.Append(xmlkey, buffer.getString());
    return 1;
  }

  // Секция найдена - заполним описание
  while(1)
  {
    char * value = ReadString();
    if (value == 0)
      break;

    if (*value == '[')
      // Начало новой секции - конец данных
      break;

    // Выделим из строки ключ и значение
    char * current = value;
    char * limit   = current + strlen(value);

    // Убрать начальные пробелы
    while(current < limit)
    {
      if (*current != ' ')
        break;
      current++;
    }

    char * first = current;

    while(current < limit)
    {
      if (*current == '=')
      {
        for(char * point = current - 1; point >= first; point--)                  // 30/03/21
        {
          // Убрать пробелы в конце имени ключа
          if (*point != ' ')
            break;

          *point = 0; 
        }

        *current = 0;
        current++;

        if (first == current)
        {
          // Нет имени ключа
          break;
        }


        const char * key = first;

        int i = (int)strlen(current);
        if ((i > 0) && (current[i-1] == '\n'))
        {
          current[i-1] = 0;
          i--;
#ifdef LINUXAPI
          // Уберем, если есть windows-символ '\r'
          i = strlen(current);
          if ((i > 0) && (current[i-1] == '\r'))
          {
            current[i-1] = 0;
            i--;
          }
#endif
        }

        const char * name = current;

        // Строка обработана
        KeyList.Append(key, name);
        break;
      }

      current++;
    }
  }

  return 1;
}

// ---------------------------------------------------------------------------
// Выделить память под входную переменную Input
// ---------------------------------------------------------------------------
int TMapProfile::AllocateInputParm()
{
  if (Input == 0)
  {
    // Выделим память
    try
    {
      Input = new char[MAXSECSTRINGSIZE + 16];
    }
    catch(...)
    {
      Input = 0;
    }

    if (Input == 0)
      return 0;

    InputSize = MAXSECSTRINGSIZE;
  }

  Input[0] = 0;

  return 1;
}

// ------------------------------------------------------------------
// Обновить содержание секции
// ------------------------------------------------------------------
int _fastcall TMapProfile::Update(int reopen)
{
  if ((IsWrite && hFile) == 0)
    return 0;

  if (Input == 0 || InputSize == 0)
    return 0;

  // Если ничего не редактировали - не перезаписывать
  if (IsDirty == 0)
    return 1;

  // Стать на начало файла - файл может быть пустым
  BeginRead();

  // Переписать основной файл во временный с заменой секции
  int sectionflag = 0;

  int writeSectionFlag = 0;

  TLongString temp;

  // Найти секцию
  while(1)
  {
    char * value = ReadString();
    if (value == 0)
    {
      // Считаем, что секция не найдена
      break;
    }

    // Это начало секции ?
    if (value[0] == '[')
    {
      if (sectionflag != 0)
        sectionflag = 0;

      char * current = &value[1];
      char symbol = 0;
      int size = (int)strlen(value);
      for (int i = 1; i < size; i++)
      {
        if ((*current == ']') || (*current == ' '))
        {
          symbol   = *current;
          *current = 0;
          break;
        }

        current++;
      }

      if (_stricmp(&value[1], SectionName) == 0)
      {
        // Секция найдена
        sectionflag = 1;

        if ((IsCreate == 0) && (KeyList.Count() > 0))
        {
          writeSectionFlag = 1;

          // Дописать во временный файл секцию
          temp.Append("[");
          temp.Append(SectionName);
          temp.Append("]");
#ifndef LINUXAPI
          temp.Append("\r\n");
#else
          temp.Append("\n");
#endif
          AppendSection(temp);
        }
      }
      else
      {
        if (symbol)
          *current = symbol;
      }
    }

    // Это не пропускаемая секция?
    if (sectionflag == 0)
    {
      // Записать строку во временный файл
      temp.Append(value);
#ifndef LINUXAPI
      temp.Append("\r\n");
#else
      temp.Append("\n");
#endif
    }
  }

  if (writeSectionFlag == 0)
  {
    // Дописать во временный файл секцию
    temp.Append("[");
    temp.Append(SectionName);
    temp.Append("]");

#ifndef LINUXAPI
    temp.Append("\r\n");
#else
    temp.Append("\n");
#endif

    if (IsCreate != 0)
    {
      // Дописать буфер
      if ((Buffer.getString() != 0) && (Buffer.getSize() > 0))
        temp.Append(Buffer.getString());
    }
    else
    {
      AppendSection(temp);
    }
  }

  // Сохранить новое содержание файла
  OffsetTheFile(hFile, 0, FILE_BEGIN);
  long int ret;
  WriteTheFile(hFile, temp.getString(), temp.getSize(), &ret);
  EndTheFile(hFile);

  return 1;
}

// ------------------------------------------------------------------
// Дописать секцию в буфер
// ------------------------------------------------------------------
int _fastcall TMapProfile::AppendSection(TLongString& temp)
{
  if (_strnicmp(SectionName, "#xml#", 5) == 0)                                    // 30/10/20
  {
    const char * name = KeyList.GetName(1);
    if ((name == 0) || (*name == 0))
      return 0;

    temp.Append(name);
#ifndef LINUXAPI
    temp.Append("\r\n");
#else
    temp.Append("\n");
#endif
    return 1;
  }

  int count = KeyList.Count();

  for (int i = 1; i <= count; i++)
  {
    const char * key = KeyList.GetKey(i);
    if ((key == 0) || (*key == 0))
      continue;

    temp.Append(key);
    temp.Append("=");

    const char * name = KeyList.GetName(i);
    if ((name != 0) && (*name != 0))
    {
      temp.Append(name);

#ifndef LINUXAPI
      temp.Append("\r\n");
#else
      temp.Append("\n");
#endif
    }
  }

  return count;
}

// ------------------------------------------------------------------
// Найти значение по ключу
// ------------------------------------------------------------------
const char * _fastcall TMapProfile::GetNumberByKey(const char * findkey, int& number)
{
  if ((findkey == 0) || (*findkey == 0))
    return 0;

  int count = KeyList.Count();

  if ((number < 0) || (number >= count))
    return 0;

  int size = (int)strlen(findkey);

  for(int i = number+1; i <= count; i++)
  {
    const char * key = KeyList.GetKey(i);
    if ((key == 0) || (*key == 0))
      continue;

    if (_strnicmp(key, findkey, size) == 0)
    {
      number = i;
      return key;                                                                 // 25/05/20
    }
  }

  return 0;
}

// ------------------------------------------------------------------
// Запросить ключ по номеру
// ------------------------------------------------------------------
const char * _fastcall TMapProfile::GetKeyByNumber(int number)
{
  return KeyList.GetKey(number);
}

// ------------------------------------------------------------------
// Запросить значение по номеру
// ------------------------------------------------------------------
int _fastcall TMapProfile::GetValueByNumber(int number, WCHAR * buffer, int size)
{
  if (buffer == 0)
    return 0;

  *buffer = 0;

  if (KeyList.GetKey(number) == 0)
    return 0;

  const char * value = KeyList.GetName(number);
  if ((value == 0) || (*value == 0))
    return 1;

  if (*value == '%')
    return Unicode8ToUnicode(value+1, buffer, size);

  return AnsiStringToUnicode(value, buffer, size);                                // 09/06/20
}

int _fastcall TMapProfile::GetValueByNumber(int number, char * buffer, int size)
{
  if (buffer == 0)
    return 0;

  *buffer = 0;

  if (KeyList.GetKey(number) == 0)
    return 0;

  const char * value = KeyList.GetName(number);
  if ((value == 0) || (*value == 0))
    return 1;

  return (StrCopy(buffer, value, size) != 0);
}

// ------------------------------------------------------------------
// Запросить значение по номеру
// ------------------------------------------------------------------
int _fastcall TMapProfile::GetValueIntByNumber(int number)
{
  if (KeyList.GetKey(number) == 0)
    return 0;

  const char * value = KeyList.GetName(number);
  if ((value == 0) || (*value == 0))
    return 0;

  return atoi(value);
}

// ------------------------------------------------------------------
// Запросить значение строки по ключу
// ------------------------------------------------------------------
const char * TMapProfile::GetValueByKey(const char * findkey)
{
  if ((findkey == 0) || (*findkey == 0))
    return 0;

  int count = KeyList.Count();

  int size = (int)strlen(findkey);

  for(int i = 1; i <= count; i++)
  {
    const char * key = KeyList.GetKey(i);
    if ((key == 0) || (*key == 0))
      continue;

    if ((_stricmp(key, findkey) == 0) && (size == (int)strlen(key)))
      return KeyList.GetName(i);
  }

  return 0;
}

// ------------------------------------------------------------------
// Retrieve an integer value from the profile
// ------------------------------------------------------------------
int _fastcall TMapProfile::GetInt(const char * key, int defaultInt)
{
  const char * value = GetValueByKey(key);
  if (value != 0)
  {
    // 16-ричные числа не обрабатываются функцией atol()
    int i = 0;
    sscanf(value, "%i", &i);
    return i;
  }

  return defaultInt;
}

// ------------------------------------------------------------------
// Return all section values if key==0
// ------------------------------------------------------------------
const char * _fastcall TMapProfile::GetString8(const char * key, int& isutf8)
{
  isutf8 = 0;

  char key8[MAX_PATH];
  StrCopy(key8, key, sizeof(key8));
  StrCat(key8, "%", sizeof(key8));

  const char * value = GetValueByKey(key8);
  if (value == 0 || value[0] == 0)
  {
    value = GetValueByKey(key);
    if (value == 0)
      return 0;
  }

  if (*value == '%')
  {
    isutf8 = 1;
    value++;
  }

  return value;
}

int _fastcall TMapProfile::GetString(const char * key, char * buff,
                                     unsigned int buffSize, const char * defaultString)
{
  if ((buff == 0) || (buffSize <= 0))
    return 0;

  int isutf8 = 0;
  const char * value = GetString8(key, isutf8);
  if (value == 0)
  {
    if (defaultString)
    {
      StrCopy(buff, defaultString, buffSize);
      return 1;
    }
    return 0;
  }

  if (isutf8)
    return Unicode8ToString(value, buff, buffSize);

  StrCopy(buff, value, buffSize);
  return 1;
}

int _fastcall TMapProfile::GetString(const WCHAR * key, WCHAR * buff,
                                     unsigned int buffSize, const WCHAR * defaultString)
{
  if ((key == 0) || (buff == 0) || (buffSize < 2))
    return 0;

  char akey[MAX_KEYWORDSIZE];
  UnicodeToString(key, akey, sizeof(akey));

  int isutf8 = 0;
  const char * value = GetString8(akey, isutf8);
  if (value == 0)
  {
    if (defaultString)
    {
      WcsCopy(buff, defaultString, buffSize);
      return 1;
    }
    return 0;
  }

  if (isutf8)
    return Unicode8ToUnicode(value, buff, buffSize);

  return AnsiStringToUnicode(value, buff, buffSize);
}

int _fastcall TMapProfile::GetString(const char * key, WCHAR * buff,
                                     unsigned int buffSize, const char * defaultString)
{
  if ((key == 0) || (buff == 0) || (buffSize == 0))
    return 0;

  int isutf8 = 0;
  const char * value = GetString8(key, isutf8);
  if (value == 0)
  {
    if (defaultString)
    {
      StringToUnicode(defaultString, buff, buffSize);
      return 1;
    }
    return 0;
  }

  if (isutf8)
    return Unicode8ToUnicode(value, buff, buffSize);

  return AnsiStringToUnicode(value, buff, buffSize);
}

int _fastcall TMapProfile::GetIndexString(const char * key, int index, char * buff,
                                          unsigned int buffSize, const char * defaultValue)
{
  char akey[64];
  StrCopy(akey, key, 32);
  int size = (int)strlen(akey);
  IntToStr(index, akey+size, sizeof(akey)-size);

  return GetString(akey, buff, buffSize, defaultValue);
}

int _fastcall TMapProfile::GetIndexString(const char * key, int index, WCHAR * buff,
                                          unsigned int buffSize, const char * defaultValue)
{
  char akey[64];
  StrCopy(akey, key, 32);
  int size = (int)strlen(akey);
  IntToStr(index, akey+size, sizeof(akey)-size);

  return GetString(akey, buff, buffSize, defaultValue);
}

int _fastcall TMapProfile::GetString(const char * key, WCHAR * buff,
                                     unsigned int buffSize, const WCHAR * defaultString)
{
  if ((key == 0) || (buff == 0) || (buffSize == 0))
    return 0;

  int isutf8 = 0;
  const char * value = GetString8(key, isutf8);
  if (value == 0)
    {
      if (defaultString)
        {
          WcsCopy(buff, defaultString, buffSize);
          return 1;
        }
      return 0;
    }

  if (isutf8)
    return Unicode8ToUnicode(value, buff, buffSize);

  return AnsiStringToUnicode(value, buff, buffSize);
}

// ------------------------------------------------------------------
// Удалить запись
// ------------------------------------------------------------------
void _fastcall TMapProfile::DeleteKey(const char * key, int index)
{
  char akey[64];
  StrCopy(akey, key, 32);
  int size = (int)strlen(akey);
  IntToStr(index, akey+size, sizeof(akey)-size);

  DeleteKey(akey);
}

// ------------------------------------------------------------------
// Write an string value to the profile
// ------------------------------------------------------------------
int _fastcall TMapProfile::WriteString(const WCHAR * key, const WCHAR * str)
{
  if (key == 0)
    return 0;

  char akey[MAX_KEYWORDSIZE];
  UnicodeToString(key, akey, sizeof(akey));
  return WriteString(akey, str);
}

int _fastcall TMapProfile::WriteIndexString(const char * key, int index, const WCHAR * str)
{
  char akey[64];
  StrCopy(akey, key, 32);
  int size = (int)strlen(akey);
  IntToStr(index, akey+size, sizeof(akey)-size);

  return WriteString(akey, str);
}

int _fastcall TMapProfile::WriteString(const char * key, const WCHAR * str)
{
  if ((str == 0) || (*str == 0))
    return WriteString8(key, 0);

  // Выделить память под входную переменную Input
  if (AllocateInputParm() == 0)
    return 0;

  Input[0] = '%';
  UnicodeToUnicode8(str, Input+1, InputSize-1);

  return WriteString(key);
}

int _fastcall TMapProfile::WriteIndexString(const char * key, int index, const char * str)
{
  char akey[64];
  StrCopy(akey, key, 32);
  int size = (int)strlen(akey);
  IntToStr(index, akey+size, sizeof(akey)-size);

  return WriteString(akey, str);
}

int _fastcall TMapProfile::WriteString(const char * key, const char * str)
{
  if ((str == 0) || (*str == 0))
    return WriteString8(key, str);

  // Выделить память под входную переменную Input
  if (AllocateInputParm() == 0)
    return 0;

  Input[0] = '%';
  StringToUnicode8(str, Input+1, InputSize-1);

  return WriteString(key);
}

int _fastcall TMapProfile::WriteString(const char * key)                          // 29/04/20
{
  if (key == 0)
    return 0;

  int isutf8 = 0;
  signed char * point = (signed char *)Input;
  int size = InputSize;
  while(size-- > 0)
  {
    if (*point == 0) break;
    if (*point < 0)
    {
      isutf8 = 1;
      break;
    }

    point++;
  }

  char key8[MAX_PATH];
  StrCopy(key8, key, sizeof(key8));
  StrCat(key8, "%", sizeof(key8));

  if (isutf8)
  {
    // Удалить строку
    WriteString8(key, 0);

    return WriteString8(key8, Input);
  }
  else
  {
    // Удалить строку
    WriteString8(key8, 0);
  }

  // Записать обычный ключ в ANSI
  return WriteString8(key, Input+1);
}

// ------------------------------------------------------------------
// Write an string value to the profile
// ------------------------------------------------------------------
int _fastcall TMapProfile::WriteString8(const char * key, const char * str)
{
  if ((key == 0) || (*key == 0))
    return 0;

  if (IsCreate != 0)
  {
    if ((str == 0) || (*str == 0))
      return 0;

    Buffer.Append(key, (int)strlen(key));
    Buffer.Append("=", 1);
    Buffer.Append(str, (int)strlen(str));
#ifdef WIN32API
    Buffer.Append("\r\n", 2);
#else
    Buffer.Append("\n", 1);
#endif
    IsDirty = 1;
    return 1;
  }

  int number = KeyList.FindNumber(key);
  if (number == 0)
  {
    if ((str == 0) || (*str == 0))
    {
      char akey[256];
      StrCopy(akey, key, sizeof(akey));
      int size = (int)strlen(akey);
      if (akey[size-1] == '%')
        akey[size-1] = 0;
      else
        StrCat(akey, "%", sizeof(akey));
      number = KeyList.FindNumber(akey);

      if (number == 0)
        return 0;

      IsDirty = 1;

      // Значение удалено
      return KeyList.Delete(number);
    }

    IsDirty = 1;

    // Добавить новое значение
    return KeyList.Append(key, str);
  }

  if ((str == 0) || (*str == 0))
  {
    IsDirty = 1;

    // Значение удалено
    return KeyList.Delete(number);
  }

  IsDirty = 1;

  // Значение обновлено
  return KeyList.UpdateName(number, str);
}

// ------------------------------------------------------------------
// Read XML section
// ------------------------------------------------------------------
int _fastcall TMapProfile::ReadXml(const char * section, const char * key, TLongString& xmlvalue)
{
  TMapProfile temp(hFile, section);

  xmlvalue.Clear();
  int isutf8;
  return (xmlvalue.Append(temp.GetString8(key, isutf8)) != 0);
}

// ------------------------------------------------------------------
// Write XML section
// ------------------------------------------------------------------
int  _fastcall TMapProfile::WriteXml(const char * section, const char * xmlvalue)
{
  if ((section == 0) || (xmlvalue == 0))
    return 0;

  if (_strnicmp(section, "#xml#", 5) != 0)
    return 0;

  CloseSection();

  // Установить имя текущей секции
  StrCopy(SectionName, section, sizeof(SectionName));

  IsCreate = 0;

  WriteString8(section + 5, xmlvalue);

  CloseSection();
  return 1;
}

