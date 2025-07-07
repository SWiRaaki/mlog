#ifndef MLOG_H
#define MLOG_H

#include <stdint.h>

/**
 * @brief Default log print
 * @param message	Message format string
 */
#define Info( message ) Log( LogInfo, message )

/**
 * @brief Default log print
 * @param message	Message format string 
 * @param ...		List of parameters for the string format
 */
#define InfoV( message, ... ) Log( LogInfo, message, __VA_ARGS__ )

/**
 * @brief Default log print
 * @param module	Pointer to the logger module
 * @param message	Message format string 
 */
#define InfoEx( logger, message ) LogEx( logger, LogInfo, message )

/**
 * @brief Default log print
 * @param module	Pointer to the logger module
 * @param message	Message format string 
 * @param ...		List of parameters for the string format
 */
#define InfoExV( logger, message, ... ) LogEx( logger, LogInfo, message, __VA_ARGS__ )

/**
 * @brief Warning log prints, color highlighted, indicating possible error sources.
 * @param message	Message format string 
 */
#define Warning( message ) Log( LogWarning, message );

/**
 * @brief Warning log prints, color highlighted, indicating possible error sources.
 * @param message	Message format string 
 * @param ...		List of parameters for the string format
 */
#define WarningV( message, ... ) Log( LogWarning, message, __VA_ARGS__ )

/**
 * @brief Warning log prints, color highlighted, indicating possible error sources.
 * @param module	Pointer to the logger module
 * @param message	Message format string 
 */
#define WarningEx( logger, message ) LogEx( logger, LogWarning, message )

/**
 * @brief Warning log prints, color highlighted, indicating possible error sources.
 * @param module	Pointer to the logger module
 * @param message	Message format string 
 * @param ...		List of parameters for the string format
 */
#define WarningExV( logger, message, ... ) LogEx( logger, LogWarning, message, __VA_ARGS__ )

/**
 * @brief Error log prints, color highlighted, indicating a non-fatal error
 * @param message	Message format string 
 */
#define Error( message ) Log( LogError, message );

/**
 * @brief Error log prints, color highlighted, indicating a non-fatal error
 * @param message	Message format string 
 * @param ...		List of parameters for the string format
 */
#define ErrorV( message, ... ) Log( LogError, message, __VA_ARGS__ )

/**
 *
 * @brief Error log prints, color highlighted, indicating a non-fatal error
 * @param module	Pointer to the logger module
 * @param message	Message format string 
 */
#define ErrorEx( logger, message ) LogEx( logger, LogError, message )

/**
 * @brief Error log prints, color highlighted, indicating a non-fatal error
 * @param module	Pointer to the logger module
 * @param message	Message format string 
 * @param ...		List of parameters for the string format
 */
#define ErrorExV( logger, message, ... ) LogEx( logger, LogError, message, __VA_ARGS__ )

/**
 * @brief Fatal error log prints, color highlighted, indicating a fatal error
 * @param message	Message format string 
 */
#define Fatal( message ) Log( LogFatal, message )

/**
 * @brief Fatal error log prints, color highlighted, indicating a fatal error
 * @param message	Message format string 
 * @param ...		List of parameters for the string format
 */
#define FatalV( message, ... ) Log( LogFatal, message, __VA_ARGS__ )

/**
 * @brief Fatal error log prints, color highlighted, indicating a fatal error
 * @param module	Pointer to the logger module
 * @param message	Message format string 
 */
#define FatalEx( logger, message ) LogEx( logger, LogFatal, message )

/**
 * @brief Fatal error log prints, color highlighted, indicating a fatal error
 * @param module	Pointer to the logger module
 * @param message	Message format string 
 * @param ...		List of parameters for the string format
 */
#define FatalExV( logger, message, ... ) LogEx( logger, LogFatal, message, __VA_ARGS__ )

/**
 * @brief Debug log prints, color highlighted, only activated in debug builds unless manually activated
 * @param message	Message format string 
 */
#define Debug( message ) Log( LogDebug, message )

/**
 * @brief Debug log prints, color highlighted, only activated in debug builds unless manually activated
 * @param message	Message format string 
 * @param ...		List of parameters for the string format
 */
#define DebugV( message, ... ) Log( LogDebug, message, __VA_ARGS__ )

