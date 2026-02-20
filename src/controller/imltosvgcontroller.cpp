#include "imltosvgcontroller.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QTextCodec>
#include "svgimage.h"
#include "rsctosvg.h"
#include "rsc.h"
#include "semutils1.h"
#include "string.h"
#include "maptype.h"
#include "debug.h"
#include "myexception.h"

ImlToSvgController::ImlToSvgController(){

}

/** Generates the response */
void ImlToSvgController::service(HttpRequest& request, HttpResponse& response){
    QByteArray path=request.getPath();

    QByteArray inputBody = request.getBody();
    QJsonParseError jsonParseError;
    QJsonDocument json = QJsonDocument::fromJson(inputBody, &jsonParseError);

    QByteArray outputBody("");
    if (jsonParseError.error != QJsonParseError::NoError) {
        buildErrorMessage(QString("JsonParser:%1 offset=%2").
                          arg(jsonParseError.errorString()).
                          arg(jsonParseError.offset),
                          outputBody);
    } else {
        if (path.startsWith("/imlToSvg/createSvgBuilder")){
          createSvgBuilder(json.object(), outputBody);
        } else
        if (path.startsWith("/imlToSvg/getIncodeForExcode")){
          getIncodeForExcode(json.object(), outputBody);
        } else
        if (path.startsWith("/imlToSvg/getIncodeForKey")){
          getIncodeForKey(json.object(), outputBody);
        } else
        if (path.startsWith("/imlToSvg/getRscSegmentCount")){
          getRscSegmentCount(json.object(), outputBody);
        } else
        if (path.startsWith("/imlToSvg/getRscObjectCount")){
          getRscObjectCount(json.object(), outputBody);
        } else
        if (path.startsWith("/imlToSvg/getRscSegment")){
          getRscSegment(json.object(), outputBody);
        } else
        if (path.startsWith("/imlToSvg/getRscObject")){
          getRscObject(json.object(), outputBody);
        } else
        if (path.startsWith("/imlToSvg/getSvgImage")){
          getSvgImage(json.object(), outputBody);
        }
    }

    response.setHeader("Content-Type", "application/json; charset=UTF-8");
    response.write(outputBody,true);
}

void ImlToSvgController::buildErrorMessage(const QString &errorString, QByteArray &outputBody){
    outputBody.append("{");
    outputBody.append("\"error\":\"" + errorString + "\"");
    outputBody.append("}");
}

