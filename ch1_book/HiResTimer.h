#ifndef __TIMER_H_INCLUDED__
#define __TIMER_H_INCLUDED__

//#include <windows.h>


class CHiResTimer
{
public:
  CHiResTimer() {}
  ~CHiResTimer() {}

/*****************************************************************************
 Init()

 If the hi-res timer is present, the tick rate is stored and the function
 returns true. Otherwise, the function returns false, and the timer should
 not be used.
*****************************************************************************/
bool Init()
{
  if (!QueryPerformanceFrequency(&m_ticksPerSecond))
  {
    // system doesn't support hi-res timer
    return false;
  }
  else
  {
    QueryPerformanceCounter(&m_startTime);
    return true;
  }
} // end Init()


float GetElapsedSeconds(unsigned long elapsedFrames = 1)
{
  static LARGE_INTEGER s_lastTime = m_startTime;
  LARGE_INTEGER currentTime;

  QueryPerformanceCounter(&currentTime);

  float seconds =  ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart) / (float)m_ticksPerSecond.QuadPart;

  // reset the timer
  s_lastTime = currentTime;

  return seconds;
} // end GetElapsedSeconds()


/***************************************************************************
 GetFPS()

 Returns the average frames per second over elapsedFrames, which defaults to
 one. If this is not called every frame, the client should track the number
 of frames itself, and reset the value after this is called.
***************************************************************************/
float GetFPS(unsigned long elapsedFrames = 1)
{
  static LARGE_INTEGER s_lastTime = m_startTime;
  LARGE_INTEGER currentTime;

  m_totalFrames += elapsedFrames;

  QueryPerformanceCounter(&currentTime);

  float fps = (float)elapsedFrames * (float)m_ticksPerSecond.QuadPart / ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart);

  // reset the timer
  s_lastTime = currentTime;

  return fps;
} // end GetFPS


/***************************************************************************
 GetAggregateFPS()

 Returns the average fps since the last time Reset() was called. Note that
 this is meant to be used in conjunction with LockFPS or GetFPS. Otherwise,
 the function has no way of knowing how many frames have actually elapsed.
***************************************************************************/
float GetAggregateFPS()
{
  LARGE_INTEGER currentTime;

  QueryPerformanceCounter(&currentTime);

  return ((float) m_totalFrames * (float)m_ticksPerSecond.QuadPart / ((float)currentTime.QuadPart - (float)m_startTime.QuadPart));
} // end GetAggregateFPS()


/***************************************************************************
 LockFPS()

 Used to lock the frame rate to a set amount. This will block until enough
 time has passed to ensure that the fps won't go over the requested amount.
 Note that this can only keep the fps from going above the specified level;
 it can still drop below it. It is assumed that if used, this function will
 be called every frame. The value returned is the instantaneous fps, which
 will be <= targetFPS.
***************************************************************************/
float LockFPS(unsigned char targetFPS)
{
  if (targetFPS == 0)
    targetFPS = 1;

  m_totalFrames++;

  static LARGE_INTEGER s_lastTime = m_startTime;
  LARGE_INTEGER currentTime;
  float   fps;

  // delay to maintain a constant frame rate
  do {
    QueryPerformanceCounter(&currentTime);
    fps = (float)m_ticksPerSecond.QuadPart/((float)(currentTime.QuadPart - s_lastTime.QuadPart));
  } while (fps > (float)targetFPS);

  // reset the timer
  s_lastTime = m_startTime;

  return fps;
} // end LockFPS()


private:
  LARGE_INTEGER   m_startTime;
  LARGE_INTEGER   m_ticksPerSecond;
  unsigned long   m_totalFrames;
};

#endif // __TIMER_H_INCLUDED_