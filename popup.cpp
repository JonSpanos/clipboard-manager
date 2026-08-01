#include "popup.hpp"
#include <QWidget>
#include <QListWidget>
#include <QClipboard>
#include <QVBoxLayout>
#include <QStringList>

    // Constructor
    HistoryPopup::HistoryPopup(QClipboard* clipboard, QWidget* parent)
        : QWidget(parent, Qt::Popup)
    {
        auto* layout = new QVBoxLayout(this);
        list = new QListWidget(this);
        layout->addWidget(list);
        setLayout(layout);

        connect(list, &QListWidget::itemClicked, this, [=](QListWidgetItem* item){
            clipboard->setText(item->text());
            close();
        });
    }

    // update list on GUI
    void HistoryPopup::refresh(const QStringList &history) {
        list->clear();
        for (const auto &item : history) list->addItem(item);
    }