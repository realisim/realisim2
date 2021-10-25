
#include "IReflectable.h"

using namespace std;
using namespace Reflect;

string Reflect::IReflectable::mDummyMemberName = "noName";

//---------------------------------------------------------------------------------------------------------------------
IReflectable::IReflectable()
{}

//---------------------------------------------------------------------------------------------------------------------
const string& IReflectable::getMemberName(int iIndex) const
{
    string* r = &mDummyMemberName;

    if (iIndex >= 0 && iIndex < (int)mMembers.size()) {
        *r = mMembers[iIndex].mName; }

    return *r;
}

//---------------------------------------------------------------------------------------------------------------------
const MemberType IReflectable::getMemberType(int iIndex) const
{
    MemberType r = MemberType::mtUndefined;

    if (iIndex >= 0 && iIndex < (int)mMembers.size()) {
        r = mMembers[iIndex].mType;
    }

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
void IReflectable::visitReflectedMembers(IReflectableVisitor* ipVisitor, int iDepth) {
    (void)ipVisitor;
    (void)iDepth;
}
