//
// C++ Implementation: BattleChannel
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "BattleChannel.h"
#include "MapInfoLoader.h"

BattleChannel::BattleChannel( QString id, Battles* battles, QObject * parent ) : AbstractChannel( id, parent ) {
    this->battles = battles;
    m_battle = battles->battleManager->getBattle( id.toInt() );
    battleWindowForm = new Ui::battleWindowForm();
    activeIcon = QIcon( ":/icons/battle.xpm" );
}

void BattleChannel::setupUi( QWidget * tab ) {
    AbstractChannel::setupUi( tab );
    QWidget * t = new QWidget( tab );
    battleWindowForm->setupUi( t );
    battleWindowForm->descriptionLabel->setWordWrap(true);
    gridLayout->addWidget( t, 0, 0, 1, 1 );
    connect( battleWindowForm->readyCheckBox, SIGNAL( toggled( bool ) ),
             this, SLOT( onReadyCheckBoxChanged( bool ) ) );
    connect(battleWindowForm->specCheckBox, SIGNAL( toggled ( bool ) ),
            this,SLOT(onSpecCheckBoxChanged(bool))); // NEW
    connect(battleWindowForm->factionsComboBox, SIGNAL( currentIndexChanged( int)),
            this,SLOT(onSideComboBoxChanged(int)));
    requestMapInfo( m_battle.mapName );
    fillModOptions();
    fillSides();
    //   gameHostingStartPushButton->setEnabled(false);
}

