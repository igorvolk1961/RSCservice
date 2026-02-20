/****  classmacro.h  ********  Zheleznyakov  V.A. ***** 15/04/22 ***
*****  classmacro.h  ********  Savelov A.S.       ***** 16/10/18 ***
*****  classmacro.h  ********  Letov V.I.         ***** 28/05/19 ***
*****  classmacro.h  ********  Derevyagina Zh.A.  ***** 21/04/17 ***
*****  classmacro.h  ********  Belenkov O.V.      ***** 26/11/21 ***
*****  classmacro.h  ********  Portnyagin E.V.    ***** 28/02/22 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2022              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*                  Определение имён классов                        *
*            Необходимо устнавить в настройках проекта             *
*        condition defines CLASSPREFIX=Название префикса класса    *
*                                                                  *
*******************************************************************/

#ifndef CLASSMACRO_H
#define CLASSMACRO_H

// Для обеспечения надежной работы 64-ех разрядных приложений,
// включающих несколько динамических библиотек, в которых применяются
// одни и те же общие классы, необходимо обеспечить, чтобы компилятор
// автоматически формировал разные имена классам в разных библиотеках.
// Для этого в проектах библиотек необходимо определить уникальный "префикс"
// проекта.
// Например, в gis64mapacces: CLASSPREFIX=MapAcc, в objectmath: CLASSPREFIX=Math
// Определение префикса необходимо делать в Windows и Linux

#ifndef CLASSNAME
  #define CLASSNAME
  #define _XClassName(a,b) _ClassName(a,b) // стрингификация
  #define _ClassName(a,b) a##b // склеивание
#endif

// simlist
#define TFastHandleList _XClassName(TFastHandleList, CLASSPREFIX)
#define THandleList _XClassName(THandleList, CLASSPREFIX)
#define TFastSimpleListBase _XClassName(TFastSimpleListBase, CLASSPREFIX)
#define TFastSimpleList _XClassName(TFastSimpleList, CLASSPREFIX)
#define TSimpleList _XClassName(TSimpleList, CLASSPREFIX)
#define TNumberList _XClassName(TNumberList, CLASSPREFIX)
#define TFastNumberList _XClassName(TFastNumberList, CLASSPREFIX)
#define TNumber64List _XClassName(TNumber64List, CLASSPREFIX)
#define TDoubleList _XClassName(TDoubleList, CLASSPREFIX)
#define TNumberListOrder _XClassName(TNumberListOrder, CLASSPREFIX)
#define TFastNumberListOrder _XClassName(TFastNumberListOrder, CLASSPREFIX)
#define TBigNumberList _XClassName(TBigNumberList, CLASSPREFIX)
#define TSimpleListOrder _XClassName(TSimpleListOrder, CLASSPREFIX)
#define TFastSimpleListOrder _XClassName(TFastSimpleListOrder, CLASSPREFIX)
#define TUniqueInt64ListOrder _XClassName(TUniqueInt64ListOrder, CLASSPREFIX)
#define TInt32ListOrder _XClassName(TInt32ListOrder, CLASSPREFIX)
#define TFastInt32ListOrder _XClassName(TFastInt32ListOrder, CLASSPREFIX)
#define TInt32ListOrderEx _XClassName(TInt32ListOrderEx, CLASSPREFIX)
#define TBigSimpleListOrder _XClassName(TBigSimpleListOrder, CLASSPREFIX)
#define TNumberListIndexOrder _XClassName(TNumberListIndexOrder, CLASSPREFIX)
#define TFastNumberListIndexOrder _XClassName(TFastNumberListIndexOrder, CLASSPREFIX)
#define TUniqueNumberList _XClassName(TUniqueNumberList, CLASSPREFIX)
#define TKeyTreeList _XClassName(TKeyTreeList, CLASSPREFIX)
#define TKeyListOrder _XClassName(TKeyListOrder, CLASSPREFIX)
#define TInt32List _XClassName(TInt32List, CLASSPREFIX)
#define TDFrameList _XClassName(TDFrameList, CLASSPREFIX)
#define TLongPointList _XClassName(TLongPointList, CLASSPREFIX)
#define TIdentNames _XClassName(TIdentNames, CLASSPREFIX)
#define TIdentNamesA _XClassName(TIdentNamesA, CLASSPREFIX)
#define TRectListOrder _XClassName(TRectListOrder, CLASSPREFIX)
#define TRectList _XClassName(TRectList, CLASSPREFIX)
#define TSheetRectList _XClassName(TSheetRectList, CLASSPREFIX)

// cmdparser
#define TParmList _XClassName(TParmList, CLASSPREFIX)
#define TLayerList _XClassName(TLayerList, CLASSPREFIX)
#define TRoleList _XClassName(TRoleList, CLASSPREFIX)
#define TParmSet _XClassName(TParmSet, CLASSPREFIX)
#define TParmParser _XClassName(TParmParser, CLASSPREFIX)

