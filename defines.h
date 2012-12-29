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
 * @file  defines.h
 * @brief Global application macros / configuration.
 */
#ifndef _SESS_DEFINES_H
#define _SESS_DEFINES_H

/**
 * @brief Application name.
 */
#define SESS_APP_NAME "sessionchooser"
/**
 * @brief Application version.
 */
#define SESS_APP_VERSION "1.2.5"
/**
 * @brief Short application copyright notice.
 *
 * This shows up in the application's version info.
 */
#define SESS_APP_COPYRIGHT "(c) 2010-2012 by Tilman Blumenbach"
/**
 * @brief Default search path for X session files (desktop entries).
 */
#define SESS_DEFAULT_XSESSION_SEARCH_PATH "/usr/share/xsessions:/etc/X11/sessions"
/**
 * @brief Default name for the environment variable used to override the
 *        default search path for X session files (desktop entries).
 * 
 *        If you change this, update dist/helper.sh as well.
 */
#define SESS_XSESSION_SEARCH_PATH_ENV "SESSIONCHOOSER_XPATH"
/**
 * @brief Default search path for text session files.
 */
#define SESS_DEFAULT_TEXTSESSION_SEARCH_PATH "/etc/sessionchooser/textsessions"
/**
 * @brief Default name for the environment variable used to override the
 *        default search path for text session files.
 * 
 *        If you change this, update dist/helper.sh as well.
 */
#define SESS_TEXTSESSION_SEARCH_PATH_ENV "SESSIONCHOOSER_TEXTPATH"
/**
 * @brief Default name for the environment variable used to specify additional
 *        arguments for the X server.
 * 
 *        If you change this, update dist/helper.sh as well.
 */
#define SESS_XARGS_ENV "SESSIONCHOOSER_XARGS"
/**
 * @brief The directory containing gettext message catalogs.
 */
#define SESS_GETTEXT_DIR "/usr/share/locale"
/**
 * @brief Name of the file used to save the last used session.
 *
 * This file resides in the user's home directory.
 */
#define SESS_LASTSESSION_FILE ".lastsession"

#endif /* ifndef _SESS_DEFINES_H */