void BattleChannel::receiveCommand( Command command ) {
    //   qDebug() << command.toQString();
    command.name = command.name.toUpper();
    QStringList battleChannelCommands;
    battleChannelCommands
            << "BATTLECLOSED"
            << "JOINBATTLE"
            << "JOINEDBATTLE"
            << "LEFTBATTLE"
            << "JOINBATTLEFAILED"
            << "OPENBATTLEFAILED"
            << "UPDATEBATTLEINFO"
            << "SAIDBATTLE"
            << "SAIDBATTLEEX"
            << "CLIENTSTATUS"
            << "CLIENTBATTLESTATUS"
            << "FORCEQUITBATTLE"
            << "DISABLEUNITS"
            << "ENABLEUNITS"
            << "ENABLEALLUNITS"
            << "RING"
            << "REDIRECT"
            << "BROADCAST"
            << "ADDBOT"
            << "REMOVEBOT"
            << "UPDATEBOT"
            << "ADDSTARTRECT"
            << "REMOVESTARTRECT"
            << "MAPGRADES"
            << "SCRIPTSTART"
            << "SCRIPT"
            << "SCRIPTEND"
            << "SETSCRIPTTAGS"
            << "REMOVESCRIPTTAGS";

    /*
** server commands:
  done (in lobbytabs) BATTLECLOSED BATTLE_ID
  done (in battles) JOINBATTLE BATTLE_ID [password]
  done (in users) JOINEDBATTLE BATTLE_ID username
  done (in users) LEFTBATTLE BATTLE_ID username
  JOINBATTLEFAILED {reason}
  OPENBATTLEFAILED {reason}
  done (locked is shown here, speccount and hash in battles) UPDATEBATTLEINFO BATTLE_ID SpectatorCount locked maphash {mapname}
  done SAIDBATTLE username {message}
  done SAIDBATTLEEX username {message}
  done (in users) CLIENTSTATUS username status
  done (in users, battles) CLIENTBATTLESTATUS username battlestatus teamcolor
  done (in lobbytabs) FORCEQUITBATTLE
  DISABLEUNITS unitname1 unitname2 ...
  ENABLEUNITS unitname1 unitname2 ...
  ENABLEALLUNITS
  RING username
  REDIRECT ipaddress
  BROADCAST {message}
  ADDBOT BATTLE_ID name owner battlestatus teamcolor {AIDLL}
  REMOVEBOT BATTLE_ID name
  UPDATEBOT BATTLE_ID name battlestatus teamcolor
  ADDSTARTRECT allyno left top right bottom
  REMOVESTARTRECT allyno
  MAPGRADES hashlist
  SCRIPTSTART
  SCRIPT {line}
  SCRIPTEND
  SETSCRIPTTAGS {pair1} [{pair2}] [{pair3}] [{...}]
  REMOVESCRIPTTAGS key1 [key2] [key3] [...]
  TESTLOGINACCEPT
  TESTLOGINDENY
  ACQUIREUSERID
  USERID userID

**  client commands:
  TESTLOGIN username password
  REMOVESCRIPTTAGS key1 [key2] [key3] [...]
  SETSCRIPTTAGS {pair1} [{pair2}] [{pair3}] [{...}]
  SCRIPTEND
  SCRIPT {line}
  SCRIPTSTART
  MAPGRADESFAILED {reason}
  MAPGRADES hashlist
  REMOVESTARTRECT allyno
  ADDSTARTRECT allyno left top right bottom
  UPDATEBOT name battlestatus teamcolor
  REMOVEBOT name
  ADDBOT name battlestatus teamcolor {AIDLL}
  ENABLEALLUNITS
  ENABLEUNITS unitname1 unitname2 ...
  DISABLEUNITS unitname1 unitname2 ...
  FORCESPECTATORMODE username
  FORCETEAMCOLOR username color
  FORCEALLYNO username teamno
  FORCETEAMNO username teamno
  HANDICAP username value
  KICKFROMBATTLE username
  done (in users) REQUESTBATTLESTATUS
  done (in users) MYBATTLESTATUS battlestatus myteamcolor
  done SAYBATTLEEX {message}
  done SAYBATTLE {message}
  UPDATEBATTLEINFO SpectatorCount locked maphash {mapname}
  done LEAVEBATTLE
  */
    //   if ( !battleChannelCommands.contains( command.name ) )
    //     return;
    QString line = QString( "%1\n" );
    if ( command.name == "SAIDBATTLE" ) {
        QString userName = command.attributes.takeFirst();
        insertLine( flag( userName ) + line
                    .arg( "&lt;%1&gt; %2" )
                    .arg( userName )
                    .arg( command.attributes.join( " " ) ) );
    }
    if ( command.name == "RING" ) {
        QString userName = command.attributes.takeFirst();
        emit playSample( RingSample );
        emit newTrayMessage( QString("RING!? Where are you!? Get up! %1!").arg(userName) );
    }
    else if ( command.name == "SAIDBATTLEEX" ) {
        QString userName = command.attributes.takeFirst();
        insertLine( flag( userName ) + line
                    .arg( "<span style=\"color:magenta;\">* %1 %2</span>" )
                    .arg( userName )
                    .arg( command.attributes.join( " " ) ) );
    }
    else if ( command.name == "FORCEQUITBATTLE" ) {
        QMessageBox::critical(NULL, "Kicked from battle", "You have been kicked from the battle, poor you!");
        insertLine("You have been kicked from the battle!\n");
    }
    else if ( command.name == "JOINEDBATTLE" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            QString userName = command.attributes.takeFirst();
            insertLine( flag( userName ) + line
                        .arg( "<span style=\"color:darkgreen;\">** %1</span>" )
                        .arg( tr( "%2 joined the battle." ) )
                        .arg( userName ) );
        }
    }
    else if ( command.name == "LEFTBATTLE" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            QString userName = command.attributes.takeFirst();
            insertLine( flag( userName ) + line
                        .arg( "<span style=\"color:darkgreen;\">** %1</span>" )
                        .arg( tr( "%1 left the battle." ) )
                        .arg( userName ) );
        }
    }
    else if ( command.name == "JOINBATTLE" ) {}
    else if ( command.name == "CLIENTMSG" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            insertLine( line
                        .arg( "<span style=\"color:blue;\">** CLIENTMSG ** %1</span>" )
                        .arg( command.attributes.join( " " ) ) );
        }
    }
    else if ( command.name == "BATTLECLOSED" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            //nothing to do yet
        }
    }
    else if ( command.name == "UPDATEBATTLEINFO" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            command.attributes.removeFirst(); // spectator count
            bool locked = command.attributes.takeFirst().toInt() > 0;
            command.attributes.removeFirst(); // map hash
            QString mapName = command.attributes.join( " " );
            if ( battleWindowForm->lockGameCheckBox->isChecked() != locked ) {
                insertLine( line
                            .arg( "<span style=\"color:blue;\">** %1</span>" )
                            .arg( locked ? tr( "Battle locked." ) : tr( "Battle unlocked." ) ) );
                battleWindowForm->lockGameCheckBox->setChecked( locked );
            }
            requestMapInfo( mapName );
        }
    }
    else if ( command.name == "SETSCRIPTTAGS" ) {
        command.attributes = command.attributes.join( " " ).split( "\t" );
        qDebug("SETSCRIPTTAGS %s", command.attributes.join( " " ).toStdString().c_str());
        foreach( QString s, command.attributes ) {
            QString key = s.section( "=", 0, 0 ).section( "/", 1, 1 ).toLower();
            int val = s.section( "=", 1, 1 ).toInt();
            /*if ( key == "startmetal" )
                battleWindowForm->metalSpinBox->setValue( val );
            else if ( key == "startenergy" )
                battleWindowForm->energySpinBox->setValue( val );
            else if ( key == "maxunits" )
                battleWindowForm->unitsSpinBox->setValue( val );
            else */
            if ( key == "startpostype" ) {
                val = qMax( qMin( val, 2 ), 0 );
                battleWindowForm->startPositionComboBox->setCurrentIndex( val );
            } else if ( key == "gamemode" ) {
                val = qMax( qMin( val, 2 ), 0 );
                battleWindowForm->gameEndComboBox->setCurrentIndex( val );
            } else if ( key == "limitdgun" )
                battleWindowForm->limitDGunCheckBox->setChecked( val > 0 );
            else if ( key == "diminishingmms" )
                battleWindowForm->diminishingMetalMakersCheckBox->setChecked( val > 0 );
            else if ( key == "ghostedbuildings" )
                battleWindowForm->ghostedBuildingsCheckBox->setChecked( val > 0 );
            if(key == "modoptions") {
                key = s.section( "=", 0, 0 ).section( "/", 2, 2 ).toLower();
                m_battle.options[key] = s.section( "=", 1, 1 );
                qDebug("%s set to %s", key.toStdString().c_str(), s.section( "=", 1, 1 ).toStdString().c_str());
            }
        }
        fillModOptions();
    }
}

