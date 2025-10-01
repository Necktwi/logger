/* 
 * Gowtham Kudupudi 20/08/2016
 * MIT License
 */
                                                                                
#include "logger.h"
#include <cstdarg>
#include <stdio.h>
#if defined(unix) || defined(__unix__) || defined(__unix)
#include <unistd.h>
#elif defined(_WIN64) || defined(_WIN32)
#include <io.h>
#endif
#include <sys/types.h>
#ifdef __CYGWIN__
#include <pthread.h>
#elif __APPLE__
#include <thread>
#elif _WIN64 || _WIN32
#include <Windows.h>
#else
#include <sys/syscall.h>
#endif
#include <stdlib.h>
#include <ferrybase/FerryTimeStamp.h>

/**
 * Log level count
 */
#define FFLT_COUNT 10
static const char * const log_type_names[] = {
	"ERR",
	"WRN",
	"NTC",
	"DBG",
	"INF",
	"PSR",
	"HDR",
	"EXT",
	"CLN",
	"LNC",
};

/*int _ff_log(_ff_log_type t, const char* format, ...) {
	char buf[300];
	va_list ap;
	if (!(ff_log_type & t))
		return 0;

	va_start(ap, format);
	vsnprintf(buf, sizeof (buf), format, ap);
	buf[sizeof (buf) - 1] = '\0';
	va_end(ap);

	char buf2[300];
	unsigned long long now;
	int n;

	buf2[0] = '\0';
	for (n = 0; n < FFLT_COUNT; n++)
		if (t == (1 << n)) {
			//now = time_in_microseconds() / 100;
			sprintf(buf2, "[%s %s]: ", (const char*) getuTime().c_str(), log_type_names[n]);
			break;
		}

	fprintf(stderr, "%s%s\n", buf2, buf);
	fflush(stderr);
	return 0;
};*/

int _ff_log(const char* func, FF_LOG_TYPE t,
            unsigned int l, const char* format, ...) {
	char buf[1000];
	unsigned int nnl = 1 << 31;
	bool no_new_line = (l & nnl) == nnl;
	l &= ~nnl;
	va_list ap;

    if (!(fflAllowedType & t) || !(fflAllowedBlks & l))
		return 0;

	va_start(ap, format);
	vsnprintf(buf, sizeof (buf), format, ap);
	buf[sizeof (buf) - 1] = '\0';
	va_end(ap);

	char buf2[300];
	int n;
	buf2[0] = '\0';
    FerryTimeStamp fTS;
    fTS.Update();
	for (n = 0; n < FFLT_COUNT; n++)
		if (t == (1 << n)) {
			//now = time_in_microseconds() / 100;
			sprintf(buf2, "[%s %s %s]: ", (const char*) fTS.GetTime().c_str(),
					log_type_names[n], func);
			break;
		}
	if (no_new_line) {
		fprintf(stderr, "%s%s", buf2, buf);
	} else {
		fprintf(stderr, "%s%s\n", buf2, buf);
	}
	fflush(stderr);
	return 0;
};

int _ff_log (FF_LOG_TYPE t, unsigned int l, const char* func,
  const char* file_name, int line_no, const char* format, ...
) {
	char buf[1000];
	unsigned int nnl = 1 << 31;
	bool no_new_line = (l & nnl) == nnl;
	l &= ~nnl;
	va_list ap;

  if (!(fflAllowedType & t) || !(fflAllowedBlks & l))
		return 0;
  va_start(ap, format);
	vsnprintf(buf, sizeof (buf), format, ap);
	buf[sizeof (buf) - 1] = '\0';
	va_end(ap);

	char buf2[300];
	int n;
	buf2[0] = '\0';
  FerryTimeStamp fTS;
  fTS.Update();
  for (n = 0; n < FFLT_COUNT; n++)
		if (t == (1 << n)) {
			//now = time_in_microseconds() / 100;
			sprintf(buf2, "[%s %s %05ld %s:%s:%d]: ",
        (const char*) fTS.GetUTime().c_str(), log_type_names[n],
#ifdef __CYGWIN__
			  pthread_self(),
#elif __APPLE__
        std::hash<std::thread::id>()(std::this_thread::get_id()),
#elif _WIN64 || _WIN32
        GetCurrentThreadId(),
#else
			  syscall(SYS_gettid),
#endif
			  func, file_name, line_no
      );
			break;
		}

  if (no_new_line) {
		fprintf(stderr, "%s%s", buf2, buf);
	} else {
		fprintf(stderr, "%s%s\n", buf2, buf);
	}
	fflush(stderr);
	return 0;
};

int _ff_log_contnu(FF_LOG_TYPE t,
		unsigned int l,
		const char* format,
		...) {
	char buf[1000];
	va_list ap;

	if (!(fflAllowedType & t) || !(fflAllowedBlks & l))
		return 0;

	va_start(ap, format);
	vsnprintf(buf, sizeof (buf), format, ap);
	buf[sizeof (buf) - 1] = '\0';
	va_end(ap);
   fprintf(stderr, "%s", buf);
	fflush(stderr);
	return 0;
};

bool _ffl_level(FF_LOG_TYPE t,
		unsigned int l) {
	if (((fflAllowedType & t) == t)&&((fflAllowedBlks & l) == l)) {
		return true;
	}
	return false;
};

//void SetLogType(FF_LOG_TYPE t) {
//	fflAllowedType = t;
//}
//
//void SetLogLevel(unsigned int l) {
//	fflAllowedBlks = l;
//}

void PrintLogTypeNLevel(){
    printf("fflAllowedType: %08X, fflAllowedBlks: %08X\n", (unsigned int)fflAllowedType, (unsigned int)fflAllowedBlks);
}
