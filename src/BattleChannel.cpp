// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "BattleChannel.h"
#include "DownloadsModel.h"
#include <QSplitter>
#include <QInputDialog>

#define BLOCK_UI_SIGNALS battleWindowForm->startPositionComboBox->blockSignals(true); \
battleWindowForm->gameEndComboBox->blockSignals(true); \
        battleWindowForm->limitDGunCheckBox->blockSignals(true); \
        battleWindowForm->diminishingMetalMakersCheckBox->blockSignals(true); \
        battleWindowForm->ghostedBuildingsCheckBox->blockSignals(true); \
        battleWindowForm->undeformableMapSpeedCheckBox->blockSignals(true); \
        battleWindowForm->readyCheckBox->blockSignals(true); \
        battleWindowForm->specCheckBox->blockSignals(true); \
        battleWindowForm->factionsComboBox->blockSignals(true); \
        battleWindowForm->teamNoSpinBox->blockSignals(true); \
        battleWindowForm->teamAllyNoSpinBox->blockSignals(true); \
        battleWindowForm->colorToolButton->blockSignals(true); \
        battleWindowForm->gameEndComboBox->blockSignals(true); \
        battleWindowForm->startPositionComboBox->blockSignals(true);


#define UNBLOCK_UI_SIGNALS battleWindowForm->startPositionComboBox->blockSignals(false); \
battleWindowForm->gameEndComboBox->blockSignals(false); \
        battleWindowForm->limitDGunCheckBox->blockSignals(false); \
        battleWindowForm->diminishingMetalMakersCheckBox->blockSignals(false); \
        battleWindowForm->ghostedBuildingsCheckBox->blockSignals(false); \
        battleWindowForm->undeformableMapSpeedCheckBox->blockSignals(false); \
        battleWindowForm->readyCheckBox->blockSignals(false); \
        battleWindowForm->specCheckBox->blockSignals(false); \
        battleWindowForm->factionsComboBox->blockSignals(false); \
        battleWindowForm->teamNoSpinBox->blockSignals(false); \
        battleWindowForm->teamAllyNoSpinBox->blockSignals(false); \
        battleWindowForm->colorToolButton->blockSignals(false); \
        battleWindowForm->gameEndComboBox->blockSignals(false); \
        battleWindowForm->startPositionComboBox->blockSignals(false);

BattleChannel::BattleChannel( QString id, Battles* battles, QObject * parent ) : AbstractChannel( id, parent ) {
    this->battles = battles;
    m_battle = battles->battleManager->getBattle( id.toInt() );
    //setObjectName(m_battle.founder);
    battleWindowForm = new Ui::BattleWindowForm();
    activeIcon = QIcon( ":/icons/battle.xpm" );
    mapOverviewDialog = new MapOverviewDialog();
    loader = new MapInfoLoader(this);
    connect(loader, SIGNAL(loadCompleted(QString)), SLOT(updateMapInfo(QString)));
    connect(battles, SIGNAL(addStartRect(int,QRect)), SLOT(onAddStartRect(int,QRect)));
    noMapUpdates = false;
    locked = false;
    wasKicked = false;
    notify = Notification::getInstance();
}

BattleChannel::~BattleChannel() {
    delete battleWindowForm;
    delete mapOverviewDialog;
}

