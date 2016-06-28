#include "../include/log.h"
namespace chen {
	namespace common {
		char LOG_TITLE[][32] = { "XXX", "DBG", "WRN", "ERR", "FTL", "ACT" , "PLY", "XXX", "XXX", "INF", "XXX" };
		FileLog::FileLog() : current_size_(0)	, size_limit_(0), max_file_num_(1) {
			file_ = NULL;
			print_ = false;
			//lock_ = new MutexLock();
		}

		FileLog::FileLog(File* f) : current_size_(0), size_limit_(0), max_file_num_(1) {
			file_ = f;
			print_ = false;
			// TODO lock
			//lock_ = new MutexLock();
		}

		void FileLog::SetLevel(uint32_t log_level) {
			log_level_ = log_level;
		}

		uint32_t FileLog::GetLevel() {
			return log_level_;
		}

		const char* FileLog::Level(uint32_t level) {
			if (level < 10)
				return LOG_TITLE[level];
			return "XXX";
		}

		void FileLog::SetConsole(bool print) {
			print_ = print;
		}

		size_t FileLog::Log(uint32_t log_level, const char* fmt, ...) {
			Time now = Time::Now();
			va_list ap;
			va_start(ap);
			size_t res = Log(log_level, &now, fmt, ap);
			va_end(ap);
			return res;
		}

		size_t FileLog::Log(uint32_t log_level, const char* fmt, va_list ap) {
			Time now = Time::Now();
			return Log(log_level, &now, fmt, ap);
		}

		size_t FileLog::LogNoTime(uint32_t log_level, const char* fmt, ...) {
			va_list ap;
			va_start(ap);
			size_t res = Log(log_level, NULL, fmt, ap);
			va_end(ap);
			return res;
		}

		size_t FileLog::LogNoTime(uint32_t log_level, const char* fmt, va_list ap) {
			return Log(log_level, NULL, fmt, ap);
		}

		size_t FileLog::Log(uint32_t log_level, Time* t, const char* fmt, va_list ap) {
			if (log_level < log_level_)
				return 0;

			// shift at first in case of the newfile is empty
			//lock_->Lock();
			ShiftFile(t);
			// lock_->Unlock();

			char logbuf[kMaxLogSizePerLine] = { 0 };
			size_t ret = 0;
			if (t != NULL) {
				Date date;
				t->Date(&date);

				if (log_level == 5 || log_level == 6) // action && player log
				{
					snprintf(logbuf, sizeof(logbuf), "%04d-%02d-%02d %02d:%02d:%02d; "
						, date.tm_yday + 1900
						, date.tm_mon + 1
						, date.tm_mday
						, date.tm_hour
						, date.tm_min
						, date.tm_sec);
				}
				else
				{
					snprintf(logbuf, sizeof(logbuf), "[%04d-%02d-%02d %02d:%02d:%02d.%llu] [%s] "	,
						date.tm_yday + 1900,
						date.tm_mon + 1	,
						date.tm_mday	,
						date.tm_hour ,
						date.tm_min ,
						date.tm_sec ,
						(t->MilliSecond() / 1000) ,
						Level(log_level) );
				}

				ret = strlen(logbuf);
			}

			vsnprintf(logbuf + ret, sizeof(logbuf) - ret, fmt, ap);

			// lock_->Lock();
			ret = file_->Write(logbuf, strlen(logbuf));
			if (ret > 0) {
				file_->Sync();
				current_size_ += ret;
			}
			//lock_->Unlock();

			if (print_) {
				OutputFmtInfo(logbuf);
			}

			return 0;
		}

		int FileLog::OpenLogFile(const char* filename) {
			file_ = FileOpen(filename, "at+");
			if (file_ == NULL) {
				OutputFmtInfo("FileLog Open %s error\n", filename);
				return -1;
			}

			current_size_ = file_->GetSize();
			return 0;
		}

		void FileLog::Close() {
			if (file_) {
				FileClose(file_);
				file_ = NULL;
			}
		}
		const int kMaxSuffixSize = 16;
	
		int DailyLog::Init(const char* path, int log_level, size_t file_size) {
			size_t len = strlen(path);
			assert(len <= kMaxLogSize);
			assert(file_size >= kMinLogSize &&file_size <= kMaxLogSize);			
			strncpy_s(file_path_, path, kMaxLogPath);
			log_level_ = log_level;
			size_limit_ = file_size;
			current_size_ = 0;
			int ret = ShiftFile(NULL);
			if (ret != 0) {
				return ret;
			}
			return 0;
		}

		int DailyLog::ShiftFile(Time* t) {
			bool need_new_file = false;
			Date date;

			if (t == NULL) {
				Time tt = Time::Now();
				t = &tt;
			}

			t->Date(&date);
			if (date.tm_year != now_date_.tm_year ||date.tm_mon != now_date_.tm_mon ||date.tm_mday != now_date_.tm_mday) {
				OutputFmtInfo("other day[%04d-%02d-%02d], other logfile\n", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
				now_date_ = date;
				need_new_file = true;
			}

			if (current_size_ > size_limit_) {
				OutputFmtInfo("currentsize:%u > size_limit:%u, need new log\n", current_size_, size_limit_);
				need_new_file = true;
			}

			if (!need_new_file)
				return 1;

			char   newfile[512] = { 0 };
			char   newpath[512] = { 0 };		
			snprintf(newpath, sizeof(newpath)	, "%s%c%04d%02d%02d"	, file_path_, SI_PATH_SEPRATOR, date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
			if (PathIsExist(newpath) == false) {
				MakeDirectory(newpath);
			}
			snprintf(newfile, sizeof(newfile)	, "%s%c%02d-%02d-%02d.%llu.log", newpath, SI_PATH_SEPRATOR, date.tm_hour, date.tm_min, date.tm_sec	, (t->MilliSecond() / 1000));
			Close();
			return OpenLogFile(newfile);
		}
	}
}
