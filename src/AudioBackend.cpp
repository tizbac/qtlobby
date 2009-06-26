//
// C++ Implementation: AudioBackend
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "AudioBackend.h"

AudioBackend::AudioBackend( QObject *parent ) : QObject( parent ) {
#if QT_VERSION >= 0x040400
  qDebug() << "PhononBackend loaded";
#endif
}

AudioBackend::~AudioBackend() { }

void AudioBackend::playSample( SampleCollection s ) {
  qDebug() << __FUNCTION__ << ": playing a sample: " << s;
#if QT_VERSION >= 0x040400
  qDebug() << "phonon called made";
 // phononBackend.play();
#else
  qDebug() << "can't play sample, PhononBackend not found. Maybe your version of QT is < 4.4.x";
#endif
}
