#include "../include/file.h"
#include <varargs.h>
namespace chen {
	namespace common {

		bool File::IsFileExist(const char* filename) {
			return (_access(filename, 0) == 0);
		}

		bool File::Rename(const char* oldname, const char* newname) {
			return  rename(oldname, newname) == 0;
		}

		FileImpl::FileImpl():file_handle_(nullptr) {
		}

		FileImpl::~FileImpl() {
		}

		int  FileImpl::OpenFile(const char *path, const char *mode) {
			FILE* handle = nullptr;
			int err = fopen_s(&handle, path, mode);
			if (err) {
				return (-err);
			}
			Close();
			file_handle_ = handle;
			return 0;
		}

		void FileImpl::Close() {
			if (file_handle_) {
				fclose(file_handle_);
				file_handle_ = nullptr;
			}
		}

		size_t FileImpl::Read(char *buf, size_t len) {			
			return fread_s(buf, len, sizeof(char), len, file_handle_);
		}

		int FileImpl::ReadLine(char *buf, int len) {
			if (fgets(buf, len, file_handle_)) {
				return (int)strlen(buf);
			}
			else {
				return ferror(file_handle_) == 0 ? 0 : -1;				
			}
		}

		size_t FileImpl::Write(const char *buf, size_t len) {			
			return fwrite(buf, sizeof(char), len, file_handle_);
		}

		size_t FileImpl::Write(const char* fmt, va_list ap) {
			return vfprintf(file_handle_, fmt, ap);
		}

		size_t FileImpl::Write(const char* fmt, ...) {
			va_list ap;
			va_start(ap);
			auto res = Write(fmt, ap);
			va_end(ap);
			return res;
		}

		size_t FileImpl::GetSize() {			
			return  _filelength(_fileno(file_handle_));
		}

		bool FileImpl::IsEof() {
			return (ferror(file_handle_) != 0);
		}

		bool FileImpl::IsError() {
			return (feof(file_handle_) != 0);
		}

		int FileImpl::Seek(size_t offset, int origin) {
			return _fseeki64(file_handle_, offset, origin);
		}

		int FileImpl::Sync() {
			return  fflush(file_handle_);
		}
	}
}