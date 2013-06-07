#include "sharedkey.h"

SharedKey::SharedKey(QObject *parent) 
    : ContentObject(parent)
{
    version = 0;
    chunkSize = 0;
    cacheVersion = 0;
    cacheChunkSize = 0;
}

int SharedKey::create()
{
    QByteArray dummyKey("sharedkey");
    updateChunk(1, 1, 1, dummyKey);
}

int SharedKey::renew()
{
    QByteArray dummyKey("renewedSharedkey");
    updateChunk(version + 1, 1, 1, dummyKey);
}

int SharedKey::readChunk(const int chunkNum, QByteArray &outputBuffer)
{
    if (content.contains(chunkNum)) {
        outputBuffer.push_back(content[chunkNum]->data());
        return content[chunkNum]->length();
    } else {
        return -1;
    }
}

int SharedKey::writeChunkCache(const int version, const int chunkNum, const int chunkSize, 
                               const QByteArray &chunkData)
{
    if (cacheVersion == 0) {
            // current cache is empty
        cacheVersion = version;
        cacheChunkSize = chunkSize;
        cache.insert(chunkNum, new QByteArray(chunkData));
    } else {
            // existing partial chunks in cahce
        if (version == cacheVersion) {
            if (!cache.contains(chunkNum)) {
                cache.insert(chunkNum, new QByteArray(chunkData));
            }
        } else {
                // version > cacheVersion : new update occurs before last update finish
                // first, clear stale cache
            for (int i = 1; i <= cacheChunkSize; i ++) {
                if (cache.contains(i)) {
                    QByteArray *chunk = cache[i];
                    cache.remove(i);
                    delete chunk;
                }
            }
                // then, insert new data into cache
            cacheVersion = version;
            cacheChunkSize = chunkSize;
            cache.insert(chunkNum, new QByteArray(chunkData));
        }
    }
    return 0;
}

bool SharedKey::update()
{
    for (int i = 1; i <= cacheChunkSize; ++i) {
        if (!cache.contains(i)) {
            return false;
        }
    }
        // replace content with cache
    for (int i = 1; i <= chunkSize; ++i) {
        QByteArray *chunk = content[i];
        content.remove(i);
        delete chunk;
    }
    for (int i = 1; i <= cacheChunkSize; ++i) {
        QByteArray *chunk = cache[i];
        content.insert(i, chunk);
        cache.remove(i);
    }
    version = cacheVersion;
    cacheVersion = 0;
    chunkSize = cacheChunkSize;
    cacheChunkSize = 0; 
    
    return true;
}

void SharedKey::getDebugInfo(QString &outputBuffer) const
{
    char verText[10];
    sprintf(verText, "%d, ", version);
    outputBuffer.append(verText);
    if (content.contains(1)) {
        outputBuffer.append((content[1])->data());
    }
}


#if WAF
#include "sharedkey.moc"
#include "sharedkey.cpp.moc"
#endif
