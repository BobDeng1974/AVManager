
#ifndef FILE_UTILS_H_
#define FILE_UTILS_H_

#include <stdio.h>
#include <direct.h>




#if defined(UNICODE) || defined(_UNICODE)
#define get_ext_name get_ext_nameW
#define get_file_name get_file_nameW
#else
#define get_ext_name get_file_nameA
#define get_file_name get_file_nameA
#endif



#ifdef _MSC_VER
#include <sys/stat.h>
#else
 #include <unistd.h>
#endif
namespace utility
{

	static bool is_file(const char* path)
	{
#ifdef _MSC_VER
		struct _stat64 buf;
		if (_stat64(path, &buf) == -1) {
			return false;
		} else {
			if ( buf.st_mode & _S_IFREG ) {		
				return true;
			}   
		}
#else
		struct stat64 buf;
		if (stat64(path, &buf) == -1) {
			return false;
		} else {
			if (buf.st_mode & _S_IFREG ) {
				return true;
			}
		}
#endif
		return false;
	}

	static bool is_dir(const char* path)
	{
#ifdef _MSC_VER
		struct _stat64 buf;
		if (_stat64(path, &buf) == -1) {
			return false;
		} else {
			if (buf.st_mode & _S_IFDIR) {		
				return true;
			}   
		}
#else
		struct stat64 buf;
		if (stat64(path, &buf) == -1) {
			return false;
		} else {
			if (buf.st_mode & _S_IFDIR) {
				return true;
			}
		}
#endif

		return false;
	}

	static void make_dir(const char* path)
	{
#ifdef _MSC_VER
		if (strlen(path) <= 3)
			return;
#else
		if (strlen(path) <= 1)
			return;
#endif
		if (is_dir(path)) return;

		char szpath[260];
		strcpy_s(szpath, path);

		char* pos = strrchr(szpath, '\\');
		if (pos == 0)
		{
			pos = strrchr(szpath, '/');
		}

		if (pos)
			*pos = 0;

#ifdef _MSC_VER
		if (strlen(szpath) <= 2)
			strcat(szpath, "\\");

		make_dir(szpath);

		if (!is_dir(path)) _mkdir(path);
#else
		if (strlen(szpath) <= 2)
			strcat(szpath, "\\");

		make_dir(szpath);

		if (!make_dir(path)) mkdir(path, S_IRWXU);
#endif

	}

	static __int64 get_file_size(const char* filename)
	{
		int file_exists = 1;

#ifdef _MSC_VER
		struct _stat64 buf;
		if (_stat64(filename, &buf) == -1) {
			file_exists = 0;
		} else {
			if (buf.st_mode & _S_IFDIR) {		
				file_exists = 0;
			}   
		}
#else
		struct stat64 buf;
		if (stat64(filename, &buf) == -1) {
			file_exists = 0;
		} else {
			if (buf.st_mode & _S_IFDIR) {
				file_exists = 0;
			}
		}

#endif

		if (!file_exists)
			return 0;

		return buf.st_size;
	}

	static const char* get_file_nameA(const char* fullname)
	{
		const char *pos, *pos1, *pos2;
		pos1 = strrchr(fullname, '\\');
		pos2 = strrchr(fullname, '/');
		pos = pos1 > pos2 ? pos1 : pos2;

		return pos == NULL ? fullname : &pos[1];
	}

	static const char* get_ext_nameA(const char* fullname)
	{
		const char* pos = strrchr(fullname, '.');

		return pos == NULL ? "" : &pos[1];
	}

	static const wchar_t* get_file_nameW(const wchar_t* fullname)
	{
		const wchar_t *pos, *pos1, *pos2;
		pos1 = wcsrchr(fullname, '\\');
		pos2 = wcsrchr(fullname, '/');
		pos = pos1 > pos2 ? pos1 : pos2;

		return pos == NULL ? fullname : &pos[1];
	}

	static const wchar_t* get_ext_nameW(const wchar_t* fullname)
	{
		const wchar_t* pos = wcsrchr(fullname, '.');

		return pos == NULL ? L"" : &pos[1];
	}


}



#endif //FILE_UTILS_H_
