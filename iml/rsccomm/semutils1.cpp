#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "semblockinfo.h"
#include "semutils1.h"
#include <cstring>
#include <cstdlib>
#include "semantic.h"
#include <QJsonObject>

char* loadSemantic1(const QJsonArray &semantics){

    size_t semanticSize = sizeof(SEMANTIC);

    int semanticLength = semantics.size();
    SemBlockInfo* semBlockInfos[semanticLength];
    for (int j = 0; j < semanticLength; ++j){
        QJsonObject semanticObject = semantics[j].toObject();
        int semType = semanticObject["semType"].toInt();
        int semCode = semanticObject["semCode"].toInt();
        QString semValue = semanticObject["semValue"].toString();
        SemBlockInfo* semBlockInfo = SemBlockInfo::buildSemBlock(semType, semCode, qPrintable(semValue));
        semanticSize += semBlockInfo->semBlockSize;
        semBlockInfos[j] = semBlockInfo;
    }

    char* semantic = new char[semanticSize];
    SEMANTIC* sem = (SEMANTIC*) semantic;
    sem->Ident = 0x7FFF;
    sem->Length = semanticSize;
    char* p = semantic;
    p += sizeof(SEMANTIC);

    for (int j = 0; j < semanticLength; ++j){
        SemBlockInfo* semBlockInfo = semBlockInfos[j];
        memcpy(p, semBlockInfo->semBlock, semBlockInfo->semBlockSize);
        p += semBlockInfo->semBlockSize;
        delete semBlockInfo;
    }

    return semantic;
}
