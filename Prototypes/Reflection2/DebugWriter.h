
#pragma once

#include "IReflectable.h"
#include <sstream>

class DebugWriter : public Reflect::IReflectableVisitor {
public:
    DebugWriter() {}
    explicit DebugWriter(Reflect::IReflectable&);
    virtual ~DebugWriter() {}

    std::string getString() { return mOss.str(); }

    virtual void visitClass(const Reflect::IReflectable&, int) override final;

    virtual void visitContainerValue(const uint8_t& iV) override final;
    virtual void visitContainerValue(const uint16_t& iV) override final;
    virtual void visitContainerValue(const uint32_t& iV) override final;
    virtual void visitContainerValue(const uint64_t& iV) override final;

    virtual void visit(const Reflect::ReflectedMember&, int) override final;
    virtual void visit(const Reflect::ReflectedMember& iR, const uint8_t& iV, int iDepth) override final;
    virtual void visit(const Reflect::ReflectedMember& iR, const uint16_t& iV, int iDepth) override final;
    virtual void visit(const Reflect::ReflectedMember& iR, const uint32_t& iV, int iDepth) override final;
    virtual void visit(const Reflect::ReflectedMember& iR, const uint64_t& iV, int iDepth) override final;

    void write(Reflect::IReflectable&);

protected:
    virtual void beginVector(int iDepth) override final;
    virtual void endVector(int iDepth) override final;

    template<typename T> void printIntegers(const T& iInteger);
    template<typename T> void printIntegers(const Reflect::ReflectedMember& iR, const T& iInteger, int iDepth);

    std::ostringstream mOss;
};

//-------------------------------------------------------------------------------------------------------------------- -
template<typename T>
void DebugWriter::printIntegers(const T& iInteger)
{
    static_assert(std::is_integral_v<T>, "printIntegers should only be used on integral types.");
    mOss << to_string(iInteger) << ',';
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
void DebugWriter::printIntegers(const Reflect::ReflectedMember& iR, const T& iInteger, int iDepth)
{
    static_assert(std::is_integral_v<T>, "printIntegers should only be used on integral types.");
    std::string spaces(iDepth * 4, ' ');
    mOss <<
        spaces << "name : " << iR.mName << "\n" <<
        spaces << "size : " << iR.mSize << "\n" <<
        spaces << "value: " << to_string(iInteger) << "\n";
}