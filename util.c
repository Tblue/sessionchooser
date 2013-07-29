/* sessionchooser - a text-based session chooser
 *
 * Copyright (c) 2010-2012, Tilman Blumenbach <tilman AT ax86 DOT net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of the copyright holder nor the names of the
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <glob.h>
#include <string.h>
#include <curses.h>
#include <term.h>
#include <unistd.h>
#include <stropts.h>
#include <linux/vt.h>

#include "defines.h"
#include "util.h"

void sess_strsplit_foreach( const gchar *string, const gchar *delimiter,
                            gint max_tokens, GFunc func, gpointer user_data )
{
    gchar **parts = g_strsplit( string, delimiter, max_tokens );
    gchar **parts_p;

    for( parts_p = parts; *parts_p; parts_p++ )
    {
        func( *parts_p, user_data );
    }

    g_strfreev( parts );
}

gint get_next_free_XDisplay( void )
{
    char *file;
    char *offset;
    size_t i;
    gint display = -1;
    gint l_display;
    glob_t matches;
    gchar *pattern = g_build_path( G_DIR_SEPARATOR_S, g_get_tmp_dir(),
        ".X[0-9]*-lock", NULL );
    int res = glob( pattern, GLOB_ERR | GLOB_NOSORT | GLOB_NOESCAPE, NULL,
        & matches );

    g_free( pattern );

    switch( res )
    {
        case 0:
            /* Everything went fine! */
            break;

        case GLOB_NOMATCH:
            /* No matches, which means the first available X display is 0. */
            globfree( & matches );
            return 0;

        case GLOB_ABORTED:
            g_critical( "Could not glob() for X lock files (read error)!" );
            globfree( & matches );
            return -1;

        case GLOB_NOSPACE:
            g_error( "Out of memory!" ); /* Calls abort()! */
            break;

        default:
            g_critical( "Unhandled glob() return value: %d", res );
            globfree( & matches );
            return -1;
    }
    
    for( i = 0; i < matches.gl_pathc; i++ )
    {
        file = matches.gl_pathv[i];
        if( ! g_file_test( file, G_FILE_TEST_IS_REGULAR ) )
        {   /* Skip directories. */
            continue;
        }

        offset = strrchr( file, G_DIR_SEPARATOR );
        g_assert( offset != NULL );
        offset += 3;

        /* No error expected as the pattern ensures a leading digit. */
        l_display = g_ascii_strtoull( offset, NULL, 10 ) + 1;
        if( l_display > display )
        {
            display = l_display;
        }
    }

    globfree( & matches );
    return display;
}

gshort get_active_vt( void )
{
    struct vt_stat vtstat;

    if( ioctl( STDIN_FILENO, VT_GETSTATE, & vtstat ) == -1 )
    {
        return -1;
    }

    return vtstat.v_active;
}

gboolean terminfo_init( void )
{
    if( setupterm( NULL, STDOUT_FILENO, NULL ) == ERR )
    {
        return FALSE;
    }

    return TRUE;
}

void terminfo_clear_screen( void )
{
    putp( clear_screen );
}

gchar *get_last_session( void )
{
    gchar *file = g_build_filename( g_get_home_dir(), SESS_LASTSESSION_FILE,
        NULL );
    gchar *last_session = NULL;

    g_file_get_contents( file, & last_session, NULL, NULL );
    g_free( file );

    /* If g_file_get_contents() failed, we return NULL. */
    return last_session;
} 

gboolean set_last_session( const gchar *sess_name, GError **error )
{
    /* TODO: String already built in get_last_session(), avoid doing it
     *       a second time here. */
    gchar *file = g_build_filename( g_get_home_dir(), SESS_LASTSESSION_FILE,
        NULL );
    GError *int_error = NULL;

    g_file_set_contents( file, sess_name, -1, & int_error );
    g_free( file );

    if( int_error )
    {
        g_propagate_error( error, int_error );
        return FALSE;
    }

    return TRUE;
}

gchar *filename_to_utf8_nofail( const gchar *path )
{
    gchar *file_path_utf8;

    file_path_utf8 = g_filename_to_utf8( file_path, -1, NULL, NULL, NULL );
    if( ! file_path_utf8 )
    {   /* Conversion failed, use original file path: */
        file_path_utf8 = g_strdup( path );
    }

    return file_path_utf8;
}
