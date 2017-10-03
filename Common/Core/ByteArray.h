#pragma once

#include <atomic>
#include <string>

namespace Realisim
{
namespace Core
{
    /*
    This class eases byte array manipulation. Mostly the memory is carefully 
    handled so it can be shared across different instance (until it is modified,
    please refer to copy-on-write mechanism: http://doc.qt.io/qt-5/implicit-sharing.html).

    Assigning data
        A byte array can be created empty via ByteArray() and filled with methods
        such as:
            append(), fill(), operator+(), operator+=(), operator[], and set().

        They usually come with an overrride for char*, std::string and Bytearray.
        The ByteArray takes ownership of the data by copying it. It is thus safe
        to delete the original buffer data after an assignation to the ByteArray.

        When using the char* variant, such as append(const char*) or 
        operator+(const char*), it will behave as with c buffer. I.e: data will
        be added until a '\0' is found.

        When dealing with binary data, the content of the data might have '\0', so
        using method such as append(const char*, int iSize), will guarantee that
        all iSize bytes pointed by the char* will be copied and owned by the
        ByteArray.

    Retrieving data
        Data from the ByteArray can be retrieve in many ways.
            data(), constData(), constString(), mid(), operator[]

        When retrieving data, most methods are const and guarantee the no
        copy of the data will be performed. The exception are with method
        data(), nonConstString() and operator[]. Since they are not const and could be modified,
        the byteArray will perform a deep copy of that data only if it is
        shared with other ByteArray instance. Otherwise, no copy is performed.
        To gurantee that no copy is performed, constData(), asString() and
        mid() is the way to go.

        If the data contained must be passed to C api which expects a \0 terminated
        string, it is advised to use method asString().c_str(). This will guarantee
        that a null terminated string.

    The bytearray can be released by calling clear(). The memory will be released
    when the last instance using the data will be cleared or deleted.

    This is class is thread safe.
    
    Note:
       method constData(), constString() quarantees that no copy will occurs.
       method data(), nonConstString() will copy the content since it might be overwritten.*/
    class ByteArray
    {
    public:
        ByteArray();
        explicit ByteArray(const char* iData, int = -1);
        explicit ByteArray(const std::string&);
        ByteArray(const ByteArray&);
        ByteArray& operator=(const ByteArray&);
        ByteArray& operator=(const char*);
        ~ByteArray();

        ByteArray& append(const char*);
        ByteArray& append(const char*, int iSize);
        ByteArray& append(const std::string&);
        ByteArray& append(const ByteArray&);
        char at(size_t) const;
        size_t capacity() const;
        void clear();
        const char* constData() const;
        const std::string& constString() const;
        char* data();
        ByteArray& fill(char, int = -1);
//static ByteArray fromRawData(const char*, int iSize);
//ByteArra& insert(int iPos, const char*);
//ByteArra& insert(int iPos, const char*, int iSize);  
//ByteArra& insert(int iPos, const ByteArray&);
        bool isEmpty() const;
        ByteArray mid(size_t iPos, int iLength = -1) const;
        std::string& nonConstString();
        bool operator==(const ByteArray&) const;
        bool operator!=(const ByteArray&) const;
        ByteArray operator+(const ByteArray&) const;
        ByteArray operator+(const char*);
        ByteArray operator+(const std::string&);
        ByteArray& operator+=(const ByteArray&);
        ByteArray& operator+=(const char*);
        ByteArray& operator+=(const std::string&);
        char operator[](size_t) const;
        char& operator[](size_t);
//ByteArra& prepend(const char*);
//ByteArra& prepend(const char*, int iSize);  
//ByteArra& prepend(const ByteArray&);
//ByteArra& remove(int iPos, int iLength = -1);
        int refCount() const;
        void reserve(size_t);
        void resize(size_t);
        void set(const char *ipData, size_t iLength);
        void set(const std::string &iData);
        void set(const ByteArray &iData);
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