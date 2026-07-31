#include <QApplication>
#include <QClipboard>
#include <QObject>
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QClipboard* clipboard = QGuiApplication::clipboard();

    QObject::connect(clipboard, &QClipboard::dataChanged, [=]() {
        QString text = clipboard->text();
        std::cout << text.toStdString() << std::endl;
    });

    return app.exec();
}