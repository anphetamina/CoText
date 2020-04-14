#ifndef ACTIONEVENTWATCHER_H
#define ACTIONEVENTWATCHER_H

#include <QObject>

class ActionEventWatcher : public QObject
{
    Q_OBJECT
public:
    explicit ActionEventWatcher(QObject *parent = nullptr);

signals:

};

#endif // ACTIONEVENTWATCHER_H
