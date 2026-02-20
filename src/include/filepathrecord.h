#ifndef FILEPATHRECORD_H
#define FILEPATHRECORD_H

#include <QByteArray>
#include <QDateTime>

class FilePathRecord {
public:
    FilePathRecord(QByteArray &filePath, QByteArray &sourceFilePath);
    ~FilePathRecord();

    QByteArray getFilePath();
    QByteArray getSourceFilePath();
private:
    QByteArray sourceFilePath;
    QByteArray filePath;
    QDateTime lastUseDateTime;
};

#endif // FILEPATHRECORD_H