void BattleChannel::setupUi( QWidget * tab ) {
    AbstractChannel::setupUi( tab );
    QSplitter* s = new QSplitter(Qt::Vertical, tab);
    gridLayout->removeWidget(channelTextBrowser);
    battleWindowForm_ui = new QWidget;
    battleWindowForm->setupUi( battleWindowForm_ui );
    s->addWidget(battleWindowForm_ui);
    s->addWidget(channelTextBrowser);
    QSettings* settings = Settings::Instance();
    if (settings->contains("mainwindow/chatsplitter"))
        s->restoreState(settings->value("mainwindow/chatsplitter").toByteArray());
    connect(s, SIGNAL(splitterMoved(int,int)), SLOT(onChatSplitterMoved(int,int)));
    gridLayout->addWidget( s, 0, 0, 1, 1 );
    connect(battleWindowForm->specCheckBox, SIGNAL( stateChanged ( int ) ),
            this,SLOT(onSpecCheckBoxChanged(int))); // NEW
    connect( battleWindowForm->readyCheckBox, SIGNAL( stateChanged ( int ) ),
             Users::getCurrentUsers(), SLOT( onReadyStateChanged( int ) ));
    connect(battleWindowForm->specCheckBox, SIGNAL( stateChanged ( int ) ),
            Users::getCurrentUsers(),SLOT(onSpecStateChanged( int ))); // NEW
    connect(battleWindowForm->factionsComboBox, SIGNAL( activated( int)),
            this,SLOT(onSideComboBoxChanged( int )));
    connect(battleWindowForm->factionsComboBox, SIGNAL( activated( int)),
            Users::getCurrentUsers(),SLOT(onSideComboBoxChanged( int )));
    connect(battleWindowForm->colorToolButton, SIGNAL(clicked()),
            this, SLOT(onColorClicked()));
    connect(this, SIGNAL( colorChanged(QColor)),
            Users::getCurrentUsers(),SLOT(onColorChanged(QColor)));
    connect(battleWindowForm->teamNoSpinBox, SIGNAL(valueChanged(int)),
            Users::getCurrentUsers(), SLOT(onTeamNumberChanged(int)));
    connect(battleWindowForm->teamAllyNoSpinBox, SIGNAL(valueChanged(int)),
            Users::getCurrentUsers(), SLOT(onAllyTeamNumberChanged(int)));
    connect(Users::getCurrentUsers(), SIGNAL(myStateChanged(User)),
            this, SLOT(onMyStateChanged(User)));
    connect(battleWindowForm->teamAllyNoSpinBox, SIGNAL(valueChanged(int)),
            battleWindowForm->minimapWidget, SLOT(setMyAllyTeam(int)));
    connect(battleWindowForm->teamAllyNoSpinBox, SIGNAL(valueChanged(int)),
            battleWindowForm->heightmapWidget, SLOT(setMyAllyTeam(int)));
    connect(battleWindowForm->teamAllyNoSpinBox, SIGNAL(valueChanged(int)),
            battleWindowForm->metalmapWidget, SLOT(setMyAllyTeam(int)));
    connect(battleWindowForm->teamAllyNoSpinBox, SIGNAL(valueChanged(int)),
            mapOverviewDialog, SLOT(setMyAllyTeam(int)));
    connect(battleWindowForm->undeformableMapSpeedCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(onUndeformableMapSpeedCheckBoxToggled(bool)));
    connect(battleWindowForm->diminishingMetalMakersCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(onDiminishingMetalMakersCheckBoxToggled(bool)));
    connect(battleWindowForm->ghostedBuildingsCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(onGhostedBuildingsCheckBoxToggled(bool)));
    connect(battleWindowForm->limitDGunCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(onLimitDGunCheckBoxToggled(bool)));
    connect(battleWindowForm->gameEndComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onGameEndComboBoxChanged(int)));
    connect(battleWindowForm->startPositionComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onStartPositionComboBoxChanged(int)));
    connect(battleWindowForm->modOptions, SIGNAL(anchorClicked(QUrl)),
            this, SLOT(onModOptionsAnchorClicked(QUrl)));
    connect(battles, SIGNAL(addStartRect(int,QRect)), SLOT(onAddStartRect(int,QRect)));
    connect(battles, SIGNAL(removeStartRect(int)), SLOT(onRemoveStartRect(int)));
    connect(battleWindowForm->overviewPushButton, SIGNAL(clicked()), SLOT(openMapOverview()));

    currentMap = m_battle.mapName;
    requestMapInfo( m_battle.mapName );
    fillSides();
    battleWindowForm->factionsComboBox->setCurrentIndex(settings->value("sidesPreferences/" + m_battle.modName, 0).toInt());
}

