#include "sharedkey.h"

SharedKey::SharedKey()
{
    version = 0;
}

int SharedKey::getVersion() const
{
    return version;
}

void SharedKey::setVersion(const int version)
{
    this->version = version;
}

int SharedKey::genNewSharedKey()
{
    version = 1;
    size = 1;
    // TODO: fill in chunk content with some crypto algorithm
    content.clear();
    QByteArray key("abcde");
    content.insert(1, key);
    return version;
}

int SharedKey::readChunk(const int chunkNum, QByteArray outputBuffer) const
{
    if (content.contains(chunkNum)) {
        outputBuffer.push_back(content[chunkNum]);
        return content[chunkNum].length();
    } else {
        return -1;
    }
}

int SharedKey::getSize() const
{
    return size;
}

void SharedKey::setSize(const int size)
{
    this->size = size;
}

void SharedKey::renew()
{
    size = 0;
    content.clear();
}

bool SharedKey::isComplete() const
{
    if (size > 0) {
        for (int i = 1; i <= size; i++) {
            if (!content.contains(i)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

int SharedKey::saveChunk(const int chunkNum, const QByteArray &chunkData)
{
    if (content.contains(chunkNum)) {
        // do nothing
    } else {
        QByteArray key(chunkData);
        content.insert(chunkNum, key);
    }
    return 0;
}
