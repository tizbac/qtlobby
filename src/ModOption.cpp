// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "ModOption.h"

ModOption::ModOption(QString str, bool game) {
    m_value = str;
    m_game = game;
}

QString ModOption::toString() const {
    return m_value;
}

void ModOption::fromString(QString str) {
    m_value = str;
}

ModOption& ModOption::operator=(const QString& str) {
    m_value = str;
    return *this;
}

ModOption& ModOption::operator=(int val) {
    m_value = QString::number(val);
    return *this;
}

bool ModOption::isGameOption() const {
    return m_game;
}

void ModOption::setGameOption(bool b) {
    m_game = b;
}

bool ModOption::isModified() const {
    return m_modified;
}

void ModOption::setModified(bool b) {
    m_modified = b;
}

float ModOption::toFloat() const {
    return m_value.toFloat();
}

void ModOption::fromFloat(float number) {
    m_value = QString::number(number);
}