void BattleChannel::fillSides()
{
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    if(!unitSyncLib->setCurrentMod(m_battle.modName)) {
        return;
    }
    int sidec = unitSyncLib->getSideNameCount(m_battle.modName);
    for(int i=0;i<sidec;i++)
    {
        QString sidename = unitSyncLib->sideName(m_battle.modName, i);
        battleWindowForm->factionsComboBox->addItem(unitSyncLib->getSideIcon(m_battle.modName,sidename) ,sidename, QVariant(sidename.toUpper()));
    }
}

void BattleChannel::receiveInput( QString input ) {
    if ( executeChannelInput( input ) )
        return;
    input.replace( "<", "&lt;" );
    input.replace( ">", "&gt;" );
    QStringList inputList = input.split( " " );
    QString firstWord = inputList.takeFirst();
    Command ret;
    if ( "/me" == firstWord ) {
        ret.name = "SAYBATTLEEX";
        ret.attributes << inputList;
    }
    else if ( "/slap" == firstWord ) {
        ret.name = "SAYBATTLEEX";
        ret.attributes << QString( "slaps %1 around a bit with a large trout" ).arg( inputList.join( " " ) );
    }
    else if ( QString( "/leave" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
        ret.name = "LEAVEBATTLE";
    }
    else {
        ret.name = "SAYBATTLE";
        ret.attributes << input;
    }
    emit sendCommand( ret );
}

void BattleChannel::onReadyCheckBoxChanged( bool isChecked ) {
    emit readyStateChanged( isChecked );
}

/* NEW */
void BattleChannel::onSideComboBoxChanged( int index )
{
    qDebug() << "onSideComboBoxChanged called!";
    emit sideChanged(index);
}

/* NEW */
void BattleChannel::onSpecCheckBoxChanged(bool isChecked ) {
    emit specStateChanged( isChecked );
    battleWindowForm->readyCheckBox->setDisabled( isChecked );
}

void BattleChannel::fillModOptions() {
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    if(!unitSyncLib->setCurrentMod(m_battle.modName)) {
        battleWindowForm->modOptions->setHtml("<font size=\"16\" color=\"red\">Please, download " + m_battle.modName+"</font>");
        return;
    }
    QString buffer;
    int num_options = unitSyncLib->getModOptionCount();
    buffer.append("<table cellpadding=\"4\">");
    for(int i = 0; i < num_options; i++) {
        if(unitSyncLib->getOptionType(i) == SECTION && i != num_options - 1 && unitSyncLib->getOptionType(i+1) == SECTION) continue;
        if(unitSyncLib->getOptionType(i) == SECTION) {
            buffer.append("</table>");
            buffer.append("<h3>"+unitSyncLib->getOptionName(i)+"</h3>");
            buffer.append("<table cellpadding=\"4\">");
            continue;
        }
        if(unitSyncLib->isGameOption(i)) continue;
        buffer.append("<tr>");
        buffer.append("<td>");
        buffer.append(unitSyncLib->getOptionName(i));
        buffer.append("</td>");
        buffer.append("<td>");
        bool nondefault = m_battle.options.contains(unitSyncLib->getOptionKey(i));
        qDebug("Parsing %s, nondefault is %s", unitSyncLib->getOptionKey(i).toStdString().c_str(), nondefault ? "true" : "false");
        switch(unitSyncLib->getOptionType(i)) {
        case UNDEFINED:
            buffer.append("Undefined");
            break;
        case BOOLEAN:
            nondefault = nondefault && m_battle.options[unitSyncLib->getOptionKey(i)].toFloat() != unitSyncLib->getOptionBoolDef(i);
            if(nondefault)
                buffer.append(m_battle.options[unitSyncLib->getOptionKey(i)].toFloat() ? "<b><font color=\"green\">Yes</font><b>" : "<b><font color=\"red\">No</font><b>" );
            else
                buffer.append(unitSyncLib->getOptionBoolDef(i) ? "<font color=\"green\">Yes</font>" : "<font color=\"red\">No</font>" );
            break;
        case LIST:
            nondefault = nondefault && m_battle.options[unitSyncLib->getOptionKey(i)].toString() != unitSyncLib->getOptionListDef(i);
            if(nondefault)
                buffer.append("<b>"+m_battle.options[unitSyncLib->getOptionKey(i)].toString()+"</b>");
            else
                buffer.append(unitSyncLib->getOptionListDef(i));
            break;
        case FLOAT:
            nondefault = nondefault && m_battle.options[unitSyncLib->getOptionKey(i)].toFloat() != unitSyncLib->getOptionNumberDef(i);
            if(nondefault)
                buffer.append("<b>"+m_battle.options[unitSyncLib->getOptionKey(i)].toString()+"</b>");
            else
                buffer.append(QString::number(unitSyncLib->getOptionNumberDef(i)));
            break;
        case STRING:
            nondefault = nondefault && m_battle.options[unitSyncLib->getOptionKey(i)].toString() != unitSyncLib->getOptionStringDef(i);
            if(nondefault)
                buffer.append("<b>"+m_battle.options[unitSyncLib->getOptionKey(i)].toString()+"</b>");
            else
                buffer.append(unitSyncLib->getOptionStringDef(i));
            break;
        }
        buffer.append("</td>");
        buffer.append("</tr>");
    }
    buffer.append("</table>");
    battleWindowForm->modOptions->setHtml(buffer);
}

void BattleChannel::requestMapInfo( QString mapName ) {
    MapInfoLoader* loader = new MapInfoLoader(mapName, this);
    battleWindowForm->minimapWidget->setErrorMessage("Loading " + mapName + "...");
    battleWindowForm->heightmapWidget->setErrorMessage("Loading " + mapName + "...");
    battleWindowForm->metalmapWidget->setErrorMessage("Loading " + mapName + "...");
    connect(loader, SIGNAL(loadCompleted(QString)), SLOT(updateMapInfo(QString)));
    loader->start();
}

void BattleChannel::updateMapInfo( QString mapName ) {
    MapInfoLoader* loader = qobject_cast<MapInfoLoader*>(sender());
    if(!loader) return;
    battleWindowForm->nameLabel->setText(mapName);
    if(!loader->mapPresent) {
        battleWindowForm->minimapWidget->setErrorMessage("Map " + mapName + " not found");
        battleWindowForm->heightmapWidget->setErrorMessage("Map " + mapName + " not found");
        battleWindowForm->metalmapWidget->setErrorMessage("Map " + mapName + " not found");
    } else {
        battleWindowForm->minimapWidget->setErrorMessage(QString::null);
        battleWindowForm->heightmapWidget->setErrorMessage(QString::null);
        battleWindowForm->metalmapWidget->setErrorMessage(QString::null);
        battleWindowForm->sizeLabel->setText(QString("%1x%2").arg(loader->mapinfo.width).arg(loader->mapinfo.height));
        battleWindowForm->windspeedLabel->setText(QString("%1x%2").arg(loader->mapinfo.minWind).arg(loader->mapinfo.maxWind));
        battleWindowForm->tidalLabel->setText(QString::number(loader->mapinfo.tidalStrength));
        battleWindowForm->authorLabel->setText(loader->mapinfo.author);
        battleWindowForm->descriptionLabel->setText(loader->mapinfo.description);
        battleWindowForm->minimapWidget->setImage(loader->minimap);
        battleWindowForm->heightmapWidget->setImage(loader->heightmap);
        battleWindowForm->metalmapWidget->setImage(loader->metalmap);
    }
    loader->rawHeightmap.free();
    delete loader;
    //loader->deleteLater();
}

