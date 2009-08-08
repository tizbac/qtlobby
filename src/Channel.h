// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef CHANNEL_H
#define CHANNEL_H
#include <QDateTime>
#include "AbstractChannel.h"

/**
 * This channel displays the chat messages
 */
class Channel : public AbstractChannel {
    Q_OBJECT
public:
    Channel( QString name, QObject * parent = 0 );
    virtual ~Channel();
signals:
    void enableJoinLeaveDefault(bool b);

public slots:
    void receiveInput( QString input );
    void receiveCommand( Command command );
    void anchorClicked(QUrl url);
    void setShowJoinLeave(bool b);
private slots:
    void onEnableJoinLeave(bool b);
protected:
    virtual void setupUi( QWidget * tab );
private:
    bool showJoinLeave;
    bool showJoinLeaveDefault;
    bool isDefault;
    bool uiSetup;
};

#endif
