#!/bin/sh
# This file is part of sessionchooser - a text-based session chooser.
#
# This is the helper script which runs sessionchooser and tries to start
# the session that has been chosen. Simply source it e. g. from your
# ~/.bash_login or ~/.zprofile.
#
# Copyright (c) 2010, Tilman Blumenbach <tilman@ax86.net>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  - Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  - Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#  - Neither the name of the copyright holder nor the names of the
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Delete the temporary file used to store the session command line and
# unset our environment variables.
sess_cleanup() {
    rm -f "${_SESS_TMPFILE}"

    unset _SESS_TMPFILE ret
    # NB: These names are defined in defines.h.
    unset SESSIONCHOOSER_XPATH SESSIONCHOOSER_TEXTPATH SESSIONCHOOSER_XARGS
    unset -f sess_cleanup
}

_SESS_TMPFILE="$(mktemp)"

sessionchooser 3> "${_SESS_TMPFILE}"
ret=$?

if [ $ret -eq 0 ]
then
    _SESS_CMD="exec $(cat "${_SESS_TMPFILE}")"
    sess_cleanup
    eval "${_SESS_CMD}"
elif [ $ret -gt 1 ]
then
    echo "NOTE: sessionchooser failed! Please read the above error messages" \
         "for more information."
fi

sess_cleanup
