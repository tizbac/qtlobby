#include "ModOptionNumber.h"

ModOptionNumber::ModOptionNumber(QString str, bool game) : ModOption(str, game) {

}

QString ModOptionNumber::toString() {
    return QString::number(m_value);
}

void ModOptionNumber::fromString(QString str) {
    m_value = str.toFloat();
}
