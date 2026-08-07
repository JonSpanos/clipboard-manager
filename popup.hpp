#ifndef POPUP_HPP
#define POPUP_HPP

#include <QWidget>
#include <QListWidget>
#include <QClipboard>
#include <QStringList>

class HistoryPopup : public QWidget {
    Q_OBJECT
    public:
        // Constructor
        HistoryPopup(QClipboard* clipboard, QWidget* parent = nullptr);

        // update list on GUI
        void updateHistory(const QString str);

    private:
        void refresh();
        void open();
        QListWidget* list;
        QStringList history;
};

#endif