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
#include <glib.h>
#include <glib/gprintf.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "defines.h"
#include "error.h"
#include "i18n.h"
#include "session.h"
#include "util.h"

/**
 * @brief Current session number.
 *
 * Used by _display_session(). Initialize to 1.
 */
static guint _sess_num;

/* The following variables get set by the command line parser. */
/**
 * @brief Whether to NOT clear the screen using terminfo on program start
 *        and end.
 */
static gboolean no_clear_screen = FALSE;
/**
 * @brief Where to write the session command string (file descriptor number).
 */
static gint output_fd           = 3;
/**
 * @brief If this variable is FALSE, the last used session is neither
 *        read nor recorded.
 */
static gboolean no_lastsession = FALSE;

/**
 * @brief The option context used to parse command line options in main().
 *
 * Used by _opt_info_cb().
 */
static GOptionContext *opt_ctx = NULL;

/**
 * @brief Callback for various command line options displaying information
 *        on program usage.
 *
 * The following command line options cause this function to be called:
 *   -h/--help
 *   -v/--version
 *
 * Calling this function terminates the program.
 * 
 * @see GOptionArgFunc()
 */
static gboolean _opt_info_cb( const gchar *opt_name,
                              const gchar G_GNUC_UNUSED( *val ),
                              gpointer G_GNUC_UNUSED( data ),
                              GError **error )
{
    gchar *default_help;

    if( strcmp( opt_name, "-h" ) == 0 || strcmp( opt_name, "-?" ) == 0 ||
            strcmp( opt_name, "--help" ) == 0 )
    {   /* Display help. */
        default_help = g_option_context_get_help( opt_ctx, FALSE, NULL );
        puts( default_help );
        g_free( default_help );

        /* TODO: Perhaps this should be split into multiple strings. */
        g_printf( _( "Additionally, the following environment variables are recognized:\n"

            "  %s\t\tSearch path for desktop entry files (follows standard PATH\n"
            "\t\t\t\tsyntax). The desktop entries have to be of type '%s'\n"
            "\t\t\t\tor 'XSession'.\n"
            "\t\t\t\tDefault: %s\n"

            "  %s\tSearch path for text session files (follows standard PATH\n"
            "\t\t\t\tsyntax). Text session files are simple shell scripts. Non-executable\n"
            "\t\t\t\tfiles are skipped.\n"
            "\t\t\t\tDefault: %s\n"

            "  %s\t\tAdditional arguments for the X server.\n\n" ),
            SESS_XSESSION_SEARCH_PATH_ENV, G_KEY_FILE_DESKTOP_TYPE_APPLICATION,
            SESS_DEFAULT_XSESSION_SEARCH_PATH,
            SESS_TEXTSESSION_SEARCH_PATH_ENV, SESS_DEFAULT_TEXTSESSION_SEARCH_PATH,
            SESS_XARGS_ENV );

        puts( _( "Compile-time defaults:" ) );
        g_printf( _( "  Gettext message catalog directory\t%s\n" ), SESS_GETTEXT_DIR );
        g_printf( _( "  Session save file\t\t\t~/%s\n" ), SESS_LASTSESSION_FILE );

        exit( 0 );
    }
    else if( strcmp( opt_name, "-v" ) == 0 || strcmp( opt_name, "--version" ) == 0 )
    {   /* Display version information. */
        puts( SESS_APP_NAME " v" SESS_APP_VERSION );
        g_printf( _( "Compiled on: %s\n\n" ), __DATE__ " " __TIME__ );

        puts( "(c) 2010-2011 by Tilman Blumenbach <tilman@ax86.net>" );
        puts( _( "Released under the BSD license." ) );

        exit( 0 );
    }

    /* Unhandled option! */
    g_set_error( error, SESS_CMDLINEOPTION_ERROR,
        SESS_CMDLINEOPTION_ERROR_UNHANDLED_OPT, "Internal error: Unhandled "
            "option %s", opt_name );
    return FALSE;
}

/**
 * @brief Available command line options.
 */
