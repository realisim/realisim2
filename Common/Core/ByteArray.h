#include <atomic>
#include <string>

namespace Realisim
{
namespace Core
{
    /*
     This class uses a copy-on-write mechanism. Please refer to http://doc.qt.io/qt-5/implicit-sharing.html
     for a detailed explanation.

     Note:
        method constData() quarantees that no copy will occurs.
        method data() will copy the content since it might be overwritten.
    */
    class ByteArray
    {
    public:
        ByteArray();
        ByteArray(const char*, int = -1);
        ByteArray(const std::string&);
        ByteArray(const ByteArray&);
        ByteArray& operator=(const ByteArray&);
        ByteArray& operator=(const char*);
        ~ByteArray();

        ByteArray& append(const char*);
        ByteArray& append(const char*, int iSize);
        ByteArray& append(const std::string&);
        ByteArray& append(const ByteArray&);
        const std::string& asString() const;
        char at(size_t) const;
        size_t capacity() const;
        void clear();
        const char* constData() const;
        char* data();
        ByteArray& fill(char, int = -1);
//static ByteArray fromRawData(const char*, int iSize);
//ByteArra& insert(int iPos, const char*);
//ByteArra& insert(int iPos, const char*, int iSize);  
//ByteArra& insert(int iPos, const ByteArray&);
        bool isEmpty() const;
        ByteArray mid(size_t iPos, int iLength = -1) const;
        bool operator==(const ByteArray&) const;
        bool operator!=(const ByteArray&) const;
        ByteArray operator+(const ByteArray&) const;
        ByteArray operator+(const char*);
        ByteArray operator+(const std::string&);
        ByteArray& operator+=(const ByteArray&);
        ByteArray& operator+=(const char*);
        ByteArray& operator+=(const std::string&);
        char operator[](int) const;
        char& operator[](int);
//ByteArra& prepend(const char*);
//ByteArra& prepend(const char*, int iSize);  
//ByteArra& prepend(const ByteArray&);
//ByteArra& remove(int iPos, int iLength = -1);
        int refCount() const;
        void reserve(int);
        void resize(int);
        size_t size() const;

    private:
        struct Guts
        {
            Guts();
            Guts(const Guts&);
            Guts& operator=(const Guts&) = delete;
            ~Guts() = default;

            std::atomic<int> mRefCount;
            std::string mData; //metre un char* pour supporter fromRawData...
        };

        size_t capLength(int iLength) const;
        void removeTrailing0();
        void addTrailing0();

        // the 4 following methods are for copy-on-write
        // functionnalities.
        //
        void deleteGuts();
        void detachGuts();
        void makeGuts();
        void shareGuts(Guts *);

        Guts *mpGuts;
    };
}
}