/**
 * @brief Debug log prints, color highlighted, only activated in debug builds unless manually activated
 * @param module	Pointer to the logger module
 * @param message	Message format string 
 */
#define DebugEx( logger, message ) LogEx( logger, LogDebug, message )

/**
 * @brief Debug log prints, color highlighted, only activated in debug builds unless manually activated
 * @param module	Pointer to the logger module
 * @param message	Message format string 
 * @param ...		List of parameters for the string format
 */
#define DebugExV( logger, message, ... ) LogEx( logger, LogDebug, message, __VA_ARGS__ )

// @brief Enumeration of all std log level flags
typedef enum {
	LogFlagInfo		= 0x00000001,
	LogFlagWarning	= 0X00000002,
	LogFlagError	= 0X00000004,
	LogFlagFatal	= 0x00000008,
	LogFlagDebug	= 0x00000010,
	LogFlagAllNoDbg	= 0x7FFFFFEF,
	LogFlagAll		= 0x7FFFFFFF
} LogLevelFlag;

// @brief Enumeration of all std log levels
typedef enum {
	LogInfo    = 1,
	LogWarning = 2,
	LogError   = 3,
	LogFatal   = 4,
	LogDebug   = 5
} LogLevel;

typedef struct {
	// @brief Buffer used by the logger to format strings
	void *			Buffer;
	// @brief Format string that provides how to format the logging time stamp
	char const *	TimeFormat;
	// @brief Prefix string that identifies the logger of a log
	char const *	Prefix;
	// @brief Bitflag of logs that are enabled or disabled ( bit set = enabled )
	LogLevelFlag	Level;
	// @brief Maximum length of a single log line
	uint32_t		Length;
} Logger;

/**
 * @brief Initializes a global Logger
 */
void LogInitGlobal( void );

/**
* @brief Get a readonly pointer to the global logger module
 * @return Logger const* Readonly pointer to the global logger
 */
Logger const * LogGetGlobal( void );

/**
 * @brief Set the log level of the global logger
 * @param level	Bitwise OR'd log level flags
 */
void LogSetLevel( LogLevelFlag level );

/**
 * @brief Set the maximum log line length of the global logger
 * @param length	Maximum length a log line can be
 */
void LogSetLength( uint32_t length );

/**
 * @brief Set the timestamp format of the global logger
 * @param time_format	Format string conforming the strftime format
 */
void LogSetTimeFormat( char const * time_format );

/**
 * @brief Set the module prefix of the global logger
 * @param prefix	Module prefix string
 */
void LogSetPrefix( char const * prefix );

/**
 * @brief Print a log of a specifig log level
 * @param level		Level to print the log to
 * @param message	Message to format
 * @param ...		List of parameters for the string format
 */
void Log( LogLevel level, char const * message, ... );

/**
 * @brief Creates a new Logger state
 * @param level			The initial log level of the logger
 * @param length		Maximum length of a single log line
 * @param time_format	[OPTIONAL] Format string that provides how to format the logging time stamp
 * @param module		[OPTIONAL] Prefix string that identifies the logger of a log
 * @return Logger		Logger state
 */
Logger LoggerCreate( LogLevelFlag level, uint32_t length, char const * time_format, char const * module );

/**
 * @brief Set the log level of the global logger
 * @param module	Pointer to the logger module
 * @param level	Bitwise OR'd log levels
 */
void LogSetLevelEx( Logger * module, LogLevelFlag level );

/**
 * @brief Set the maximum log line length of the global logger
 * @param module	Pointer to the logger module
 * @param length	Maximum length a log line can be
 */
void LogSetLengthEx( Logger * module, uint32_t length );

/**
 * @brief Set the timestamp format of the global logger
 * @param module	Pointer to the logger module
 * @param time_format	Format string conforming the strftime format
 */
void LogSetTimeFormatEx( Logger * module, char const * time_format );

/**
 * @brief Set the module prefix of the global logger
 * @param module	Pointer to the logger module
 * @param prefix	Module prefix string
 */
void LogSetPrefixEx( Logger * module, char const * prefix );

/**
 * @brief Print a log of a specifig log level
 * @param module	Pointer to the logger module
 * @param level		Level to print the log to
 * @param message	Message to format
 * @param ...		List of parameters for the string format
 */
void LogEx( Logger const * module, LogLevel level, char const * message, ... );

#endif // MLOG_H
