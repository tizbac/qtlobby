// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "AudioBackend.h"

AudioBackend::AudioBackend( QObject *parent ) : QObject( parent ) {

}

AudioBackend::~AudioBackend() { }

void AudioBackend::playSample( SampleCollection s ) {
    qDebug() << __FUNCTION__ << ": playing a sample: " << s;
    qDebug() << "can't play sample, PhononBackend not found. Maybe your version of QT is < 4.4.x";
}
