#include "publickey.h"

PublicKey::PublicKey(QObject *parent) 
    : ContentObject(parent), version(0)

{

}

int PublicKey::readChunk(const int chunkNum, QByteArray &outputBuffer)
{
    return 0;
    
}

int PublicKey::writeChunkCache(const int version, const int chunkNum, const int chunkSize, 
                               const QByteArray &chunkData)
{
    return 0;
    
}

bool PublicKey::update()
{
    return false;    
}

