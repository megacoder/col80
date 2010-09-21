/*
 *------------------------------------------------------------------------
 * vim: ts=8 sw=8
 *------------------------------------------------------------------------
 * Author:   tf135c (James Reynolds)
 * Filename: col80.c
 * Created:  2006-11-27 13:34:05
 *------------------------------------------------------------------------
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include <gcc-compat.h>

static	char *		me = "main";
static	unsigned	nonfatal;
static	unsigned	debugLevel;
static	unsigned	multi;

/*
 *------------------------------------------------------------------------
 * say: output printf-style message to stderr
 *------------------------------------------------------------------------
 */

static	void
vsay(
	char const * const	fmt,
	va_list			ap
)
{
	fprintf(  stderr, "%s: ", me );
	vfprintf( stderr, fmt, ap );
	fprintf(  stderr, ".\n" );
}

static	void _printf(1,2)
say(
	char const * const	fmt,
	...
)
{
	va_list		ap;

	va_start( ap, fmt );
	vsay( fmt, ap );
	va_end( ap );
}

/*
 *------------------------------------------------------------------------
 * debug: output debug message if sufficient demand
 *------------------------------------------------------------------------
 */

static	void			_printf(3,4)
debug(
	unsigned		theLevel,
	int			e,
	char const * const	fmt,
	...
)
{
	if( theLevel <= debugLevel )	{
		va_list		ap;

		fprintf( stderr, "%s ", me );
		va_start( ap, fmt );
		vfprintf( stderr, fmt, ap );
		va_end( ap );
		if( e )	{
			fprintf( stderr, "; errno=%d (%s)", e, strerror( e ) );
		}
		fprintf( stderr, ".\n" );
	}
}

/*
 *------------------------------------------------------------------------
 * usage: how to run this program
 *------------------------------------------------------------------------
 */

static	void _printf(1,2)
usage(
	char const * const	fmt,
	...
)
{
	static char const	opt_fmt[] = "  %-11s %s\n";

	if( fmt )	{
		va_list		ap;

		va_start( ap, fmt );
		vsay( fmt, ap );
		va_end( ap );
		printf( "\n" );
	}
	printf( "NAME\n" );
	printf( "  %s - add newlines to 80-column card images\n", me );
	printf( "\n" );
	printf( "SYNOPSIS\n" );
	printf( "  %s", me );
	printf( " [-D]" );
	printf( " [file..]" );
	printf( "\n" );
	printf( "\n" );
	printf( "DESCRIPTION\n" );
	printf( "  The %s program expects input of 80-column card\n", me );
	printf( "  images.  Each card image is simply copied to the output\n" );
	printf( "  with a newline appended.\n" );
	printf( "\n" );
	printf( "  Input is read from stdin if the 'file' argument is\n" );
	printf( "  not provided.\n" );
	printf( "\n" );
	printf( "OPTIONS\n" );
	printf( opt_fmt, "-D",		"Increase debug output verbosity" );
	printf( opt_fmt, "file",	"80-column card image file" );
	printf( "\n" );
}

/*
 *------------------------------------------------------------------------
 * process: do whatever is necesssary
 *------------------------------------------------------------------------
 */

static	void
process(
	char * const	fn,
	FILE *		fyle
)
{
	debug( 1, 0, "process( '%s', ... )", fn );
	if( multi )	{
		printf( "<-- %s -->\n", fn );
	}
	while( !feof( fyle ) )	{
		char	image[ 80 ];
		size_t	qty;

		qty = fread( image, sizeof( image[0] ), DIM( image ), fyle );
		fwrite( image, sizeof( image[0] ), qty, stdout );
		putchar( '\n' );
	}
}

/*
 *------------------------------------------------------------------------
 * main: central control logic
 *------------------------------------------------------------------------
 */

int
main(
	int		argc,
	char * *	argv
)
{
	do	{
		char *		bp;
		int		c;

		/* Figure out our process name				*/
		me = argv[ 0 ];
		if( (bp = strrchr( me, '/' )) != NULL )	{
			me = bp + 1;
		}
		/* Process any command line switches			*/
		opterr = 0;
		while( (c = getopt( argc, argv, "D" )) != EOF )	{
			switch( c )	{
			default:
				say( "switch -%c not implemented yet", c );
				++nonfatal;
				break;
			case '?':
				say( "unknown switch -%c", optopt );
				++nonfatal;
				break;
			case 'D':
				++debugLevel;
				break;
			}
		}
		/* Can't proceed if we've had bad switches		*/
		if( nonfatal )	{
			usage( "Illegal switch(es) used" );
			break;
		}
	} while( 0 );
	if( !nonfatal ) do	{
		/* Process command line arguments, if any		*/
		if( optind < argc )	{
			multi = ((argc - optind) > 1);
			while( optind < argc )	{
				char * const	fn = argv[ optind++ ];
				FILE *		fyle;

				fyle = fopen( fn, "rt" );
				if( !fyle )	{
					say(
					"could not open '%s' for reading",
						fn
					);
					++nonfatal;
					continue;
				}
				process( fn, fyle );
				fclose( fyle );
			}
		} else	{
			process( "{stdin}", stdin );
		}
	} while( 0 );
	return( nonfatal ? 1 : 0 );
}
