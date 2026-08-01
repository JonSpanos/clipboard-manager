#include <QApplication>
#include <QClipboard>
#include <QObject>
#include <QVector>
#include <QWidget>
#include <iostream>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLocalSocket>
#include <QCursor>
#include <QLocalServer>

#include "popup.hpp"    

const QString SERVER_NAME = "clipboard-manager-instance";

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // If the app is relaunched, we'll take this as a message to show the popup.
    QLocalSocket socket;
    socket.connectToServer(SERVER_NAME);
    if (socket.waitForConnected(200)) {
        // Another instance is running, ping it to show popup and close.
        socket.write("showpopup");
        socket.waitForBytesWritten();
        return 0;
    }


    QStringList history;

    QClipboard* clipboard = QGuiApplication::clipboard();

    HistoryPopup popup(clipboard);
    
    QObject::connect(clipboard, &QClipboard::dataChanged, [&]() {
        QString text = clipboard->text();
        if (text.isEmpty()) return; // no empty
        if (history.contains(text)) {
            // Bring it to front of list
            size_t idx = history.indexOf(text);
            history.move(idx, history.length()-1);
            return;
        }
        history.push_back(text);
    });

    // Listen for commands from socket.
    QLocalServer server;
    QLocalServer::removeServer(SERVER_NAME); // clean up
    server.listen(SERVER_NAME);
    
    QObject::connect(&server, &QLocalServer::newConnection, [&]() {
        QLocalSocket* client = server.nextPendingConnection();
        client->waitForReadyRead(200);
        QByteArray msg = client->readAll();
        if (msg == "showpopup") {
            popup.refresh(history); // Update popup with current history
            popup.move(QCursor::pos()); // Ensures popup is placed on cursor
            popup.show(); // Shows popup
            popup.activateWindow(); // Gives window keyboard
        }
    });
    

    return app.exec();
}