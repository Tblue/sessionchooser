/* sessionchooser - a text-based session chooser
 *
 * Copyright (c) 2010, Tilman Blumenbach <tilman@ax86.net>
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
 * @file  desktopentry.h
 * @brief Implements a Desktop Entry parser using glib.
 */

#ifndef _SESS_DESKTOPENTRY_H
#define _SESS_DESKTOPENTRY_H

#include <glib.h>

/**
 * @brief Parse a desktop entry and return its Name and Exec keys.
 *
 * On failure, error is set to one of:
 *  - a GFileError or a GKeyFileError: When the file could not be opened.
 *  - G_KEY_FILE_ERROR_KEY_NOT_FOUND or G_KEY_FILE_ERROR_GROUP_NOT_FOUND:
 *      When a requested key or a group could not be found.
 *  - SESS_DESKTOPENTRY_ERROR_INVALID_TYPE: When the desktop file has an
 *      invalid type (not Application or XSession).
 * 
 * @param  path     The path to the file to parse.
 * @param  app_name Return location for the application name (free using
 *                  g_free()).
 * @param  app_exec Return location for the Exec key (free using
 *                  g_free()).
 * @param  error    Return location for a GError, or NULL.
 * @return          TRUE on success, FALSE on failure.
 */
gboolean sess_desktopentry_read( const gchar *path, gchar **app_name,
                                 gchar **app_exec, GError **error );

#endif /* ifndef _SESS_DESKTOPENTRY_H */
