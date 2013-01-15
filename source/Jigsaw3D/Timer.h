#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
	Timer();

	void update();
	void reset();

	double getSeconds() const;
	double getTotalSeconds() const;
	double getMilliseconds() const;
	double getTotalMilliseconds() const;
	double getMicroseconds() const;
	double getTotalMicroseconds() const;

private:
#if defined(_WIN32)
	typedef double timerType;
#endif

	// Time in miliseconds.
	timerType m_previousTicks;
	timerType m_totalTicks;
	timerType m_ticks;

#if defined(_WIN32)
	static void initTime();
	static const double ms_frequency;
#endif
};

#endif