static GOptionEntry options[]   = {
    { "help", 'h', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, _opt_info_cb,
        N_( "Show help options" ), NULL },
    { "help-long-dummy", '?', G_OPTION_FLAG_HIDDEN | G_OPTION_FLAG_NO_ARG,
        G_OPTION_ARG_CALLBACK, _opt_info_cb,
        N_( "Show help options" ), NULL },
    { "no-clear", 'n', 0, G_OPTION_ARG_NONE, & no_clear_screen,
        N_( "Do not clear the screen on program start and end" ), NULL },
    { "output-fd", 'o', 0, G_OPTION_ARG_INT, & output_fd, N_( "Use file "
            "descriptor FD to output the command string (default: 3)" ),
        "FD" },
    { "no-lastsession", 'l', 0, G_OPTION_ARG_NONE, & no_lastsession,
        N_( "Do not remember the last used session" ), NULL },
    { "version", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
        _opt_info_cb, N_( "Display the program version" ), NULL },
    { NULL, 0, 0, 0, NULL, NULL, NULL }
};

/**
 * @brief Sorting callback for g_slist_sort().
 * @param a First (pointer to a) SessSession object.
 * @param b Second (pointer to a) SessSession object.
 * @return See g_slist_sort().
 */
static gint _session_sort_func( gconstpointer a, gconstpointer b )
{
    return strcmp( sess_session_get_name_coll_key_case( (SessSession *)a ),
        sess_session_get_name_coll_key_case( (SessSession *)b ) );
}

/**
 * @brief Callback for g_slist_foreach() to display a session to the user.
 * @param _session Pointer to a SessSession object.
 * @param unused (unused parameter)
 */
static void _display_session( gpointer _session, gpointer G_GNUC_UNUSED( unused ) )
{
    SessSession *session = (SessSession *)_session;
    const gchar *format;

    if( session->use_xinit )
    {   /* X Session */
        format = _( "  (%d) %s\n" );
    }
    else
    {   /* Text session */
        format = _( "  (%d) Text: %s\n" );
    }

    g_printf( format, _sess_num++, sess_session_get_name_locale( session ) );
}

/**
 * @brief Signal handler.
 * @param sig Number of signal to handle. Currently supported: SIGINT.
 */
void sighandler( int sig )
{
    switch( sig )
    {
        case SIGINT:
            /* Disallow quitting using e. g. ^C in order to be able to
             * easily return an appropriate exit code. */
            puts( _( "\nUse EOF (e. g. ^D) to quit." ) );
            break;

        /* Other signals are ignored. Currently, sighandler() is only being
         * called for SIGINT, anyway. */
    }
}

/**
 * @brief Write the command string for a session to output_fd.
 * @param sess The session to operate on.
 * @return TRUE on success, FALSE on failure.
 */
