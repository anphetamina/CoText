#ifndef PDS_CPP_NETWORKSERVER_H
#define PDS_CPP_NETWORKSERVER_H

#include <queue>
#include <memory>
#include "SharedEditor.h"
#include "Message.h"

class NetworkServer {
private:
    std::vector<SharedEditor*> editors;
    std::deque<Message> messages;
    int id;

public:
    NetworkServer();
    int connect(SharedEditor* sharedEditor);
    void disconnect(SharedEditor* sharedEditor);
    void send(const Message& m);
    void dispatchMessages();
};


#endif //PDS_CPP_NETWORKSERVER_H
