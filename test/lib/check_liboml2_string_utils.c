/*
 * Copyright 2013 National ICT Australia Limited (NICTA).
 *
 * This software may be used and distributed solely under the terms of
 * the MIT license (License).  You should find a copy of the License
 * in LICENSE.TXT or at http://opensource.org/licenses/MIT. By
 * downloading or using this software you accept the terms and the
 * liability disclaimer in the License.
 */

#include <check.h>
#include <string.h>
#include <string_utils.h>

START_TEST(test_round_trip)
{
  char in[BUFSIZ+1], out[2*BUFSIZ+1];
  /* encode */

  strcpy(in, "");
  str_backslash_encode(in, out);
  fail_unless(strlen(out) == 0);
  fail_unless('\0' == out[0]);

  strcpy(in, "a");
  str_backslash_encode(in, out);
  fail_unless(strlen(out) == 1);
  fail_unless(strcmp(in, out) == 0);

  strcpy(in, "\t");
  str_backslash_encode(in, out);
  fail_unless(strlen(out) == 2);
  fail_unless(strcmp("\\t", out) == 0);

  strcpy(in, "\\");
  str_backslash_encode(in, out);
  fail_unless(strlen(out) == 2);
  fail_unless(strcmp("\\\\", out) == 0);

  strcpy(in, "\\x");
  str_backslash_encode(in, out);
  fail_unless(strlen(out) == 3);
  fail_unless(strcmp("\\\\x", out) == 0);

  strcpy(in, "\\\\");
  str_backslash_encode(in, out);
  fail_unless(strlen(out) == 4);
  fail_unless(strcmp("\\\\\\\\", out) == 0);

  strcpy(in, "foo\\bar");
  str_backslash_encode(in, out);
  fail_unless(strlen(out) == 8);
  fail_unless(strcmp("foo\\\\bar", out) == 0);

  strcpy(in, "\t\r\n");
  str_backslash_encode(in, out);
  fail_unless(strlen(out) == 6);
  fail_unless(strcmp("\\t\\r\\n", out) == 0);

  strcpy(in, "foo\tbar\rbaz\n");
  str_backslash_encode(in, out);
  fail_unless(strlen(out) == 15);
  fail_unless(strcmp("foo\\tbar\\rbaz\\n", out) == 0);

  strcpy(in, "\\a\\b\\c\\d\\e");
  str_backslash_encode(in, out);
  fail_unless(strlen(out) == 15);
  fail_unless(strcmp("\\\\a\\\\b\\\\c\\\\d\\\\e", out) == 0);

  /* decode */

  strcpy(in, "");
  str_backslash_decode(in, out);
  fail_unless(strlen(out) == 0);
  fail_unless('\0' == out[0]);

  strcpy(in, "a");
  str_backslash_decode(in, out);
  fail_unless(strlen(out) == 1);
  fail_unless(strcmp(in, out) == 0);

  strcpy(in, "\\t");
  str_backslash_decode(in, out);
  fail_unless(strlen(out) == 1);
  fail_unless(strcmp("\t", out) == 0);

  strcpy(in, "\\t\\r\\n");
  str_backslash_decode(in, out);
  fail_unless(strlen(out) == 3);
  fail_unless(strcmp("\t\r\n", out) == 0);

  strcpy(in, "foo\\tbar\\rbaz\\n");
  str_backslash_decode(in, out);
  fail_unless(strlen(out) == 12);
  fail_unless(strcmp("foo\tbar\rbaz\n", out) == 0);

  strcpy(in, "\\\\");
  str_backslash_decode(in, out);
  fail_unless(strlen(out) == 1);
  fail_unless(strcmp("\\", out) == 0);

  strcpy(in, "\\a\\b\\c\\d");
  str_backslash_decode(in, out);
  fail_unless(strlen(out) == 4);
  fail_unless(strcmp("abcd", out) == 0);
}
EXIT_TEST

Suite*
string_utils_suite(void)
{
  Suite *s = suite_create("string_utils");
  TCase *tc_core = tcase_create("string_utils");
  tcase_add_test(tc_core, test_round_trip);
  suite_add_tcase(s, tc_core);
  return s;
}

/*
 Local Variables:
 mode: C
 tab-width: 2
 indent-tabs-mode: nil
 End:
 vim: sw=2:sts=2:expandtab
*/
