
#include "gtest/gtest.h"
#include "Utilities/ByteArray.h"

using namespace Realisim;
using namespace Utilities;

const std::string referenceString = "the quick brown fox jumps over the lazy dog.";
const std::string loremIpsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
const std::string referenceData = {'\x0', '\x1', '\x0', '\x1',
    '\x0', '\x1', '\x0', '\x1',
    '\x0', '\x1', '\x0', '\x1'};

TEST(ByteArray, Constructor)
{
    //ByteArray()
    {
        ByteArray ba;
        EXPECT_STREQ(ba.constData(), "");
        EXPECT_TRUE(ba.isEmpty());
    }
    
    //ByteArray(const char*, int = -1)
    {
        ByteArray ba("the quick brown fox jumps over the lazy dog.");
        EXPECT_STREQ(ba.constData(), "the quick brown fox jumps over the lazy dog.");
        
        char c[7] = "a\0b\0c\0";
        ByteArray ba2(c);
        EXPECT_TRUE(ba2.size() == 1);
        EXPECT_STREQ(ba2.constData(), "a");
        
        ByteArray ba3(c, 6);
        EXPECT_TRUE(ba3.size() == 6);
        EXPECT_STREQ(ba3.constData(), "a\0b\0c\0");
    }
    
    //ByteArray(const std::string&)
    {
        ByteArray ba(referenceString);
        EXPECT_STREQ(ba.constData(), referenceString.c_str());
        
        std::string s2("Les patates\0sont bonnes.");
        ByteArray ba2(s2);
        EXPECT_TRUE(ba2.size() == 11);
        EXPECT_STREQ(ba2.constData(), "Les patates");
    }
    
    //ByteArray(const ByteArray&)
    {
        ByteArray ba(referenceString);
        ByteArray ba2(ba);
        EXPECT_TRUE(ba2.refCount() == 2);
        EXPECT_STREQ(ba2.constData(), referenceString.c_str());
    }
    
    //ByteArray& operator=(const ByteArray&)
    {
        ByteArray ba(referenceString);
        ByteArray ba2 = ba;
        EXPECT_TRUE(ba2.refCount() == 2);
        EXPECT_STREQ(ba2.constData(), referenceString.c_str());
    }
    
    //ByteArray& operator=(const char*)
    {
        ByteArray ba("test1");
        ba = "test2";
        EXPECT_STREQ(ba.constData(), "test2");
    }
}

TEST(ByteArray, append)
{
    // ByteArray& append(const char*);
    {
        ByteArray ba;
        ba.append("the quick brown");
        ba.append(" fox jumps");
        ba.append(" over the lazy dog.");
        EXPECT_TRUE(ba.refCount() == 1);
        EXPECT_STREQ(ba.constData(), referenceString.c_str());
    }
    
    //ByteArray& append(const char*, int iSize);
    {
        ByteArray ba;
        ba.append("abc\0", 4);
        ba.append("def\0", 4);
        EXPECT_TRUE(ba.refCount() == 1);
        EXPECT_TRUE(ba.size() == 8);
        EXPECT_STREQ(ba.constData(), "abc\0def\0");
    }
    
    //ByteArray& append(const std::string&);
    {
        ByteArray ba;
        ba.append(std::string("abc\0", 4));
        ba.append(std::string("def\0", 4));
        EXPECT_TRUE(ba.refCount() == 1);
        EXPECT_TRUE(ba.size() == 8);
        EXPECT_STREQ(ba.constData(), "abc\0def\0");
    }
    
    //ByteArray& append(const ByteArray&);
    {
        ByteArray a("a\0bc\0", 5);
        ByteArray b("d\0ef\0", 5);
        ByteArray c;
        c.append(a);
        c.append(b);
        EXPECT_TRUE(c.refCount() == 1);
        EXPECT_TRUE(c.size() == 10);
        EXPECT_STREQ(c.constData(), "a\0bc\0d\0ef\0");
    }
}

TEST(ByteArray, operator)
{

    //bool operator==(const ByteArray&) const
    {
        ByteArray ba("abc");
        ByteArray ba2("abc");
        EXPECT_TRUE(ba == ba);
        EXPECT_TRUE(ba == ba2);
    }
    
    //bool operator!=(const ByteArray&) const
    {
        ByteArray ba("abc");
        ByteArray ba2("abcd");
        EXPECT_TRUE(ba != ba2);
    }
    
    //ByteArray operator+(const ByteArray&) const
    {
        ByteArray a("abc");
        ByteArray b("def");
        ByteArray c("ghi");
        ByteArray d = a + b + c;
        EXPECT_TRUE(d.refCount() == 1);
        EXPECT_TRUE(d.size() == 9);
        EXPECT_STREQ(d.constData(), "abcdefghi");
    }
    
    //ByteArray operator+(const char*)
    {
        ByteArray a("abc");
        ByteArray b = a + "def" + "ghi" + "jkl";
        EXPECT_TRUE(b.refCount() == 1);
        EXPECT_TRUE(b.size() == 12);
        EXPECT_STREQ(b.constData(), "abcdefghijkl");
    }
    
    //ByteArray operator+(const std::string&)
    {
        ByteArray a("abc");
        ByteArray b = a + std::string("def") + std::string("ghi") + std::string("jkl");
        EXPECT_TRUE(b.refCount() == 1);
        EXPECT_TRUE(b.size() == 12);
        EXPECT_STREQ(b.constData(), "abcdefghijkl");
    }
    
    //ByteArray& operator+=(const ByteArray&)
    //ByteArray& operator+=(const char*);
    //ByteArray& operator+=(const std::string&);
    {
        ByteArray a("abc");
        ByteArray b;
        b += a;
        b += "def";
        b += std::string("ghi");
        EXPECT_TRUE(b.refCount() == 1);
        EXPECT_TRUE(b.size() == 9);
        EXPECT_STREQ(b.constData(), "abcdefghi");
    }
    
    //char operator[](int) const
    {
        char ref[7] = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};
        const ByteArray ba(&ref[0]);
        ByteArray ba2(ba);
        EXPECT_TRUE(ba.refCount() == 2);
        EXPECT_TRUE(ba.size() == 6);
        for(int i = 0; i < ba.size(); ++i)
        {
            EXPECT_EQ(ba[i], ref[i]);
        }
        // ref count should still be 2 since operator[] const
        // should not detach guts...
        //
        EXPECT_TRUE(ba.refCount() == 2);
    }
    
    //char& operator[](int)
    {
        char ref[7] = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};
        ByteArray ba(&ref[0]);
        ByteArray ba2(ba);
        EXPECT_TRUE(ba.refCount() == 2);
        EXPECT_TRUE(ba.size() == 6);
        
        ba[0] = 'A';
        ba[1] = 'B';
        ba[2] = 'C';
        EXPECT_TRUE(ba.refCount() == 1);
        EXPECT_TRUE(ba2.refCount() == 1);
        EXPECT_STREQ(ba.constData(), "ABCdef");
        EXPECT_STREQ(ba2.constData(), "abcdef");
    }
}

