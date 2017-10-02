
#include <sstream>
#include "StringUtilities.h"

namespace Realisim
{
namespace Core
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
    vector<string> toVector(const string& iInput, char iSeparator)
    {
        //stringstream ss(iInput);
        //string item;
        //vector<string> tokens;
        //while (getline(ss, item, iSeparator))
        //{ tokens.push_back(item); }
        //return tokens;
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