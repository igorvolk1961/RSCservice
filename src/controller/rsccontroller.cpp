#include "rsccontroller.h"
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

QString rscFileRootPath = "e:/common/var";
//QString rscFileRootPath = "/home/igorvolk/common/var";
//QString rscFileRootPath = "/var";

RscController::RscController(const QSettings */*settings*/){

}

/** Generates the response */
void RscController::service(HttpRequest& request, HttpResponse& response){
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
        if (path.startsWith("/rsc/createSvgBuilder")){
          createSvgBuilder(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getIncodeForExcode")){
          getIncodeForExcode(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getIncodeForKey")){
          getIncodeForKey(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getRscSegmentCount")){
          getRscSegmentCount(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getRscObjectCount")){
          getRscObjectCount(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getRscSegmentLocalCount")){
          getRscSegmentLocalCount(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getRscObject")){
          getRscObject(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getRscSemanticClassCount")){
          getRscSemanticClassCount(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getRscSemanticClass")){
          getRscSemanticClass(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getRscSimpleSemanticCount")){
          getRscSimpleSemanticCount(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getRscSimpleSemantic")){
          getRscSimpleSemantic(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getRscSegmentLocalIncodes")){
          getRscSegmentLocalIncodes(json.object(), outputBody);
//        } else
//        if (path.startsWith("/rsc/getRscSegmentLocalIncodeCount")){
//          getRscSegmentLocalIncodesCount(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getRscSegment")){
          getRscSegment(json.object(), outputBody);
        } else
        if (path.startsWith("/rsc/getSvgImage")){
          getSvgImage(json.object(), outputBody);
        } else {
            buildErrorMessage(QString("Unknown request %1").
                              arg(QString(path)),
                              outputBody);
        }
    }

    response.setHeader("Content-Type", "application/json; charset=UTF-8");
    qDebug("%s =\n%s", path.data(), outputBody.data());
    response.write(outputBody,true);
}

void RscController::buildErrorMessage(const QString &errorString, QByteArray &outputBody){
    outputBody.append("{");
    outputBody.append("\"error\":\"" + errorString + "\"");
    outputBody.append("}");
}

void RscController::createSvgBuilder(const QJsonObject &input, QByteArray &outputBody){
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
    rscFilePath = rscFilePath.replace("\\","/")
                   .replace(rscFileRootPath,"/media/sf_common/var");

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
         QString s = QString("  \"svgBuilderIndex\": %1").arg(svgBuilderIndex);
         outputBody.append(s);
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

void RscController::getIncodeForExcode(const QJsonObject &input, QByteArray &outputBody){
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

void RscController::getIncodeForKey(const QJsonObject &input, QByteArray &outputBody){
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

void RscController::getRscSegmentCount(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    try {
      try {
         long rscSegmentCount = RscToSvg::getSegmentCount(svgBuilderIndex);
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

void RscController::getRscObjectCount(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    try {
      try {
         long rscObjectCount = RscToSvg::getObjectCount(svgBuilderIndex);
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

void RscController::getRscObject(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    long incode = 0;
    QString key = "00000000";
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    if (input.contains("incode") && input["incode"].isDouble()) {
        incode = input["incode"].toDouble();
        qDebug("incode=%d", incode);
    } else
    if (input.contains("key") && input["key"].isString()) {
        key = input["key"].toString();
        qDebug("key=%s", qPrintable(key));
    }
    try {
      try {
        if (incode == 0) {
          incode = RscToSvg::getIncodeForKey(svgBuilderIndex, qPrintable(key));
        }
         OBJECTHEADER* objectHeader = RscToSvg::getObjectHeader(svgBuilderIndex, incode);
         if (objectHeader) {
             int bufferLen = 1024;
             char buffer[bufferLen];
             char* objectName_c = RscToSvg::convertToUtf8(svgBuilderIndex, (char*)objectHeader->Name, buffer, bufferLen);
             QString objectName = QString(objectName_c).trimmed();
             objectName = objectName.replace("\\","/").replace("\"", "'");

             const char* key = RscToSvg::getObjectKey(svgBuilderIndex, incode);

             char buffer1[bufferLen];
             char* key_c = RscToSvg::convertToUtf8(svgBuilderIndex, key, buffer1, bufferLen);

             long excode = objectHeader->Excode;
             int local = objectHeader->Local;
             OBJECTSEMANTIC* objectSemantic =  RscToSvg::getObjectSemantics(svgBuilderIndex, excode, local);
             int* obligatorySemantics =  RscToSvg::getObligatorySemantics(svgBuilderIndex, excode, local);
             int* optionalSemantics =    RscToSvg::getOptionalSemantics(svgBuilderIndex, excode, local);

           outputBody.append("{\n");
           outputBody.append(QString("  \"Incode\": %1,\n").arg(incode));       // ВНУТРЕННИЙ КОД
           outputBody.append(QString("  \"Excode\": %1,\n").arg(excode));       // КЛАССИФИКАЦИОННЫЙ КОД
           outputBody.append(QString("  \"Local\": %1,\n").arg(local));     // ХАРАКТЕР ЛОКАЛИЗАЦИИ  (OBJECT_LOCAL)
           outputBody.append(QString("  \"Extension\": %1,\n").arg(objectHeader->Extension)); //Номер объекта в серии
           outputBody.append(QString("  \"Segment\": %1,\n").arg(objectHeader->Segment)); // Н0МЕР СЛОЯ ( НЕ БОЛЕЕ 255)
           outputBody.append(QString("  \"Scalable\": %1,\n").arg(objectHeader->Scalable));     // 1 - ОБ'ЕКТ МАСШТАБИРУЕТСЯ // 0 - ОБ'ЕКТ НЕ МАСШТАБИРУЕТСЯ
           outputBody.append(QString("  \"LowerScale\": %1,\n").arg(objectHeader->LowerScale));     // Нижняя граница видимости
           outputBody.append(QString("  \"UpperScale\": %1,\n").arg(objectHeader->UpperScale));     // Верхняя граница видимости
           outputBody.append(QString("  \"Direct\": %1,\n").arg(objectHeader->Direct));   // НАПРАВЛЕНИЕ ЦИФРОВАНИЯ ОБ'ЕКТА (OBJECT_DIRECT)
           outputBody.append(QString("  \"Name\": \"%1\",\n").arg(objectName));   // НАЗВАНИЕ
           outputBody.append(QString("  \"Key\": \"%1\",\n").arg(QString(key_c).trimmed()));   // НАЗВАНИЕ
           outputBody.append(QString("  \"semantics\": [\n"));              // Семантика
           if (objectSemantic) {
             for (int j = 0; j < objectSemantic->ObligatorySemanticCount; ++j){
               if (j == 0){
                   outputBody.append("\n");
               } else {
                   outputBody.append(",\n");
               }
               outputBody.append("  {\n");
               int semCode = obligatorySemantics[j];
               writeSemantic(svgBuilderIndex, incode, semCode, outputBody);
               outputBody.append("  }");
             }
             if (objectSemantic->ObligatorySemanticCount){
                 outputBody.append(",\n");
             }
             for (int j = 0; j < objectSemantic->OptionalSemanticCount; ++j){
               if (j == 0){
                   outputBody.append("\n");
               } else {
                   outputBody.append(",\n");
               }
               outputBody.append("  {\n");
               int semCode = optionalSemantics[j];
               writeSemantic(svgBuilderIndex, incode, semCode, outputBody);
               outputBody.append("  }");
             }
           }
           outputBody.append(QString("  ]\n"));
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

void RscController::writeSemantic(long svgBuilderIndex, long incode, int semCode, QByteArray &outputBody){
    SEMANTICHEADER* semantic = RscToSvg::getSemantic(svgBuilderIndex,semCode);
    int bufferLen = 1024;
    char buffer2[bufferLen];
    char* semanticName_c = RscToSvg::convertToUtf8(svgBuilderIndex, (char*)semantic->Name, buffer2, bufferLen);
    QString semanticName = QString(semanticName_c).trimmed();
    semanticName = semanticName.replace("\\","/").replace("\"", "'");
    char buffer3[bufferLen];
    char* semanticShortName_c = RscToSvg::convertToUtf8(svgBuilderIndex, (char*)semantic->ShortName, buffer3, bufferLen);
    QString semanticShortName = QString(semanticShortName_c).trimmed();
    semanticShortName = semanticShortName.replace("\\","/").replace("\"", "'");
    char buffer4[bufferLen];
    char* mesureUnit_c = RscToSvg::convertToUtf8(svgBuilderIndex, (char*)semantic->MesureUnit, buffer4, bufferLen);
    QString mesureUnit = QString(mesureUnit_c).trimmed();
    mesureUnit = mesureUnit.replace("\\","/").replace("\"", "'");
    if (mesureUnit.length() > 0) {
        semanticName = semanticName + ", " + mesureUnit;
    }
    int semType = getSemType(svgBuilderIndex, semantic->Type, semCode);
    outputBody.append(QString("  \"semName\": \"%1\",\n").arg(semanticName));    // НАЗВАНИЕ СЕМАНТИКИ
    outputBody.append(QString("  \"semShortName\": \"%1\",\n").arg(semanticShortName)); // КОРОТКОЕ НАЗВАНИЕ СЕМАНТИКИ
    outputBody.append(QString("  \"semType\": %1,\n").arg(semType));             // ТИП СЕМАНТИКИ

    DEFAULTVALUE* defaultValue = RscToSvg::getSemanticDefaultValue(svgBuilderIndex, incode, semCode);
    if (defaultValue == NULL) {
        defaultValue = RscToSvg::getSemanticDefaultValue(svgBuilderIndex, semCode);
    }
    if (defaultValue == NULL) {
        outputBody.append(QString("  \"semCode\": %1\n").arg(semCode));             // КОД СЕМАНТИКИ
    } else {
        outputBody.append(QString("  \"semCode\": %1,\n").arg(semCode));             // КОД СЕМАНТИКИ
        outputBody.append(QString("  \"semDefValue\": %1\n").arg(defaultValue->DefaultValue));  // ЗНАЧЕНИЕ СЕМАНТИКИ ПО УМОЛЧАНИЮ
    }
}

int RscController::getSemType(long svgBuilderIndex, unsigned short type, int semCode){
    if (type == 1 || type == 12){
        if (RscToSvg::getSemanticClassByCode(svgBuilderIndex, semCode) != NULL) {
          return 4;
        } else {
          return 5;
        }
    } else {
        return 1;
    }
}

void RscController::getRscSegment(const QJsonObject &input, QByteArray &outputBody){
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
         long result = RscToSvg::getSegment(svgBuilderIndex, &rscSegment, incode);
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
             QString segmentName = QString(segmentName_c).trimmed();
             segmentName = segmentName.replace("\\","/").replace("\"", "'");

           outputBody.append("{\n");
           outputBody.append(QString("  \"Order\": %1,\n").arg(rscSegment.Order));    // ПОРЯДОК ВЫВОДА СЛОЯ НА ПЕЧАТЬ
           outputBody.append(QString("  \"Name\": \"%1\"\n").arg(segmentName));  // НАЗВАНИЕ ( НЕ БОЛЕЕ 30)
//           outputBody.append("  \"Name\": \"").append(segmentName_c).append("\"\n");  // НАЗВАНИЕ ( НЕ БОЛЕЕ 30)
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

void RscController::getRscSegmentLocalCount(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    long incode = 0;
    long local = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    if (input.contains("incode") && input["incode"].isDouble()) {
        incode = input["incode"].toDouble();
        qDebug("incode=%d", incode);
    }
    if (input.contains("local") && input["local"].isDouble()) {
        local = input["local"].toDouble();
        qDebug("local=%d", local);
    }
    try {
      try {
         int count = 0;
         long result = RscToSvg::getSegmentLocalCount(svgBuilderIndex, &count, incode, local);
         if (!result) {
           outputBody.append("{\n");
           outputBody.append(QString("  \"count\": %1,\n").arg(count));
           outputBody.append("\n}");
         } else {
             throw MyException("getRscSegmentLocalCount failed");
         }
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getRscSegmentLocalCount  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}

void RscController::getRscSegmentLocalIncodes(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    long segment = 0;
    long local = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    if (input.contains("incode") && input["incode"].isDouble()) {
        segment = input["incode"].toDouble();
        qDebug("incode=%d", segment);
    }
    if (input.contains("local") && input["local"].isDouble()) {
        local = input["local"].toDouble();
        qDebug("local=%d", local);
    }
    try {
      try {
         int  count;
         int* incodes;
         RscToSvg::getSegmentLocalCount(svgBuilderIndex, &count, segment, local);
         long result = RscToSvg::getSegmentLocalIncodes(svgBuilderIndex, &incodes, segment, local);
         if (!result) {
           outputBody.append("{\n");
           outputBody.append("  \"incodes\": [\n");
           for (int j = 0; j < count; ++j){
               if (j > 0) {
                   outputBody.append(", ");
               }
               outputBody.append(QString("%1").arg(incodes[j]));
           }
           outputBody.append("             ]\n");
           outputBody.append("\n}");
         } else {
             throw MyException("getRscSegmentLocalIncodes failed");
         }
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getRscSegmentLocalIncodes  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}

void RscController::getRscSemanticClassCount(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    try {
      try {
         long count = RscToSvg::getSemanticClassCount(svgBuilderIndex);
         outputBody.append("{\n");
         outputBody.append(QString("  \"count\": %1,\n").arg(count));
         outputBody.append("\n}");
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getRscSemanticClassCount  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}

void RscController::getRscSemanticClass(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    long index = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    if (input.contains("index") && input["index"].isDouble()) {
        index = input["index"].toDouble();
        qDebug("index=%d", index);
    }
    try {
      try {
         SEMANTICHEADER* semantic = RscToSvg::getSemanticByClassIndex(svgBuilderIndex, index);
         int bufferLen = 1024;
         char buffer[bufferLen];
         char* semName_c;
         try {
             semName_c = RscToSvg::convertToUtf8(svgBuilderIndex, (char*)semantic->Name, buffer, bufferLen);
         } catch (MyException myException) {
             throw MyException(myException.d);
         } catch (const char* message) {
             throw MyException(message);
         } catch (...) {
             throw MyException("getRscSemanticClass Unknown Exception");
         }

         QString semName = QString(semName_c).trimmed();
         semName = semName.replace("\\","/").replace("\"", "'");

         Class_SemClass_Map* semanticClass =  RscToSvg::getSemanticClassByIndex(svgBuilderIndex, index);
         if (semanticClass) {
           outputBody.append("{\n");
           outputBody.append(QString("  \"sem_name\": \"%1\",\n").arg(semName));
           outputBody.append(QString("  \"sem_code\": %1,\n").arg(semantic->Code));
           outputBody.append("  \"items\": [\n");

           int j = 0;
           Class_SemClass_Map::iterator it = semanticClass->begin();
           while(it != semanticClass->end()) {
               int itemCode = it->first;
               SEMANTICCLASS* item = it->second;
               char buffer1[bufferLen];
               char* itemName_c;
               try {
                   itemName_c = RscToSvg::convertToUtf8(svgBuilderIndex, (char*)item->Name, buffer1, bufferLen);
               } catch (MyException myException) {
                   throw MyException(myException.d);
               } catch (const char* message) {
                   throw MyException(message);
               } catch (...) {
                   throw MyException("getRscSemanticClass Unknown Exception");
               }
               QString itemName = QString(itemName_c).trimmed();
               itemName = itemName.replace("\\","/").replace("\"", "'");

               if (j > 0) {
                   outputBody.append(", ");
               }
               outputBody.append("{\n");
               outputBody.append(QString("  \"item_code\": %1,\n").arg(itemCode));
               outputBody.append(QString("  \"item_value\": %1,\n").arg(itemCode));
               semName = semName.replace("\\","/").replace("\"", "'");
               outputBody.append(QString("  \"item_name\": \"%1\"\n").arg(itemName));
               outputBody.append("}\n");
               ++j;
               ++it;
           }
           outputBody.append("            ]\n");
           outputBody.append("\n}");
         } else {
             throw MyException("getRscSemanticClass failed");
         }
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getRscSemanticClass  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}


void RscController::getRscSimpleSemanticCount(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    try {
      try {
         long count = RscToSvg::getSimpleSemanticCount(svgBuilderIndex);
         outputBody.append("{\n");
         outputBody.append(QString("  \"count\": %1,\n").arg(count));
         outputBody.append("\n}");
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getSimpleSemanticCount  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}

void RscController::getRscSimpleSemantic(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    long index = 0;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }
    if (input.contains("index") && input["index"].isDouble()) {
        index = input["index"].toDouble();
        qDebug("index=%d", index);
    }
    try {
      try {
         SEMANTICHEADER* semantic = RscToSvg::getSimpleSemanticByIndex(svgBuilderIndex, index);
         int bufferLen = 1024;
         char buffer[bufferLen];
         char buffer1[bufferLen];
         char* semName_c;
         char* semanticShortName_c;
         try {
             semName_c = RscToSvg::convertToUtf8(svgBuilderIndex, (char*)semantic->Name, buffer, bufferLen);
             semanticShortName_c = RscToSvg::convertToUtf8(svgBuilderIndex, (char*)semantic->ShortName, buffer1, bufferLen);
         } catch (MyException myException) {
             throw MyException(myException.d);
         } catch (const char* message) {
             throw MyException(message);
         } catch (...) {
             throw MyException("getRscSimpleSemantic Unknown Exception");
         }

         QString semName = QString(semName_c).trimmed();
         semName = semName.replace("\\","/").replace("\"", "'");
         QString semanticShortName = QString(semanticShortName_c).trimmed();
         semanticShortName = semanticShortName.replace("\\","/").replace("\"", "'");

         int semType = getSemType(svgBuilderIndex, semantic->Type, semantic->Code);

         Class_SemClass_Map* semanticClass =  RscToSvg::getSemanticClassByIndex(svgBuilderIndex, index);
         if (semanticClass) {
           outputBody.append("{\n");
           outputBody.append(QString("  \"semName\": \"%1\",\n").arg(semName));
           outputBody.append(QString("  \"semCode\": %1,\n").arg(semantic->Code));
           outputBody.append(QString("  \"semShortName\": \"%1\",\n").arg(semanticShortName));
           outputBody.append(QString("  \"semType\": %1,\n").arg(semType));

           outputBody.append("  \"items\": [\n");

           int j = 0;
           Class_SemClass_Map::iterator it = semanticClass->begin();
           while(it != semanticClass->end()) {
               int itemCode = it->first;
               SEMANTICCLASS* item = it->second;
               char buffer1[bufferLen];
               char* itemName_c;
               try {
                   itemName_c = RscToSvg::convertToUtf8(svgBuilderIndex, (char*)item->Name, buffer1, bufferLen);
               } catch (MyException myException) {
                   throw MyException(myException.d);
               } catch (const char* message) {
                   throw MyException(message);
               } catch (...) {
                   throw MyException("getRscSimpleSemantic Unknown Exception");
               }
               QString itemName = QString(itemName_c).trimmed();
               itemName = itemName.replace("\\","/").replace("\"", "'");

               if (j > 0) {
                   outputBody.append(", ");
               }
               outputBody.append("{\n");
               outputBody.append(QString("  \"item_code\": %1,\n").arg(itemCode));
               outputBody.append(QString("  \"item_value\": %1,\n").arg(itemCode));
               semName = semName.replace("\\","/").replace("\"", "'");
               outputBody.append(QString("  \"item_name\": \"%1\"\n").arg(itemName));
               outputBody.append("}\n");
               ++j;
               ++it;
           }
           outputBody.append("            ]\n");
           outputBody.append("\n}");
         } else {
             throw MyException("getRscSimpleSemantic failed");
         }
         return;
      } catch (MyException myException) {
            throw MyException(myException);
      } catch (const char* message) {
            throw MyException(message);
      } catch (...) {
            throw MyException("getRscSimpleSemantic  Unknown exception");
      }
    } catch(MyException myException) {
         buildErrorMessage(myException.d, outputBody);
        return;
    }
    outputBody.append("{}");
}

void RscController::getSvgImage(const QJsonObject &input, QByteArray &outputBody){
    long svgBuilderIndex = 0;
    long incode = 0;
    QString key;
    double** metricCoords = 0;
    int* metricLengths = 0;
    int metricCount = 0;
    char* semantics = 0;
    int textCount = 0;
    char** texts = 0;
    int symbolType = 0;
    double rotation = 0;
    double symbolSizeFactor = 1.;
    if (input.contains("svgBuilderIndex") && input["svgBuilderIndex"].isDouble()) {
        svgBuilderIndex = input["svgBuilderIndex"].toDouble();
        qDebug("svgBuilderIndex=%d", svgBuilderIndex);
    }

    if (input.contains("incode") && input["incode"].isDouble()) {
        incode = input["incode"].toDouble();
        qDebug("incode=%d", incode);
    } else
    if (input.contains("key") && input["key"].isString()) {
        key = input["key"].toString();
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
        textCount = textsArray.size();
        if (textCount > 0) {
          texts = new char*[textCount];
          for (int j = 0; j < textCount; ++j) {
              const char* text = qPrintable(textsArray[j].toString());
              texts[j] = new char[strlen(text) + 1];
              strcpy(texts[j], text);
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
    if (input.contains("symbolSizeFactor") && input["symbolSizeFactor"].isDouble()) {
        symbolSizeFactor = input["symbolSizeFactor"].toDouble();
        qDebug("symbolSizeFactor=%f0.2", symbolSizeFactor);
    }
    try {
      try {
         SvgImage svgImage;         
         long result;
         if (key.isEmpty()) {
             result = RscToSvg::getSvgImageByIncode(svgBuilderIndex, incode, metricCoords, metricLengths, metricCount,
                                               semantics, texts, &svgImage, symbolType, rotation, symbolSizeFactor);
         } else {
             result = RscToSvg::getSvgImageByKey(svgBuilderIndex, qPrintable(key), metricCoords, metricLengths, metricCount,
                                                 semantics, texts, &svgImage, symbolType, rotation, symbolSizeFactor);
         }
         if (result) {
           outputBody.append("{\n");
           QString svg = svgImage.svg;
           svg = svg.replace("\"", "&quot;").replace("\n", "&br;");
           qDebug("%s", qPrintable(svg));
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

    delete[] metricLengths;
    for (int j = 0; j < metricCount; ++j) {
        delete[] metricCoords[j];
    }
    delete[] metricCoords;
    for (int j = 0; j < textCount; ++j) {
        delete[] texts[j];
    }
    delete[] texts;
    delete[] semantics;
}
