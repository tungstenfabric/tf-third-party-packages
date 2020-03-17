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
** ipfix_col.h - export declarations of ipfix collector funcs
**
** Copyright Fraunhofer FOKUS
**
** $Id: ipfix_col.h 166 2010-02-23 10:15:04Z csc $
**
*/
#ifndef IPFIX_COL_H
#define IPFIX_COL_H

#include <limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ipfix.h>
#ifdef DBSUPPORT
#include <ipfix_db.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define DFLT_MYSQL_HOST      "localhost"
#define DFLT_MYSQL_DBNAME    "ipfix"
#define DFLT_MYSQL_USER      "ipfix"
#define DFLT_MYSQL_PASSWORD  "ipfix"

#define MAXTEMPLIDENT        120

typedef enum {
    IPFIX_INPUT_FILE, IPFIX_INPUT_IPCON
} ipfix_input_type_t;

typedef struct ipfix_input {
    ipfix_input_type_t      type;
    union {
        struct {
            struct sockaddr *addr;           /* exporter addess */
            socklen_t       addrlen;         /* exporter address length */
        } ipcon;
        struct {
            char            *name;           /* filename */
        } file;
    } u;
} ipfix_input_t;

typedef struct ipfixt_node
{
    struct ipfixt_node   *next;
    time_t               expire_time;
    ipfix_template_t     *ipfixt;
    char                 ident[MAXTEMPLIDENT];
#ifdef DBSUPPORT
    char                 tablename[MAXTABLENAMELEN+1]; /* make this dynamic */
    unsigned             template_id;      /* id from template table (hack) */
    unsigned             message_snr;      /* last ipfix message snr (hack) */
#endif
} ipfixt_node_t;

typedef struct ipfixs_node
{
    struct ipfixs_node   *next;
    uint32_t             odid;                        /* observation domain */
    ipfix_input_t        *input;                     /* ipfix input details */
    ipfixt_node_t        *templates;                   /* list of templates */
    char                 fname[PATH_MAX+1];
    FILE                 *fp;
    unsigned int         last_seqno;
#ifdef DBSUPPORT
    unsigned int         last_msgid;                      /* ugh! move this */
    unsigned int         last_message_snr;
    unsigned int         exporterid;
#endif
} ipfixs_node_t;

typedef struct ipfixi_node
{
    struct ipfixe_node      *next;
    ipfix_input_t           *input;          /* ipfix input details */
    struct ipfixs_node      *ods;            /* list of observation domains */
} ipfixi_node_t;

typedef struct ipfix_col_info
{
    int (*export_newsource)(ipfixs_node_t*,void*);
    int (*export_newmsg)(ipfixs_node_t*,ipfix_hdr_t*,void*);
    int (*export_trecord)(ipfixs_node_t*,ipfixt_node_t*,void*);
    int (*export_dset)(ipfixt_node_t*,const uint8_t*,size_t,void*);
    int (*export_drecord)(ipfixs_node_t*,ipfixt_node_t*,
                          ipfix_datarecord_t*,void*);
    int (*export_rawmsg)(ipfixs_node_t *source, const uint8_t* data, size_t len, void *arg);
    void (*export_cleanup)(void*);
    void *data;
} ipfix_col_info_t;

typedef struct ipfix_col_info_node
{
    struct ipfix_col_info_node *next;
    struct ipfix_col_info      *elem;

} ipfixe_node_t;

typedef void* ipfix_col_t;

void ipfix_col_init( void );
int  ipfix_col_init_fileexport( char *datadir );
void ipfix_col_stop_fileexport( void );
int  ipfix_col_init_mysqlexport( char *host, char *user, char *pw, char *name );
void ipfix_col_stop_mysqlexport( void );
int  ipfix_col_register_export( ipfix_col_info_t *colinfo );
int  ipfix_col_cancel_export( ipfix_col_info_t *colinfo );
int  ipfix_col_listen( int *nfds, int **fds, ipfix_proto_t protocol, 
                       int port, int family, int maxcon );
int  ipfix_col_start_msglog( FILE *fpout ); 
void ipfix_col_stop_msglog( void ); 
int  ipfix_col_close( int fd );
void ipfix_col_cleanup( void );

/* internal, experimental */
int  ipfix_parse_hdr( const uint8_t *buf, size_t buflen, ipfix_hdr_t *hdr );
int  ipfix_parse_raw_msg( ipfixs_node_t *source, ipfixe_node_t  *g_exporter,
                      const uint8_t *msg, size_t nbytes );
int  ipfix_parse_msg( ipfix_input_t *input, ipfixs_node_t **sources, 
                      const uint8_t *msg, size_t nbytes );
int  ipfix_get_template_ident( ipfix_template_t *t, char *buf, size_t buflen );
int  ipfix_col_listen_ssl( ipfix_col_t **handle, ipfix_proto_t protocol, 
                           int port, int family, int maxcon,
                           ipfix_ssl_opts_t *ssl_opts );
int  ipfix_col_close_ssl( ipfix_col_t *handle );


const char *ipfix_col_input_get_ident( ipfix_input_t *input );

#ifdef __cplusplus
}
#endif
#endif