void ImlToSvgController::createSvgBuilder(const QJsonObject &input, QByteArray &outputBody){
    QString rscFilePath = "/usr/Operator13/operator.rsc";
    double baseLineLength = 15000;
    double baseLineLength_px = 100;
    int debug_mode = 0;
    int encoding = 126;
    if (input.contains("rscFilePath") && input["rscFilePath"].isString()) {
        rscFilePath = input["rscFilePath"].toString();
        qDebug("rscFilePath=%s", qPrintable(rscFilePath));
    }
    if (input.contains("baseLineLength") && input["baseLineLength"].isDouble()) {
        baseLineLength = input["baseLineLength"].toDouble();
        qDebug("baseLineLength=%0.1f", baseLineLength);
    }
    if (input.contains("baseLineLength_px") && input["baseLineLength_px"].isDouble()) {
        baseLineLength_px = input["baseLineLength_px"].toDouble();
        qDebug("baseLineLength_px=%0.1f", baseLineLength_px);
    }
    if (input.contains("debug_mode") && input["debug_mode"].isDouble()) {
        debug_mode = input["debug_mode"].toInt();
        qDebug("debug_mode=%d", debug_mode);
    }
    if (input.contains("encoding") && input["encoding"].isDouble()) {
        encoding = input["encoding"].toInt();
        qDebug("encoding=%d", encoding);
    }
    try {
        if ((encoding != 0) &&
            (encoding != TRsc::CONV_KOI8_R) &&
            (encoding != TRsc::CONV_CP1251) &&
            (encoding != TRsc::CONV_DOS)) {
            buildErrorMessage("createSvgBuilder  Unsupported encoding", outputBody);
            return;
        }
      try {
         long svgBuilderIndex = RscToSvg::createSvgBuilder(qPrintable(rscFilePath),
                                       baseLineLength, baseLineLength_px,
                                       debug_mode, encoding);
         outputBody.append("{\n");
         outputBody.append(QString("  \"svgBuilderIndex\": %1").arg(svgBuilderIndex));
         outputBody.append("\n}");
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("JNI ImlToSvg_createSvgBuilder  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}

void ImlToSvgController::getIncodeForExcode(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    long excode = 0;
    int local = 1;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toInt();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    if (input.contains("excode") && input["excode"].isDouble()) {
        excode = input["excode"].toDouble();
        qDebug("excode=%d", excode);
    }
    if (input.contains("local") && input["local"].isDouble()) {
        local = input["local"].toDouble();
        qDebug("local=%0.1f", local);
    }
    try {
      try {
         long incode = RscToSvg::getIncodeForExcode(svgBuilderIndex, excode, local);
         outputBody.append("{\n");
         outputBody.append(QString("  \"incode\": %1").arg(incode));
         outputBody.append("\n}");
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getIncodeForExcode  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}

void ImlToSvgController::getIncodeForKey(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    QString key = "00000000";
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    if (input.contains("key") && input["key"].isString()) {
        key = input["key"].toString();
        qDebug("key=%s", qPrintable(key));
    }
    try {
      try {
         long incode = RscToSvg::getIncodeForKey(svgBuilderIndex, qPrintable(key));
         outputBody.append("{\n");
         outputBody.append(QString("  \"incode\": %1").arg(incode));
         outputBody.append("\n}");
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getIncodeForKey  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}

void ImlToSvgController::getRscSegmentCount(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    try {
      try {
         long rscSegmentCount = RscToSvg::mapGetRscSegmentCount(svgBuilderIndex);
         outputBody.append("{\n");
         outputBody.append(QString("  \"rscSegmentCount\": %1").arg(rscSegmentCount));
         outputBody.append("\n}");
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getRscSegmentCount  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}

void ImlToSvgController::getRscObjectCount(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    try {
      try {
         long rscObjectCount = RscToSvg::mapGetRscObjectCount(svgBuilderIndex);
         outputBody.append("{\n");
         outputBody.append(QString("  \"rscObjectCount\": %1").arg(rscObjectCount));
         outputBody.append("\n}");
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getRscObjectCount  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}

void ImlToSvgController::getRscObject(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    long incode = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    if (input.contains("incode") && input["incode"].isDouble()) {
        incode = input["incode"].toDouble();
        qDebug("incode=%d", incode);
    }
    try {
      try {
         RSCOBJECT rscObject;
         long result = RscToSvg::mapGetRscObject(svgBuilderIndex, incode, &rscObject);
         if (!result) {
             int bufferLen = 1024;
             char buffer[bufferLen];
             char* objectName_c;
             try {
                 objectName_c = RscToSvg::convertToUtf8(svgBuilderIndex, (char*)rscObject.Name, buffer, bufferLen);
             } catch (MyException myException) {
                 throw MyException(myException.d);
             } catch (const char* message) {
                 throw MyException(message);
             } catch (...) {
                 throw MyException("getRscObject Unknown Exception");
             }

             const char* key;
             try {
               key = RscToSvg::mapGetRscObjectKey(svgBuilderIndex, incode);
             } catch (MyException myException) {
                 throw MyException(myException.d);
             } catch (const char* message) {
                 throw MyException(message);
             } catch (...) {
                 throw MyException("mapGetRscObjectKey failed");
             }

             char buffer1[bufferLen];
             char* key_c;
             try{
                key_c = RscToSvg::convertToUtf8(svgBuilderIndex, key, buffer1, bufferLen);
             } catch (MyException myException) {
                 throw MyException(myException.d);
             } catch (const char* message) {
                 throw MyException(message);
             } catch (...) {
                 throw MyException("mapGetRscObjectKey failed");
             }

           outputBody.append("{\n");
           outputBody.append(QString("  \"Code\": %1,\n").arg(rscObject.Code));       // КЛАССИФИКАЦИОННЫЙ КОД
           outputBody.append(QString("  \"Local\": %1,\n").arg(rscObject.Local));     // ХАРАКТЕР ЛОКАЛИЗАЦИИ  (OBJECT_LOCAL)
           outputBody.append(QString("  \"Segment\": %1,\n").arg(rscObject.Segment)); // Н0МЕР СЛОЯ ( НЕ БОЛЕЕ 255)
           outputBody.append(QString("  \"Scale\": %1,\n").arg(rscObject.Scale));     // 1 - ОБ'ЕКТ МАСШТАБИРУЕТСЯ // 0 - ОБ'ЕКТ НЕ МАСШТАБИРУЕТСЯ
           outputBody.append(QString("  \"Direct\": %1,\n").arg(rscObject.Direct));   // НАПРАВЛЕНИЕ ЦИФРОВАНИЯ ОБ'ЕКТА (OBJECT_DIRECT)
           outputBody.append(QString("  \"Bot\": %1,\n").arg(rscObject.Bot));         // НИЖНЯЯ ГРАНИЦА ВИДИМОСТИ (ЗНАМЕНАТЕЛЬ МАСШТАБА)
           outputBody.append(QString("  \"Top\": %1,\n").arg(rscObject.Top));         // ВЕРХНЯЯ ГРАНИЦА ВИДИМОСТИ (ЗНАМЕНАТЕЛЬ МАСШТАБА)
           outputBody.append(QString("  \"Name\": \"%1\",\n").arg(QString(objectName_c).trimmed()));   // НАЗВАНИЕ
           outputBody.append(QString("  \"Key\": \"%1\",\n").arg(QString(key_c).trimmed()));   // НАЗВАНИЕ
           outputBody.append("\n}");
         } else {
             throw MyException("mapGetRscObject failed");
         }

         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getRscObject  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}

void ImlToSvgController::getRscSegment(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    long incode = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    if (input.contains("incode") && input["incode"].isDouble()) {
        incode = input["incode"].toDouble();
        qDebug("incode=%d", incode);
    }
    try {
      try {
         RSCSEGMENT rscSegment;
         long result = RscToSvg::mapGetRscSegment(svgBuilderIndex, &rscSegment, incode);
         if (!result) {
             int bufferLen = 1024;
             char buffer[bufferLen];
             char* segmentName_c;
             try {
                 segmentName_c = RscToSvg::convertToUtf8(svgBuilderIndex, (char*)rscSegment.Name, buffer, bufferLen);
             } catch (MyException myException) {
                 throw MyException(myException.d);
             } catch (const char* message) {
                 throw MyException(message);
             } catch (...) {
                 throw MyException("getRscObject Unknown Exception");
             }

           outputBody.append("{\n");
           outputBody.append(QString("  \"Order\": %1,\n").arg(rscSegment.Order));    // ПОРЯДОК ВЫВОДА СЛОЯ НА ПЕЧАТЬ
           outputBody.append(QString("  \"Name\": \"%1\"\n").arg(QString(segmentName_c).trimmed()));  // НАЗВАНИЕ ( НЕ БОЛЕЕ 30)
           outputBody.append("\n}");
         } else {
             throw MyException("mapGetRscSegment failed");
         }
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getRscSegment  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}

void ImlToSvgController::getSvgImage(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    long incode = 0;
    QString key;
    double** metricCoords = 0;
    int* metricLengths = 0;
    int metricCount = 0;
    char* semantics = 0;
    char** texts = 0;
    int symbolType = 0;
    double rotation = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }

    if (input.contains("incode") && input["incode"].isDouble()) {
        incode = input["incode"].toDouble();
        qDebug("incode=%d", incode);
    } else
    if (input.contains("classCode") && input["classCode"].isString()) {
        key = input["classCode"].toString();
        qDebug("key=%s", qPrintable(key));
    }


    if (input.contains("metricCoords") && input["metricCoords"].isArray()) {
        QJsonArray metricCoordsArray = input["metricCoords"].toArray();
        metricCount = metricCoordsArray.size();
        metricLengths = new int[metricCount];
        metricCoords = new double*[metricCount];
        for (int j = 0; j < metricCount; ++j) {
          QJsonArray metricCoordsSubArray = metricCoordsArray[j].toArray();
          metricLengths[j] = metricCoordsSubArray.size();
          qDebug("metricLengths[%d]=%d", j, metricLengths[j]);
          metricCoords[j] = new double[metricLengths[j]];
          for (int m = 0; m < metricLengths[j]; ++m){
            metricCoords[j][m] = metricCoordsSubArray[m].toDouble();
            qDebug("metricCoords[%d][%d]=%f0.2", j, m, metricCoords[j][m]);
          }
        }
    }
    if (input.contains("texts") && input["texts"].isArray()) {
        QJsonArray textsArray = input["texts"].toArray();
        int textCount = textsArray.size();
        if (textCount > 0) {
          texts = new char*[textCount];
          for (int j = 0; j < textCount; ++j) {
            texts[j] = const_cast<char*>(qPrintable(textsArray[j].toString()));
          }
        } else {
           texts = 0;
        }
    }
    if (input.contains("semantics") && input["semantics"].isArray()) {
        QJsonArray semanticsArray = input["semantics"].toArray();
        semantics = loadSemantic1(semanticsArray);
    }
    if (input.contains("symbolType") && input["symbolType"].isDouble()) {
        symbolType = input["symbolType"].toDouble();
        qDebug("symbolType=%d", symbolType);
    }
    if (input.contains("rotation") && input["rotation"].isDouble()) {
        rotation = input["rotation"].toDouble();
        qDebug("rotation=%f0.2", rotation);
    }
    try {
      try {
         SvgImage svgImage;         
         long result;
         if (key.isEmpty()) {
             result = RscToSvg::getSvgImageByIncode(svgBuilderIndex, incode, metricCoords, metricLengths, metricCount,
                                               semantics, texts, &svgImage, symbolType, rotation);
         } else {
             result = RscToSvg::getSvgImageByKey(svgBuilderIndex, qPrintable(key), metricCoords, metricLengths, metricCount,
                                                 semantics, texts, &svgImage, symbolType, rotation);
         }
         if (result) {
           outputBody.append("{\n");
           QString svg = svgImage.svg;
           svg = svg.replace("\"", "&quot;").replace("\n", "&br;");
           outputBody.append(QString("  \"svg\": \"%1\",\n").arg(svg));
           outputBody.append(QString("  \"width_px\": %1,\n").arg(svgImage.width_px));
           outputBody.append(QString("  \"height_px\": %1,\n").arg(svgImage.height_px));
           outputBody.append(QString("  \"minX_px\": %1,\n").arg(svgImage.minX_px));
           outputBody.append(QString("  \"minY_px\": %1,\n").arg(svgImage.minY_px));
           outputBody.append("\n}");

           delete[] svgImage.svg;
         } else {
             throw MyException("getSvgImage failed");
         }
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getSvgImage  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}
