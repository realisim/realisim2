#include <algorithm>
#include <cmath>
#include "Core/Statistics.h"
#include <limits>
#include <sstream>

using namespace Realisim;
using namespace Core;
using namespace std;

Statistics::Statistics() :
  mKeepSamples(false),
  mSamples(),
  mNumberOfSamples(0),
  mSum(0.0),
  mSumSquared(0.0),
  mMin(std::numeric_limits<double>::max()),
  mMax(-std::numeric_limits<double>::max())
{}

//-------------------------------------------------------------------------
Statistics::~Statistics()
{ clear(); }

//-------------------------------------------------------------------------
void Statistics::add(double iSample)
{
  if(isKeepingSamples())
  { mSamples.push_back(iSample); }
  
  ++mNumberOfSamples;
  mSum += iSample;
  mSumSquared += iSample * iSample;
  mMin = std::min(mMin, iSample);
  mMax = std::max(mMax, iSample);
}

//-------------------------------------------------------------------------
void Statistics::add(const std::vector<double>& iSamples)
{
  for(size_t i = 0; i < iSamples.size(); ++i )
  { add(iSamples[i]); }
}

//-------------------------------------------------------------------------
void Statistics::add(const double* iSamples, unsigned int iNum)
{
  for(size_t i = 0; i < iNum; ++i )
  { add( iSamples[i] ); }
}
 
//-------------------------------------------------------------------------
double Statistics::average() const
{
    return mSum / max((double)numberOfSamples(), 1.0);
}

//-------------------------------------------------------------------------
void Statistics::clear()
{
  mSamples.clear();
  mNumberOfSamples = 0;
  mSum = 0.0;
  mSumSquared = 0.0;
  mMin = std::numeric_limits<double>::max();
  mMax = -std::numeric_limits<double>::max();
}

//-------------------------------------------------------------------------
double Statistics::sample(unsigned int iIndex) const
{
  double r = 0.0;
  if(iIndex < numberOfSamples())
  { r = mSamples[iIndex]; }
  return r;
}

//-------------------------------------------------------------------------
double Statistics::standardDeviation() const
{
  const unsigned int n = numberOfSamples();
  return sqrt( (mSumSquared - ( mSum * mSum / n)) / (n - 1) );
}


//-------------------------------------------------------------------------
std::string Statistics::toCsv() const
{
	ostringstream oss;
	oss << "Samples\n";

	for (auto it : mSamples)
	{
		oss << it << "\n";
	}

	//summary
	oss << "\n\n";
	oss << ",Number of samples,average,minimum,maximum,standard deviation\n";
	oss << "," << numberOfSamples() << "," << average() << "," << minimum() << "," << maximum() << "," << standardDeviation();


	return oss.str();
}

