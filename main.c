/* sessionchooser - a text-based session chooser
 *
 * Copyright (c) 2010-2016, Tilman Blumenbach <tilman AT ax86 DOT net>
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
 * @brief If this variable is TRUE, then the stderr of the generated command
 *        is redirected to /dev/null. Note that currently, stdout is always
 *        discarded.
 */
static gboolean discard_stderr = FALSE;

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
                              G_GNUC_UNUSED const gchar *val,
                              G_GNUC_UNUSED gpointer data,
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
        g_printf( _( "  Session save file\t\t\t~/%s\n" ), SESS_LASTSESSION_FILE );

        exit( 0 );
    }
    else if( strcmp( opt_name, "-v" ) == 0 || strcmp( opt_name, "--version" ) == 0 )
    {   /* Display version information. */
        g_printf( _( "%s v%s (Git revision: %s)\n" ), SESS_APP_NAME, SESS_APP_VERSION,
                SESS_GITREV );
        g_printf( _( "Compiled on: %s\n\n" ), __DATE__ " " __TIME__ );

        puts( SESS_APP_COPYRIGHT );
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
    { "discard-stderr", 'S', 0, G_OPTION_ARG_NONE, & discard_stderr,
        N_( "Redirect stderr of the generated command to /dev/null" ), NULL },
    { "version", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
        _opt_info_cb, N_( "Display the program version" ), NULL },
    { NULL, 0, 0, 0, NULL, NULL, NULL }
};

/**
 * @brief Callback for g_ptr_array_foreach() to display a session to the user.
 * @param _session Pointer to a SessSession object.
 * @param unused (unused parameter)
 */
static void _display_session( gpointer _session, G_GNUC_UNUSED gpointer unused )
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
                            "1>/dev/null%s",
                        sess_session_get_exec_locale( sess ), XDisplay,
                        active_vt, xargs ? xargs : "",
                        discard_stderr ? " 2>&1" : "" );
        }
        else
        {
            command = g_strdup_printf( "xinit %s -- :%d %s "
                            "1>/dev/null%s",
                        sess_session_get_exec_locale( sess ), XDisplay,
                        xargs ? xargs : "",
                        discard_stderr ? " 2>&1" : "" );
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
 */
#define CHOICE_BUF_LEN 32
/**
 * @brief Ask the user which session he would like to start.
 * @param session_list A pointer to a SessList (session list).
 * @param last_session Last used session's normalized name or NULL.
 *                     "" (empty string) means that the last session was
 *                     the shell.
 * @return TRUE if the user chose either a X session or a text session.
 *         FALSE if he/she wants to continue with the current shell.
 */
gboolean ask_for_session( SessList *session_list, gchar *last_session )
{
    GError *error = NULL;
    unsigned int choice;
    char buf[CHOICE_BUF_LEN];
    char *endptr, *choice_text;
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
            default_session = sess_session_find_by_path_normalized( session_list,
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

        g_ptr_array_foreach( session_list->list, _display_session, NULL );

        if( default_session )
        {
            choice_text = default_session->use_xinit
                ? _("\nYour choice [%s]: ") /* X session */
                : _("\nYour choice [Text: %s]: "); /* Text session */
            g_printf( choice_text,
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
        
        g_strchug( buf );
        if( ! *buf  )
        {   /* User didn't enter anything -- use default value. */
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
            else if( choice > session_list->list->len )
            {   /* Invalid session specified. */
                puts( _( "No such session. Please try again..." ) );
                sleep( 1 );
                continue;
            }

            chosen_session = g_ptr_array_index( session_list->list, choice - 1 );
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
                ? sess_session_get_path_normalized( chosen_session )
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
    SessList *session_list;
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
        g_critical( "Could not parse options: %s", error->message );
        return SESS_MAIN_ERROR_OPTPARSE;
    }
    else if( ! isatty( STDIN_FILENO ) )
    {   /* If we are not running interactively, exit immediately. */
        return SESS_MAIN_ERROR_NOT_A_TTY;
    }
    else if( fcntl( output_fd, F_GETFD ) == -1 )
    {
        g_critical( "File descriptor %d is invalid.", output_fd );
        return SESS_MAIN_ERROR_INV_OUTFD;
    }

    /* Initialize session list. */
    session_list = sess_list_new();

    /* Get search paths. */
    /* Desktop entry search path: */
    temp = g_getenv( SESS_XSESSION_SEARCH_PATH_ENV );
    if( ! temp )
    {   /* Use default X session search path: */
        temp = SESS_DEFAULT_XSESSION_SEARCH_PATH;
    }
    
    /* Search the X session search path for desktop entry files: */
    sess_strsplit_foreach( temp, G_SEARCHPATH_SEPARATOR_S, -1,
        parse_xsession_files_in_dir, session_list );

    /* Text session search path: */
    temp = g_getenv( SESS_TEXTSESSION_SEARCH_PATH_ENV );
    if( ! temp )
    {   /* Use default text session search path_ */
        temp = SESS_DEFAULT_TEXTSESSION_SEARCH_PATH;
    }
    
    /* Search the text session search path: */
    sess_strsplit_foreach( temp, G_SEARCHPATH_SEPARATOR_S, -1,
        parse_textsession_files_in_dir, session_list );


    /* Sanity check: Are there any sessions? */
    if( ! session_list->list->len )
    {   /* No elements?! */
        sess_list_free( session_list );
        /* A return code of 1 means "use current shell". */
        return 1;
    }

    /* session_list now should contain a list of sessions found in all the
     * directories. Sort the list: */
    sess_list_sort( session_list );

    /* Do some more initializations (for features used later on). */
    if( ! no_clear_screen && ! terminfo_init() )
    {
        g_critical( "Could not initialize terminfo DB." );
        return SESS_MAIN_ERROR_TERMINFO;
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
    sess_list_free( session_list );

    return ret;
}
