#ifndef USERMENUBUILDER_H
#define USERMENUBUILDER_H

#include <QObject>
#include <QMenu>
#include <QAction>
#include <QRegExp>
#include "Singleton.h"
#include "User.h"
#include "UserGroup.h"
#include "Command.h"

class UserMenuBuilder : public QObject, public Singleton<UserMenuBuilder> {
    Q_OBJECT
    friend class Singleton<UserMenuBuilder>;
public:
    QMenu* buildMenu( const User& u );
    void processMenuAction(QAction *action);
    void setBattle(bool b);
signals:
    void joinSameBattle(User u);
    void sendInput(QString input);
    void invalidateModel();
    void openGroupsDialog();
    void sendCommand(Command c);

private:
    UserMenuBuilder();
    void addUserToGroup(QString user, QString group);
    void removeUserFromGroup(QString user);
    void toggleIgnoreUser( User u );

    UserGroupList* m_groupList;
    bool m_battle;
    QMenu* m_userMenu;
    QMenu* m_groupsMenu;
    QAction* m_removeFromGroupAction;
    QMenu* m_clanGroupsMenu;
    QAction* m_removeClanFromGroupAction;
    QAction* m_openPrivateChannelAction;
    QAction* m_slapAction;
    QAction* m_joinSameBattleAction;
    QAction* m_ignoreAction;
    QMenu* m_forceMenu;
    QAction* m_forceTeam;
    QAction* m_forceAlly;
    QAction* m_forceColor;
    QAction* m_forceSpec;
    QAction* m_kick;
    QRegExp m_clanRegexp;
    User m_user;
    QString m_clan;
};

inline void UserMenuBuilder::setBattle(bool b) {
    m_battle = b;
}

#endif // USERMENUBUILDER_H