TEST(ByteArray, Functions)
{
    //const std::string& asString() const
    {
        ByteArray ba(referenceData);
        EXPECT_TRUE(ba.size() == 12);
        const std::string& s = ba.asString();
        for(int i = 0; i < referenceData.size(); ++i)
        {
            EXPECT_EQ( s[i], referenceData[i] );
        }
    }
    
    //char at(size_t) const
    {
        ByteArray ba(referenceData);
        EXPECT_TRUE(ba.size() == 12);
        for(int i = 0; i < ba.size(); ++i)
        {
            EXPECT_EQ( ba.at(i), referenceData[i] );
        }
    }
    
    //size_t capacity() const
    {

    }
    
    //void clear()
    {
        ByteArray ba(referenceData);
        ba.clear();
        //int s = ba.size();
        
        EXPECT_TRUE(ba.size() == 0);
        EXPECT_TRUE(ba.isEmpty());
    }
    
    //ByteArray& fill(char, int = -1)
    {
        ByteArray ba;
        ba.resize(12);
        ba.fill('\x62');
        EXPECT_STREQ(ba.constData(), "\x62\x62\x62\x62\x62\x62\x62\x62\x62\x62\x62\x62\x0");
        
        ba.fill('\x63', 6);
        EXPECT_STREQ(ba.constData(), "\x63\x63\x63\x63\x63\x63\x62\x62\x62\x62\x62\x62\x0");
    }
    
    //bool isEmpty() const
    {
        ByteArray ba;
        EXPECT_TRUE(ba.isEmpty());
        ba += "hello";
        EXPECT_FALSE(ba.isEmpty());
    }
    
    //ByteArray mid(size_t iPos, int iLength = -1) const
    {
        ByteArray ba(referenceString);
        ByteArray ba2 = ba.mid(4, 5);
        EXPECT_EQ(ba2.size(), 5);
        EXPECT_STREQ(ba2.constData(), "quick");
    }
    
    //int refCount() const
    {
        //already tested above...
    }
    
    //void reserve(int)
    {

    }
    
    //void resize(int)
    {
        ByteArray ba;
        ba.resize(12);
        EXPECT_EQ(ba.size(), 12);
    }
    
    //size_t size() const
    {

    }
    
}

TEST(ByteArray, TrailingZero)
{
    ByteArray ba(referenceString);
    // create a char* twice as long as ba.
    char *pString = new char[ba.size()*2];
    //fill it with garbage...
    memset(pString, 62, ba.size() * 2);
    
    // +1 on the memcpy to capture the trailing \0
    memcpy(pString, ba.constData(), ba.size()+1);
    
    // constructing a string from pString which has a \0 right at the end.
    // should make a new string exactly the size of reference string.
    EXPECT_TRUE( std::string(pString).size() == referenceString.size() );
    EXPECT_STREQ(pString, referenceString.c_str());
}

TEST(ByteArray, ReadTest)
{
    ByteArray ba(loremIpsum);
    std::istringstream iss(ba.constData());
    
    char *charRead = new char[ba.size()];
    iss.read(charRead, ba.size());
    
    EXPECT_STREQ(loremIpsum.c_str(), charRead);
}

TEST(ByteArray, WriteTest_usingOperatorBracket)
{
    ByteArray ba(referenceString);
    ByteArray ba2(ba);
    EXPECT_TRUE(ba2.refCount() == 2);
    EXPECT_STREQ(ba2.constData(), referenceString.c_str());
    
    // get the data pointer, which will detach ba from ba and
    // write into it.
    
    char *pData = ba.data();
    pData[0] = 'T'; pData[1] = 'H'; pData[2] = 'E';
    
    EXPECT_TRUE(ba.refCount() == 1);
    EXPECT_STREQ(ba.constData(), "THE quick brown fox jumps over the lazy dog.");
    EXPECT_TRUE(ba2.refCount() == 1);
    EXPECT_STREQ(ba2.constData(), referenceString.c_str());
}

TEST(ByteArray, WriteTest_usingDataPointer)
{
    // make a byte array big enough to contain the data
    ByteArray ba;
    ba.resize(referenceString.size());
    
    // read the data directly into the byte array
    std::istringstream iss(referenceString);
    iss.read(ba.data(), ba.size());
    
    EXPECT_STREQ(ba.constData(), referenceString.c_str());
}