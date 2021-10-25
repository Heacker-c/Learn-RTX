#ifndef _TIMER_H_
#define _TIMER_H_

#include <deque>
#include <iostream>

namespace CppUtil {
	namespace Basic {
		class Timer
		{
		public:
			enum ENUM_STATE
			{
				ENUM_STATE_INIT,
				ENUM_STATE_RUNNING,
				ENUM_STATE_STOP,
			};

			Timer(size_t maxLogNum = 32);

			bool Start();
			bool Stop();
			// ���� ���� ��ʾ����
			double Log();
			void Reset();

			double GetLog(size_t i) const;
			ENUM_STATE GetState() const;
			double GetWholeTime() const;
			std::ostream & operator <<(std::ostream & os);
		private:
			double GetCurTime() const;

			ENUM_STATE state;
			std::deque<double> logs;
			const size_t maxLogNum;
			double lastLogTime;
			double wholeTime;
		};
	}
}

#endif // !_TIMER_H_
