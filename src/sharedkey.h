#ifndef SHAREDKEY_H
#define SHAREDKEY_H

#include <QByteArray>
#include <QHash>

#include "contentobject.h"

class SharedKey : public ContentObject
{
public:
    explicit SharedKey(QObject *parent);

// The following two functions used by organizer only
    int create();
    int renew();

    int readChunk(const int chunkNum, QByteArray &outputBuffer);    

protected:
    int writeChunkCache(const int version, const int chunkNum, const int chunkSize, 
                        const QByteArray &chunkData);
    bool update();

private:
    QHash<int, QByteArray *> content;    
    QHash<int, QByteArray *> cache;
    int cacheVersion;
    int cacheChunkSize;
};

#endif // SHAREDKEY_H
