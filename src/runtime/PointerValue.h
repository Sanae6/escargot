#ifndef __EscargotPointerValue__
#define __EscargotPointerValue__

namespace Escargot {

class String;
class Object;
class FunctionObject;
class ArrayObject;
class StringObject;
class NumberObject;
class BooleanObject;
class RegExpObject;
class DateObject;
#if ESCARGOT_ENABLE_PROMISE
class PromiseObject;
#endif
class DoubleInSmallValue;
class JSGetterSetter;

class PointerValue : public gc {
public:
    enum Type {
        StringType,
        ObjectType,
        FunctionObjectType,
        ArrayObjectType,
        DoubleInSmallValueType,
        DummyInSmallValueType,
        JSGetterSetterType,
        ExtraDataType
    };

    virtual Type type() = 0;
    virtual bool isString()
    {
        return false;
    }

    virtual bool isObject()
    {
        return false;
    }

    virtual bool isFunctionObject()
    {
        return false;
    }

    virtual bool isArrayObject()
    {
        return false;
    }

    virtual bool isStringObject()
    {
        return false;
    }

    virtual bool isNumberObject()
    {
        return false;
    }

    virtual bool isBooleanObject()
    {
        return false;
    }

    virtual bool isDateObject()
    {
        return false;
    }

    virtual bool isRegExpObject()
    {
        return false;
    }

    virtual bool isErrorObject()
    {
        return false;
    }

    virtual bool isGlobalObject()
    {
        return false;
    }

#if ESCARGOT_ENABLE_PROMISE
    virtual bool isPromiseObject()
    {
        return false;
    }
#endif

#if ESCARGOT_ENABLE_TYPEDARRAY
    virtual bool isTypedArrayObject()
    {
        return false;
    }
#endif

    virtual bool isDoubleInSmallValue()
    {
        return false;
    }

    virtual bool isDummyInSmallValueType()
    {
        return false;
    }

    virtual bool isJSGetterSetter()
    {
        return false;
    }

    String* asString()
    {
        ASSERT(isString());
        return (String*)this;
    }

    Object* asObject()
    {
        ASSERT(isObject());
        return (Object*)this;
    }

    FunctionObject* asFunctionObject()
    {
        ASSERT(isFunctionObject());
        return (FunctionObject*)this;
    }

    StringObject* asStringObject()
    {
        ASSERT(isStringObject());
        return (StringObject*)this;
    }

    NumberObject* asNumberObject()
    {
        ASSERT(isNumberObject());
        return (NumberObject*)this;
    }

    BooleanObject* asBooleanObject()
    {
        ASSERT(isBooleanObject());
        return (BooleanObject*)this;
    }

    RegExpObject* asRegExpObject()
    {
        ASSERT(isRegExpObject());
        return (RegExpObject*)this;
    }

    DateObject* asDateObject()
    {
        ASSERT(isDateObject());
        return (DateObject*)this;
    }

#if ESCARGOT_ENABLE_PROMISE
    PromiseObject* asPromiseObject()
    {
        ASSERT(isPromiseObject());
        return (PromiseObject*)this;
    }
#endif

    DoubleInSmallValue* asDoubleInSmallValue()
    {
        ASSERT(isDoubleInSmallValue());
        return (DoubleInSmallValue*)this;
    }

    JSGetterSetter* asJSGetterSetter()
    {
        ASSERT(isJSGetterSetter());
        return (JSGetterSetter*)this;
    }
};
}

#endif
