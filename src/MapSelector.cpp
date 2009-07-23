// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#include "MapSelector.h"

MapSelector::MapSelector(QDialog* parent) : QDialog( parent) {
    scrollArea = new QScrollArea;

    QWidget* container = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    for (int i = 0 ; i < 10; ++i ) {
        mapElementWidget* a = new mapElementWidget;
        mainLayout->addWidget(a);
    }

    container->setLayout(mainLayout);
    scrollArea->setWidget(container);
    //   scrollArea->show();
}

MapSelector::~MapSelector() {
}

