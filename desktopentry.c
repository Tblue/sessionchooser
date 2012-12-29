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
#include <string.h>

#include "desktopentry.h"
#include "error.h"

gboolean sess_desktopentry_read( const gchar *path, gchar **app_name,
                                 gchar **app_exec, GError **error )
{
    GKeyFile *key_file;
    GError *int_error = NULL;

    gchar *type;
    gchar *temp_app_name;
    gchar *temp_app_exec;
    gchar *executable;
    GString *escaped_exec;
    
    gint exec_argc;
    gchar **exec_argv;
    gchar **parts;
    gchar *part;

    /* Make sure the caller handles errors properly: */
    g_return_val_if_fail( ! error || ! *error, FALSE );

    key_file = g_key_file_new();

    g_key_file_load_from_file( key_file, path, G_KEY_FILE_NONE, & int_error );
    if( int_error )
    {   /* Oops, something went wrong... */
        goto error;
    }

    /* Now that the file has been parsed, let's see whether this is a
     * desktop file we want to handle: */
    type = g_key_file_get_string( key_file, G_KEY_FILE_DESKTOP_GROUP,
        G_KEY_FILE_DESKTOP_KEY_TYPE, & int_error );
    if( int_error )
    {   /* Group or key not found. */
        goto error;
    }

    /* Check type -- we need "Application" or "XSession". */
    if( strcmp( type, G_KEY_FILE_DESKTOP_TYPE_APPLICATION ) != 0 &&
        strcmp( type, "XSession" ) != 0 )
    {   /* "This is not the type you are looking for." */
        g_set_error( error, SESS_DESKTOPENTRY_ERROR,
            SESS_DESKTOPENTRY_ERROR_INVALID_TYPE,
            "Desktop entry has invalid type: '%s'", type );
        g_free( type );
        g_key_file_free( key_file );
        return FALSE;
    }
    g_free( type );
    
    /* Now let's see whether there is an "Exec" key. */
    /* TODO: Handle escapes in the value of the Exec key. Only %i, %c and %k make sense for us. */
    temp_app_exec = g_key_file_get_string( key_file, G_KEY_FILE_DESKTOP_GROUP,
        G_KEY_FILE_DESKTOP_KEY_EXEC, & int_error );
    if( int_error )
    {   /* An error occurred, handle as usual: */
        goto error;
    }

    /* Get the localized application name: */
    temp_app_name = g_key_file_get_locale_string( key_file,
        G_KEY_FILE_DESKTOP_GROUP, G_KEY_FILE_DESKTOP_KEY_NAME,
        /* use current locale */ NULL, & int_error );
    if( int_error )
    {
        g_free( temp_app_exec );
        goto error;
    }

    /* If the Exec key does not contain a full path, we need to search $PATH. */
    /* Because the executable path can be quoted as well, we use g_shell_parse_argv(). */
    g_shell_parse_argv( temp_app_exec, & exec_argc, & exec_argv, & int_error );
    if( int_error )
    {
        g_free( temp_app_exec );
        g_free( temp_app_name );
        goto error;
    }
    
    executable = g_find_program_in_path( *exec_argv );
    if( executable )
    {   /* Program found in $PATH. Rebuild the command line: */
        g_free( temp_app_exec );
        g_free( *exec_argv );
        *exec_argv = executable;

        escaped_exec = g_string_new( NULL );
        parts = exec_argv;
        do
        {
            part = g_shell_quote( *parts );
            g_string_append( escaped_exec, part );
            g_string_append_c( escaped_exec, ' ' );
            g_free( part );
        }
        while( *(++parts) );
        
        *app_exec = g_string_free( escaped_exec, FALSE );
    }
    else
    {   /* It's not in $PATH. Bad. */
        g_warning( "Program `%s' not found in $PATH, launching session `%s' will most likely fail.",
                *exec_argv, path );
        *app_exec = temp_app_exec;
    }

    g_strfreev( exec_argv );
    *app_name = temp_app_name;
    
    /*g_debug( "%s => %s (%s)", path, *app_name, *app_exec );*/
    
    g_key_file_free( key_file );
    return TRUE;

error:
    g_propagate_error( error, int_error );
    g_key_file_free( key_file );
    return FALSE;
}
