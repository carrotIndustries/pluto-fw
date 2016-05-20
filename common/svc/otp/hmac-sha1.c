/* hmac-sha1.c -- hashed message authentication codes
   Copyright (C) 2005-2006, 2009-2015 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.  */

/* Written by Simon Josefsson.  */

#include "hmac.h"

#include "memxor.h"
#include "mbedtls/sha1.h"

#include <string.h>

#define IPAD 0x36
#define OPAD 0x5c

int
hmac_sha1 (const void *key, size_t keylen,
           const void *in, size_t inlen, void *resbuf)
{
  mbedtls_sha1_context inner;
  mbedtls_sha1_context outer;
  char optkeybuf[20];
  char block[64];
  char innerhash[20];

  /* Reduce the key's size, so that it becomes <= 64 bytes large.  */

  if (keylen > 64)
    {
      mbedtls_sha1_context keyhash;

      mbedtls_sha1_init (&keyhash);
      mbedtls_sha1_starts (&keyhash);
      mbedtls_sha1_update (&keyhash, key, keylen);
      mbedtls_sha1_finish (&keyhash, optkeybuf);

      key = optkeybuf;
      keylen = 20;
    }

  /* Compute INNERHASH from KEY and IN.  */

  mbedtls_sha1_init (&inner);
  mbedtls_sha1_starts (&inner);

  memset (block, IPAD, sizeof (block));
  memxor (block, key, keylen);

  mbedtls_sha1_update (&inner, block, 64);
  mbedtls_sha1_update (&inner, in, inlen);

  mbedtls_sha1_finish (&inner, innerhash);

  /* Compute result from KEY and INNERHASH.  */

  mbedtls_sha1_init (&outer);
  mbedtls_sha1_starts (&outer);

  memset (block, OPAD, sizeof (block));
  memxor (block, key, keylen);

  mbedtls_sha1_update (&outer, block, 64);
  mbedtls_sha1_update (&outer, innerhash, 20);

  mbedtls_sha1_finish (&outer, resbuf);

  return 0;
}
