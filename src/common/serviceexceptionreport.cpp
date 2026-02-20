#include "serviceexceptionreport.h"

ServiceExceptionReport::ServiceExceptionReport(){

}

void ServiceExceptionReport::write(const QString &errorMessage,
                                   const QString &errorCode,
                                   HttpResponse& response){
    qCritical(qPrintable(errorMessage));

    QByteArray outputBody;
    outputBody.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    outputBody.append("<ServiceExceptionReport version=\"1.3.0\"");
    outputBody.append("   xmlns=\"http://www.opengis.net/ogc\"\n");
    outputBody.append("   xmlns:xsi=\"http://www.w3.org/2001/ogc/XMLSchema-instance\"\n");
    outputBody.append("   xsi:schemaLocation=\"http://www.opengis.net/ogc\n");
    outputBody.append("                    http://schemas.opengis.net/wms/1.3.0/exceptions_1_3_0.xsd\"\n");
    outputBody.append(">\n");
    outputBody.append("<  ServiceException");
    if (!errorCode.isEmpty()) {
        outputBody.append("  code=\"");
        outputBody.append(errorCode);
        outputBody.append("\"");
    }
    outputBody.append(">\n>");
    if (!errorMessage.isEmpty()) {
        outputBody.append("    ");
        outputBody.append(errorMessage);
        outputBody.append("\n");
    }
    outputBody.append("<  /ServiceException\n>");
    outputBody.append("</ServiceExceptionReport>\n");
    response.setHeader("Content-Type", "application/xml");
    response.write(outputBody);
}