void BattleChannel::receiveCommand( Command command ) {
    //   qDebug() << command.toQString();
    if(wasKicked) return;
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
                    .arg( processInput(command.attributes.join( " " ))));
    }
    if ( command.name == "RING" ) {
        QString userName = command.attributes.takeFirst();
        emit playSample( RingSample );
        //emit newTrayMessage( QString("RING!? Where are you!? Get up! %1!").arg(userName) );
        QString title = QString(tr("Ring!"));
        QString message = QString(tr("%1 wants attention!")).arg(userName);
        notify->showMessage(title, message);
    } else if ( command.name == "SAIDBATTLEEX" ) {
        QString userName = command.attributes.takeFirst();
        insertLine( flag( userName ) + line
                    .arg( "<span style=\"color:magenta;\">* %1 %2</span>" )
                    .arg( userName )
                    .arg( processInput(command.attributes.join( " " ), false)));
    } else if ( command.name == "FORCEQUITBATTLE" ) {
        QMessageBox::critical(NULL, tr("Kicked from battle"), tr("You have been kicked from the battle!"));
        insertLine(tr("You have been kicked from the battle!\n"));
        battleWindowForm_ui->setEnabled(false);
        wasKicked = true;
    } else if ( command.name == "JOINEDBATTLE" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            QString userName = command.attributes.takeFirst();
            insertLine( flag( userName ) + line
                        .arg( "<span style=\"color:darkgreen;\">** %1</span>" )
                        .arg( tr( "%2 joined the battle." ) )
                        .arg( userName ) );
        }
    } else if ( command.name == "LEFTBATTLE" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            QString userName = command.attributes.takeFirst();
            insertLine( flag( userName ) + line
                        .arg( "<span style=\"color:darkkhaki;\">** %1</span>" )
                        .arg( tr( "%1 left the battle." ) )
                        .arg( userName ) );
        }
    } else if ( command.name == "JOINBATTLE" ) {} else if ( command.name == "CLIENTMSG" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            insertLine( line
                        .arg( "<span style=\"color:blue;\">** CLIENTMSG ** %1</span>" )
                        .arg( command.attributes.join( " " ) ) );
        }
    } else if ( command.name == "BATTLECLOSED" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            //nothing to do yet
        }
    } else if ( command.name == "UPDATEBATTLEINFO" ) {
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
            if (currentMap != mapName) requestMapInfo( mapName );
            currentMap = mapName;
        }
    } else if ( command.name == "SETSCRIPTTAGS" ) {
        command.attributes = command.attributes.join( " " ).split( "\t" );
        QRegExp re_modoption("game/modoptions/(.*)=(.*)");
        QRegExp re_game("game/(.*)=(.*)");
        foreach( QString s, command.attributes ) {
            if(re_modoption.exactMatch(s)) {
                QString key = re_modoption.cap(1);
                float val  = re_modoption.cap(2).toFloat();
                BLOCK_UI_SIGNALS;
                if ( key == "gamemode" ) {
                    int ival = qMax( qMin( (int)val, 2 ), 0 );
                    battleWindowForm->gameEndComboBox->setCurrentIndex( ival );
                } else if ( key == "limitdgun" )
                    battleWindowForm->limitDGunCheckBox->setChecked( val > 0 );
                else if ( key == "diminishingmms" )
                    battleWindowForm->diminishingMetalMakersCheckBox->setChecked( val > 0 );
                else if ( key == "ghostedbuildings" )
                    battleWindowForm->ghostedBuildingsCheckBox->setChecked( val > 0 );
                else if ( key == "disablemapdamage" )
                    battleWindowForm->undeformableMapSpeedCheckBox->setChecked( val > 0 );
                m_battle.options[key] = val;
                UNBLOCK_UI_SIGNALS;
            } else if ( re_game.exactMatch(s) ) {
                QString key = re_game.cap(1);
                int val  = re_game.cap(2).toInt();
                if(key == "startpostype") {
                    val = qMax( qMin( val, 3 ), 0 );
                    BLOCK_UI_SIGNALS
                            battleWindowForm->startPositionComboBox->setCurrentIndex( val );
                    UNBLOCK_UI_SIGNALS
                        }
            }
        }
        fillModOptions();
    }
}

