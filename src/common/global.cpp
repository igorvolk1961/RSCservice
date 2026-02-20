/**
  @file
  @author Stefan Frings
*/

#include "global.h"
#include "myexception.h"
#include "iconv.h"

//HttpSessionStore* sessionStore;
StaticFileController* staticFileController;
FileLogger* logger;

int g_debug_mode;

QByteArray g_ca_cert_path;
long long g_ssl_verify_peer = 1L;
long long g_ssl_verify_host = 1L;

QNetworkAccessManager g_naManager;

FILE* g_df = fopen("/usr/local/src/tmp/trace.txt", "w");

void trace(const char* text){
//  g_df = fopen("/usr/local/src/tmp/trace.txt", "a");
//  if (g_df) {
//    fprintf(g_df, text);
//    fclose(g_df);
//  } else {
//    throw MyException("Cannt open trace file");
//  }
}

