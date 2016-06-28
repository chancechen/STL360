#include "../include/file.h"
#include <varargs.h>
#include <shlwapi.h>
#include <direct.h>
namespace chen {
	namespace common {

		bool FileExists(const std::string &Filename)
		{
			return _access(Filename.c_str(), 0) == 0;
		}
		void OutputInfo(const char * info) {
			assert(info);
			if (info == NULL)
				return;
			printf("%s\n", info);
		}

		void OutputFmtInfo(const char * format, ...) {
			assert(format);
			if (format == NULL)
				return;			
			va_list args;
			va_start(args, format);
			int ret = vfprintf(stderr, format, args);
			va_end(args);
			if (ret == -1)
				return;
		}

		bool PathIsExist(const char * path) {
			assert(path);
			if (path)
				return (FileExists(path) != FALSE);
			else
				return false;
		}
		File * FileOpen(const char *path, const char *mode) {
			FileImpl *f = new FileImpl;
			if (f) {
				if (f->OpenFile(path, mode) < 0) {
					delete f;
					return NULL;
				}
			}
			return f;
		}
		bool MakeDirectory(const char * path, bool is_hide/* = false*/) {
			if (!path)
				return false;
			if (FileExists(path))
				return true;
			std::string directory_path(path);
			if (directory_path.size() == 0) {
				return true;
			}
			auto segment_offset = directory_path.rfind(SI_PATH_SEPRATOR);
			if (segment_offset != directory_path.npos) {
				std::string parent_path = directory_path.substr(0, segment_offset);
				if (!FileExists(parent_path.c_str())	&& !MakeDirectory(parent_path.c_str(), is_hide)) {
					return false;
				}
			}
			if (_mkdir(directory_path.c_str()) != 0) {
				assert(false);
				return false;
			}
			if (is_hide)
				SetFileAttributesA(directory_path.c_str(),
					(FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM));
			return true;
		}

		std::string& WorkDiretory() {
			char process_file[MAX_PATH];
			int  position = GetModuleFileNameA(NULL, process_file, sizeof(process_file));
			if (position == 0)
				return std::string("");
			while ((position > 0) && process_file[position] != SI_PATH_SEPRATOR)
				--position;
			process_file[position] = '\0';
			return std::string(process_file);
		}

		void FileClose(File *file) {
			assert(file);
			if (file) {
				FileImpl *f = reinterpret_cast<FileImpl *>(file);
				f->Close();
				delete f;
			}
		}

		bool File::IsFileExist(const char* filename) {
			return (_access(filename, 0) == 0);
		}

		bool File::Rename(const char* oldname, const char* newname) {
			return  rename(oldname, newname) == 0;
		}

		FileImpl::FileImpl():file_handle_(nullptr) {}
		FileImpl::~FileImpl() {}
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
			va_start(ap, fmt);
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