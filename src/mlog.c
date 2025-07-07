#include "mlog.h"
#include "mlog.module.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define LOGLEVEL_MIN 1
#define LOGLEVEL_MAX 31
#define LOGLEVEL_ASSERT( loglevel ) if ( loglevel < LOGLEVEL_MIN || loglevel > LOGLEVEL_MAX ) return

#define DEFAULT_LOGGER_BUFFERNEW  calloc( DEFAULT_LOGGER_LENGTH, 1 )
#define DEFAULT_LOGGER_TIMEFORMAT "%F %T"
#define DEFAULT_LOGGER_MODULE     ""
#ifdef DEBUG
#define DEFAULT_LOGGER_LEVEL      LogFlagAll
#else
#define DEFAULT_LOGGER_LEVEL      LogFlagAllNoDbg
#endif
#define DEFAULT_LOGGER_LENGTH     9182

#define PREFIX_INFO    "INF"
#define PREFIX_WARNING "WRN"
#define PREFIX_ERROR   "ERR"
#define PREFIX_FATAL   "FTL"
#define PREFIX_DEBUG   "DBG"

static Logger _global_logger = {0};

static LogDescriptor _log_desc[31] = {
	{ PREFIX_INFO, Print, LogColorDefault, LogColorDefault, LogInfo, LogFlagInfo },
	{ PREFIX_WARNING, Print, LogColorYellow, LogColorDefault, LogWarning, LogFlagWarning },
	{ PREFIX_ERROR, Print, LogColorRed, LogColorDefault, LogError, LogFlagError },
	{ PREFIX_FATAL, Print, LogColorDefault, LogColorRed, LogFatal, LogFlagFatal },
	{ PREFIX_DEBUG, Print, LogColorCyan, LogColorDefault, LogDebug, LogFlagDebug },
	{ 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 },
	{ 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 },
	{ 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }
};

uint32_t RegisterLogLevel( LogDescriptor * p_log )  {
	for ( uint32_t i = 0; i < 31; ++i ) {
		uint32_t idx = (p_log->Level - 1 + i) % 31;
		if ( _log_desc[idx].Level == 0 ) {
			p_log->Level = idx + 1;
			p_log->LevelBit = (1 << idx);
			if ( !p_log->Prefix )
				p_log->Prefix = "";
			if ( !p_log->PrintFn )
				p_log->PrintFn = Print;
			_log_desc[idx] = *p_log;
			return p_log->Level;
		}
	}

	return 0;
}

void Print( Logger const * p_module, LogDescriptor const * p_desc, char const * p_message, void * p_args ) {
	if ( !p_module || !p_desc )
		return;

	if ( (p_module->Level & ( 1 << (p_desc->Level - 1))) == 0 )
		return;

	time_t t = time( NULL );
	struct tm * now = localtime( &t );
	char * buffer = p_module->Buffer;
	size_t cur = 0;

	buffer[cur++] = '[';
	cur += strftime( buffer + cur, p_module->Length - cur, p_module->TimeFormat, now );
	buffer[cur++] = ']';

	if ( p_desc->Prefix && p_desc->Prefix[0] != 0 ) {
		cur += (size_t)snprintf( buffer + cur, p_module->Length - cur, "[%s]", p_desc->Prefix );
	}

	if ( p_module->Prefix && p_module->Prefix[0] != 0 ) {
		cur += (size_t)snprintf( buffer + cur, p_module->Length - cur, "%s", p_module->Prefix );
	}

	cur += (size_t)snprintf( buffer + cur, p_module->Length - cur, ": " );

	if ( !p_message )
		p_message = "";

	cur += (size_t)vsnprintf( buffer + cur, p_module->Length - cur, p_message, *(va_list *)p_args );

	printf( "%s\033[0m\n", buffer );
}

void SetPrintForeColor( LogColor p_color ) {
	switch( (int)(p_color > 9) ) {
	case 0:
		printf( "\033[22;3%dm", p_color );
		break;
	default:
		printf( "\033[1;3%dm", p_color % 10 );
		break;
	}
}

void SetPrintBackColor( LogColor p_color ) {
	switch( (int)(p_color > 9) ) {
	case 0:
		printf( "\033[22;4%dm", p_color );
		break;
	default:
		printf( "\033[1;4%dm", p_color % 10 );
		break;
	}
}

void ResetPrintColor( void ) {
	printf( "\033[0m" );
}

void LogInitGlobal( void ) {
	_global_logger = (Logger){
		.Buffer     = DEFAULT_LOGGER_BUFFERNEW,
		.TimeFormat = DEFAULT_LOGGER_TIMEFORMAT,
		.Prefix     = DEFAULT_LOGGER_MODULE,
		.Level      = DEFAULT_LOGGER_LEVEL,
		.Length     = DEFAULT_LOGGER_LENGTH
	};
}

Logger const * LogGetGlobal( void ) {
	return &_global_logger;
}

void LogSetLevel( LogLevelFlag p_level ) {
	LogSetLevelEx( &_global_logger, p_level );
}

void LogSetLength( uint32_t p_length ) {
	LogSetLengthEx( &_global_logger, p_length );
}

void LogSetTimeFormat( char const * p_time_format ) {
	LogSetTimeFormatEx( &_global_logger, p_time_format );
}

void LogSetPrefix( char const * p_prefix ) {
	LogSetPrefixEx( &_global_logger, p_prefix );
}

void Log( LogLevel p_level, char const * p_message, ... ) {
	LOGLEVEL_ASSERT( p_level );

	LogDescriptor const * desc = &_log_desc[p_level - 1];
	if ( (desc->LevelBit & _global_logger.Level) == 0 )
		return;

	SetPrintForeColor( desc->Foreground );
	SetPrintBackColor( desc->Background );
	va_list args;
	va_start( args, p_message );
	desc->PrintFn( &_global_logger, desc, p_message, args );
	va_end( args );
}

Logger LoggerCreate( LogLevelFlag p_level, uint32_t p_length, char const * p_time_format, char const * p_module ) {
	return (Logger){
		.Buffer     = calloc( p_length, 1 ),
		.TimeFormat = p_time_format ? p_time_format : DEFAULT_LOGGER_TIMEFORMAT,
		.Prefix     = p_module,
		.Level      = p_level,
		.Length     = p_length
	};
}

void LogSetLevelEx( Logger * p_module, uint32_t p_level ) {
	if ( !p_module )
		return;

	p_module->Level = p_level;
}

void LogSetLengthEx( Logger * p_module, uint32_t p_length ) {
	if ( !p_module )
		return;

	p_module->Buffer = realloc( p_module->Buffer, p_length );
	p_module->Length = p_length;
}

void LogSetTimeFormatEx( Logger * p_module, char const * p_time_format ) {
	if ( !p_module )
		return;

	p_module->TimeFormat = p_time_format ? p_time_format : DEFAULT_LOGGER_TIMEFORMAT;
}

void LogSetPrefixEx( Logger * p_module, char const * p_prefix ) {
	if ( !p_module )
		return;

	p_module->Prefix = p_prefix;
}

void LogEx( Logger const * p_module, LogLevel p_level, char const * p_message, ... ) {
	if ( !p_module )
		return;

	LOGLEVEL_ASSERT( p_level );

	LogDescriptor const * desc = &_log_desc[p_level - 1];
	if ( (desc->LevelBit & p_module->Level) == 0 )
		return;

	SetPrintForeColor( desc->Foreground );
	SetPrintBackColor( desc->Background );
	va_list args;
	va_start( args, p_message );
	desc->PrintFn( p_module, desc, p_message, args );
	va_end( args );
}
