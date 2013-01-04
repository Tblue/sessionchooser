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
 * @file  session.h
 * @brief Session list functions.
 */

#ifndef _SESS_SESSION_H
#define _SESS_SESSION_H

#include <glib.h>
#include <glib/gstdio.h>

/**
 * @brief Represents a single session.
 *
 * - name: The name of the session (UTF-8 encoded).
 * - name_normalized: Normalized "name" suitable for comparison with another
 *                    normalized UTF-8 string.
 * - name_coll_key_case: Result of g_utf8_collate_key() called with the
 *                       string returned by g_utf8_casefold() for "name"
 *                       -- i. e. a case-insensitive representation of
 *                       "name" suitable for comparison with another
 *                       collation key. Used for locale-dependent ordering.
 * - name_locale: name in a locale-dependent charset (might actually be
 *                the same as "name" if the charset conversion failed).
 * - exec: The "Exec" key of the session (UTF-8 encoded).
 * - exec_locale: "exec" in a locale-dependent charset (might actually be
 *                the same as "exec" if the charset conversion failed).
 * - use_xinit: Whether to execute the session using xinit. Default: FALSE.
 */
typedef struct
{
    gchar *name;
    gchar *name_normalized;
    gchar *name_coll_key_case;
    gchar *name_locale;
    gchar *exec;
    gchar *exec_locale;
    gboolean use_xinit;
} SessSession;

/**
 * @brief  Create a new SessSession struct.
 * @return A pointer to a newly allocated, zeroed SessSession struct.
 */
SessSession *sess_session_new( void );
/**
 * @brief Free resources used by a SessSession struct.
 * @param sess A pointer to a SessSession struct.
 */
void sess_session_free( SessSession *sess );
/**
 * @brief Get the "name_normalized" key of a SessSession struct.
 *
 * The key is created when missing.
 *
 * @param sess Pointer to a SessSession struct.
 * @return The "name_normalized" key.
 */
gchar *sess_session_get_name_normalized( SessSession *sess );
/**
 * @brief Get the "name_coll_key_case" key of a SessSession struct.
 *
 * The key is created when missing.
 *
 * @param sess Pointer to a SessSession struct.
 * @return The "name_coll_key_case" key.
 */
gchar *sess_session_get_name_coll_key_case( SessSession *sess );
/**
 * @brief Get the "name_locale" key of a SessSession struct.
 *
 * The key is created when missing.
 *
 * @param sess Pointer to a SessSession struct.
 * @return The "name_locale" key.
 */
gchar *sess_session_get_name_locale( SessSession *sess );
/**
 * @brief Get the "exec_locale" key of a SessSession struct.
 *
 * The key is created when missing.
 *
 * @param sess Pointer to a SessSession struct.
 * @return The "exec_locale" key.
 */
gchar *sess_session_get_exec_locale( SessSession *sess );
/**
 * @brief Try to find a SessSession by its normalized name.
 * @param sess_list A pointer to a GPtrArray of SessSession structs.
 * @param name_normalized The normalized name of a session to search for.
 * @return The session, if found; NULL otherwise.
 */
SessSession *sess_session_find_by_name_normalized( GPtrArray *sess_list,
                                        const gchar *name_normalized );

/**
 * @brief Parse all session entry files in a directory.
 * @param _dir Pointer to a const gchar containing the directory to search.
 * @param _session_list Pointer to a GPtrArray used to store SessSession structs.
 */
void parse_xsession_files_in_dir( gpointer _dir, gpointer _session_list );
/**
 * @brief Get all executable files in a directory.
 * @param _dir Pointer to a const gchar containing the directory to search.
 * @param _session_list Pointer to a GPtrArray used to store SessSession structs.
 */
void parse_textsession_files_in_dir( gpointer _dir, gpointer _session_list );

#endif /* ifndef _SESS_SESSION_H */
