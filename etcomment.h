#ifndef ETCOMMENT_H
#define ETCOMMENT_H

#include <QObject>

class EtComment : public QObject
{
    Q_OBJECT
public:
    explicit EtComment(QObject *parent = nullptr);

signals:
};

#endif // ETCOMMENT_H
