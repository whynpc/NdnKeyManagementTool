#ifndef PUBLICKEY_H
#define PUBLICKEY_H

#include <QByteArray>

#include "contentobject.h"

class PublicKey : public ContentObject
{
public:
    explicit PublicKey(QObject *parent);
    int readChunk(const int chunkNum, QByteArray &outputBuffer);
    
protected:
    int writeChunkCache(const int version, const int chunkNum, const int chunkSize,
                        const QByteArray &chunkData);
    bool update();

};    


#endif // PUBLICKEY_H
