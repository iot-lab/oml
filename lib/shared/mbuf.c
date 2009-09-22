/*
 * Copyright 2007-2009 National ICT Australia (NICTA), Australia
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mbuf.h"

#define DEF_BUF_SIZE 512

/*
 *  mbuf->base : base address of the underlying storage
 *
 *  mbuf->length : number of bytes allocated for underlying storage
 *
 *  mbuf->fill : number of bytes filled with data
 *
 *  mbuf->wr_remaining : remaining space left to write into (between fill and end of allocated space)
 *
 *  mbuf->rd_remaining : number of bytes of valid data left to read (difference between mbuf->wrptr and mbuf->rdptr)
 *
 *  mbuf->rdptr : When reading the buffer, pointer to the next byte to be read
 *
 *  mbuf->wrptr : When writing the buffer, pointer to the next byte to be written
 *
 *  mbuf->message_start : When reading the buffer, pointer to the base of the start of the current message
 *
 */

static void
mbuf_check_invariant (OmlMBufferEx* mbuf)
{
  assert (mbuf != NULL);
  assert (mbuf->base != NULL);
  assert (mbuf->wr_remaining >= 0 && mbuf->wr_remaining <= mbuf->length);
  assert (mbuf->fill >= 0 && mbuf->fill <= mbuf->length);
  assert (mbuf->fill + mbuf->wr_remaining == mbuf->length);
  assert (mbuf->wrptr >= mbuf->rdptr);
  assert (mbuf->wrptr - mbuf->base == mbuf->fill);
  assert (mbuf->rd_remaining == (mbuf->wrptr - mbuf->rdptr));
}

OmlMBufferEx*
mbuf_create (void)
{
  OmlMBufferEx* mbuf = (OmlMBufferEx*) malloc (sizeof (OmlMBufferEx));
  if (mbuf == NULL) return NULL;
  memset (mbuf, 0, sizeof (OmlMBufferEx));

  mbuf->length = DEF_BUF_SIZE;
  mbuf->base = (uint8_t*) malloc (mbuf->length * sizeof (uint8_t));

  if (mbuf->base == NULL)
	{
	  free (mbuf);
	  return NULL;
	}
  memset (mbuf->base, 0, mbuf->length);

  mbuf->rdptr = mbuf->base;
  mbuf->wrptr = mbuf->base;
  mbuf->fill = 0;
  mbuf->wr_remaining = mbuf->length;
  mbuf->rd_remaining = mbuf->wrptr - mbuf->rdptr;

  mbuf_check_invariant(mbuf);

  return mbuf;
}

void
mbuf_destroy (OmlMBufferEx* mbuf)
{
  free (mbuf->base);
  free (mbuf);
}

uint8_t*
mbuf_buffer (OmlMBufferEx* mbuf)
{
  return mbuf->base;
}

size_t
mbuf_length (OmlMBufferEx* mbuf)
{
  return mbuf->length;
}

size_t
mbuf_remaining (OmlMBufferEx* mbuf)
{
  // ???
  return mbuf->fill;
}

size_t
mbuf_fill (OmlMBufferEx* mbuf)
{
  return mbuf->fill;
}

size_t
mbuf_read_offset (OmlMBufferEx* mbuf)
{
  return mbuf->rdptr - mbuf->base;
}

size_t
mbuf_write_offset (OmlMBufferEx* mbuf)
{
  return mbuf->wrptr - mbuf->base;
}

size_t
mbuf_message_offset (OmlMBufferEx* mbuf)
{
  return mbuf->message_start - mbuf->base;
}

int
mbuf_resize (OmlMBufferEx* mbuf, size_t new_length)
{
  mbuf_check_invariant (mbuf);

  if (mbuf == NULL || mbuf->base == NULL)
	return -1;

  /* Don't resize if we don't have to */
  if (new_length <= mbuf->length)
	return 0;

  uint8_t* new = (uint8_t*) malloc (new_length * sizeof (uint8_t));
  if (new == NULL)
	return -1;

  int wr_offset = mbuf->wrptr - mbuf->base;
  int rd_offset = mbuf->rdptr - mbuf->base;
  int msg_offset = mbuf->message_start - mbuf->base;

  assert (wr_offset == mbuf->fill);

  memcpy (new, mbuf->base, mbuf->fill);
  free (mbuf->base);

  mbuf->wrptr = mbuf->base + wr_offset;
  mbuf->rdptr = mbuf->base + rd_offset;
  mbuf->message_start = mbuf->base + msg_offset;

  mbuf->length = new_length;
  mbuf->wr_remaining = mbuf->length - mbuf->fill;

  mbuf_check_invariant (mbuf);

  return 0;
}

int
mbuf_check_resize (OmlMBufferEx* mbuf, size_t bytes)
{
  if (mbuf->wr_remaining < bytes)
	return mbuf_resize (mbuf, 2 * mbuf->length);
  else
	return 0;
}

/**
 *  Write bytes into the mbuf.
 *
 *  Write len bytes from the raw buffer pointed to by buf into the
 *  mbuf.  If the mbuf doen't have enough remaining space to fit len
 *  bytes, no bytes are written to mbuf and the function will fail.
 *
 *  If the function succeeds, the bytes are written starting at the
 *  mbuf's write pointer, and the write pointer is advanced by len
 *  bytes.
 *
 *  @return 0 on success, -1 on failure.
 *
 */
