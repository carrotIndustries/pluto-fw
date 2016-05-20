/*
 * totp.c - implementation of the OATH TOTP algorithm
 * Copyright (C) 2011-2015 Simon Josefsson
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include "oath.h"
#include "hotp.h"

/**
 * oath_totp_generate:
 * @secret: the shared secret string
 * @secret_length: length of @secret
 * @now: Unix time value to compute TOTP for
 * @time_step_size: time step system parameter (typically 30)
 * @start_offset: Unix time of when to start counting time steps (typically 0)
 * @digits: number of requested digits in the OTP, excluding checksum
 * @output_otp: output buffer, must have room for the output OTP plus zero
 *
 * Generate a one-time-password using the time-variant TOTP algorithm
 * described in RFC 6238.  The input parameters are taken as time
 * values.
 *
 * The system parameter @time_step_size describes how long the time
 * window for each OTP is.  The recommended value is 30 seconds, and
 * you can use the value 0 or the symbol
 * %OATH_TOTP_DEFAULT_TIME_STEP_SIZE to indicate this.
 *
 * The system parameter @start_offset denote the Unix time when time
 * steps are started to be counted.  The recommended value is 0, to
 * fall back on the Unix epoch) and you can use the symbol
 * %OATH_TOTP_DEFAULT_START_TIME to indicate this.
 *
 * The @output_otp buffer must have room for at least @digits
 * characters, plus one for the terminating NUL.
 *
 * Currently only values 6, 7 and 8 for @digits are supported.  This
 * restriction may be lifted in future versions.
 *
 * Returns: On success, %OATH_OK (zero) is returned, otherwise an
 *   error code is returned.
 *
 * Since: 1.4.0
 **/
int
oath_totp_generate (const char *secret,
		    size_t secret_length,
		    time_t now,
		    unsigned time_step_size,
		    time_t start_offset, unsigned digits, int32_t *output_otp)
{
  return oath_totp_generate2 (secret, secret_length, now, time_step_size,
			      start_offset, digits, 0, output_otp);
}

/**
 * oath_totp_generate2:
 * @secret: the shared secret string
 * @secret_length: length of @secret
 * @now: Unix time value to compute TOTP for
 * @time_step_size: time step system parameter (typically 30)
 * @start_offset: Unix time of when to start counting time steps (typically 0)
 * @digits: number of requested digits in the OTP, excluding checksum
 * @flags: flags indicating mode, one of #oath_totp_flags
 * @output_otp: output buffer, must have room for the output OTP plus zero
 *
 * Generate a one-time-password using the time-variant TOTP algorithm
 * described in RFC 6238.  The input parameters are taken as time
 * values.
 *
 * The system parameter @time_step_size describes how long the time
 * window for each OTP is.  The recommended value is 30 seconds, and
 * you can use the value 0 or the symbol
 * %OATH_TOTP_DEFAULT_TIME_STEP_SIZE to indicate this.
 *
 * The system parameter @start_offset denote the Unix time when time
 * steps are started to be counted.  The recommended value is 0, to
 * fall back on the Unix epoch) and you can use the symbol
 * %OATH_TOTP_DEFAULT_START_TIME to indicate this.
 *
 * The @output_otp buffer must have room for at least @digits
 * characters, plus one for the terminating NUL.
 *
 * Currently only values 6, 7 and 8 for @digits are supported.  This
 * restriction may be lifted in future versions.
 *
 * The @flags parameter may be used to change the MAC function, for
 * example %OATH_TOTP_HMAC_SHA256 or %OATH_TOTP_HMAC_SHA512.
 *
 * Returns: On success, %OATH_OK (zero) is returned, otherwise an
 *   error code is returned.
 *
 * Since: 2.6.0
 **/
int
oath_totp_generate2 (const char *secret,
		     size_t secret_length,
		     time_t now,
		     unsigned time_step_size,
		     time_t start_offset,
		     unsigned digits, int flags, int32_t *output_otp)
{
  uint64_t nts;

  if (time_step_size == 0)
    time_step_size = OATH_TOTP_DEFAULT_TIME_STEP_SIZE;

  nts = (now - start_offset) / time_step_size;

  return _oath_hotp_generate2 (secret,
			       secret_length,
			       nts,
			       digits,
			       false, OATH_HOTP_DYNAMIC_TRUNCATION, flags,
			       output_otp);
}
