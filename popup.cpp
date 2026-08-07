#include <QWidget>
#include <QListWidget>
#include <QClipboard>
#include <QVBoxLayout>
#include <QApplication>
#include <QStringList>
#include <QShortcut>

#include <qhotkey.h>

#include "popup.hpp"

// Constant
#define WindowHeight 600
#define WindowWidth 400

// Constructor
HistoryPopup::HistoryPopup(QClipboard* clipboard, QWidget* parent)
    : QWidget(parent, Qt::Popup)
{   
    setAttribute(Qt::WA_TranslucentBackground, true);

    setFixedWidth(WindowWidth);
    setFixedHeight(WindowHeight);

    // Setup GUI
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12); // Margins of text in each row
    
    list = new QListWidget(this);
    // Scroll Behavior
    list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // Selection
    list->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // Style
    list->setUniformItemSizes(false);
    list->setWordWrap(false);
    list->setSpacing(8);
    list->setStyleSheet("QListWidget::item {border: 2px solid black; border-radius: 8px;} \
                         QListWidget::item::selected {color: black;}");

    layout->addWidget(list);


    setLayout(layout);

    // Assign selected clipboard history (click/enter) to your clipboard and close the popup
    connect(list, &QListWidget::itemClicked, this, [=](QListWidgetItem* item){
        clipboard->setText(item->text());
        close();
    });
    connect(list, &QListWidget::itemActivated, this, [=](QListWidgetItem* item){
        clipboard->setText(item->text());
        close();
    });
    
    // Set up hotkey to open popup
    auto* hotkey = new QHotkey(QKeySequence("CTRL+SHIFT+V"), true, this);
    qDebug() << "Hotkey registered: " << hotkey->isRegistered();
    connect(hotkey, &QHotkey::activated, this, &HistoryPopup::open);

}

// Adds new entry to history / brings old entry up to front if copied again.
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

// Updates list with new history, places popup at cursor, and shows it for user.
void HistoryPopup::open() {
    refresh(); // Update popup with current history
    move(QCursor::pos()); // Places popup on cursor, at the bottom-left corner
    show(); // Shows popup
}

// update list on GUI
void HistoryPopup::refresh() {
    list->clear();
    for (auto i = history.rbegin(); i != history.rend(); i++) {
        auto* it = new QListWidgetItem(*i);
        it->setSizeHint(QSize(WindowWidth, WindowHeight/4));
        list->addItem(it);
    }
}