void BattleChannel::fillSides() {
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    if (!unitSyncLib->setCurrentMod(m_battle.modName)) {
        return;
    }
    int sidec = unitSyncLib->getSideNameCount();

    for (int i=0;i<sidec;i++) {
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
    } else if ( "/slap" == firstWord ) {
        ret.name = "SAYBATTLEEX";
        ret.attributes << QString( "slaps %1 around a bit with a large trout" ).arg( inputList.join( " " ) );
    } else if ( QString( "/leave" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
        ret.name = "LEAVEBATTLE";
        disconnect(battles, SIGNAL(addStartRect(int,QRect)), this, SLOT(onAddStartRect(int,QRect)));
        disconnect(battles, SIGNAL(removeStartRect(int)), this, SLOT(onRemoveStartRect(int)));
        Settings::Instance()->setValue("mainwindow/chatsplitter", splitterState);
        noMapUpdates = true;
        if(wasKicked) return;
    } else {
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
    if (!unitSyncLib->setCurrentMod(m_battle.modName)) {
        battleWindowForm->modOptions->setHtml(QString("<font size=\"16\" color=\"red\">") + tr("Please, download %1").arg(m_battle.modName) + "</font>");
        if(QMessageBox::information(0,
                                    tr("Missing content"),
                                    tr("You don't have the mod or you have a broken version.\nDo you want to download it?"),
                                    QMessageBox::Yes,
                                    QMessageBox::No) == QMessageBox::Yes) {
            DownloadsModel::getInstance()->startModDownload(m_battle.modName);
        }
        return;
    }
    QString buffer;
    int num_options = unitSyncLib->getModOptionCount();
    buffer.append("<table cellpadding=\"4\">");
    for (int i = 0; i < num_options; i++) {
        if (unitSyncLib->getOptionType(i) == OT_SECTION && i != num_options - 1 && unitSyncLib->getOptionType(i+1) == OT_SECTION) continue;
        if (unitSyncLib->getOptionType(i) == OT_SECTION) {
            buffer.append("</table>");
            buffer.append("<h3>"+unitSyncLib->getOptionName(i)+"</h3>");
            buffer.append("<table cellpadding=\"4\">");
            continue;
        }
        if (unitSyncLib->isGameOption(i)) continue;
        buffer.append("<tr>");
        buffer.append("<td>");
        buffer.append("<a href=\"");
        buffer.append(QString::number(i));
        buffer.append("\">");
        buffer.append(unitSyncLib->getOptionName(i));
        buffer.append("</a>");
        buffer.append("</td>");
        buffer.append("<td>");
        bool nondefault = m_battle.options.contains(unitSyncLib->getOptionKey(i));
        QString bold = "<b>%1</b>", colored = "<font color=\"%1\">%2</font>";
        switch (unitSyncLib->getOptionType(i)) {
        case OT_UNDEFINED:
            buffer.append("Undefined");
            break;
        case OT_BOOLEAN:
            nondefault = nondefault && m_battle.options[unitSyncLib->getOptionKey(i)].toFloat() != unitSyncLib->getOptionBoolDef(i);
            if (nondefault)
                buffer.append(m_battle.options[unitSyncLib->getOptionKey(i)].toFloat() ? bold.arg(colored.arg("green").arg(tr("Yes"))) : bold.arg(colored.arg("red").arg(tr("No"))) );
            else
                buffer.append(unitSyncLib->getOptionBoolDef(i) ? colored.arg("green").arg(tr("Yes")) : colored.arg("red").arg(tr("No")) );
            break;
        case OT_LIST:
            nondefault = nondefault && m_battle.options[unitSyncLib->getOptionKey(i)].toString() != unitSyncLib->getOptionListDef(i);
            if (nondefault)
                buffer.append("<b>"+m_battle.options[unitSyncLib->getOptionKey(i)].toString()+"</b>");
            else
                buffer.append(unitSyncLib->getOptionListDef(i));
            break;
        case OT_FLOAT:
            nondefault = nondefault && m_battle.options[unitSyncLib->getOptionKey(i)].toFloat() != unitSyncLib->getOptionNumberDef(i);
            if (nondefault)
                buffer.append("<b>"+m_battle.options[unitSyncLib->getOptionKey(i)].toString()+"</b>");
            else
                buffer.append(QString::number(unitSyncLib->getOptionNumberDef(i)));
            break;
        case OT_STRING:
            nondefault = nondefault && m_battle.options[unitSyncLib->getOptionKey(i)].toString() != unitSyncLib->getOptionStringDef(i);
            if (nondefault)
                buffer.append("<b>"+m_battle.options[unitSyncLib->getOptionKey(i)].toString()+"</b>");
            else
                buffer.append(unitSyncLib->getOptionStringDef(i));
            break;
        case OT_SECTION:
            break;
        }
        buffer.append("</td>");
        buffer.append("</tr>");
    }
    buffer.append("</table>");
    int scroll = battleWindowForm->modOptions->verticalScrollBar()->value();
    battleWindowForm->modOptions->setHtml(buffer);
    battleWindowForm->modOptions->verticalScrollBar()->setValue(scroll);
}

void BattleChannel::requestMapInfo( QString mapName ) {
    if (noMapUpdates) return;
    if(!loader) {
        loader = new MapInfoLoader(this);
        connect(loader, SIGNAL(loadCompleted(QString)), SLOT(updateMapInfo(QString)));
    }
    if (loader->isRunning()) {
        loader->cleanup();
        loader = new MapInfoLoader(this);
        connect(loader, SIGNAL(loadCompleted(QString)), SLOT(updateMapInfo(QString)));
    }
    loader->setMap(mapName);
    QString lodingMapError = tr("Loading %1...").arg(mapName);
    battleWindowForm->minimapWidget->setErrorMessage(lodingMapError);
    battleWindowForm->heightmapWidget->setErrorMessage(lodingMapError);
    battleWindowForm->metalmapWidget->setErrorMessage(lodingMapError);
    loader->start();
}

void BattleChannel::updateMapInfo( QString mapName ) {
    if (noMapUpdates) return;
    battleWindowForm->nameLabel->setText(mapName);
    if (!loader->mapPresent) {
        QString mapNotFoundError = tr("Map %1 not found").arg(mapName);
        battleWindowForm->minimapWidget->setErrorMessage(mapNotFoundError);
        battleWindowForm->heightmapWidget->setErrorMessage(mapNotFoundError);
        battleWindowForm->metalmapWidget->setErrorMessage(mapNotFoundError);
        if(QMessageBox::information(0,
                                    tr("Missing content"),
                                    tr("You don't have the map or you have a broken version.\nDo you want to download it?"),
                                    QMessageBox::Yes,
                                    QMessageBox::No) == QMessageBox::Yes) {
            DownloadsModel::getInstance()->startMapDownload(mapName);
        }
    } else {
        battleWindowForm->minimapWidget->setErrorMessage(QString::null);
        battleWindowForm->heightmapWidget->setErrorMessage(QString::null);
        battleWindowForm->metalmapWidget->setErrorMessage(QString::null);
        battleWindowForm->sizeLabel->setText(QString("%1x%2").arg(loader->mapinfo.width/512).arg(loader->mapinfo.height/512));
        battleWindowForm->windspeedLabel->setText(QString("%1-%2").arg(loader->mapinfo.minWind).arg(loader->mapinfo.maxWind));
        battleWindowForm->tidalLabel->setText(QString::number(loader->mapinfo.tidalStrength));
        //battleWindowForm->authorLabel->setText(loader->mapinfo.author);
        //battleWindowForm->descriptionLabel->setText();
        battleWindowForm->minimapWidget->setImage(loader->minimap);
        battleWindowForm->heightmapWidget->setImage(loader->heightmap);
        battleWindowForm->metalmapWidget->setImage(loader->metalmap);
        mapOverviewDialog->setSource(mapName, loader->mapinfo.description, loader->minimap, loader->rawHeightmap);
        loader->cleanup();
        loader = 0;
    }
}

void BattleChannel::openMapOverview() {
    mapOverviewDialog->exec();
}

void BattleChannel::onChatSplitterMoved ( int /*pos*/, int /*index*/ ) {
    QSplitter* splitter = qobject_cast<QSplitter*>(sender());
    if (!splitter) return;
    splitterState = splitter->saveState();

}

void BattleChannel::onBattleSplitterMoved ( int /*pos*/, int /*index*/ ) {

}

void BattleChannel::onColorClicked() {
    QColor newColor = QColorDialog::getColor (currentcolor, 0, tr("Select color for group"));
    if (!newColor.isValid()) return;
    currentcolor = newColor;
    QPixmap color(16,16);
    color.fill(newColor);
    battleWindowForm->colorToolButton->setIcon(color);
    emit colorChanged(newColor);
}

void BattleChannel::onMyStateChanged(User u) {
    if (noMapUpdates) return;
    BLOCK_UI_SIGNALS;

    battleWindowForm->readyCheckBox->setChecked(u.battleState.isReady());
    battleWindowForm->specCheckBox->setChecked(!u.battleState.isPlayer());
    battleWindowForm->factionsComboBox->setCurrentIndex(u.battleState.getSide());
    battleWindowForm->teamNoSpinBox->setValue(u.battleState.getTeamNo()+1);
    quint8 ally = u.battleState.getAllyTeamNo()+1;
    battleWindowForm->teamAllyNoSpinBox->setValue(ally);
    battleWindowForm->minimapWidget->setMyAllyTeam(ally);
    battleWindowForm->heightmapWidget->setMyAllyTeam(ally);
    battleWindowForm->metalmapWidget->setMyAllyTeam(ally);
    mapOverviewDialog->setMyAllyTeam(ally);
    QPixmap color(16,16);
    color.fill(u.m_color);
    battleWindowForm->colorToolButton->setIcon(color);
    currentcolor = u.m_color;

    UNBLOCK_UI_SIGNALS;
}

void BattleChannel::onAddStartRect(int ally, QRect r) {
    battleWindowForm->minimapWidget->addStartRect(ally, r);
    battleWindowForm->heightmapWidget->addStartRect(ally, r);
    battleWindowForm->metalmapWidget->addStartRect(ally, r);
    mapOverviewDialog->addStartRect(ally, r);
}

void BattleChannel::onRemoveStartRect(int ally) {
    battleWindowForm->minimapWidget->removeStartRect(ally);
    battleWindowForm->heightmapWidget->removeStartRect(ally);
    battleWindowForm->metalmapWidget->removeStartRect(ally);
    mapOverviewDialog->removeStartRect(ally);
}

QString BattleChannel::getTabTitle() {
    return m_battle.founder;
}

void BattleChannel::refreshMapAndModOptions() {
    fillModOptions();
    fillSides();
    requestMapInfo(m_battle.mapName);
}

void BattleChannel::onUndeformableMapSpeedCheckBoxToggled(bool checked) {
    battleWindowForm->undeformableMapSpeedCheckBox->blockSignals(true);
    battleWindowForm->undeformableMapSpeedCheckBox->setChecked(!checked);
    battleWindowForm->undeformableMapSpeedCheckBox->blockSignals(false);
    receiveInput(QString("!bset disablemapdamage %1").arg( checked ? "1" : "0" ));
}

void BattleChannel::onDiminishingMetalMakersCheckBoxToggled(bool checked) {
    battleWindowForm->diminishingMetalMakersCheckBox->blockSignals(true);
    battleWindowForm->diminishingMetalMakersCheckBox->setChecked(!checked);
    battleWindowForm->diminishingMetalMakersCheckBox->blockSignals(false);
    receiveInput(QString("!bset diminishingmms %1").arg( checked ? "1" : "0" ));
}

void BattleChannel::onGhostedBuildingsCheckBoxToggled(bool checked) {
    battleWindowForm->ghostedBuildingsCheckBox->blockSignals(true);
    battleWindowForm->ghostedBuildingsCheckBox->setChecked(!checked);
    battleWindowForm->ghostedBuildingsCheckBox->blockSignals(false);
    receiveInput(QString("!bset ghostedbuildings %1").arg( checked ? "1" : "0" ));
}

void BattleChannel::onLimitDGunCheckBoxToggled(bool checked) {
    battleWindowForm->limitDGunCheckBox->blockSignals(true);
    battleWindowForm->limitDGunCheckBox->setChecked(!checked);
    battleWindowForm->limitDGunCheckBox->blockSignals(false);
    receiveInput(QString("!bset limitdgun %1").arg( checked ? "1" : "0" ));
}

void BattleChannel::onGameEndComboBoxChanged(int index) {
    static int lastIndex = -1;
    if(lastIndex == -1) lastIndex = index;
    battleWindowForm->gameEndComboBox->blockSignals(true);
    battleWindowForm->gameEndComboBox->setCurrentIndex(lastIndex);
    battleWindowForm->gameEndComboBox->blockSignals(false);
    receiveInput(QString("!bset gamemode %1").arg( index ));
}

void BattleChannel::onStartPositionComboBoxChanged(int index) {
    static int lastIndex = -1;
    if(lastIndex == -1) lastIndex = index;
    battleWindowForm->startPositionComboBox->blockSignals(true);
    battleWindowForm->startPositionComboBox->setCurrentIndex(lastIndex);
    battleWindowForm->startPositionComboBox->blockSignals(false);
    receiveInput(QString("!bset startpostype %1").arg( index ));
}

void BattleChannel::onModOptionsAnchorClicked(QUrl url) {
    float min, max, step, val;
    bool ok = false;
    int i = url.toString().toInt();
    int length;
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    QString key = unitSyncLib->getOptionKey(i);
    QString item;
    switch (unitSyncLib->getOptionType(i)) {
    case OT_BOOLEAN:
        if(m_battle.options[key].toString() == "0") {
            receiveInput(QString("!bset %1 1").arg(key));
        } else {
            receiveInput(QString("!bset %1 0").arg(key));
        }
        break;
    case OT_LIST:
        item = QInputDialog::getItem(NULL,
                                     tr("Select value"),
                                     tr("Select value for %1").arg(unitSyncLib->getOptionName(i)),
                                     unitSyncLib->getOptionListItems(i),
                                     unitSyncLib->getOptionListItems(i).indexOf(m_battle.options[key].toString()),
                                     true,
                                     &ok);
        if(ok)
            receiveInput(QString("!bset %1 %2").arg(key).arg(item));
        break;
    case OT_FLOAT:
        min = unitSyncLib->getOptionNumberMin(i);
        max = unitSyncLib->getOptionNumberMax(i);
        step = unitSyncLib->getOptionNumberStep(i);
        if(step == (int)step) {
            val = QInputDialog::getInt(NULL,
                                       tr("Select value"),
                                       tr("Select value for %1").arg(unitSyncLib->getOptionName(i)),
                                       m_battle.options[key].toFloat(),
                                       min,
                                       max,
                                       step,
                                       &ok);
        } else {
            int decimals = qMax( 1, (int) ceil(-log10(fabs(step)))); // log10(0.01)=-2
            val = QInputDialog::getDouble(NULL,
                                          tr("Select value"),
                                          tr("Select value for %1").arg(unitSyncLib->getOptionName(i)),
                                          m_battle.options[key].toFloat(),
                                          min,
                                          max,
                                          decimals, &ok);
            val = qRound( val/step )*step;
        }
        if (ok)
            receiveInput(QString("!bset %1 %2").arg(key).arg(val));
        break;
    case OT_STRING:
        length = unitSyncLib->getOptionStringMaxLen(i);
        do {
            item = QInputDialog::getText(NULL,
                                         tr("Select value"),
                                         tr("Select value for %1").arg(unitSyncLib->getOptionName(i)),
                                         QLineEdit::Normal,
                                         m_battle.options[key].toString()
                                         );
        } while (item.length() < length);
        receiveInput(QString("!bset %1 %2").arg(key).arg(item));
    case OT_SECTION:
    case OT_UNDEFINED:
        break;
    }
}

bool BattleChannel::isBlocked() const {
    return wasKicked;
}

void BattleChannel::onSideComboBoxChanged(int side) {
    Settings::Instance()->setValue("sidesPreferences/" + m_battle.modName, side);
}
