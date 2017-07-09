#include "gtest/gtest.h"
#include "Utilities/StringUtilities.h"

using namespace Realisim;
using namespace Utilities;
using namespace std;

TEST(StringUtilities, toInt)
{
    {
        const string a("0");
        const string b("1");
        const string c("12");
        const string d("123");
        const string e("1234");
        const string f("12345");
        const string g("123456");
        const string h("1234567");
        const string i("12345678");
        const string j("123456789");
        const string k("1234567890");
        const string l("2147483647"); //upper limit
        const string m("2247483647"); //invalid out of limit
        
        bool ba, bc, bm;
        const int ia = toInt(a, &ba);
        const int ib = toInt(b);
        const int ic = toInt(c, &bc);
        const int id = toInt(d);
        const int ie = toInt(e);
        const int i_f = toInt(f);
        const int ig = toInt(g);
        const int ih = toInt(h);
        const int ii = toInt(i);
        const int ij = toInt(j);
        const int ik = toInt(k);
        const int il = toInt(l);
        const int im = toInt(m, &bm);
        
        EXPECT_EQ(ia, 0);
        EXPECT_TRUE(ba);
        EXPECT_EQ(ib, 1);
        EXPECT_EQ(ic, 12);
        EXPECT_TRUE(bc);
        EXPECT_EQ(id, 123);
        EXPECT_EQ(ie, 1234);
        EXPECT_EQ(i_f, 12345);
        EXPECT_EQ(ig, 123456);
        EXPECT_EQ(ih, 1234567);
        EXPECT_EQ(ii, 12345678);
        EXPECT_EQ(ij, 123456789);
        EXPECT_EQ(ik, 1234567890);
        EXPECT_EQ(il, 2147483647);
        EXPECT_EQ(im, 0);
        EXPECT_FALSE(bm);
    }

    {
        const string a("-0");
        const string b("-1");
        const string c("-12");
        const string d("-123");
        const string e("-1234");
        const string f("-12345");
        const string g("-123456");
        const string h("-1234567");
        const string i("-12345678");
        const string j("-123456789");
        const string k("-1234567890");
        const string l("-2147483647"); //lower limit
        const string m("-2247483647"); //invalid out of limit
        
        bool ba, bc, bm;
        const int ia = toInt(a, &ba);
        const int ib = toInt(b);
        const int ic = toInt(c, &bc);
        const int id = toInt(d);
        const int ie = toInt(e);
        const int i_f = toInt(f);
        const int ig = toInt(g);
        const int ih = toInt(h);
        const int ii = toInt(i);
        const int ij = toInt(j);
        const int ik = toInt(k);
        const int il = toInt(l);
        const int im = toInt(m, &bm);
        
        EXPECT_EQ(ia, 0);
        EXPECT_TRUE(ba);
        EXPECT_EQ(ib, -1);
        EXPECT_EQ(ic, -12);
        EXPECT_TRUE(bc);
        EXPECT_EQ(id, -123);
        EXPECT_EQ(ie, -1234);
        EXPECT_EQ(i_f, -12345);
        EXPECT_EQ(ig, -123456);
        EXPECT_EQ(ih, -1234567);
        EXPECT_EQ(ii, -12345678);
        EXPECT_EQ(ij, -123456789);
        EXPECT_EQ(ik, -1234567890);
        EXPECT_EQ(il, -2147483647);
        EXPECT_EQ(im, 0);
        EXPECT_FALSE(bm);

    }
    
    {
        const string test("");
        const string test0("-");
        const string test1("-123asd123");
        const string test2("123-123");
        
        bool t, t0, t1, t2;
        int v;
        v = toInt(test, &t);
        v = toInt(test0, &t0);
        v = toInt(test1, &t1);
        v = toInt(test2, &t2);
        
        EXPECT_FALSE(t);
        EXPECT_FALSE(t0);
        EXPECT_FALSE(t1);
        EXPECT_FALSE(t2);
    }
}

TEST(StringUtilities, toUnsignedInt)
{
    {
        const string a("0");
        const string b("1");
        const string c("12");
        const string d("123");
        const string e("1234");
        const string f("12345");
        const string g("123456");
        const string h("1234567");
        const string i("12345678");
        const string j("123456789");
        const string k("1234567890");
        const string l("2147483647"); //
        const string m("4294967295"); //upperLimit
        const string n("4294967296"); //invalid, outside range
        const string o("-1"); //invalid, outside range
        
        bool ba, bc, bm, bn, bo;
        const unsigned int ia = toUnsignedInt(a, &ba);
        const unsigned int ib = toUnsignedInt(b);
        const unsigned int ic = toUnsignedInt(c, &bc);
        const unsigned int id = toUnsignedInt(d);
        const unsigned int ie = toUnsignedInt(e);
        const unsigned int i_f = toUnsignedInt(f);
        const unsigned int ig = toUnsignedInt(g);
        const unsigned int ih = toUnsignedInt(h);
        const unsigned int ii = toUnsignedInt(i);
        const unsigned int ij = toUnsignedInt(j);
        const unsigned int ik = toUnsignedInt(k);
        const unsigned int il = toUnsignedInt(l);
        const unsigned int im = toUnsignedInt(m, &bm);
        const unsigned int in = toUnsignedInt(n, &bn);
        const unsigned int io = toUnsignedInt(o, &bo);
        
        EXPECT_EQ(ia, 0);
        EXPECT_TRUE(ba);
        EXPECT_EQ(ib, 1);
        EXPECT_EQ(ic, 12);
        EXPECT_TRUE(bc);
        EXPECT_EQ(id, 123);
        EXPECT_EQ(ie, 1234);
        EXPECT_EQ(i_f, 12345);
        EXPECT_EQ(ig, 123456);
        EXPECT_EQ(ih, 1234567);
        EXPECT_EQ(ii, 12345678);
        EXPECT_EQ(ij, 123456789);
        EXPECT_EQ(ik, 1234567890);
        EXPECT_EQ(il, 2147483647);
        EXPECT_EQ(im, 4294967295);
        EXPECT_TRUE(bm);
        EXPECT_EQ(in, 0);
        EXPECT_FALSE(bn);
        EXPECT_EQ(io, 0);
        EXPECT_FALSE(bo);
    }
    
    {
        const string test("");
        const string test0("-");
        const string test1("-123asd123");
        const string test2("123-123");
        
        bool t, t0, t1, t2;
        int v;
        v = toInt(test, &t);
        v = toInt(test0, &t0);
        v = toInt(test1, &t1);
        v = toInt(test2, &t2);
        
        EXPECT_FALSE(t);
        EXPECT_FALSE(t0);
        EXPECT_FALSE(t1);
        EXPECT_FALSE(t2);
    }
}