// xmlrpc
#define TParmXmlRpcParser _XClassName(TParmXmlRpcParser, CLASSPREFIX)

// log
#define TLog _XClassName(TLog, CLASSPREFIX)
#define TAppLog _XClassName(TAppLog, CLASSPREFIX)
#define TSystemLog _XClassName(TSystemLog, CLASSPREFIX)
#define TDiagnostics _XClassName(TDiagnostics, CLASSPREFIX)

// longstring
// Volkov I.A. BEGIN
//#define TLongString _XClassName(TLongString, CLASSPREFIX)
// Volkov I.A. END
#define TLongStringUn _XClassName(TLongStringUn, CLASSPREFIX)
#define TLongStringArray _XClassName(TLongStringArray, CLASSPREFIX)
#define TLongStringList _XClassName(TLongStringList, CLASSPREFIX)
#define TLongStringArrayUn _XClassName(TLongStringArrayUn, CLASSPREFIX)
#define TLongStringArrayUnOrder _XClassName(TLongStringArrayUnOrder, CLASSPREFIX)
#define TKeyNames _XClassName(TKeyNames, CLASSPREFIX)
#define TKeyNamesA _XClassName(TKeyNames, CLASSPREFIX)
#define TFileList _XClassName(TFileList, CLASSPREFIX)
#define TLongStringBigList _XClassName(TLongStringBigList, CLASSPREFIX)
#define TLongStringListUn _XClassName(TLongStringListUn, CLASSPREFIX)
#define TLongStringAndIdList _XClassName(TLongStringAndIdList, CLASSPREFIX)
#define ReplaceSymbol _XClassName(ReplaceSymbol, CLASSPREFIX)
#define SearchSubString _XClassName(SearchSubString, CLASSPREFIX)
#define ReplaceForbiddenFolderSymbol _XClassName(ReplaceForbiddenFolderSymbol, CLASSPREFIX)
#define ReplaceCharSymbol _XClassName(ReplaceCharSymbol, CLASSPREFIX)
#define IsStringDigit _XClassName(IsStringDigit, CLASSPREFIX)
#define RemoveSymbol _XClassName(RemoveSymbol, CLASSPREFIX)
#define RemoveCharSymbol _XClassName(RemoveCharSymbol, CLASSPREFIX)
#define RemoveLastSymbols _XClassName(RemoveLastSymbols, CLASSPREFIX)
#define RemoveLastCharSymbols _XClassName(RemoveLastCharSymbols, CLASSPREFIX)
#define ReplaceEachSymbol _XClassName(ReplaceEachSymbol, CLASSPREFIX)
#define ReplaceEachCharSymbol _XClassName(ReplaceEachCharSymbol, CLASSPREFIX)
#define RemoveEachSymbol _XClassName(RemoveEachSymbol, CLASSPREFIX)
#define RemoveEachCharSymbol _XClassName(RemoveEachCharSymbol, CLASSPREFIX)
#define ReplaceSlashSymbol _XClassName(ReplaceSlashSymbol, CLASSPREFIX)
#define TLongStringArrayUnList _XClassName(TLongStringArrayUn, CLASSPREFIX)
#define TLongStringArrayType _XClassName(TLongStringArrayType, CLASSPREFIX)
#define RemoveQuote _XClassName(RemoveQuote, CLASSPREFIX)
#define TLongStringUnAndIdList _XClassName(TLongStringUnAndIdList, CLASSPREFIX)
#define TFileList8 _XClassName(TFileList8, CLASSPREFIX)
#define TFileBuffer _XClassName(TFileBuffer, CLASSPREFIX)

// xmlfile
#define XMLATTRVALUEUN _XClassName(XMLATTRVALUEUN, CLASSPREFIX)
#define XMLATTRVALUE _XClassName(XMLATTRVALUE, CLASSPREFIX)
#define XMLNODE _XClassName(XMLNODE, CLASSPREFIX)
#define XMLATTRNODE _XClassName(XMLATTRNODE, CLASSPREFIX)
#define TXmlFile _XClassName(TXmlFile, CLASSPREFIX)
#define THtmReport _XClassName(THtmReport, CLASSPREFIX)
#define BIGFILE _XClassName(BIGFILE, CLASSPREFIX)
#define xwGetStringValue _XClassName(xwGetStringValue, CLASSPREFIX)
#define TCsvFile _XClassName(TCsvFile, CLASSPREFIX)

// transaction
#define TTransaction _XClassName(TTransaction, CLASSPREFIX)
#define TSitX _XClassName(TSitX, CLASSPREFIX)
#define LFILE _XClassName(LFILE, CLASSPREFIX)

// mapprof
#define TIniProfile _XClassName(TIniProfile, CLASSPREFIX)
#define TMapProfile _XClassName(TMapProfile, CLASSPREFIX)

