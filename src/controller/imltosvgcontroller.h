#ifndef IMLTOSVGCONTROLLER_H
#define IMLTOSVGCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

#include <QJsonObject>

using namespace stefanfrings;

/**
  This controller process queries to imlToSvg converter
*/

class ImlToSvgController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(ImlToSvgController)
public:

    /** Constructor */
    ImlToSvgController();

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

private:
    void buildErrorMessage(const QString &errorString, QByteArray &outputBody);
    void createSvgBuilder(const QJsonObject &input, QByteArray &outputBody);
    void getIncodeForExcode(const QJsonObject &input, QByteArray &outputBody);
    void getIncodeForKey(const QJsonObject &input, QByteArray &outputBody);
    void getRscSegmentCount(const QJsonObject &input, QByteArray &outputBody);
    void getRscObjectCount(const QJsonObject &input, QByteArray &outputBody);
    void getRscObject(const QJsonObject &input, QByteArray &outputBody);
    void getRscSegment(const QJsonObject &input, QByteArray &outputBody);
    void getSvgImage(const QJsonObject &input, QByteArray &outputBody);
};

#endif // IMLTOSVGCONTROLLER_H
