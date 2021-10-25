
#include "DebugWriter.h"

using namespace std;

DebugWriter::DebugWriter(Reflect::IReflectable& iR) {
    write(iR);
}

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::beginVector(int iDepth)
{
    std::string spaces(iDepth * 4, ' ');
    mOss <<
        spaces << "values: [";
}

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::endVector(int iDepth)
{
    (void)iDepth;
    mOss << "]\n";
}

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::visitClass(const Reflect::IReflectable& iR, int iDepth) {
    std::string spaces(iDepth * 4, ' ');
    mOss <<
        spaces << "class: " << iR.getClassName() << "\n" <<
        spaces << "size:  " << iR.getClassSize() << "\n";
}

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::visitContainerValue(const uint8_t& iV)
{ printIntegers(iV); }

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::visitContainerValue(const uint16_t& iV)
{ printIntegers(iV); }

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::visitContainerValue(const uint32_t& iV)
{ printIntegers(iV); }

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::visitContainerValue(const uint64_t& iV)
{ printIntegers(iV); }

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::visit(const Reflect::ReflectedMember& iR, int iDepth)
{
    std::string spaces(iDepth * 4, ' ');
    mOss <<
        spaces << "name : " << iR.mName << "\n" <<
        spaces << "size : " << iR.mSize << "\n";
}

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::visit(const Reflect::ReflectedMember& iR, const uint8_t& iV, int iDepth)
{ printIntegers(iR, iV, iDepth); }

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::visit(const Reflect::ReflectedMember& iR, const uint16_t& iV, int iDepth)
{ printIntegers(iR, iV, iDepth);}

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::visit(const Reflect::ReflectedMember& iR, const uint32_t& iV, int iDepth)
{ printIntegers(iR, iV, iDepth); }

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::visit(const Reflect::ReflectedMember& iR, const uint64_t& iV, int iDepth)
{ printIntegers(iR, iV, iDepth); }

//---------------------------------------------------------------------------------------------------------------------
void DebugWriter::write(Reflect::IReflectable& iR) {
    visitClass(iR, 0);
    iR.visitReflectedMembers(this, 1);
}
