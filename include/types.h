#ifndef TYEPS_H_
#define TYPES_H_

#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif


#ifdef _WIN32
#define int8_t				char
#define int16_t				short
#define int32_t				int
#define int64_t				__int64
#define	uint8_t				unsigned char
#define uint16_t			unsigned short
#define uint32_t			unsigned int
#define uint64_t			unsigned __int64
#else
#define int8_t				char
#define int16_t				short
#define int32_t				int
#define int64_t				long long
#define	uint8_t				unsigned char
#define uint16_t			unsigned short
#define uint32_t			unsigned int
#define uint64_t			unsigned long long
#endif

#define int8				int8_t
#define uint8				uint8_t
#define int16				int16_t
#define uint16				uint16_t
#define int32				int32_t
#define uint32				uint32_t
//#define int64				int64_t
#define uint64				uint64_t

typedef int socklen_t;

#endif