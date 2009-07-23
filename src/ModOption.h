// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef MODOPTION_H
#define MODOPTION_H

#include <QString>

class ModOption {
public:
    ModOption(QString str = QString(), bool game = false);
    virtual ~ModOption() {}
    virtual QString toString() const;
    virtual void fromString(QString str);
    virtual float toFloat() const;
    virtual void fromFloat(float number);
    virtual ModOption& operator=(const QString& str);
    virtual ModOption& operator=(int val);
    bool isGameOption() const;
    void setGameOption(bool b);
    bool isModified() const;
    void setModified(bool b);
private:
    QString m_value;
    bool m_game;
    bool m_modified;
};

#endif // MODOPTION_H

