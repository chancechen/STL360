// Copyright (c) 2016 chen
// All rights reserved.
// desc : file logic
// version    :  0.0.1
// author     :  chen
// email      : cy_chance@hotmail.com
// createtime : 2016/6/15
#pragma once
#include "common.h"
namespace chen {
	namespace common {

		class File {
		public:
			virtual int    ReadLine(char *buf, int len) = 0;
			virtual size_t Read(char *buf, size_t len) = 0;
			virtual size_t Write(const char *buf, size_t len) = 0;
			virtual size_t Write(const char* fmt, va_list ap) = 0;
			virtual size_t Write(const char* fmt, ...) = 0;
			virtual size_t GetSize() = 0;
			virtual bool IsEof() = 0;
			virtual bool IsError() = 0;
			virtual int  Seek(size_t offset, int origin) = 0;
			virtual int  Sync() = 0;
			static bool	IsFileExist(const char* filename);
			static bool	Rename(const char* oldname, const char* newname);
		protected:
			File() {}
			virtual ~File() {}
		};

		class FileImpl : public File {
		public:
			FileImpl();
			virtual ~FileImpl();

		public:
			int  OpenFile(const char *path, const char *mode);
			void Close();

		public:
			virtual int ReadLine(char *buf, int len);
			virtual size_t Read(char *buf, size_t len);
			virtual size_t Write(const char *buf, size_t len);
			virtual size_t Write(const char* fmt, va_list ap);
			virtual size_t Write(const char* fmt, ...);
			virtual size_t GetSize();

			virtual bool IsEof();
			virtual bool IsError();

			virtual int  Seek(size_t offset, int origin);
			virtual int  Sync();

		protected:
			FILE*   file_handle_;
		};

	}
}