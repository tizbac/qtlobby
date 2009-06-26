#ifndef MODOPTIONNUMBER_H
#define MODOPTIONNUMBER_H

#include "ModOption.h"

class ModOptionNumber : public ModOption {
public:
    ModOptionNumber(QString str = QString(), bool game = false);
    QString toString();
    void fromString(QString str);
private:
    float m_value;
};

#endif // MODOPTIONNUMBER_H
