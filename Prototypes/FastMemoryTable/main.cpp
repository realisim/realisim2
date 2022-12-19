
#include <cassert>
#include "Core/Timer.h"
#include "FastMemoryTable.h"
#include <string>

struct MonData {
    MonData() = default;
    ~MonData() = default;
    MonData(int iA, float iB, const std::string& iC) : mA(iA), mB(iB), mC(iC) { ; }

    int mA;
    float mB;
    std::string mC;
};

class MonDataStringIndex : public SearchIndex<MonData>
{
public:
    MonDataStringIndex() = default;
    virtual ~MonDataStringIndex() { ; }

    virtual uint64_t makeHash(const MonData* ipData) override final {
        uint64_t r = 0;
        memcpy(&r, &(ipData->mC[0]), std::min((size_t)8, ipData->mC.size()));
        return r;
    }

    virtual bool compare(const MonData* ipLhs, const MonData* ipRhs) override final {
        return ipLhs->mC == ipRhs->mC;
    }
};

//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    FastMemoryTable<MonData> fmt;
    int searchIndex = fmt.addSearchIndex(new MonDataStringIndex());

    const int numItems = 5;
    MonData* pMonDataStore = new MonData[numItems];

    Realisim::Core::Timer _timer;
    fmt.reserve(numItems);
    for (int i = 0; i < numItems; ++i) {
        MonData md(i, i * 0.33f, std::to_string(i));
        fmt.push_back(md);
        pMonDataStore[i] = md;
    }
    const double allocationElapsed = _timer.elapsed();

    _timer.start();
    for (int i = 0; i < numItems; ++i)
    {
        if (fmt.findFirst(searchIndex, pMonDataStore[i]) != i)
            printf("search failed\n");
    }
    const double searchElapsed = _timer.elapsed();

    delete[] pMonDataStore;

    printf("Allocation time (sec): %f\n", allocationElapsed);
    printf("search time (sec): %f\n", searchElapsed);

   /* uint64_t findIndex = (uint64_t )-1;
    findIndex = fmt.findFirst(searchIndex, MonData(0, 0 * 0.33f, std::to_string(0)));
    findIndex = fmt.findFirst(searchIndex, MonData(1, 1 * 0.33f, std::to_string(1)));
    findIndex = fmt.findFirst(searchIndex, MonData(2, 2 * 0.33f, std::to_string(2)));
    findIndex = fmt.findFirst(searchIndex, MonData(3, 3 * 0.33f, std::to_string(3)));
    findIndex = fmt.findFirst(searchIndex, MonData(4, 4 * 0.33f, std::to_string(4)));

    std::vector<uint64_t> mIndices;
    mIndices = fmt.equalRange(searchIndex, MonData(0, 0 * 0.33f, std::to_string(2)));*/

    //print data and address
    /*for (int i = 0; i < numItems; ++i) {
        const MonData& md = fmt.getData(i);
        printf("i: %d, address: %p, mA: %d, mB: %f, mC: %s\n", i, &md, md.mA, md.mB, md.mC.c_str());
    }*/
    return 0;
}

