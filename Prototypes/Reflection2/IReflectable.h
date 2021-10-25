
#pragma once

#include <string>
#include <vector>
#include <stdint.h>

namespace Reflect
{
    class IReflectableVisitor;

    enum class MemberType {
        mtUndefined,
        mtUint8, mtUint16, mtUint32, mtUint64,
        mtInt8, mtInt16, mtInt32, mtInt64,
        mtFloat32, mtFloat64,
        mtClass,
        mtArray, mtVector, mtMap
    };

    class ReflectedClass {
    public:
        ReflectedClass() : mName { "noName" }, mSize{ 0 } {}
        ReflectedClass(const std::string& iName, int iSize) : mName{ iName }, mSize{ iSize } {}

        std::string mName;
        int mSize;
    };

    struct ReflectedMember {
        ReflectedMember() : mName{ "noName" }, mType{ MemberType::mtUndefined }, mOffset{ 0 }, mSize{ 0 } {}
        ReflectedMember(const std::string& iName, MemberType iType, int iOffset, int iSize) : 
            mName{ iName }, mType{ iType }, mOffset{ iOffset }, mSize{ iSize } {}

        std::string mName;
        MemberType mType;
        int mOffset;
        int mSize;
    };

    //---------------------------------------------------------------------------------------------------------------------
    //--- IReflectable
    //---------------------------------------------------------------------------------------------------------------------
    class IReflectable {
    public:
        friend IReflectableVisitor; // to give the visitor access to the members directly

        IReflectable();
        virtual ~IReflectable() = 0 {}

        const std::string& getClassName() const { return mClass.mName; }
        int getClassSize() const { return mClass.mSize; }
        const std::string& getMemberName(int iIndex) const;
        const MemberType getMemberType(int iIndex) const;
        int getNumberOfMembers() const { return (int)mMembers.size(); }


        virtual void visitReflectedMembers(IReflectableVisitor* ipVisitor, int iDepth) = 0;

    protected:
        virtual void initReflectedMembers() = 0 { ; }

        static std::string mDummyMemberName;

        ReflectedClass mClass;
        std::vector<ReflectedMember> mMembers;
    };



    //---------------------------------------------------------------------------------------------------------------------
    //--- IReflectableVisitor
    //---------------------------------------------------------------------------------------------------------------------
    class IReflectableVisitor {
    public:
        IReflectableVisitor() = default;
        IReflectableVisitor(const IReflectableVisitor&) = default;
        IReflectableVisitor& operator=(const IReflectableVisitor&) = default;
        virtual ~IReflectableVisitor() = default;

        virtual void visitClass(const IReflectable&, int) {}

        // this is the catch all... overload must be provided
        //template<typename T> void visit(const ReflectedMember& /*iR*/, const T& /*iV*/, int /*iDepth*/);

        virtual void visitContainerValue(const uint8_t&) {}
        virtual void visitContainerValue(const uint16_t&) {}
        virtual void visitContainerValue(const uint32_t&) {}
        virtual void visitContainerValue(const uint64_t&) {}

        virtual void visit(const ReflectedMember&, int) {}
        virtual void visit(const ReflectedMember&, const uint8_t&, int) {}
        virtual void visit(const ReflectedMember&, const uint16_t&, int) {}
        virtual void visit(const ReflectedMember&, const uint32_t&, int) {}
        virtual void visit(const ReflectedMember&, const uint64_t&, int) {}

        //virtual void visit(const ReflectedMember&, const std::vector<uint32_t>&, int) {}

        //catch all derived IReflectable 
        template <class T, typename std::enable_if<std::is_base_of<IReflectable,T>::value,int>::type = 0>
        void visit(const ReflectedMember& iRm, T& iV, int iDepth) {
            (void)iRm;
            visitClass(iV, iDepth);
            iV.visitReflectedMembers(this, ++iDepth);
        }

        // catch all std::vector of all derived from IReflectable
        // std::vector<IReflectable&>...
        template<typename T,
                 typename std::enable_if<std::is_base_of<IReflectable, T>::value, int>::type = 0 >
            void visit(const ReflectedMember& iRm, std::vector<T>& iValues, int iDepth) {

            visit(iRm, iDepth);
            beginVector(iDepth);
            for (auto & v : iValues) {
                visitClass(v, iDepth);
                v.visitReflectedMembers(this, iDepth + 1);
            }
            endVector(iDepth);
        }

        // cath all std::Vector of non IReflectable
        // std::vector<NOT IReflectable&>...
        template<typename T,
            typename std::enable_if<!std::is_base_of<IReflectable, T>::value, int>::type = 0 >
            void visit(const ReflectedMember& iRm, std::vector<T>& iValues, int iDepth) {

            visit(iRm, iDepth);
            beginVector(iDepth);
            for (auto& v : iValues) {
                visitContainerValue(v);
            }
            endVector(iDepth);
        }

    protected:
        virtual void beginVector(int iDepth) { (void)iDepth; }
        virtual void endVector(int iDepth) { (void)iDepth; }
    };


    //---------------------------------------------------------------------------------------------------------------------
    // Explain the macros...
    //
    #define REFLECT()\
        public:\
            virtual void visitReflectedMembers(Reflect::IReflectableVisitor* ipVisitor, int iDepth) override final;\
        protected:\
            virtual void initReflectedMembers() override final;

    #define REFLECT_CLASS_BEGIN(className)\
        void className::initReflectedMembers() {\
            mClass = {#className, sizeof(className)};\
        
    #define REFLECT_MEMBERS_BEGIN()\
        mMembers = {

    #define REFLECT_MEMBER(className, member)\
        {#member, Reflect::MemberType::mtUndefined, offsetof(className, member), sizeof(member)},

    #define REFLECT_MEMBERS_END()\
        };

    #define REFLECT_CLASS_END()\
    }

    //--------------------------------------
    #define REFLECT_VISITOR_BEGIN(className)\
        void className::visitReflectedMembers(Reflect::IReflectableVisitor* ipVisitor, int iDepth) \
        {

    #define REFLECT_VISIT_MEMBER(index, member) \
        (*ipVisitor).visit(mMembers[index], member, iDepth);

    #define REFLECT_VISITOR_END()\
        }
}


