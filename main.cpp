#include <QApplication>
#include <QClipboard>
#include <QObject>
#include <QVector>
#include <QWidget>
#include <iostream>
#include <QListWidget>
#include <QVBoxLayout>
#include <QCursor>

#include "popup.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QStringList history;

    QClipboard* clipboard = QGuiApplication::clipboard();

    HistoryPopup popup(clipboard);
    
    QObject::connect(clipboard, &QClipboard::dataChanged, [&]() {
        QString text = clipboard->text();
        std::cout << text.toStdString() << std::endl;
        history.push_back(text);
        
        // For testing, open popup on copy
        popup.refresh(history);
        popup.move(QCursor::pos());
        popup.show();
    });
    

    

    return app.exec();
}