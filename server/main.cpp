#include <QtCore/QCoreApplication>
#include "sslechoserver.h"
#include "DBConf.h"


int main(int argc, char *argv[])
{
    int port = 12345;  // this is the default cotext port
    // If the command line argument was set, use the user define port
    if (argc > 1) {
        std::string arg = argv[1];
        try {
            std::size_t pos;
            int x = std::stoi(arg, &pos);
            if (pos < arg.size()) {
                std::cerr << "Trailing characters after number: " << arg << '.\n';
            }
            else {
                port = x;
            }
        }
        catch (std::invalid_argument const& ex) {
            std::cerr << "Invalid number: " << arg << '\n';
        }
        catch (std::out_of_range const& ex) {
            std::cerr << "Number out of range: " << arg << '\n';
        }

    }


    QCoreApplication a(argc, argv);
    SslEchoServer server(port);
    dbConfigure();
    qsrand(QTime::currentTime().msec());
    
    Q_UNUSED(server);
    //test();
    return a.exec();
}

void test() {
    //getUserlist();
    //qDebug() <<  createDoc("AAA", 1);
    //addDocPermission(1,2);
    //qDebug() <<  checkDocPermission(1,1);
    // Create invitation code (invURI)
    //QString invURI = createInvite(1);
    //qDebug() <<invURI;
    // Accept the invite for a given code and user id
    //acceptInvite(invURI,2);
    // Check if all went good
    qDebug() << checkDocPermission(1, 2);

}