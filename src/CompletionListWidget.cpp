#include "CompletionListWidget.h"

CompletionListWidget::CompletionListWidget(QWidget* parent) : QListWidget(parent) {
    connect(this, SIGNAL(itemActivated(QListWidgetItem*)), SLOT(selectionCompleted()));
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectionCompleted()));
    m_start = 0;
    m_length = 0;
}

void CompletionListWidget::setCompletionParams(int start, int length) {
    m_start = start;
    m_length = length;
}

void CompletionListWidget::selectionCompleted() {
    hide();
    emit completionSelected(m_start, m_length, selectedItems()[0]->text());
    deleteLater();
}
