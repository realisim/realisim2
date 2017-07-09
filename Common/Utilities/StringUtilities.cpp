
#include <cassert>
#include <cmath>
#include <sstream>
#include "StringUtilities.h"

namespace Realisim
{
namespace Utilities
{
    using namespace std;
    //-------------------------------------------------------------------------
    string fromVector(vector<string>& iInput, char iSeparator)
    {
        string r;
        for(size_t i = 0; i < iInput.size(); ++i)
        { 
            r += iInput[i];
            if(i < iInput.size() - 1)
            { r += iSeparator; }
        }
        return r;
    }

    //-------------------------------------------------------------------------
    // Convert input string to int.
    // Returns an int.
    // the optionnal parameter oSuccess can be passed in to determine if
    // the convertion was successfull or not.
    //
    // In case of failure, oSuccess will be false and the function returns 0.
    //
    int toInt(const std::string& iInput, bool *oSuccess)
    {
        int r = 0;
        long v = 0; //using long to catch overflow
        bool success = false;
        
        // must have at least 1 char, and all char must be digits or '-'
        // can't have more than 11 digits [-2147483647, 2147483647]
        // cannot be outside range of int.
        const int s = iInput.size();
        if(s > 0 &&
           s <= 11)
        {
            success = true;
            for(int i = s-1, count = 0;
                success && i >= 0;
                --i, count++)
            {
                // '-'
                // the first char can be '-', but there must be
                // more char after!
                if(i == 0 && iInput[0] == '-' && s > 1)
                {
                    v *= -1;
                }
                
                // must be a digit
                else if(iInput[i] >= '0' && iInput[i] <= '9')
                {
                    v += (iInput[i] - 48) * pow(10, count); //48 is the ascii code for Zero.
                }
                
                else
                {
                    success = false;
                }
            }
        }
        
        // make sure we are in the int range
        success &= numeric_limits<int>::min() <= v && v <= numeric_limits<int>::max();
        if(success)
        {
            r = v;
        }
            
        // assign optional output.
        if(oSuccess != nullptr)
        { *oSuccess = success; }
        
        return r;
    }
    
    //-------------------------------------------------------------------------
    // see method toInt()
    //
    unsigned int toUnsignedInt(const std::string& iInput, bool *oSuccess)
    {
        unsigned int r = 0;
        unsigned long v = 0; //using a long to catch overflow...
        bool success = false;
        
        // must have at least 1 char, and all char must be digits
        // can't have more than 10 digits [0, (2^32)-1]
        // cannot be outside range of unsigned int.
        const int s = iInput.size();
        if(s > 0 &&
           s <= 10)
        {
            success = true;
            for(int i = s-1, count = 0;
                success && i >= 0;
                --i, count++)
            {
                // must be a digit
                if(iInput[i] >= '0' && iInput[i] <= '9')
                {
                    v += (iInput[i] - 48) * pow(10, count); //48 is the ascii code for Zero.
                }
                
                else
                {
                    success = false;
                }
            }
        }
        
        // make sure we are in the int range
        success &= v <= numeric_limits<unsigned int>::max();
        if(success)
        {
            r = v;
        }
        
        // assign optional output.
        if(oSuccess != nullptr)
        { *oSuccess = success; }
        
        return r;
    }
    
    //-------------------------------------------------------------------------
    // return a string from a given int iV
    // see toString(unsigned int) for details
    //
    std::string toString(int iV)
    {
        string r;
        bool negative = iV < 0;
        iV = abs(iV);
        
        r = toString((unsigned int)iV);
        
        // add a minus sign in front.
        if(negative)
        {
            string t;
            t.resize(r.size()+1);
            t[0] = '-';
            for(int i = 1; i < t.size(); ++i)
            {
                t[i] = r[i-1];
            }
            r = t;
        }
        
        return r;
    }
    
    //-------------------------------------------------------------------------
    std::string toString(unsigned int iV)
    {
        string r;
        unsigned int one = 1;
        // log10 must not be done on 0. so log10(max(iV,1))
        // even if log10 is 0, there is a least one character, so max((int)ceil(log10(max(iV,1))),1)
        // if negative, add one more char.
        int n = max((unsigned int)ceil(log10(max(iV,one))),one);
        r.resize(n);
        
        // start from the end, smallest digit first and then
        // walk the way back to highest digit.
        unsigned int c = n-1;
        bool done = false;
        while(!done)
        {
            int v = iV % 10;
            iV = iV / 10;
            done = iV == 0;
            
            r[c--] = 48 + v; //48 is 0 in ascii
        }
        
        return r;
    }
    
    //-------------------------------------------------------------------------
    vector<string> toVector(const string& iInput, char iSeparator)
    {
		return toVector(iInput, string(1, iSeparator));
    }

	//-------------------------------------------------------------------------
	vector<string> toVector(const string& iInput, const std::string& iSeparatorList)
	{
		size_t c = 0;
		int l = 0;
		vector<string> tokens;
		for (size_t n = 0; n < iInput.size(); )
		{
			// current char is a separator
			if (iSeparatorList.find(iInput[n]) != string::npos)
			{
				tokens.push_back(iInput.substr(c, l));
				l = -1;
				c = n+1; // +1 to account for the separator;
			}
			++n;
			++l;
		}

		// last token
		if(l != -1)
		{
			tokens.push_back(iInput.substr(c, l));
		}
		return tokens;
	}
}
}