/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include "global.h"
#include "requestmapper.h"
#include "filelogger.h"
#include "staticfilecontroller.h"
#include "controller/dumpcontroller.h"
#include "controller/rsccontroller.h"
//#include "controller/fileuploadcontroller.h"
//#include "controller/sessioncontroller.h"

#include "myexception.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>

RequestMapper::RequestMapper(const QSettings *settings, QObject* parent)
    :HttpRequestHandler(parent)
{
    qDebug("RequestMapper: cre1ated");

    this->settings = settings;

//    QString ca_cert_path = settings->value("ca_cert_path", "").toString();
//    g_ca_cert_path.clear();
//    g_ca_cert_path.append(ca_cert_path);

//    g_ssl_verify_peer = settings->value("ssl_verify_peer", 1).toLongLong();
//    g_ssl_verify_host = settings->value("ssl_verify_host", 1).toLongLong();


    int nofile = settings->value("nofile", "2048").toInt();
    rlimit rlim;
    rlim.rlim_max = nofile;
    rlim.rlim_cur = nofile;
    if (setrlimit(RLIMIT_NOFILE, &rlim)) {
        char* err = strerror(errno);
        throw MyException(qPrintable(QString("Error in RequestMapper : %1").arg(err)));
    }

}


RequestMapper::~RequestMapper()
{
    qDebug("RequestMapper: deleted");
}


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

    // For the following pathes, each request gets its own new instance of the related controller.

    if (path.startsWith("/dump"))
    {
        DumpController().service(request, response);
    } else
    if (path.startsWith("/rsc")){
        RscController(settings).service(request, response);
    }

//    else if (path.startsWith("/file"))
//    {
//        FileUploadController().service(request, response);
//    }

//    else if (path.startsWith("/session"))
//    {
//        SessionController().service(request, response);
//    }

    // All other pathes are mapped to the static file controller.
    // In this case, a single instance is used for multiple requests.
    else
    {
        staticFileController->service(request, response);
    }

    qDebug("RequestMapper: finished request");

    // Clear the log buffer
    if (logger)
    {
       logger->clear();
    }
}
