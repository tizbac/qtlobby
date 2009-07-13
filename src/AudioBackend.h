//
// C++ Interface: AudioBackend
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

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




