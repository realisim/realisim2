#pragma once

class IReflectable {
public:
    IReflectable() { ; }
    virtual ~IReflectable() { ; }
};


#define REFLECT_BEGIN(className)\
    template<typename Visitor> \
    void describe(Visitor& v) { \
        v.Name(#className); \

#define REFLECT_MEMBER(memberName) \
    v.field("memberName", memberName);

#define REFLECT_END()\
    }