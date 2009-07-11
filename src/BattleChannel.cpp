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
#include <QSplitter>

BattleChannel::BattleChannel( QString id, Battles* battles, QObject * parent ) : AbstractChannel( id, parent ) {
    this->battles = battles;
    m_battle = battles->battleManager->getBattle( id.toInt() );
    battleWindowForm = new Ui::battleWindowForm();
    activeIcon = QIcon( ":/icons/battle.xpm" );
    mapOverviewDialog = new MapOverviewDialog();
    loader = new MapInfoLoader(this);
    connect(loader, SIGNAL(loadCompleted(QString)), SLOT(updateMapInfo(QString)));
    noMapUpdates = false;
    locked = false;
}

void BattleChannel::setupUi( QWidget * tab ) {
    AbstractChannel::setupUi( tab );
    QSplitter* s = new QSplitter(Qt::Vertical, tab);
    gridLayout->removeWidget(channelTextBrowser);
    QWidget * t = new QWidget;
    battleWindowForm->setupUi( t );
    //battleWindowForm->descriptionLabel->setWordWrap(true);
    s->addWidget(t);
    s->addWidget(channelTextBrowser);
    QSettings* settings = Settings::Instance();
    if(settings->contains("mainwindow/chatsplitter"))
        s->restoreState(settings->value("mainwindow/chatsplitter").toByteArray());
    connect(s, SIGNAL(splitterMoved(int,int)), SLOT(onChatSplitterMoved(int,int)));
    gridLayout->addWidget( s, 0, 0, 1, 1 );
    /*void onReadyStateChanged( bool isReady );
  void onSpecStateChanged( bool isSpec ); // NEW
  void onSideComboBoxChanged( int index ); // NEW*/
    connect(battleWindowForm->specCheckBox, SIGNAL( stateChanged ( int ) ),
            this,SLOT(onSpecCheckBoxChanged(int))); // NEW
    connect( battleWindowForm->readyCheckBox, SIGNAL( stateChanged ( int ) ),
             Users::getCurrentUsers(), SLOT( onReadyStateChanged( int ) ));
    connect(battleWindowForm->specCheckBox, SIGNAL( stateChanged ( int ) ),
            Users::getCurrentUsers(),SLOT(onSpecStateChanged( int ))); // NEW
    connect(battleWindowForm->factionsComboBox, SIGNAL( currentIndexChanged( int)),
            Users::getCurrentUsers(),SLOT(onSideComboBoxChanged( int )));
    connect(battleWindowForm->colorLabel, SIGNAL(linkActivated (const QString &)),
            this, SLOT(onColorClicked()));
    connect(this, SIGNAL( colorChanged(QColor)),
            Users::getCurrentUsers(),SLOT(onColorChanged(QColor)));
    connect(battleWindowForm->teamNoSpinBox, SIGNAL(valueChanged(int)),
            Users::getCurrentUsers(), SLOT(onTeamNumberChanged(int)));
    connect(battleWindowForm->teamAllyNoSpinBox, SIGNAL(valueChanged(int)),
            Users::getCurrentUsers(), SLOT(onAllyTeamNumberChanged(int)));
    connect(Users::getCurrentUsers(), SIGNAL(myStateChanged(User)),
            this, SLOT(onMyStateChanged(User)));
    currentMap = m_battle.mapName;
    requestMapInfo( m_battle.mapName );
    connect(battleWindowForm->overviewPushButton, SIGNAL(clicked()), SLOT(openMapOverview()));
    //fillModOptions();
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
            if ( this->locked != locked ) {
                insertLine( line
                            .arg( "<span style=\"color:blue;\">** %1</span>" )
                            .arg( locked ? tr( "Battle locked." ) : tr( "Battle unlocked." ) ) );
                this->locked = locked;
            }
            if(currentMap != mapName) requestMapInfo( mapName );
            currentMap = mapName;
        }
    }
    else if ( command.name == "SETSCRIPTTAGS" ) {
        command.attributes = command.attributes.join( " " ).split( "\t" );
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
    int sidec = unitSyncLib->getSideNameCount();
    for(int i=0;i<sidec;i++)
    {
        QString sidename = unitSyncLib->sideName(i);
        battleWindowForm->factionsComboBox->addItem(unitSyncLib->getSideIcon(sidename) ,sidename, QVariant(sidename.toUpper()));
    }
}

