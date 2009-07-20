#include "SqadsUserListClass.h"

class SqadsUserListClassPropertyIterator : public QScriptClassPropertyIterator {
public:
    SqadsUserListClassPropertyIterator(const QScriptValue &object);
    ~SqadsUserListClassPropertyIterator();

    bool hasNext() const;
    void next();

    bool hasPrevious() const;
    void previous();

    void toFront();
    void toBack();

    QScriptString name() const;
    uint id() const;

private:
    int m_index;
    int m_last;
};

static qint32 toArrayIndex(const QString &str) {
    QByteArray bytes = str.toUtf8();
    char *eptr;
    quint32 pos = strtoul(bytes.constData(), &eptr, 10);
    if ((eptr == bytes.constData() + bytes.size())
        && (QByteArray::number(pos) == bytes)) {
        return pos;
    }
    return -1;
}

SqadsUserListClass::SqadsUserListClass(QScriptEngine *engine) : QObject(engine), QScriptClass(engine) {
    qScriptRegisterMetaType<QList<User> >(engine, toScriptValue, fromScriptValue);

    length = engine->toStringHandle(QLatin1String("length"));

    proto = engine->newQObject(new SqadsUserListPrototype(this),
                               QScriptEngine::QtOwnership,
                               QScriptEngine::SkipMethodsInEnumeration
                               | QScriptEngine::ExcludeSuperClassMethods
                               | QScriptEngine::ExcludeSuperClassProperties);
    QScriptValue global = engine->globalObject();
    proto.setPrototype(global.property("Object").property("prototype"));

    ctor = engine->newFunction(construct);
    ctor.setData(qScriptValueFromValue(engine, this));
}

SqadsUserListClass::~SqadsUserListClass() {
}

QScriptClass::QueryFlags SqadsUserListClass::queryProperty(const QScriptValue &object, const QScriptString &name, QueryFlags flags, uint *id) {
    QList<User>* list = qscriptvalue_cast<QList<User>*>(object.data());
    if (!list)
        return 0;
    if (name == length) {
        return flags;
    } else {
        qint32 pos = toArrayIndex(name);
        if (pos == -1)
            return 0;
        *id = pos;
        if ((flags & HandlesReadAccess) && (pos >= ba->size()))
            flags &= ~HandlesReadAccess;
        return flags;
    }
}

QScriptValue SqadsUserListClass::property(const QScriptValue &object, const QScriptString &name, uint id) {
    QByteArray *ba = qscriptvalue_cast<QByteArray*>(object.data());
    if (!ba)
        return QScriptValue();
    if (name == length) {
        return ba->length();
    } else {
        qint32 pos = id;
        if ((pos < 0) || (pos >= ba->size()))
            return QScriptValue();
        return uint(ba->at(pos)) & 255;
    }
    return QScriptValue();
}

void SqadsUserListClass::setProperty(QScriptValue &object, const QScriptString &name, uint id, const QScriptValue &value) {
    QByteArray *ba = qscriptvalue_cast<QByteArray*>(object.data());
    if (!ba)
        return;
    if (name == length) {
        ba->resize(value.toInt32());
    } else {
        qint32 pos = id;
        if (pos < 0)
            return;
        if (ba->size() <= pos)
            ba->resize(pos + 1);
        (*ba)[pos] = char(value.toInt32());
    }
}

QScriptValue::PropertyFlags SqadsUserListClass::propertyFlags(const QScriptValue &/*object*/, const QScriptString &name, uint /*id*/) {
    if (name == length) {
        return QScriptValue::Undeletable
                | QScriptValue::SkipInEnumeration;
    }
    return QScriptValue::Undeletable;
}

QScriptClassPropertyIterator *SqadsUserListClass::newIterator(const QScriptValue &object) {
    return new SqadsUserListClassPropertyIterator(object);
}

QString SqadsUserListClass::name() const {
    return QLatin1String("ByteArray");
}

QScriptValue SqadsUserListClass::prototype() const {
    return proto;
}

QScriptValue SqadsUserListClass::constructor() {
    return ctor;
}

QScriptValue SqadsUserListClass::newInstance(int size) {
    return newInstance(QByteArray(size, /*ch=*/0));
}

QScriptValue SqadsUserListClass::newInstance(const QByteArray &ba) {
    QScriptValue data = engine()->newVariant(qVariantFromValue(ba));
    return engine()->newObject(this, data);
}

QScriptValue SqadsUserListClass::construct(QScriptContext *ctx, QScriptEngine *) {
    SqadsUserListClass *cls = qscriptvalue_cast<SqadsUserListClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    int size = ctx->argument(0).toInt32();
    return cls->newInstance(size);
}

QScriptValue SqadsUserListClass::toScriptValue(QScriptEngine *eng, const QByteArray &ba) {
    QScriptValue ctor = eng->globalObject().property("ByteArray");
    SqadsUserListClass *cls = qscriptvalue_cast<SqadsUserListClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(qVariantFromValue(ba));
    return cls->newInstance(ba);
}

void SqadsUserListClass::fromScriptValue(const QScriptValue &obj, QByteArray &ba) {
    ba = qscriptvalue_cast<QByteArray>(obj.data());
}

SqadsUserListClassPropertyIterator::SqadsUserListClassPropertyIterator(const QScriptValue &object)
    : QScriptClassPropertyIterator(object) {
    toFront();
}

SqadsUserListClassPropertyIterator::~SqadsUserListClassPropertyIterator() {
}

bool SqadsUserListClassPropertyIterator::hasNext() const {
    QByteArray *ba = qscriptvalue_cast<QByteArray*>(object().data());
    return m_index < ba->size();
}

void SqadsUserListClassPropertyIterator::next() {
    m_last = m_index;
    ++m_index;
}

bool SqadsUserListClassPropertyIterator::hasPrevious() const {
    return (m_index > 0);
}

void SqadsUserListClassPropertyIterator::previous() {
    --m_index;
    m_last = m_index;
}

void SqadsUserListClassPropertyIterator::toFront() {
    m_index = 0;
    m_last = -1;
}

void SqadsUserListClassPropertyIterator::toBack() {
    QByteArray *ba = qscriptvalue_cast<QByteArray*>(object().data());
    m_index = ba->size();
    m_last = -1;
}

QScriptString SqadsUserListClassPropertyIterator::name() const {
    return QScriptString();
}

uint SqadsUserListClassPropertyIterator::id() const {
    return m_last;
}

