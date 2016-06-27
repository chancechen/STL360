// Copyright (c) 2016 chen
// All rights reserved.
// desc : log logic
// version    :  0.0.1
// author     :  chen
// email      : cy_chance@hotmail.com
// createtime : 2016/6/15
#pragma once
#include "common.h"
#include "file.h"
#include "singleton.h"
#include <varargs.h>
namespace chen {
	namespace common {

		const size_t kMinLogSize = 16 * 1024;
		const size_t kMaxLogSize = 32 * 1024 * 1024;
		const size_t kMaxLogPath = 240;
		const size_t kMaxLogSizePerLine = 2048;

		char LOG_TITLE[][32] = { "XXX", "DBG", "WRN", "ERR", "FTL", "ACT" , "PLY", "XXX", "XXX", "INF", "XXX" };
#define LOG_AP(level) \
	va_list gap; \
	va_start(gap); \
	DailyLog::Instance().Log(level, fmt, gap); \
	va_end(gap)

		enum eLog {
			LOG_DEBUG = 1,
			LOG_WARN = 2,
			LOG_ERROR = 3,
			LOG_FATAL = 4,
			LOG_INFO = 9,
		};

		class FileLog {
		public:
			explicit FileLog(File *f);
			virtual ~FileLog()
			{
			/*	if (lock_)
					delete lock_;*/
			}

			void Flush() {
				file_->Sync();
			}

			void        SetLevel(uint32_t log_level);
			uint32_t    GetLevel();
			const char* Level(uint32_t level);

			void SetConsole(bool print);

			size_t  Log(uint32_t log_level, const char* fmt, ...);
			size_t  Log(uint32_t log_level, const char* fmt, va_list ap);
			size_t  LogNoTime(uint32_t log_level, const char* fmt, ...);
			size_t  LogNoTime(uint32_t log_level, const char* fmt, va_list ap);

		protected:
			FileLog();
			size_t  Log(uint32_t log_level, Time* t, const char* fmt, va_list ap);
			int  OpenLogFile(const char* filename);
			void Close();

		protected:
			virtual int ShiftFile(Time* t) = 0;

		protected:
			size_t		 current_size_;
			size_t		 size_limit_;
			uint32_t	 max_file_num_;
			uint32_t	 log_level_;
			char			 file_path_[512];
			File*      file_;
			//MutexLock* lock_;

			bool      print_;
		};


		class DailyLog : public FileLog	, public AutoSingleton<DailyLog> {
		public:
			DailyLog() { now_date_.tm_year = 0; }
			~DailyLog() { Close(); }
			int  Init(const char* path	, int log_level, size_t single_file_size = 10485760);
		protected:
			int  ShiftFile(Time* t);

		protected:
			Date now_date_;
		};

		class log {
		public:
			log();
			~log();
			static void Init(int level, const char* filepath, uint32_t file_size, uint32_t file_num = 0, bool print = false) {
				//DailyLog::Instance().Init(filepath, level, file_size);
				//DailyLog::Instance().SetConsole(print);
			}

			static void ActionInit(int level, const char* filepath
				, uint32_t file_size, uint32_t file_num = 0
				, bool print = false) {
				//ActionLog::Instance().Init(filepath, 5, level, file_size);
				//ActionLog::Instance().SetConsole(print);
			}

			static void LogDebug(const char* fmt, ...) {
				LOG_AP(LOG_DEBUG);
			}
			static void LogWarn(const char* fmt, ...) {
				LOG_AP(LOG_WARN);
			}
			static void LogError(const char* fmt, ...) {
				LOG_AP(LOG_ERROR);
			}
			static void LogFatal(const char* fmt, ...) {
				LOG_AP(LOG_FATAL);
			}
			static void LogInfo(const char* fmt, ...) {
				LOG_AP(LOG_INFO);
			}
		};
	}
}