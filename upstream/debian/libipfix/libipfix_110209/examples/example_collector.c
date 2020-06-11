/*

libipfix - a library which provides an implementation of the IPFIX protocol
           for flexible flow data support. IPFIX is the successor of NetFlow v9
           (see http://www.ietf.org/dyn/wg/charter/ipfix-charter.html and
            RFC5101 and RFC5102 for details) 

Copyright (c) 2005-2011, Fraunhofer FOKUS
All rights reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
 */
/*
**     collector.c - example ipfix collector
**
**     Copyright Fraunhofer FOKUS
**
**     $Date: 2005/08/16 12:57:57 $
**
**     $Revision: 1.4 $
**
**     todo: revise loggin
*/
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <signal.h>
#include <libgen.h>
#include <limits.h>
#include <stdarg.h>

#include "ipfix.h"
#include "ipfix_col.h"
#include "misc.h"

/*------ globals ---------------------------------------------------------*/

static char       progname[30];
static int        verbose_level = 0;
static int        *tcp_s=NULL, ntcp_s=0;       /* socket */

/*------ static funcs ----------------------------------------------------*/

static void usage( char *taskname)
{
    const char helptxt[] =
        "[options]\n\n"
        "options:\n"
        "  -h                  this help\n"
        "  -o <datadir>        store files of collected data in this dir\n"
        "  -p <portno>         listen on this port (default=4739)\n"
        "  -v                  increase verbose level\n"
        "\n";

    fprintf( stderr, "\nipfix example collector\n" );
    fprintf(stderr,"\nusage: %s %s\n", taskname, helptxt);

}/*usage*/

void exit_func ( int signo )
{
    if ( verbose_level && signo )
        fprintf( stderr, "\n[%s] got signo %d, bye.\n\n", progname, signo );

    if ( tcp_s ) {
        int i;

        for( i=0; i<ntcp_s; i++ )
            close( tcp_s[i] );
    }

    ipfix_col_cleanup();
    ipfix_cleanup();
    exit( 1 );
}

/*------ main ------------------------------------------------------------*/

int main (int argc, char *argv[])
{
    char          opt;          /* short options: character */
    char          optstr[] = "hp:vo:";
    int           port;         /* port number */
    char          *datadir;

    /** set default options
     */
    port    = 4739;
    datadir = ".";
    snprintf( progname, sizeof(progname), "%s", basename( argv[0]) );

    /* --- command line parsing ---
     */
    while( ( opt = getopt( argc, argv, optstr ) ) != EOF ) {

        switch ( opt )
        {
          case 'o':
              datadir = optarg;
              if ( access( optarg, W_OK ) <0 ) {
                  fprintf( stderr, "cannot access '%s': %s!\n",
                           optarg, strerror(errno) );
                  exit(1);
              }
              break;

          case 'p':
              if ((port=atoi(optarg)) <0) {
                  fprintf( stderr, "Invalid -p argument!\n" );
                  exit(1);
              }
              break;

          case 'v':
              verbose_level ++;
              break;

          case 'h':
          default:
              usage(progname);
              exit(1);
        }
    }

    /** init loggin
     */
    mlog_set_vlevel( verbose_level );

    /** init ipfix lib
     */
    if ( ipfix_init() <0 ) {
        fprintf( stderr, "ipfix_init() failed: %s\n", strerror(errno) );
        exit(1);
    }

    /** signal handler
     */
    signal( SIGKILL, exit_func );
    signal( SIGTERM, exit_func );
    signal( SIGINT,  exit_func );

    /** activate file export
     */
    (void) ipfix_col_init_fileexport( datadir );

    /** open ipfix collector port(s)
     */
    if ( ipfix_col_listen( &ntcp_s, &tcp_s, IPFIX_PROTO_TCP, 
                           port, AF_INET, 10 ) <0 ) {
        fprintf( stderr, "[%s] ipfix_listen(tcp) failed.\n",
                 progname );
        return -1;
    }

    /** event loop
     */
    (void) mpoll_loop( -1 );

    exit(1);
}

