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
#include <stdlib.h>
#include <string.h>

#include "desktopentry.h"
#include "i18n.h"
#include "session.h"
#include "util.h"

SessSession *sess_session_new( void )
{
    /* Using g_new0() conveniently sets use_xinit to 0 (FALSE). */
    return g_new0( SessSession, 1 );
}

void sess_session_free( SessSession *sess )
{
    g_free( sess->name );
    g_free( sess->name_coll_key_case );
    g_free( sess->name_locale );
    g_free( sess->path );
    g_free( sess->path_normalized );
    g_free( sess->path_coll_key );
    g_free( sess->exec );
    g_free( sess->exec_locale );

    g_free( sess );
}

gchar *sess_session_get_name_coll_key_case( SessSession *sess )
{
    gchar *folded;

    if( ! sess->name_coll_key_case )
    {
        folded = g_utf8_casefold( sess->name, -1 );
        sess->name_coll_key_case = g_utf8_collate_key( folded, -1 );
        g_free( folded );
    }

    return sess->name_coll_key_case;
}

gchar *sess_session_get_name_locale( SessSession *sess )
{
    if( ! sess->name_locale )
    {   /* Convert name to locale charset: */
        sess->name_locale = sess_locale_from_utf8( sess->name, -1 );
    }

    return sess->name_locale;
}

gchar *sess_session_get_path_normalized( SessSession *sess )
{
    gchar *normalized;

    if( ! sess->path_normalized )
    {   /* Normalize path. */
        normalized = g_utf8_normalize( sess->path, -1, G_NORMALIZE_NFD );
        if( ! normalized )
        {   /* Invalid UTF-8 string. Use original string (not perfect!): */
            sess->path_normalized = g_strdup( sess->path );
        }
        else
        {
            sess->path_normalized = normalized;
        }
    }

    return sess->path_normalized;
}

gchar *sess_session_get_path_coll_key( SessSession *sess )
{
    if( ! sess->path_coll_key )
    {
        sess->path_coll_key = g_utf8_collate_key( sess->path, -1 );
    }

    return sess->path_coll_key;
}

gchar *sess_session_get_exec_locale( SessSession *sess )
{
    if( ! sess->exec_locale )
    {   /* Convert "exec" key to current locale charset: */
        sess->exec_locale = sess_locale_from_utf8( sess->exec, -1 );
    }

    return sess->exec_locale;
}


SessList *sess_list_new( void )
{
    SessList *s = g_new0( SessList, 1 );
    s->list = g_ptr_array_new_with_free_func( (GDestroyNotify)sess_session_free );

    return s;
}

void sess_list_free( SessList *s )
{
    if( ! s )
    {
        return;
    }

    g_ptr_array_free( s->list, TRUE );
    if( s->path_idx )
    {
        g_ptr_array_free( s->path_idx, TRUE );
    }
}

/**
 * @brief Sorting callback for g_ptr_array_sort(). Sorts by name.
 * @param a First (pointer to a) SessSession object/pointer.
 * @param b Second (pointer to a) SessSession object/pointer.
 * @return See g_ptr_array_sort().
 */
static gint _session_name_sort_func( gconstpointer a, gconstpointer b )
{
    return strcmp( sess_session_get_name_coll_key_case( *(SessSession **)a ),
        sess_session_get_name_coll_key_case( *(SessSession **)b ) );
}

/**
 * @brief Sorting callback for g_ptr_array_sort(). Sorts by path.
 * @param a First (pointer to a) SessSession object/pointer.
 * @param b Second (pointer to a) SessSession object/pointer.
 * @return See g_ptr_array_sort().
 */
static gint _session_path_sort_func( gconstpointer a, gconstpointer b )
{
    return strcmp( sess_session_get_path_coll_key( *(SessSession **)a ),
        sess_session_get_path_coll_key( *(SessSession **)b ) );
}

void sess_list_sort( SessList *s )
{
    g_ptr_array_sort( s->list, _session_name_sort_func );

    if( s->path_idx )
    {
        g_ptr_array_free( s->path_idx, TRUE );
    }

    /* Since this is an index, we don't need to free the elements pointed to
     * by its elements (will be free'd when the original session list is
     * destroyed).
     */
    s->path_idx = ptr_array_copy( s->list, NULL );
    g_ptr_array_sort( s->path_idx, _session_path_sort_func );
}


