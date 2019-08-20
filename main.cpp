#include <iostream>
#include <vector>
#include <ctime>
#include <Windows.h>
#include <memory>

class Subject;

class Observer
{
public:
	virtual ~Observer() = default;

	virtual void Update(Subject* ) = 0;
};


class Subject
{
public:
	virtual ~Subject() = default;

	virtual void Attach(Observer* o);
	virtual void Detach(Observer* o);
	virtual void Notify();
private:
	std::vector<Observer*> observers;
};

void Subject::Attach(Observer* o)
{
	if (o != nullptr)
	{
		observers.push_back(o);
	}
}

void Subject::Detach(Observer* o)
{
	auto it = find(observers.begin(), observers.end(), o);
	if (o != nullptr || it != observers.end())
	{
		observers.erase(it);
	}
}

void Subject::Notify()
{
	for (auto observer : observers)
	{
		observer->Update(this);
	}
}


class Clock:public Subject
{
public:
	explicit Clock() = default;

	void Time();
	time_t GetTime() const noexcept;
	
private:
	time_t myTime;
};

void Clock::Time()
{
	srand(static_cast<unsigned int>(time(NULL)));
	int n = (rand() % 10) + 3;

	for (int i = 0; i < n; ++i)
	{
		int number = rand() % 10;
		
		if (number == 0) 
		{
		}
		else if (number % 2 == 0)
		{
			Sleep(3000);
			myTime = time(NULL);
			Notify();
		}
		else
		{
			Sleep(2000);
			myTime = time(NULL);
			Notify();
		}
		Sleep(100);
	}
	
}

time_t Clock::GetTime() const noexcept
{
	return myTime;
}


class CurrentClock :public Observer
{
public:
	explicit CurrentClock(Clock* s);
	virtual ~CurrentClock();

	virtual void Update(Subject*) override;
private:
	Clock* subject;
	time_t clockTime;
};


CurrentClock::CurrentClock(Clock* s):subject(s)
{
	subject->Attach(this);
}

CurrentClock ::~CurrentClock()
{
	subject->Detach(this);
}

void CurrentClock::Update(Subject* s)
{
	if (s == subject)
	{
		clockTime = subject->GetTime();
		char str[26];
		ctime_s(str, 26, &clockTime);
		std::cout << "System's date/time is " << str << std::endl;
	}
}



int main(int argc, char *argv[])
{
	Clock* clock = new Clock;
	CurrentClock* currClock = new CurrentClock(clock);

	clock->Time();

	system("pause");
	return 0;
}