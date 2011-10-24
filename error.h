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
/**
 * @file  error.h
 * @brief Functions related to error handling using glib.
 */

#ifndef _SESS_ERROR_H
#define _SESS_ERROR_H

#include <glib.h>

/**
 * @brief The error domain of the Desktopentry module.
 */
#define SESS_DESKTOPENTRY_ERROR sess_desktopentry_error_quark()

/**
 * @brief The error domain used for SessCmdLineOptionErrors.
 */
#define SESS_CMDLINEOPTION_ERROR sess_cmdlineoption_error_quark()


/**
 * @brief Error codes used by the Desktopentry module.
 *
 * - SESS_DESKTOPENTRY_ERROR_INVALID_TYPE: The desktop entry has an invalid
 *     type (neither "XSession" nor "Application").
 */
typedef enum
{
    SESS_DESKTOPENTRY_ERROR_INVALID_TYPE,
} SessDesktopentryError;

/**
 * @brief Error codes used by command line option parsing callbacks.
 *
 * - SESS_CMDLINEOPTION_ERROR_UNHANDLED_OPT: _opt_info_cb() [defined in main,c]
 *      was called for an command line option it does not handle.
 */
typedef enum
{
    SESS_CMDLINEOPTION_ERROR_UNHANDLED_OPT,
} SessCmdLineOptionError;

/**
 * @brief Return codes used in main.c
 *
 * These are not actually used with glib's error handling functions.
 *
 * - SESS_MAIN_ERROR_NOT_A_TTY: stdin is not associated with a TTY.
 */
typedef enum
{
    SESS_MAIN_ERROR_NOT_A_TTY = 2,
} SessMainError;


/**
 * @brief Returns a GQuark representing the error domain of the Desktopentry
 *        module.
 */
GQuark sess_desktopentry_error_quark( void );

/**
 * @brief Returns a GQuark representing the error domain used for
 *        SessCmdLineOptionErrors.
 */
GQuark sess_cmdlineoption_error_quark( void );

#endif /* ifndef _SESS_ERROR_H */
