#include <iostream>

#include <QApplication>
#include <QClipboard>
#include <QObject>
#include <QVector>
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QCursor>
#include <QShortcut>
#include <QKeySequence>

#include "popup.hpp"    

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QStringList history;

    QClipboard* clipboard = QGuiApplication::clipboard();

    HistoryPopup popup(clipboard);
    
    QObject::connect(clipboard, &QClipboard::dataChanged, [&]() { // When clipboard data updates, add it to the history stored on the popup.
        QString text = clipboard->text();
        popup.updateHistory(text);
    });

    return app.exec();
}