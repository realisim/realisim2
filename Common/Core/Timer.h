
#pragma once

#include <chrono>

namespace Realisim
{
namespace Core
{
  //Cette classe fonctionne comme un chronomètre. Elle sert a calculer
  //des intervals de temps.
  
  class Timer
  {
  public:
    Timer();
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    ~Timer();
    
    double elapsed() const;
//void pause();
    void start();
    void stop();
    
  protected:
    enum state{ sIdle, sRunning, sStopped };
    
    state getState() const {return mState;}
    void goToState(state);
    
    state mState;
    std::chrono::high_resolution_clock::time_point mT1;
    std::chrono::high_resolution_clock::time_point mT2;
  };

} // end of namespace utils
} 
