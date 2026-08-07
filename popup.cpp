#include <QWidget>
#include <QListWidget>
#include <QClipboard>
#include <QVBoxLayout>
#include <QApplication>
#include <QStringList>
#include <QShortcut>

#include <qhotkey.h>

#include "popup.hpp"

    // Constructor
    HistoryPopup::HistoryPopup(QClipboard* clipboard, QWidget* parent)
        : QWidget(parent, Qt::FramelessWindowHint | Qt::Popup)
    {   
        // Setup GUI
        auto* layout = new QVBoxLayout(this);
        list = new QListWidget(this);
        layout->addWidget(list);
        setLayout(layout);

        // Assign selected clipboard history to your clipboard and close the popup
        connect(list, &QListWidget::itemClicked, this, [=](QListWidgetItem* item){
            clipboard->setText(item->text());
            close();
        });
        
        // Set up hotkey to open popup
        auto* hotkey = new QHotkey(QKeySequence("CTRL+SHIFT+V"), true, this);
        qDebug() << "Hotkey registered: " << hotkey->isRegistered();
        connect(hotkey, &QHotkey::activated, this, &HistoryPopup::open);

    }
    
    void HistoryPopup::updateHistory(const QString str) {
        if (str.isEmpty()) return; // no empty
        if (history.contains(str)) {
            // Bring it to front of list
            size_t idx = history.indexOf(str);
            history.move(idx, history.length()-1);
            return;
        }
        history.push_back(str);
    }

    void HistoryPopup::open() {
        refresh(); // Update popup with current history
        move(QCursor::pos()); // Ensures popup is placed on cursor
        show(); // Shows popup
    }

    // update list on GUI
    void HistoryPopup::refresh() {
        list->clear();
        for (const auto &item : history) list->addItem(item);
    }