#ifndef CHANNELTEXTBROWSER_H
#define CHANNELTEXTBROWSER_H

#include <QTextBrowser>
#include <QMimeData>
#include <QDebug>
#include <QMessageBox>
#include <QRegExp>
#include <QTextDocumentFragment>

class ChannelTextBrowser : public QTextBrowser
{
public:
    ChannelTextBrowser(QWidget * parent) : QTextBrowser(parent){}
protected:
    QMimeData *createMimeDataFromSelection () const;
};

#endif // CHANNELTEXTBROWSER_H
