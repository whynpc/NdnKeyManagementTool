#ifndef CONTENTOBJECT_H
#define CONTENTOBJECT_H

#include <QObject>
#include <QByteArray>

class ContentObject : public QObject
{
    Q_OBJECT

public:
    explicit ContentObject(QObject *parent);
    
    int getVersion() const;
    void setVersion(const int version);
    int getChunkSize() const;
    void setChunkSize(const int chunkSize);
    
    virtual int readChunk(const int chunkNum, QByteArray &outputBuffer) = 0;
    int updateChunk(const int version, const int chunkNum, 
                    const int chunkSize, const QByteArray &chunkData);

signals:
// notification for finishing recv all chunks of a new version
    void updateComplete(const int version);    

protected:
// save the recved chunk in cache
// potentially clear state cache for new updates
// invoked only when version > this.version
    virtual int writeChunkCache(const int version, const int chunkNum, const int chunkSize, 
                                const QByteArray &chunkData) = 0;
// check whether has got all the chunks in cache
// if so, replace content with new data in cache & return true
// otherwise return false
    virtual bool update() = 0;   
    
    int version;    
    int chunkSize;
};

    

#endif // CONTENTOBJECT_H
