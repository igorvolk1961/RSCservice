#ifndef RSCCONTROLLER_H
#define RSCCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

#include <QJsonObject>

using namespace stefanfrings;

/**
  This controller process queries to imlToSvg converter
*/

class RscController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(RscController)
public:

    /** Constructor */
    RscController(const QSettings *settings);

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

public:
//private:
    void buildErrorMessage(const QString &errorString, QByteArray &outputBody);
    void createSvgBuilder(const QJsonObject &input, QByteArray &outputBody);
    void getIncodeForExcode(const QJsonObject &input, QByteArray &outputBody);
    void getIncodeForKey(const QJsonObject &input, QByteArray &outputBody);
    void getRscSegmentCount(const QJsonObject &input, QByteArray &outputBody);
    void getRscObjectCount(const QJsonObject &input, QByteArray &outputBody);
    void getRscObject(const QJsonObject &input, QByteArray &outputBody);
    void getRscSegment(const QJsonObject &input, QByteArray &outputBody);
    void getRscSegmentLocalCount(const QJsonObject &input, QByteArray &outputBody);
    void getRscSegmentLocalIncodes(const QJsonObject &input, QByteArray &outputBody);
    void getRscSemanticClassCount(const QJsonObject &input, QByteArray &outputBody);
    void getRscSemanticClass(const QJsonObject &input, QByteArray &outputBody);
    void getRscSimpleSemanticCount(const QJsonObject &input, QByteArray &outputBody);
    void getRscSimpleSemantic(const QJsonObject &input, QByteArray &outputBody);
    void getSvgImage(const QJsonObject &input, QByteArray &outputBody);

    void writeSemantic(long svgBuilderIndex, long incode, int semCode, QByteArray &outputBody);
    int  getSemType(long svgBuilderIndex, unsigned short type, int semCode);
};

#endif // RSCCONTROLLER_H
