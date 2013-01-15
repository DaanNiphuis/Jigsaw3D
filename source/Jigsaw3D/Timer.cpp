#include "Timer.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

double getFrequency()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return static_cast<double>(frequency.QuadPart);
}

const double Timer::ms_frequency = getFrequency();

Timer::Timer()
{
	reset();
}

void Timer::update()
{
	LARGE_INTEGER tempTicks;
	QueryPerformanceCounter(&tempTicks);
	double ticks = tempTicks.QuadPart / ms_frequency;
	m_ticks = ticks - m_previousTicks;
	m_previousTicks = ticks;
	m_totalTicks += m_ticks;
}

void Timer::reset()
{
	m_totalTicks = 0;
	m_ticks = 0;
	LARGE_INTEGER tempTicks;
	QueryPerformanceCounter(&tempTicks);
	m_previousTicks = tempTicks.QuadPart / ms_frequency;
}

double Timer::getSeconds() const 
{
	return static_cast<float>(m_ticks);
}

double Timer::getTotalSeconds() const 
{
	return m_totalTicks;
}

double Timer::getMilliseconds() const 
{
	return m_ticks * 1000.0;
}

double Timer::getTotalMilliseconds() const 
{
	return m_totalTicks * 1000.0;
}

double Timer::getMicroseconds() const 
{
	return m_ticks * 1000000.0;
}

double Timer::getTotalMicroseconds() const 
{
	return m_totalTicks * 1000000.0;
}