void parse_xsession_files_in_dir( gpointer _dir, gpointer _session_list )
{
    /* If _dir is an empty string, use the current directory (as per POSIX). */
    const gchar *dir = ! *( (const gchar *)_dir ) ? "." : (const gchar *)_dir;
    const gchar *file;
    gchar *file_path;
    gchar *sess_name;
    gchar *sess_exec;
    GPtrArray *session_list = ((SessList *)_session_list)->list;
    GDir *gdir;
    GError *error = NULL;
    SessSession *new_sess;

    //~ g_debug( "Parsing files in dir: %s", dir );

    gdir = g_dir_open( dir, 0, & error );
    if( error )
    {   /* We cannot do anything apart from showing a warning. */
        g_warning( "%s", error->message );
        g_error_free( error );
        return;
    }

    while( ( file = g_dir_read_name( gdir ) ) )
    {
        file_path = g_build_filename( dir, file, NULL );
        if( ! g_file_test( file_path, G_FILE_TEST_IS_REGULAR ) )
        {
            g_free( file_path );
            continue;
        }
        
        //~ g_debug( " Parsing file: %s", file_path );
        sess_desktopentry_read( file_path, & sess_name, & sess_exec,
            & error );
        if( error )
        {   /* Again, we can only show a warning. */
            g_warning( "Could not process file %s: %s", file_path, error->message );
            g_clear_error( & error );
            g_free( file_path );
            continue;
        }

        /* Now we got both the session name and its Exec key. Append them
         * the session list. */
        new_sess = sess_session_new();
        new_sess->name      = sess_name;
        new_sess->path      = filename_to_utf8_nofail( file_path );
        new_sess->exec      = sess_exec;
        new_sess->use_xinit = TRUE;

        g_free( file_path );
        g_ptr_array_add( session_list, new_sess );
    }

    g_dir_close( gdir );
}

void parse_textsession_files_in_dir( gpointer _dir, gpointer _session_list )
{
    /* If _dir is an empty string, use the current directory (as per POSIX). */
    const gchar *dir = ! *( (const gchar *)_dir ) ? "." : (const gchar *)_dir;
    const gchar *file;
    gchar *file_utf8;
    gchar *file_path_utf8;
    gchar *file_path;
    gchar *sess_name;
    gchar *dot_pos;
    GPtrArray *session_list = ((SessList *)_session_list)->list;
    GDir *gdir;
    GError *error = NULL;
    SessSession *new_sess;

    //~ g_debug( "Parsing files in dir: %s", dir );

    gdir = g_dir_open( dir, 0, & error );
    if( error )
    {   /* We cannot do anything apart from showing a warning. */
        g_warning( "%s", error->message );
        g_error_free( error );
        return;
    }

    while( ( file = g_dir_read_name( gdir ) ) )
    {
        file_path = g_build_filename( dir, file, NULL );
        if( ! g_file_test( file_path, G_FILE_TEST_IS_REGULAR ) ||
            ! g_file_test( file_path, G_FILE_TEST_IS_EXECUTABLE ) )
        {   /* Need a regular and executable file. */
            g_free( file_path );
            continue;
        }

        file_utf8 = filename_to_utf8_nofail( file );

        if( ( dot_pos = g_utf8_strrchr( file_utf8, -1, '.' ) ) )
        {   /* Strip file extension. */
            sess_name = g_strndup( file_utf8, dot_pos - file_utf8 );
            g_free( file_utf8 );
        }
        else
        {   /* Use original file name. */
            sess_name = file_utf8;
        }

        //~ g_debug( " Adding text session: %s", sess_name );

        /* Append new entry to session list: */
        new_sess = sess_session_new();
        new_sess->name = sess_name;

        new_sess->exec = filename_to_utf8_nofail( file_path );
        new_sess->path = g_strdup( new_sess->exec );

        g_free( file_path );
        g_ptr_array_add( session_list, new_sess );
    }

    g_dir_close( gdir );
}
