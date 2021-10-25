
#pragma once

#include "IReflectable.h"

struct DummyStruct : public Reflect::IReflectable {
    DummyStruct() : IReflectable(), mUint32(0), mUint16(0), mUint8(0) { initReflectedMembers();  }

    struct InnerData : public Reflect::IReflectable {
        InnerData() : IReflectable() { initReflectedMembers(); }
        InnerData(uint32_t a, uint16_t b, uint8_t c) : IReflectable(), mInnerUint32{ a }, mInnerUint16{ b }, mInnerUint8{ c } { initReflectedMembers(); }

        uint32_t mInnerUint32;
        uint16_t mInnerUint16;
        uint8_t mInnerUint8;

    REFLECT()
    };

    uint32_t mUint32;
    uint16_t mUint16;
    uint8_t mUint8;
    std::vector<uint32_t> mVectorOfUint32;
    InnerData mInnerData;
    std::vector<InnerData> mInnerDataVector;


    REFLECT()
};

