
#pragma once

#include <vector>

namespace Realisim
{
namespace Core
{

  //Ce bout de code est fortement inspiré de :
  //http://c.learncodethehardway.org/book/ex43.html
  
  //Cette classe sert a compiler des statistiques sur un flux d'échantillons.
  //La classe permet optionellement de conserver les échantillons, sinon,
  //les statisques sont calculés à la demande en temps constant O(1).
  //
  //On peut obtenir,
  // min
  // max
  // mean
  // nombre d'échantillons
  // écart type
  // 
  class Statistics
  {
  public:
    Statistics();
    Statistics(const Statistics&);
    Statistics& operator=(const Statistics&)=delete;
    ~Statistics();
    
    void add(double);
    void add(const std::vector<double>&);
    void add(const double*, unsigned int);
    double average() const;
    void clear();
    bool isKeepingSamples() const {return mKeepSamples;}
    void keepSamples(bool iK) {mKeepSamples = iK;}
    double maximum() const {return mMax;}
    double minimum() const {return mMin;}
    unsigned int numberOfSamples() const {return mNumberOfSamples;}
    double sample(unsigned int) const;
    double standardDeviation() const;
	std::string toCsv() const;
    
  protected:
    bool mKeepSamples;
    std::vector<double> mSamples;
    unsigned int mNumberOfSamples;
    double mSum;
    double mSumSquared;
    double mMin;
    double mMax;
  };

} // end of namespace utils
} 

