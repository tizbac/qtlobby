#include "ChannelTextBrowser.h"

QMimeData *ChannelTextBrowser::createMimeDataFromSelection() const
{
        QMimeData *res = QTextEdit::createMimeDataFromSelection();

        if(res->hasHtml())
        {
            QString html = res->html();

            int pos = html.indexOf(" <img");
            int lpos = (html.indexOf(">", pos) - pos);
            html.replace(pos,lpos+1,"");

            QTextDocumentFragment frag;
            frag = QTextDocumentFragment::fromHtml(html, document());    

            res->setText(frag.toPlainText());
        }
        return res;
}
