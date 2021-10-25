
#include "DummyStruct.h"


REFLECT_CLASS_BEGIN(DummyStruct)
    REFLECT_MEMBERS_BEGIN()
        REFLECT_MEMBER(DummyStruct, mUint32)
        REFLECT_MEMBER(DummyStruct, mUint16)
        REFLECT_MEMBER(DummyStruct, mUint8)
        REFLECT_MEMBER(DummyStruct, mVectorOfUint32)
        REFLECT_MEMBER(DummyStruct, mInnerData)
        REFLECT_MEMBER(DummyStruct, mInnerDataVector)
    REFLECT_MEMBERS_END()
REFLECT_CLASS_END()


REFLECT_VISITOR_BEGIN(DummyStruct)
    REFLECT_VISIT_MEMBER(0, mUint32)
    REFLECT_VISIT_MEMBER(1, mUint16)
    REFLECT_VISIT_MEMBER(2, mUint8)
    REFLECT_VISIT_MEMBER(3, mVectorOfUint32)
    REFLECT_VISIT_MEMBER(4, mInnerData)
    REFLECT_VISIT_MEMBER(5, mInnerDataVector)
REFLECT_VISITOR_END()


REFLECT_CLASS_BEGIN(DummyStruct::InnerData)
    REFLECT_MEMBERS_BEGIN()
        REFLECT_MEMBER(DummyStruct::InnerData, mInnerUint32)
        REFLECT_MEMBER(DummyStruct::InnerData, mInnerUint16)
        REFLECT_MEMBER(DummyStruct::InnerData, mInnerUint8)
    REFLECT_MEMBERS_END()
REFLECT_CLASS_END()


REFLECT_VISITOR_BEGIN(DummyStruct::InnerData)
    REFLECT_VISIT_MEMBER(0, mInnerUint32)
    REFLECT_VISIT_MEMBER(1, mInnerUint16)
    REFLECT_VISIT_MEMBER(2, mInnerUint8)
REFLECT_VISITOR_END()
////--------------------

////---------------------------------------------------------------------------------------------------------------------
//void DummyStruct::initReflectedMembers()
//{
//    mClass = { "DummyStruct", sizeof(DummyStruct) };
//    mMembers = {
//        {"mUint32", Reflect::MemberType::mtUint32, offsetof(DummyStruct, mUint32), sizeof(mUint32) },
//        {"mUint16", Reflect::MemberType::mtUint16, offsetof(DummyStruct, mUint16), sizeof(mUint16) },
//        {"mUint8", Reflect::MemberType::mtUint8, offsetof(DummyStruct, mUint8), sizeof(mUint8) },
//        {"mVectorOfUint32", Reflect::MemberType::mtVector, offsetof(DummyStruct, mVectorOfUint32), sizeof(mVectorOfUint32) },
//        {"mInnerData", Reflect::MemberType::mtClass, offsetof(DummyStruct, mInnerData), sizeof(mInnerData) },
//        {"mInnerDataVector", Reflect::MemberType::mtVector, offsetof(DummyStruct, mInnerDataVector), sizeof(mInnerDataVector) }
//    };
//}

////---------------------------------------------------------------------------------------------------------------------
//void DummyStruct::visitReflectedMembers(Reflect::IReflectableVisitor* ipVisitor, int iDepth)
//{
//    (*ipVisitor).visit(mMembers[0], mUint32, iDepth);
//    (*ipVisitor).visit(mMembers[1], mUint16, iDepth);
//    (*ipVisitor).visit(mMembers[2], mUint8, iDepth);
//    (*ipVisitor).visit(mMembers[3], mVectorOfUint32, iDepth);
//    (*ipVisitor).visit(mMembers[4], mInnerData, iDepth);
//    (*ipVisitor).visit(mMembers[5], mInnerDataVector, iDepth);
//}


////---------------------------------------------------------------------------------------------------------------------
//void DummyStruct::InnerData::initReflectedMembers()
//{
//    mClass = { "DummyStruct::InnerData", sizeof(DummyStruct::InnerData) };
//    mMembers = {
//        {"mInnerUint32", Reflect::MemberType::mtUint32, offsetof(DummyStruct::InnerData, mInnerUint32), sizeof(mInnerUint32) },
//        {"mInnerUint16", Reflect::MemberType::mtUint16, offsetof(DummyStruct::InnerData, mInnerUint16), sizeof(mInnerUint16) },
//        {"mInnerUint8", Reflect::MemberType::mtUint8, offsetof(DummyStruct::InnerData, mInnerUint8), sizeof(mInnerUint8) },
//    };
//}
//
////---------------------------------------------------------------------------------------------------------------------
//void DummyStruct::InnerData::visitReflectedMembers(Reflect::IReflectableVisitor* ipVisitor, int iDepth)
//{
//    (*ipVisitor).visit(mMembers[0], mInnerUint32, iDepth);
//    (*ipVisitor).visit(mMembers[1], mInnerUint16, iDepth);
//    (*ipVisitor).visit(mMembers[2], mInnerUint8, iDepth);
//}
