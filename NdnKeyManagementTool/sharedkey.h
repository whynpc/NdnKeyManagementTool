#ifndef SHAREDKEY_H
#define SHAREDKEY_H

#include <QByteArray>
#include <QHash>

class SharedKey
{
public:
    explicit SharedKey();

    int genNewSharedKey();
    int readChunk(const int chunkNum, QByteArray outputBuffer) const;
    int saveChunk(const int chunkNum, const QByteArray &chunkData);
    void renew();
    bool isComplete() const;

    int getVersion() const;
    void setVersion(const int version);
    int getSize() const;
    void setSize(const int size);

private:
    int version;
    QHash<int, QByteArray> content;
    int size;
};

#endif // SHAREDKEY_H
