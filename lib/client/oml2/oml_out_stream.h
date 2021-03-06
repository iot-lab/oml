/*
 * Copyright 2011-2015 National ICT Australia (NICTA)
 *
 * This software may be used and distributed solely under the terms of
 * the MIT license (License).  You should find a copy of the License in
 * COPYING or at http://opensource.org/licenses/MIT. By downloading or
 * using this software you accept the terms and the liability disclaimer
 * in the License.
 */
/** \file oml_out_stream.h
 * \brief Abstract interface for output streams.
 *
 * Various writers, and particularly the BufferedWriter, use this interface to
 * output data into a stream (e.g., file or socket).
 */

#ifndef OML_OUT_STREAM_H_
#define OML_OUT_STREAM_H_

#include <unistd.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct OmlOutStream;

/** Write a chunk into the lower level out stream
 *
 * \param outs OmlOutStream to write into
 * \param buffer pointer to the beginning of the data to write
 * \param length length of data to read from buffer
 * \param header pointer to the beginning of header data to write in case of disconnection
 * \param header_length length of header data to write in case of disconnection
 *
 * XXX: Why are header and normal data separate?
 *
 * \return the number of sent bytes on success, -1 otherwise
 */
typedef ssize_t (*oml_outs_write_f)(struct OmlOutStream* outs, uint8_t* buffer, size_t length, uint8_t* header, size_t header_length);

/** Close an OmlOutStream
 *
 * \param writer OmlOutStream to close
 * \return 0 on success, -1 otherwise
 */
typedef int (*oml_outs_close_f)(struct OmlOutStream* writer);

/** Immediately write a chunk into the lower level out stream
 *
 * \param outs OmlOutStream to write into
 * \param buffer pointer to the beginning of the data to write
 * \param length length of data to read from buffer
 *
 * \return the number of sent bytes on success, -1 otherwise
 */
typedef ssize_t (*oml_outs_write_f_immediate)(struct OmlOutStream* outs, uint8_t* buffer, size_t length);

/** Create an OmlOutStream for the specified URI
 *
 * \param uri 	collection URI
 * \return a pointer to the newly allocated OmlOutStream, or NULL on error
 *
 * \see create_writer
 */
struct OmlOutStream* create_out_stream(const char *uri);

/** Write header information if not already done, and record this fact
 *
 * This function call writefp to write the header data if self->header_written is 0.
 *
 * \param outs OmlOutStream to write into
 * \param writefp pointer to an oml_outs_write_f_immediate function to write the data in the stream
 * \param header pointer to the beginning of the header data to write
 * \param header_length length of header data
 *
 * \return the number of sent bytes on success (0 if no header was written), -1 otherwise
 * \see oml_outs_write_f_immediate, OmlOutStream::header_written
 */
ssize_t out_stream_write_header(struct OmlOutStream* outs, oml_outs_write_f_immediate writefp, uint8_t* header, size_t header_length);

/** A low-level output stream */
typedef struct OmlOutStream {
  /** Pointer to a function in charge of writing into the stream \see oml_outs_write_f */
  oml_outs_write_f write;
  /** Pointer to a function in charge of closing the stream \see oml_outs_close_f */
  oml_outs_close_f close;
  /** Description of this output stream, usually overriden by a URI or filename */
  char* dest;
  /** True if header has been written to the stream */
  int   header_written;
} OmlOutStream;

extern OmlOutStream *file_stream_new(const char *file);

int file_stream_set_buffered(OmlOutStream* hdl, int buffered);
int file_stream_get_buffered(OmlOutStream* hdl);

/* from net_stream.c */

extern OmlOutStream *net_stream_new(const char *transport, const char *hostname, const char *port);

#ifdef __cplusplus
}
#endif

#endif /* OML_OUT_STREAM_H */

/*
 Local Variables:
 mode: C
 tab-width: 2
 indent-tabs-mode: nil
 End:
 vim: sw=2:sts=2:expandtab
*/