// wmsacces
#define TSQLiteConnect _XClassName(TSQLiteConnect, CLASSPREFIX)
#define TBIR _XClassName(TBIR, CLASSPREFIX)
#define CrsRegisterXml _XClassName(CrsRegisterXml, CLASSPREFIX)
#define UserHandleClass _XClassName(UserHandleClass, CLASSPREFIX)
#define TCoreLog _XClassName(TCoreLog, CLASSPREFIX)
#define TOgcMatrixList _XClassName(TOgcMatrixList, CLASSPREFIX)
#define TTileMatrix _XClassName(TTileMatrix, CLASSPREFIX)
#define base64encode _XClassName(base64encode, CLASSPREFIX)
#define base64decode _XClassName(base64decode, CLASSPREFIX)
#define WmsHttpConnect _XClassName(WmsHttpConnect, CLASSPREFIX)
#define TCurlConnectLoader _XClassName(TCurlConnectLoader, CLASSPREFIX)
#define CurlConnect _XClassName(CurlConnect, CLASSPREFIX)
#define HttpConnect _XClassName(HttpConnect, CLASSPREFIX)
#define HttpResponceBuffer _XClassName(HttpResponceBuffer, CLASSPREFIX)
#define TGDALVector _XClassName(TGDALVector, CLASSPREFIX)

// tmlist
#define LISTSTACK _XClassName(LISTSTACK, CLASSPREFIX)

//mapfiles
#define TFindData _XClassName(TFindData, CLASSPREFIX)
#define TFindControl _XClassName(TFindControl, CLASSPREFIX)
#define TRunThread _XClassName(TRunThread, CLASSPREFIX)
#define TPrepareFile _XClassName(TPrepareFile, CLASSPREFIX)
#define TCopyFile _XClassName(TCopyFile, CLASSPREFIX)
#define TPrepareFileHeader _XClassName(TPrepareFileHeader, CLASSPREFIX)
#define TCompareFile _XClassName(TCompareFile, CLASSPREFIX)
#define TBigFile _XClassName(TBigFile, CLASSPREFIX)
#define TUrlFileReader _XClassName(TUrlFileReader, CLASSPREFIX)

//gdalserv
#define TWorkLog _XClassName(TWorkLog, CLASSPREFIX)

//json
#define TJsonFile _XClassName(TJsonFile, CLASSPREFIX)
#define JSONNODE _XClassName(JSONNODE, CLASSPREFIX)
#define TJsonList _XClassName(TJsonList, CLASSPREFIX)

//geojson
#define TErrorLog _XClassName(TErrorLog, CLASSPREFIX)
#define TGeoJson _XClassName(TGeoJson, CLASSPREFIX)

#ifdef BUILD_RSC
// mapdrw
#define TObjectDraw _XClassName(TObjectDraw, CLASSPREFIX)
#define TDrawEdit   _XClassName(TDrawEdit, CLASSPREFIX)
#define TTabCluster _XClassName(TTabCluster, CLASSPREFIX)

// vecimged
#define TVectorImageEdit _XClassName(TVectorImageEdit, CLASSPREFIX)

#define TMapRsc   _XClassName(TMapRsc,   CLASSPREFIX)
#define TP3DTable _XClassName(TP3DTable, CLASSPREFIX)
#define TP3DFile  _XClassName(TP3DFile,  CLASSPREFIX)
#define TBmpBase  _XClassName(TBmpBase,  CLASSPREFIX)
#define TCopyFile _XClassName(TCopyFile, CLASSPREFIX)
#define TPrepareFile _XClassName(TPrepareFile, CLASSPREFIX)
#define TDecorImageEdit _XClassName(TDecorImageEdit, CLASSPREFIX)
#define TDrawTree  _XClassName(TDrawTree, CLASSPREFIX)
#define TSeriaEdit _XClassName(TSeriaEdit, CLASSPREFIX)
#define TEditTree  _XClassName(TEditTree, CLASSPREFIX)
#define TSimpleListOrder _XClassName(TSimpleListOrder, CLASSPREFIX)
#endif

#ifdef LINUXAPI
 #define UnicodeToString _XClassName(UnicodeToString, CLASSPREFIX)
 #define StringToUnicode _XClassName(StringToUnicode, CLASSPREFIX)
 #define PLocalString _XClassName(PLocalString, CLASSPREFIX)
 #define StringTransformer _XClassName(StringTransformer, CLASSPREFIX)
#endif

// objset
#define TSetFile _XClassName(TSetFile, CLASSPREFIX)
#define TSetSeekObject _XClassName(TSetSeekObject, CLASSPREFIX)
#define TObjectSetInList _XClassName(TObjectSetInList, CLASSPREFIX)
#define TObjectSetInMap _XClassName(TObjectSetInMap, CLASSPREFIX)
#define TObjectSetTotal _XClassName(TObjectSetTotal, CLASSPREFIX)
#define TListObjectSet _XClassName(TListObjectSet, CLASSPREFIX)

#endif
