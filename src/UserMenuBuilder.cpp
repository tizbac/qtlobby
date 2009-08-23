#include "UserMenuBuilder.h"
#include <QInputDialog>
#include <QColorDialog>

UserMenuBuilder::UserMenuBuilder() {
    m_openPrivateChannelAction = new QAction( tr("Start private chat"), this );
    m_slapAction = new QAction( tr("Slap around"), this );
    m_joinSameBattleAction = new QAction( tr("Join same battle"), this );
    m_ignoreAction = new QAction( tr("Toggle ignore"), this );

    m_userMenu = new QMenu( "userListContextMenu" );
    m_userMenu->addAction( m_openPrivateChannelAction );
    m_userMenu->addAction( m_slapAction );
    m_userMenu->addAction( m_joinSameBattleAction );
    m_userMenu->addAction( m_ignoreAction );
    m_userMenu->addSeparator();
    m_groupsMenu = new QMenu(tr("Add to group"));
    m_removeFromGroupAction = new QAction(tr("Remove from group"), m_userMenu);
    m_clanGroupsMenu = new QMenu(tr("Add clan to group"));
    m_removeClanFromGroupAction = new QAction(tr("Remove clan from group"), m_userMenu);
    m_forceMenu   = new QMenu(tr("Force"));
    m_forceTeam   = m_forceMenu->addAction(tr("Team"));
    m_forceAlly   = m_forceMenu->addAction(tr("Ally"));
    m_forceColor  = m_forceMenu->addAction(tr("Color"));
    m_forceSpec   = m_forceMenu->addAction(tr("Spec"));
    m_kick        = new QAction(tr("Kick"), m_userMenu);
    m_groupList   = UserGroupList::getInstance();
    m_clanRegexp.setPattern(".*(\\[.*\\]).*");
    m_battle = false;
}

QMenu* UserMenuBuilder::buildMenu( const User& u ) {
    QColor c = m_groupList->getUserColor(u.name);
    if (m_clanRegexp.indexIn(u.name) >= 0) {
        m_clan = m_clanRegexp.capturedTexts().at(1);
    } else
        m_clan = QString::null;
    m_userMenu->removeAction(m_groupsMenu->menuAction());
    m_userMenu->removeAction(m_removeFromGroupAction);
    m_userMenu->removeAction(m_clanGroupsMenu->menuAction());
    m_userMenu->removeAction(m_removeClanFromGroupAction);

    if (c.isValid()) {
        m_userMenu->addAction(m_removeFromGroupAction);
        if (!m_clan.isEmpty())
            m_userMenu->addAction(m_removeClanFromGroupAction);
    } else {
        m_groupsMenu->clear();
        QAction* newGroup = m_groupsMenu->addAction(tr("New Group"));
        newGroup->setObjectName("create_new_group");
        m_groupsMenu->addSeparator();
        if (!m_clan.isEmpty()) {
            m_clanGroupsMenu->clear();
            m_clanGroupsMenu->addAction(newGroup);
            m_clanGroupsMenu->addSeparator();
        }
        QStringList groups = m_groupList->getGroupNames();
        for (int i = 0; i < groups.size(); i++) {
            QAction* a = m_groupsMenu->addAction(groups.at(i));
            a->setObjectName("add_to_group");
            if (!m_clan.isEmpty()) {
                a = m_clanGroupsMenu->addAction(groups.at(i));
                a->setObjectName("add_clan_to_group");
            }
        }
        m_userMenu->addMenu(m_groupsMenu);
        if (!m_clan.isEmpty())
            m_userMenu->addMenu(m_clanGroupsMenu);
    }
    if(m_battle) {
        m_userMenu->addSeparator();
        m_userMenu->addMenu(m_forceMenu);
        m_userMenu->addAction(m_kick);
    }
    m_user = u;
    return m_userMenu;
}

void UserMenuBuilder::processMenuAction ( QAction *action ) {
    if ( action ) {
        if ( action == m_openPrivateChannelAction ) {
            emit sendInput( m_user.name.prepend( "/query " ) );
        } else if ( action == m_slapAction ) {
            emit sendInput( m_user.name.prepend( "/slap " ) );
        } else if ( action == m_joinSameBattleAction ) {
            joinSameBattle( m_user );
        } else if ( action == m_ignoreAction ) {
            toggleIgnoreUser( m_user );
        } else if ( action == m_removeFromGroupAction ) {
            removeUserFromGroup(m_user.name);
        } else if ( action == m_removeClanFromGroupAction ) {
            removeUserFromGroup(m_clan);
        } else if ( action->objectName() == "add_to_group") {
            addUserToGroup(m_user.name, action->text());
        } else if ( action->objectName() == "add_clan_to_group") {
            addUserToGroup(m_clan, action->text());
        } else if (action->objectName() == "create_new_group") {
            emit openGroupsDialog();
        } else if ( action == m_forceTeam ) {
            int newTeam = QInputDialog::getInt(0,
                                               tr("Select team"),
                                               tr("Select a team to force player to"),
                                               m_user.battleState.getTeamNo()+1, 1, 16, 1);
            emit sendCommand(Command("SAYBATTLE !force " + m_user.name + " team " + QString::number(newTeam)));
        } else if ( action == m_forceAlly ) {
            int newAlly = QInputDialog::getInt(0,
                                               tr("Select ally team"),
                                               tr("Select an ally team to force player to"),
                                               m_user.battleState.getAllyTeamNo()+1, 1, 16, 1);
            emit sendCommand(Command("SAYBATTLE !force " + m_user.name + " ally " + QString::number(newAlly)));
        } else if ( action == m_forceColor ) {
            QColor newColor = QColorDialog::getColor(m_user.m_color, 0, tr("Select user color"));
            emit sendCommand(Command("SAYBATTLE !force " + m_user.name + " color " + newColor.name()));
        } else if ( action == m_forceSpec ) {
            emit sendCommand(Command("SAYBATTLE !force " + m_user.name + " spec"));
        } else if ( action == m_kick ) {
            emit sendCommand(Command("SAYBATTLE !kick " + m_user.name));
        }
    }
}

void UserMenuBuilder::addUserToGroup(QString user, QString group) {
    UserGroup* g = m_groupList->findGroup(group);
    if (!g->members.contains(user)) {
        g->members.append(user);
        m_groupList->updateMappings();
        emit invalidateModel();
    }
    m_groupList->save();
}

void UserMenuBuilder::removeUserFromGroup(QString user) {
    for (int i = 0; i < m_groupList->size(); i++) {
        UserGroup* g = m_groupList->at(i);
        int index = g->members.indexOf(user);
        if (index >= 0)
            g->members.removeAt(index);
    }
    m_groupList->updateMappings();
    emit invalidateModel();
    m_groupList->save();
}

void UserMenuBuilder::toggleIgnoreUser( User u ) {
    m_groupList->toggleIgnore( u.name );
    emit invalidateModel();
    m_groupList->save();
}
