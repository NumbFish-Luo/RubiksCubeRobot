#ifndef TIMER_H
#define TIMER_H

// https://codereview.stackexchange.com/questions/40915/simple-multithread-timer


#include <thread>
#include <chrono>

class Timer
{
public:
	typedef std::chrono::milliseconds Interval; // 延时时间
	typedef std::function<void(void)> Timeout; // 定时执行的函数

	Timer(const Timeout &timeout);
	Timer(const Timeout &timeout,
		const Interval &interval,
		bool singleShot = true); // singleShot = true 即仅执行一次
	~Timer();

	void start(bool multiThread = false);
	void stop();

	bool running() const;

	void setSingleShot(bool singleShot);
	bool isSingleShot() const;

	void setInterval(const Interval &interval);
	const Interval &interval() const;

	void setTimeout(const Timeout &timeout);
	const Timeout &timeout() const;

private:
	std::thread _thread;

	bool _running = false;
	bool _isSingleShot = true;

	Interval _interval = Interval(0);
	Timeout _timeout = nullptr;

	void _temporize();
	void _sleepThenTimeout();
};

#endif // TIMER_H