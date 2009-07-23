// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#ifndef AUDIOBACKEND_H
#define AUDIOBACKEND_H

#include <QObject>
#include <QDebug>

enum SampleCollection {
    LoginSample,
    QuerySample,
    RingSample,
    BattleRingSample
};

class AudioBackend : public QObject {
    Q_OBJECT
public:
    AudioBackend( QObject *parent = 0 );
    ~AudioBackend();

public slots:
    void playSample( SampleCollection  s );
private:
#if QT_VERSION >= 0x040400
    //PhononBackend phononBackend;
#endif
};

#endif