gboolean process_session( SessSession *sess )
{
    gchar *command;
    gchar *script_path;
    gshort active_vt;
    const gchar *xargs;
    gint XDisplay;
    ssize_t res;

    if( sess->use_xinit )
    {   /* This is a X session, we need to use xinit: */
        if( ( XDisplay = get_next_free_XDisplay() ) == -1 )
        {
            g_critical( "Could not get a free X display to use!" );
            return FALSE;
        }

        xargs = g_getenv( SESS_XARGS_ENV );
        if( ( active_vt = get_active_vt() ) != -1 )
        {   /* Start X on the current VT: */
            command = g_strdup_printf( "xinit %s -- :%d vt%hd %s "
                            "1>/dev/null 2>&1",
                        sess_session_get_exec_locale( sess ), XDisplay,
                        active_vt, xargs ? xargs : "" );
        }
        else
        {
            command = g_strdup_printf( "xinit %s -- :%d %s "
                            "1>/dev/null 2>&1",
                        sess_session_get_exec_locale( sess ), XDisplay,
                        xargs ? xargs : "" );
        }
    }
    else
    {   /* This is a text session, simply pass the file name to sh. */
        script_path = g_shell_quote( sess_session_get_exec_locale( sess ) );
        command = g_strconcat( "sh ", script_path, NULL );
        g_free( script_path );
    }
    
    res = write( output_fd, command, strlen( command ) );
    g_free( command );

    if( res == -1 )
    {
        g_critical( "Could not write() to output file descriptor (#%d)!",
            output_fd );
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief Length of the buffer used for reading the user's response.
 *
 * 5 bytes should be sufficient, it allows for a four-digit number, which
 * means the highest possible number is 9999. Nobody has that many sessions.
 */
#define CHOICE_BUF_LEN 5
/**
 * @brief Ask the user which session he would like to start.
 * @param session_list GSList of SessSession objects the user can choose
 *                     from.
 * @param last_session Last used session's normalized name or NULL.
 *                     "" (empty string) means that the last session was
 *                     the shell.
 * @return TRUE if the user chose either a X session or a text session.
 *         FALSE if he/she wants to continue with the current shell.
 */
gboolean ask_for_session( GSList *session_list, gchar *last_session )
{
    GError *error = NULL;
    unsigned int choice;
    char buf[CHOICE_BUF_LEN];
    char *endptr;
    gchar *session_name;
    SessSession *default_session = NULL;
    SessSession *chosen_session;
    /* Strictly speaking, this variable is not necessary; however, for the
       sake of code understandability, it is being used. :-) */
    gboolean default_is_current_shell = FALSE;
    /** @brief Ignore no_lastsession and do not remember the user's choice? */
    gboolean no_lastsession_override;

    if( last_session )
    {   /* We got the name of the last chosen session, try to find it: */
        if( ! *last_session )
        {   /* An empty string means the current shell. */
            default_is_current_shell = TRUE;
        }
        else
        {
            default_session = sess_session_find_by_name_normalized( session_list,
                last_session );
        }
    }

    for( ;; )
    {
        if( ! no_clear_screen )
        {
            terminfo_clear_screen();
        }

        fputs( _( "Welcome, please choose a session!\n" ), stdout );
        if( ! no_lastsession )
        {
            fputs( _( "Append '!' to the number to prevent remembering your choice.\n\n" ),
                stdout );
        }
        fputs( _( "  (0) continue using the current shell\n" ), stdout );

        chosen_session = NULL;
        _sess_num = 1;
        no_lastsession_override = FALSE;

        g_slist_foreach( session_list, _display_session, NULL );

        if( default_session )
        {
            g_printf( _( "\nYour choice [%s]: " ),
                sess_session_get_name_locale( default_session ) );
        }
        else if( default_is_current_shell )
        {
            fputs( _( "\nYour choice [<current shell>]: " ), stdout );
        }
        else {
            fputs( _( "\nYour choice: " ), stdout );
        }

        if( ! fgets( buf, CHOICE_BUF_LEN, stdin ) )
        {   /* Error or EOF -- silently exit. */
            return FALSE;
        }
        else if( *buf == '\n' )
        {   /* Use default value. */
            /* NOTE: No support for no_lastsession_override here because
             *       if we get here, then we are going to start the
             *       default session anyway -- the lastsession file won't
             *       be updated.
             */
            if( default_session )
            {
                chosen_session = default_session;
            }
            else if( ! default_is_current_shell )
            {
                puts( _( "There is no default session! Please make a choice." ) );
                sleep( 1 );
                continue;
            }
            else
            {   /* Use current shell. */
                break;
            }
        }
        else
        {
            choice = strtoul( buf, & endptr, 10 );

            if( endptr == buf )
            {   /* No digits. */
                puts( _( "Please enter a number." ) );
                sleep( 1 );
                continue;
            }
            else if( *endptr == '!' )
            {
                no_lastsession_override = TRUE;
            }

            if( ! choice )
            {   /* We want to continue with the current shell. */
                break;
            }
            else if( ! ( chosen_session = g_slist_nth_data( session_list,
                                choice - 1 ) ) )
            {   /* Invalid session specified. */
                puts( _( "No such session. Please try again..." ) );
                sleep( 1 );
                continue;
            }
        }

        /* We got a session! Let's try to process it. */
        if( process_session( chosen_session ) )
        {   /* Everything went fine. */
            break;
        }

        puts( _( "Could not process session! Please try again..." ) );
        sleep( 1 );
    }

    /* If we get here, everything went fine... Let's try to save the last
     * used session, if requested: */
    if( ! no_lastsession && ! no_lastsession_override )
    {
        /* If chosen_session is NULL, then we want to use the current shell. */
        session_name = chosen_session
                ? sess_session_get_name_normalized( chosen_session )
                : "";

        /* Only update if we need to. */
        if( ( ! last_session || strcmp( session_name, last_session ) != 0 ) &&
            ! set_last_session( session_name, & error ) )
        {
            g_warning( "Could not save last used session: %s",
                error->message );
            g_error_free( error );
        }
    }

    return chosen_session ? TRUE : FALSE;
}

/**
 * @brief This application's main function.
 * @param argc Number of elements in argv.
 * @param argv Command line arguments.
 * @return 0 on success. Read the command string to execute from output_fd.
 *         1 if the current shell is to be used.
 *         A SessMainError when an error occurred.
 */
int main( int argc, char **argv )
{
    GError *error = NULL;
    const gchar *temp;
    /**
     * @brief The last used session's normalized name.
     *
     * If the last used session was the current shell, this variable points
     * to an empty string.
     */
    gchar *last_session = NULL;
    GSList *session_list = NULL;
    int ret = 0;

    /* Initialize locale handling: */
    i18n_init();

    opt_ctx = g_option_context_new( _( "- a text-based session chooser" ) );
    g_option_context_set_summary( opt_ctx, _( "Displays a list of available "
        "sessions and prints a command line for the chosen session to the\n"
        "file descriptor given by the -o option." ) );
    g_option_context_set_translation_domain( opt_ctx, SESS_APP_NAME );
    g_option_context_set_help_enabled( opt_ctx, FALSE );
    g_option_context_add_main_entries( opt_ctx, options, SESS_APP_NAME );

    g_option_context_parse( opt_ctx, & argc, & argv, & error );
    g_option_context_free( opt_ctx );

    if( error )
    {
        g_error( "Could not parse options: %s", error->message ); /* calls abort() */
    }
    else if( ! isatty( STDIN_FILENO ) )
    {   /* If we are not running interactively, exit immediately. */
        return SESS_MAIN_ERROR_NOT_A_TTY;
    }
    else if( fcntl( output_fd, F_GETFD ) == -1 )
    {
        g_error( "File descriptor %d is invalid.", output_fd ); /* calls abort() */
    }

    /* Get search paths. */
    /* Desktop entry search path: */
    temp = g_getenv( SESS_XSESSION_SEARCH_PATH_ENV );
    if( ! temp )
    {   /* Use default X session search path: */
        temp = SESS_DEFAULT_XSESSION_SEARCH_PATH;
    }
    
    /* Search the X session search path for desktop entry files: */
    sess_strsplit_foreach( temp, G_SEARCHPATH_SEPARATOR_S, -1,
        parse_xsession_files_in_dir, & session_list );

    /* Text session search path: */
    temp = g_getenv( SESS_TEXTSESSION_SEARCH_PATH_ENV );
    if( ! temp )
    {   /* Use default text session search path_ */
        temp = SESS_DEFAULT_TEXTSESSION_SEARCH_PATH;
    }
    
    /* Search the text session search path: */
    sess_strsplit_foreach( temp, G_SEARCHPATH_SEPARATOR_S, -1,
        parse_textsession_files_in_dir, & session_list );


    /* Sanity check: Are there any sessions?
     * We do not use g_slist_length() here because we only want to check
     * whether there are *any* elements, not how many. */
    if( ! g_slist_nth( session_list, 0 ) )
    {   /* No elements?! */
        /* TODO: Not sure if this is necessary... */
        g_slist_free( session_list );
        /* A return code of 1 means "use current shell". */
        return 1;
    }

    /* session_list now should contain a list of sessions found in all the
     * directories. Sort the list: */
    session_list = g_slist_sort( session_list, _session_sort_func );

    /* Do some more initializations (for features used later on). */
    if( ! no_clear_screen && ! terminfo_init() )
    {
        g_error( "Could not initialize terminfo DB." ); /* calls abort() */
    }
    signal( SIGINT, sighandler );

    /* Read the last used session if we are allowed to do so. */
    if( ! no_lastsession )
    {
        last_session = get_last_session();
    }

    /* Now display the list to the user and allow him to select a session. */
    if( ! ask_for_session( session_list, last_session ) )
    {   /* Continue with the current shell... */
        ret = 1;
    }

    /* Clear the screen if that is okay: */
    if( ! no_clear_screen )
    {
        terminfo_clear_screen();
    }

    /* Clean up: */
    close( output_fd );
    g_free( last_session );
    sess_slist_free_all( session_list, TRUE );

    return ret;
}
