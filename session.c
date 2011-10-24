/* sessionchooser - a text-based session chooser
 *
 * Copyright (c) 2010-2011, Tilman Blumenbach <tilman@ax86.net>
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
#include <string.h>

#include "desktopentry.h"
#include "i18n.h"
#include "session.h"

SessSession *sess_session_new( void )
{
    /* Using g_new0() conveniently sets use_xinit to 0 (FALSE). */
    return g_new0( SessSession, 1 );
}

void sess_session_free( SessSession *sess, gboolean free_elements )
{
    if( free_elements )
    {
        g_free( sess->name );
        g_free( sess->name_normalized );
        g_free( sess->name_coll_key_case );
        g_free( sess->name_locale );
        g_free( sess->exec );
        g_free( sess->exec_locale );
    }

    g_free( sess );
}

gchar *sess_session_get_name_normalized( SessSession *sess )
{
    gchar *normalized;

    if( ! sess->name_normalized )
    {   /* Normalize name. */
        normalized = g_utf8_normalize( sess->name, -1, G_NORMALIZE_ALL );
        if( ! normalized )
        {   /* Invalid UTF-8 string. Use original string (not perfect!): */
            sess->name_normalized = g_strdup( sess->name );
        }
        else
        {
            sess->name_normalized = normalized;
        }
    }

    return sess->name_normalized;
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

gchar *sess_session_get_exec_locale( SessSession *sess )
{
    if( ! sess->exec_locale )
    {   /* Convert "exec" key to current locale charset: */
        sess->exec_locale = sess_locale_from_utf8( sess->exec, -1 );
    }

    return sess->exec_locale;
}

static gint _sess_session_find_by_name_normalized_cb(
    gconstpointer curr_sess,
    gconstpointer search_str )
{
    return strcmp( sess_session_get_name_normalized( (SessSession *)curr_sess ),
        (const char *)search_str );
}

SessSession *sess_session_find_by_name_normalized( GSList *sess_list,
                                        const gchar *name_normalized )
{
    GSList *res = g_slist_find_custom( sess_list, name_normalized,
        _sess_session_find_by_name_normalized_cb );

    return res ? (SessSession *)res->data : NULL;
}

static void _sess_slist_free_all_cb( gpointer data, gpointer userdata )
{
    sess_session_free( (SessSession *)data, (gboolean)userdata );
}

void sess_slist_free_all( GSList *list, gboolean free_sess_elements )
{
    g_slist_foreach( list, _sess_slist_free_all_cb, (gpointer)free_sess_elements );
    g_slist_free( list );
}

void parse_xsession_files_in_dir( gpointer _dir, gpointer _session_list )
{
    /* If _dir is an empty string, use the current directory (as per POSIX). */
    const gchar *dir = ! *( (const gchar *)_dir ) ? "." : (const gchar *)_dir;
    const gchar *file;
    gchar *file_path;
    gchar *sess_name;
    gchar *sess_exec;
    GSList **session_list = (GSList **)_session_list;
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
        g_free( file_path );

        /* Now we got both the session name and its Exec key. Append them
         * the session list. */
        new_sess = sess_session_new();
        new_sess->name      = sess_name;
        new_sess->exec      = sess_exec;
        new_sess->use_xinit = TRUE;

        *session_list = g_slist_prepend( *session_list, new_sess );
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
    gboolean free_file_utf8;
    gchar *file_path;
    gchar *sess_name;
    gchar *dot_pos;
    GSList **session_list = (GSList **)_session_list;
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

        free_file_utf8 = TRUE;
        file_utf8 = g_filename_to_utf8( file, -1, NULL, NULL, NULL );
        if( ! file_utf8 )
        {   /* Conversion failed, use original file name. */
            /* This is not const-correct (the compiler complains because
             * file is declarded const gchar*, but file_utf8 is not),
             * but note that file_utf8 will not be modified later on and
             * thus file is not being modified at any time, either. */
            file_utf8 = (gchar *)file;
            /* We *cannot* call g_free() on file_utf8 later! */
            free_file_utf8 = FALSE;
        }

        if( ( dot_pos = g_utf8_strrchr( file_utf8, -1, '.' ) ) )
        {   /* Strip file extension. */
            sess_name = g_strndup( file_utf8, dot_pos - file_utf8 );
        }
        else
        {   /* Use original file name. */
            if( free_file_utf8 )
            {   /* We can just use the converted string and NOT free it. */
                sess_name = file_utf8;
                free_file_utf8 = FALSE;
            }
            else
            {   /* We have to copy the string. */
                sess_name = g_strdup( file_utf8 );
            }
        }

        if( free_file_utf8 )
        {
            g_free( file_utf8 );
        }
        
        //~ g_debug( " Adding text session: %s", sess_name );

        /* Append new entry to session list: */
        new_sess = sess_session_new();
        new_sess->name = sess_name;

        file_path_utf8 = g_filename_to_utf8( file_path, -1, NULL, NULL, NULL );
        if( ! file_path_utf8 )
        {   /* Conversion failed, use original file path: */
            new_sess->exec = file_path;
        }
        else
        {
            //~ g_debug( "Text session: Using UTF-8 file name." );
            g_free( file_path );
            new_sess->exec = file_path_utf8;
        }

        *session_list = g_slist_prepend( *session_list, new_sess );
    }

    g_dir_close( gdir );
}
