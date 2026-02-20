#ifndef SERVICEEXCEPTIONREPORT_H
#define SERVICEEXCEPTIONREPORT_H

#include "httpresponse.h"

using namespace stefanfrings;

class ServiceExceptionReport{
public:
     ServiceExceptionReport();
     void write(const QString &errorMessage,
                const QString &errorCode,
                HttpResponse& response);
};

#endif // SERVICEEXCEPTIONREPORT_H
