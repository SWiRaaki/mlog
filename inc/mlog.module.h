#ifndef MDLOG_MODULE_H
#define MDLOG_MODULE_H

#include "mlog.h"
#include <stdint.h>

typedef enum {
	LogColorBlack         = 0,
	LogColorRed           = 1,
	LogColorGreen         = 2,
	LogColorYellow        = 3,
	LogColorBlue          = 4,
	LogColorMagenta       = 5,
	LogColorCyan          = 6,
	LogColorWhite         = 7,
	LogColorDefault       = 9,
	LogColorBrightBlack   = 10,
	LogColorBrightRed     = 11,
	LogColorBrightGreen   = 12,
	LogColorBrightYellow  = 13,
	LogColorBrightBlue    = 14,
	LogColorBrightMagenta = 15,
	LogColorBrightCyan    = 16,
	LogColorBrightWhite   = 17
} LogColor;

typedef struct LogDescriptor LogDescriptor;

typedef void (*PrintFn)( Logger const *, LogDescriptor const *, char const *, void * );

struct LogDescriptor {
	char const *	Prefix;
	PrintFn			PrintFn;
	LogColor		Foreground;
	LogColor		Background;
	LogLevel		Level;
	LogLevelFlag	LevelBit;
};

/**
 * @brief Registers a log level via LogDescriptor
 * @param log		Log descriptive meta information.
 * @return uint32_t	Level assigned to the log. If a log level is set, it will be used if available
 */
LogLevel RegisterLogLevel( LogDescriptor * log );

/**
 * @brief Formats a unified log message with timestamp and module prefix.
 */
void Print( Logger const * module, LogDescriptor const * desc, char const * message, void * args );

/**
 * @brief Sets the font color of the terminal text.
 * @param color	Base color
 * @param level Brightness level of the color
 */
void SetPrintForeColor( LogColor color );

/**
 * @brief Sets the background color of the terminal text.
 * @param color	Base color
 * @param level Brightness level of the color
 */
void SetPrintBackColor( LogColor color );

/**
 * @brief Resets the color of the terminal text to all default
 */
void ResetPrintColor( void );

#endif // MDLOG_MODULE_H
