#ifndef SQADSUSERLISTCLASS_H
#define SQADSUSERLISTCLASS_H

#include <QScriptClass>
#include <QList>
#include "User.h"

class SqadsUserListClass : public QObject, public QScriptClass {
public:
    SqadsUserListClass(QScriptEngine *engine);
    ~SqadsUserListClass();

    QScriptValue constructor();

    QScriptValue newInstance(int size = 0);
    QScriptValue newInstance(const QByteArray &ba);

    QueryFlags queryProperty(const QScriptValue &object,
                             const QScriptString &name,
                             QueryFlags flags, uint *id);

    QScriptValue property(const QScriptValue &object,
                          const QScriptString &name, uint id);

    void setProperty(QScriptValue &object, const QScriptString &name,
                     uint id, const QScriptValue &value);

    QScriptValue::PropertyFlags propertyFlags(
            const QScriptValue &object, const QScriptString &name, uint id);

    QScriptClassPropertyIterator *newIterator(const QScriptValue &object);

    QString name() const;

    QScriptValue prototype() const;

private:
    static QScriptValue construct(QScriptContext *ctx, QScriptEngine *eng);

    static QScriptValue toScriptValue(QScriptEngine *eng, const QByteArray &ba);
    static void fromScriptValue(const QScriptValue &obj, QByteArray &ba);

    QScriptString length;
    QScriptValue proto;
    QScriptValue ctor;
};

#endif // SQADSUSERLISTCLASS_H
