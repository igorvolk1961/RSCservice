/**
  @file
  @author Stefan Frings
*/

#ifndef GLOBAL_H
#define GLOBAL_H

//#include "httpsessionstore.h"
#include "staticfilecontroller.h"
#include "filelogger.h"
#include <QHash>
#include <QPair>
#include <QByteArray>
#include <QDateTime>
#include <QWaitCondition>
#include <QMutex>
#include <QSet>
#include <QNetworkAccessManager>

#include "filepathrecord.h"
#include "hmaprecord.h"

using namespace stefanfrings;

/**
  Global objects that are shared by multiple source files
  of this project.
*/

///** Storage for session cookies */
//extern  HttpSessionStore* sessionStore;

/** Controller for static files */
extern  StaticFileController* staticFileController;

/** Redirects log messages to a file */
extern  FileLogger* logger;

extern  int g_debug_mode;

extern  QByteArray g_ca_cert_path;
extern  long long g_ssl_verify_peer;
extern  long long g_ssl_verify_host;


extern QNetworkAccessManager g_naManager;


void trace(const char* text);

#endif // GLOBAL_H
