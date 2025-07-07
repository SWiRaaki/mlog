#include <stdlib.h>
#include <stdio.h>

#include "mlog.h"
#include "mlog.module.h"

LogDescriptor l30desc1 = {
	.Prefix     = "301",
	.PrintFn    = Print,
	.Level      = 30,
	.LevelBit   = 0,
	.Foreground = LogColorBlue,
	.Background = LogColorDefault
};

LogDescriptor l30desc2 = {
	.Prefix     = "302",
	.PrintFn    = Print,
	.Level      = 30,
	.LevelBit   = 0,
	.Foreground = LogColorMagenta,
	.Background = LogColorDefault
};

LogDescriptor l30desc3 = {
	.Prefix     = "303",
	.PrintFn    = Print,
	.Level      = 30,
	.LevelBit   = 0,
	.Foreground = LogColorGreen,
	.Background = LogColorDefault
};

int main( int argc, char * argv[] ) {
	(void)(argc); (void)(argv);
	LogInitGlobal();
	LogSetLevel( LogFlagAll );

	Info( "Hello Info!" );
	Warning( "Hello Warning!" );
	Error( "Hello Error!" );
	Fatal( "Hello Fatal!" );
	Debug( "Hello Debug!" );

	Info( "Registering log at level 30.." );
	if ( !RegisterLogLevel( &l30desc1 ) ) {
		Warning( "Log failed to register!" );
	} else {
		Log( l30desc1.Level, "Log level of registered log: %d", l30desc1.Level );
	}

	Info( "Registering second log at level 30" );
	if ( !RegisterLogLevel( &l30desc2 ) ) {
		Warning( "Log failed to register!" );
	} else {
		Log( l30desc2.Level, "Log level of registered log: %d", l30desc2.Level );
	}

	Info( "Registering third log at level 30" );
	if ( !RegisterLogLevel( &l30desc3 ) ) {
		Warning( "Log failed to register!" );
	} else {
		Log( l30desc3.Level, "Log level of registered log: %d", l30desc3.Level );
	}

	return 0;
}
