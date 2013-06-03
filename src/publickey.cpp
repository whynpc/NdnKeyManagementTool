#include "publickey.h"

PublicKey::PublicKey(QObject *parent) 
    : ContentObject(parent)

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


#if WAF
#include "publickey.moc"
#include "publickey.cpp.moc"
#endif
