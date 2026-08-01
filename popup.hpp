#ifndef POPUP_HPP

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
        void refresh(const QStringList &history);
        
    private:
            QListWidget* list;
};

#endif