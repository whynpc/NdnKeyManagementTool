#ifndef SHAREDKEY_H
#define SHAREDKEY_H

#include <QByteArray>
#include <QHash>

class SharedKey : public ContentObject
{
public:
    explicit SharedKey(QObject *parent);

// The following two functions used by organizer only
    int create();
    int renew();

private:
    QHash<int, QByteArray *> content;    
    QHash<int, QByteArray *> cache;
    int cacheVersion;
    int cacheChunkSize;
};

#endif // SHAREDKEY_H