TEST(StringUtilities, toString_int_)
{
    const int a = 0;
    const int b = 1;
    const int c = 12;
    const int d = 123;
    const int e = 1234;
    const int f = 12345;
    const int g = 123456;
    const int h = 1234567;
    const int i = 12345678;
    const int j = 123456789;
    const int k = 1234567890;
    
    const int na = -0;
    const int nb = -1;
    const int nc = -12;
    const int nd = -123;
    const int ne = -1234;
    const int nf = -12345;
    const int ng = -123456;
    const int nh = -1234567;
    const int ni = -12345678;
    const int nj = -123456789;
    const int nk = -1234567890;
    
    const string sa = toString(a);
    const string sb = toString(b);
    const string sc = toString(c);
    const string sd = toString(d);
    const string se = toString(e);
    const string sf = toString(f);
    const string sg = toString(g);
    const string sh = toString(h);
    const string si = toString(i);
    const string sj = toString(j);
    const string sk = toString(k);
    
    const string sna = toString(na);
    const string snb = toString(nb);
    const string snc = toString(nc);
    const string snd = toString(nd);
    const string sne = toString(ne);
    const string snf = toString(nf);
    const string sng = toString(ng);
    const string snh = toString(nh);
    const string sni = toString(ni);
    const string snj = toString(nj);
    const string snk = toString(nk);
    
    EXPECT_STREQ(sa.c_str(), "0");
    EXPECT_STREQ(sb.c_str(), "1");
    EXPECT_STREQ(sc.c_str(), "12");
    EXPECT_STREQ(sd.c_str(), "123");
    EXPECT_STREQ(se.c_str(), "1234");
    EXPECT_STREQ(sf.c_str(), "12345");
    EXPECT_STREQ(sg.c_str(), "123456");
    EXPECT_STREQ(sh.c_str(), "1234567");
    EXPECT_STREQ(si.c_str(), "12345678");
    EXPECT_STREQ(sj.c_str(), "123456789");
    EXPECT_STREQ(sk.c_str(), "1234567890");
    
    EXPECT_STREQ(sna.c_str(), "0");
    EXPECT_STREQ(snb.c_str(), "-1");
    EXPECT_STREQ(snc.c_str(), "-12");
    EXPECT_STREQ(snd.c_str(), "-123");
    EXPECT_STREQ(sne.c_str(), "-1234");
    EXPECT_STREQ(snf.c_str(), "-12345");
    EXPECT_STREQ(sng.c_str(), "-123456");
    EXPECT_STREQ(snh.c_str(), "-1234567");
    EXPECT_STREQ(sni.c_str(), "-12345678");
    EXPECT_STREQ(snj.c_str(), "-123456789");
    EXPECT_STREQ(snk.c_str(), "-1234567890");
}

TEST(StringUtilities, toString_unsigned_int_)
{
    const unsigned int a = 0;
    const unsigned int b = 1;
    const unsigned int c = 12;
    const unsigned int d = 123;
    const unsigned int e = 1234;
    const unsigned int f = 12345;
    const unsigned int g = 123456;
    const unsigned int h = 1234567;
    const unsigned int i = 12345678;
    const unsigned int j = 123456789;
    const unsigned int k = 1234567890;
    const unsigned int l = 3234567890;
    
    const string sa = toString(a);
    const string sb = toString(b);
    const string sc = toString(c);
    const string sd = toString(d);
    const string se = toString(e);
    const string sf = toString(f);
    const string sg = toString(g);
    const string sh = toString(h);
    const string si = toString(i);
    const string sj = toString(j);
    const string sk = toString(k);
    const string sl = toString(l);
    
    EXPECT_STREQ(sa.c_str(), "0");
    EXPECT_STREQ(sb.c_str(), "1");
    EXPECT_STREQ(sc.c_str(), "12");
    EXPECT_STREQ(sd.c_str(), "123");
    EXPECT_STREQ(se.c_str(), "1234");
    EXPECT_STREQ(sf.c_str(), "12345");
    EXPECT_STREQ(sg.c_str(), "123456");
    EXPECT_STREQ(sh.c_str(), "1234567");
    EXPECT_STREQ(si.c_str(), "12345678");
    EXPECT_STREQ(sj.c_str(), "123456789");
    EXPECT_STREQ(sk.c_str(), "1234567890");
    EXPECT_STREQ(sl.c_str(), "3234567890");
}