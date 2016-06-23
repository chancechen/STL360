// Copyright (c) 2016 chen
// All rights reserved.
// desc : time class
// version    :  0.0.1
// author     :  chen
// email      : cy_chance@hotmail.com
// createtime : 2016/6/15
#pragma once
#include "common.h"

namespace chen {
	namespace common {	

		typedef struct tm Date;

		class Time {
		public:	
			static uint64_t Tick();
			static void Sleep(uint32_t ms);
			static Time Now();
			static tm* ConverToTm(uint64_t sec);

			Time(int year, int month, int day, int hour, int min, int sec);

			explicit Time(uint64_t ms) {
				second_ = ms / 1000;
				millisecond_ = (ms % 1000) * 1000000;
			}
			
			Time(time_t sec, uint64_t nsec) {
				second_ = sec;
				millisecond_ = nsec;
			}
			
			void Date(struct Date *d);			
			size_t Format(const char *fmt, char *timestr, size_t len);
			size_t LocalTime(char *timestr, size_t len) { 	return Format("%H:%M:%S", timestr, len);	}
			size_t LocalDateTime(char *timestr, size_t len) { return Format("%Y-%m-%d[%H:%M:%S]", timestr, len);	}

			uint64_t Second() { return second_;}
			uint64_t MilliSecond() {	return millisecond_;	}
			uint64_t UTCSeconde( uint32_t  utc=8) { return second_ + utc*60*60; }
		protected:			
			time_t second_;
			time_t millisecond_;
			Time() : second_(0), millisecond_(0) {}
		};
	}
}