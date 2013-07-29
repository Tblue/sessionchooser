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
/**
 * @file  util.h
 * @brief Misc. utility functions.
 */

#ifndef _SESS_UTIL_H
#define _SESS_UTIL_H

#include <glib.h>
#include <glib/gstdio.h>

/**
 * @brief Split a string using a delimiter and call a callback function
 *        for each part.
 * @param string The input string.
 * @param delimiter The delimiter used to split the string.
 * @param max_tokens See g_strsplit().
 * @param func Function to call for each part.
 * @param user_data Data to pass to the callback function.
 */
void sess_strsplit_foreach( const gchar *string, const gchar *delimiter,
                            gint max_tokens, GFunc func, gpointer user_data );
/**
 * @brief Get the next free X display.
 *
 * Algorithm borrowed from qingy, http://qingy.sf.net
 *
 * @todo   This probably only works with Xorg, what about other X servers?
 * @return The next free X display, or -1 on failure.
 */
gint get_next_free_XDisplay( void );
/**
 * @brief Get the active VT.
 *
 * Stolen from qingy, http://qingy.sf.net
 *
 * @todo   This is Linux-specific! Make it portable (if possible). 
 * @return The active VT, or -1 on failure.
 */
gshort get_active_vt( void );
/**
 * @brief  Initialize the terminfo database.
 * @return FALSE on error, TRUE on success.
 */
gboolean terminfo_init( void );
/**
 * @brief Clear the screen.
 */
void terminfo_clear_screen( void );
/**
 * @brief Get the user's last session's name.
 * @return The session name or NULL on error.
 */
gchar *get_last_session( void );
/**
 * @brief  Set the user's last session's name.
 * @param  sess_name The session name.
 * @param  error Return location for a GError.
 * @return TRUE on success, FALSE otherwise.
 */
gboolean set_last_session( const gchar *sess_name, GError **error );
/**
 * @brief  Convert a filename to UTF-8.
 * @param  path The path to convert.
 * @return The converted path or a copy of the original path if
 *         the conversion failed. Never NULL.
 */
gchar *filename_to_utf8_nofail( const gchar *path );

#endif /* ifndef _SESS_UTIL_H */
