
#pragma once
#include "IReflectable.h"
#include <stdint.h>
#include <vector>

struct DummyStruct : public IReflectable {
    DummyStruct() : mUint32(0), mUint16(0), mUint8(0) { }

    struct InnerData : public IReflectable {
        InnerData() : IReflectable() {}
        InnerData(uint32_t a, uint16_t b, uint8_t c) : mInnerUint32{ a }, mInnerUint16{ b }, mInnerUint8{ c } {}

        uint32_t mInnerUint32;
        uint16_t mInnerUint16;
        uint8_t mInnerUint8;

        // visitor refelection
        template<typename Visitor>
        void describe(Visitor& v) {
            v.Name("DummyStruct::InnerData");
            v.field("mInnerUint32", mInnerUint32);
            v.field("mInnerUint16", mInnerUint16);
            v.field("mInnerUint8", mInnerUint8);
        }
    };

    uint32_t mUint32;
    uint16_t mUint16;
    uint8_t mUint8;
    std::vector<uint32_t> mVectorOfUint32;
    InnerData mInnerData;
    std::vector<InnerData> mInnerDataVector;

    // visitor refelection
    template<typename Visitor>
    void describe(Visitor& v) {
        v.Name("DummyStruct");
        v.field("mUint32", mUint32);
        v.field("mUint16", mUint16);
        v.field("mUint8", mUint8);
        v.field("mVectorOfUint32", mVectorOfUint32);
        v.field("mInnerData", mInnerData);
        v.field("mInnerDataVector", mInnerDataVector);
    }

};