void BattleChannel::receiveInput( QString input ) {
    if ( executeChannelInput( input ) )
        return;
    //input.replace( "<", "&lt;" );
    //input.replace( ">", "&gt;" );
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
        noMapUpdates = true;
    }
    else {
        ret.name = "SAYBATTLE";
        ret.attributes << input;
    }
    emit sendCommand( ret );
}

/* NEW */
void BattleChannel::onSpecCheckBoxChanged(int state) {
    battleWindowForm->readyCheckBox->setDisabled(state == Qt::Checked);
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
    if(noMapUpdates) return;
    if(loader->isRunning()) {
        loader->cleanup();
        loader = new MapInfoLoader(this);
        connect(loader, SIGNAL(loadCompleted(QString)), SLOT(updateMapInfo(QString)));
    }
    loader->setMap(mapName);
    battleWindowForm->minimapWidget->setErrorMessage("Loading " + mapName + "...");
    battleWindowForm->heightmapWidget->setErrorMessage("Loading " + mapName + "...");
    battleWindowForm->metalmapWidget->setErrorMessage("Loading " + mapName + "...");
    loader->start();
}

void BattleChannel::updateMapInfo( QString mapName ) {
    if(noMapUpdates) return;
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
        //battleWindowForm->authorLabel->setText(loader->mapinfo.author);
        //battleWindowForm->descriptionLabel->setText();
        battleWindowForm->minimapWidget->setImage(loader->minimap);
        battleWindowForm->heightmapWidget->setImage(loader->heightmap);
        battleWindowForm->metalmapWidget->setImage(loader->metalmap);
        mapOverviewDialog->setSource(mapName, loader->mapinfo.description, loader->minimap, loader->rawHeightmap);
    }
}

void BattleChannel::openMapOverview() {
    mapOverviewDialog->exec();
}

void BattleChannel::onChatSplitterMoved ( int /*pos*/, int /*index*/ ) {
    QSplitter* splitter = qobject_cast<QSplitter*>(sender());
    if(!splitter) return;
    Settings::Instance()->setValue("mainwindow/chatsplitter", splitter->saveState());
}

void BattleChannel::onBattleSplitterMoved ( int /*pos*/, int /*index*/ ) {

}

void BattleChannel::onColorClicked() {
    QColor newColor = QColorDialog::getColor (currentcolor, 0, "Select color for group");
    currentcolor = newColor;
    battleWindowForm->colorLabel->setText(QString("<a href=\"#\"><span style=\" text-decoration: underline; color:%1;\">%1</span></a>").arg(newColor.name()));
    emit colorChanged(newColor);
}

void BattleChannel::onMyStateChanged(User u) {
    if(noMapUpdates) return;
    battleWindowForm->readyCheckBox->blockSignals(true);
    battleWindowForm->specCheckBox->blockSignals(true);
    battleWindowForm->factionsComboBox->blockSignals(true);
    battleWindowForm->teamNoSpinBox->blockSignals(true);
    battleWindowForm->teamAllyNoSpinBox->blockSignals(true);
    battleWindowForm->colorLabel->blockSignals(true);

    battleWindowForm->readyCheckBox->setChecked(u.battleState.isReady());
    battleWindowForm->specCheckBox->setChecked(!u.battleState.isPlayer());
    battleWindowForm->factionsComboBox->setCurrentIndex(u.battleState.getSide());
    battleWindowForm->teamNoSpinBox->setValue(u.battleState.getTeamNo()+1);
    battleWindowForm->teamAllyNoSpinBox->setValue(u.battleState.getAllyTeamNo()+1);
    battleWindowForm->colorLabel->setText(QString("<a href=\"#\"><span style=\" text-decoration: underline; color:%1;\">%1</span></a>").arg(u.m_color.name()));
    currentcolor = u.m_color;

    battleWindowForm->readyCheckBox->blockSignals(false);
    battleWindowForm->specCheckBox->blockSignals(false);
    battleWindowForm->factionsComboBox->blockSignals(false);
    battleWindowForm->teamNoSpinBox->blockSignals(false);
    battleWindowForm->teamAllyNoSpinBox->blockSignals(false);
    battleWindowForm->colorLabel->blockSignals(false);
}