int
mbuf_write (OmlMBufferEx* mbuf, const uint8_t* buf, size_t len)
{
  mbuf_check_invariant (mbuf);

  if (mbuf == NULL || buf == NULL) return -1;

  if (mbuf->wr_remaining < len) return -1;

  memcpy (mbuf->wrptr, buf, len);

  mbuf->wrptr += len;
  mbuf->fill += len;
  mbuf->wr_remaining -= len;

  mbuf_check_invariant (mbuf);

  return 0;
}

/**
 *  Read bytes from mbuf.
 *
 *  Read len bytes from the mbuf into the raw buffer pointed to by
 *  buf.  If the mbuf doesn't have at least len bytes available to
 *  read, then the function fails.
 *
 *  If the function succeeds, the bytes are written to buf, with
 *  reading starting at the mbuf's read pointer, and the read pointer
 *  is then advanced by len bytes.
 *
 *  @return 0 on success, -1 on failure.
 *
 */
int
mbuf_read (OmlMBufferEx* mbuf, uint8_t* buf, size_t len)
{
  mbuf_check_invariant (mbuf);

  if (mbuf == NULL || buf == NULL) return -1;

  if (mbuf->rd_remaining < len) return -1;

  memcpy (buf, mbuf->rdptr, len);

  mbuf->rdptr += len;
  mbuf->rd_remaining -= len;

  mbuf_check_invariant (mbuf);

  return 0;
}

int
mbuf_begin_read (OmlMBufferEx* mbuf)
{
  mbuf_check_invariant (mbuf);

  if (mbuf == NULL) return -1;

  mbuf->message_start = mbuf->rdptr;

  mbuf_check_invariant (mbuf);

  return 0;
}

int
mbuf_begin_write (OmlMBufferEx* mbuf)
{
  mbuf_check_invariant (mbuf);

  if (mbuf == NULL) return -1;

  mbuf->message_start = mbuf->wrptr;

  mbuf_check_invariant (mbuf);

  return 0;
}

int
mbuf_clear (OmlMBufferEx* mbuf)
{
  mbuf_check_invariant (mbuf);

  memset (mbuf->base, 0, mbuf->length);

  mbuf->rdptr = mbuf->wrptr = mbuf->message_start = mbuf->base;
  mbuf->fill = 0;
  mbuf->wr_remaining = mbuf->length;
  mbuf->rd_remaining = 0;

  mbuf_check_invariant (mbuf);
}

int
mbuf_reset_write (OmlMBufferEx* mbuf)
{
  mbuf_check_invariant (mbuf);

  // If in the middle of a read, can't do a write reset
  if (mbuf->rdptr > mbuf->message_start)
	return -1;

  size_t msglen = mbuf->wrptr - mbuf->message_start;
  mbuf->fill -= msglen;
  mbuf->wr_remaining += msglen;
  mbuf->rd_remaining -= msglen;
  mbuf->wrptr = mbuf->message_start;

  mbuf_check_invariant (mbuf);
  return 0;
}

int
mbuf_reset_read (OmlMBufferEx* mbuf)
{
  mbuf_check_invariant (mbuf);

  // If in the middle of a write, can't do a read reset
  if (mbuf->message_start > mbuf->rdptr)
	return -1;

  mbuf->rdptr = mbuf->message_start;
  mbuf->rd_remaining = mbuf->wrptr - mbuf->rdptr;

  mbuf_check_invariant (mbuf);
  return 0;
}

int
mbuf_consume_message (OmlMBufferEx* mbuf)
{
  mbuf_check_invariant (mbuf);

  // Can't consume message if we're in the middle of writing it
  if (mbuf->message_start > mbuf->rdptr)
	return -1;

  mbuf->message_start = mbuf->rdptr;

  mbuf_check_invariant (mbuf);
  return 0;
}

int
mbuf_repack (OmlMBufferEx* mbuf)
{
  mbuf_check_invariant (mbuf);

  if (mbuf == NULL) return -1;

  memmove (mbuf->base, mbuf->rdptr, mbuf->rd_remaining);

  mbuf->fill = mbuf->rd_remaining;
  mbuf->wr_remaining = mbuf->length - mbuf->fill;
  mbuf->wrptr = mbuf->base + mbuf->fill;
  mbuf->message_start = mbuf->rdptr = mbuf->base;

  mbuf_check_invariant (mbuf);
  return 0;
}

// Preserve the rdptr relative to the message_start
int
mbuf_repack_message (OmlMBufferEx* mbuf)
{
  mbuf_check_invariant (mbuf);

  if (mbuf == NULL) return -1;

  size_t msg_remaining = mbuf->wrptr - mbuf->message_start;

  memmove (mbuf->base, mbuf->message_start, msg_remaining);

  mbuf->fill = mbuf->wrptr - mbuf->message_start;
  mbuf->wr_remaining = mbuf->length - mbuf->fill;
  mbuf->wrptr = mbuf->base + mbuf->fill;
  mbuf->message_start = mbuf->base;
  mbuf->rdptr = mbuf->wrptr - mbuf->rd_remaining;

  mbuf_check_invariant (mbuf);
  return 0;
}

char*
to_octets (unsigned char* buf, int len)
{
  const int octet_width = 5;
  char* out = (char*) malloc ((octet_width * len + 1) * sizeof (char));
  int i = 0;
  int count = 0;
  for (i = 0; i < len; i++)
	{
	  int n = snprintf (&out[count], octet_width + 1, "0x%02x ", (unsigned int)buf[i]);
	  count += n;
	}

  return out;
}


/*
 Local Variables:
 mode: C
 tab-width: 4
 indent-tabs-mode: nil
*/
