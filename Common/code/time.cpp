#include "../include/time.h"
#include <sysinfoapi.h>
#include <synchapi.h>

namespace chen {
	namespace common {
		tm* Time::ConverToTm(uint64_t sec) {
			time_t rawtime = sec;
			if (sec == 0) {
				time(&rawtime);
			}
			tm* time = nullptr;
			gmtime_s(time, &rawtime);
			return time;
		}

		Time Time::Now() {			
			int64_t filetime;
			GetSystemTimeAsFileTime((LPFILETIME)&filetime);
			Time t;
			// Filetime is 100s of nanoseconds since January 1, 1601.
			// Convert to nanoseconds since January 1, 1970.
			uint64_t nano = (filetime - 116444736000000000LL) * 100LL;
			t.second_ = nano / 1000000000;
			t.millisecond_ = nano - t.second_ * 1000000000;
			return t;
		}

		Time::Time(int year, int month, int day, int hour, int min, int sec) {
			struct tm tm;
			tm.tm_year = year;
			tm.tm_mon = month;
			tm.tm_mday = day;
			tm.tm_hour = hour;
			tm.tm_min = min;
			tm.tm_sec = sec;

			second_ = mktime(&tm);
			millisecond_ = 0;
		}

		Time::Time(struct tm* d){
			second_ = mktime(d);
			millisecond_ = 0;
		}

		uint64_t Time::Tick() {
			return GetTickCount();
		}

		void Time::Sleep(uint32_t ms) {
			::Sleep(ms);
		}

#if 0
		uint64_t Time::UnixMSec() {
			return sec_ * 1000 + nsec_ / 1000000;
		}

		uint64_t Time::UnixNano() {
			return sec_ * 1000000000 + nsec_;
		}
#endif

		void Time::Date(struct tm* d) {
			localtime_s((struct tm *)d, &second_);
			
		}

		size_t Time::Format(const char *fmt, char *timestr, size_t len) {
			struct tm tm;
			localtime_s(&tm, &second_);
			return strftime(timestr, len, fmt, &tm) > 0;
		}
	}
}