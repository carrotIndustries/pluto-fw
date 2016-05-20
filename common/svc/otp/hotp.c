/*
 * hotp.c - implementation of the OATH HOTP algorithm
 * Copyright (C) 2009-2015 Simon Josefsson
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
#include "hmac.h"
#include "hotp.h"
#include "gc.h"


/**
 * oath_hotp_generate:
 * @secret: the shared secret string
 * @secret_length: length of @secret
 * @moving_factor: a counter indicating the current OTP to generate
 * @digits: number of requested digits in the OTP, excluding checksum
 * @add_checksum: whether to add a checksum digit or not
 * @truncation_offset: use a specific truncation offset
 * @output_otp: output buffer, must have room for the output OTP plus zero
 *
 * Generate a one-time-password using the HOTP algorithm as described
 * in RFC 4226.
 *
 * Use a value of %OATH_HOTP_DYNAMIC_TRUNCATION for @truncation_offset
 * unless you really need a specific truncation offset.
 *
 * To find out the size of the OTP you may use the OATH_HOTP_LENGTH()
 * macro.  The @output_otp buffer must be have room for that length
 * plus one for the terminating NUL.
 *
 * Currently only values 6, 7 and 8 for @digits are supported, and the
 * @add_checksum value is ignored.  These restrictions may be lifted
 * in future versions, although some limitations are inherent in the
 * protocol.
 *
 * Returns: On success, %OATH_OK (zero) is returned, otherwise an
 *   error code is returned.
 **/
int
oath_hotp_generate (const char *secret,
		    size_t secret_length,
		    uint64_t moving_factor,
		    unsigned digits,
		    bool add_checksum,
		    size_t truncation_offset, int32_t *output_otp)
{
  return _oath_hotp_generate2 (secret, secret_length, moving_factor, digits,
			       add_checksum, truncation_offset, 0,
			       output_otp);
}

/* Takes @flags to override use of MAC.  HOTP is only specified for
   HMAC-SHA1 which is why this function is not exported. */
int
_oath_hotp_generate2 (const char *secret,
		      size_t secret_length,
		      uint64_t moving_factor,
		      unsigned digits,
		      bool add_checksum,
		      size_t truncation_offset, int flags, int32_t *output_otp)
{
  char hs[GC_SHA512_DIGEST_SIZE];
  int hssize = GC_SHA1_DIGEST_SIZE;
  int32_t S;

  (void) add_checksum;
  (void) truncation_offset;
  (void) flags;

  {
    char counter[sizeof (moving_factor)];
    size_t i;
    Gc_rc rc;

    for (i = 0; i < sizeof (counter); i++)
      counter[i] =
	(moving_factor >> ((sizeof (moving_factor) - i - 1) * 8)) & 0xFF;

    if (flags & OATH_TOTP_HMAC_SHA256)
      {
	hssize = GC_SHA256_DIGEST_SIZE;
	//rc = gc_hmac_sha256 (secret, secret_length,
	//		     counter, sizeof (moving_factor), hs);
      }
    else if (flags & OATH_TOTP_HMAC_SHA512)
      {
	hssize = GC_SHA512_DIGEST_SIZE;
	//rc = gc_hmac_sha512 (secret, secret_length,
	//		     counter, sizeof (moving_factor), hs);
      }
    else
      rc = hmac_sha1 (secret, secret_length,
			 counter, sizeof (moving_factor), hs);
    if (rc != GC_OK)
      return OATH_CRYPTO_ERROR;
  }

  {
    uint8_t offset = hs[hssize - 1] & 0x0f;

    S = (((uint32_t)(hs[offset] & 0x7f) << 24)
	 | ((uint32_t)(hs[offset + 1] & 0xff) << 16)
	 | ((uint32_t)(hs[offset + 2] & 0xff) << 8) | ((hs[offset + 3] & 0xff)));
  }

  switch (digits)
    {
    case 6:
      S = S % 1000000;
      break;

    case 7:
      S = S % 10000000;
      break;

    case 8:
      S = S % 100000000;
      break;

    default:
      return OATH_INVALID_DIGITS;
      break;
    }

  *output_otp = S;
  /*{
    int len = snprintf (output_otp, digits + 1, "%.*ld", digits, S);
    output_otp[digits] = '\0';
    if (len <= 0 || ((unsigned) len) != digits)
      return OATH_PRINTF_ERROR;
  }*/

  return OATH_OK;
}
