#include "contentobject.h"

ContentObject::ContentObject(QObject *parent) 
    : QObject(parent)
{
}

int ContentObject::getVersion() const
{
    return version;
}

void ContentObject::setVersion(const int version)
{
    this->version = version;
}

int ContentObject::getChunkSize() const
{
    return chunkSize;    
}

void ContentObject::setChunkSize(const int chunkSize)
{
    this->chunkSize = chunkSize;    
}

int ContentObject::updateChunk(const int version, const int chunkNum, 
                               const int chunkSize, const QByteArray &chunkData)
{
    if (version > this->version) {
        if (writeChunkCache(version, chunkNum, chunkSize, chunkData) < 0 ) {
            return -1;
        }   
        if (update()) {        
            emit updateComplete(version);
        }
        return 0;
    }    
    return -1;
}

#if WAF
#include "sharedkey.moc"
#include "sharedkey.cpp.moc"
#endif
