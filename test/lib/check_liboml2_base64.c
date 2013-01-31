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
#include <stdint.h>
#include <string.h>

START_TEST(test_base64_string_size)
{
  fail_unless(base64_size_string(0) == 1);
  fail_unless(base64_size_string(1) == 5);
  fail_unless(base64_size_string(2) == 5);
  fail_unless(base64_size_string(3) == 5);
  fail_unless(base64_size_string(4) == 9);
  fail_unless(base64_size_string(5) == 9);
  fail_unless(base64_size_string(6) == 9);
}
END_TEST

START_TEST(test_round_trip)
{
  struct testcase {
    const char *plain;
    const char *encoded;
  };

  static struct testcase TESTS[] = {
    { "", "" },
    { "a", "YQ==" },
    { "ab", "YWI=" },
    { "abc", "YWJj" },
    { "abcd", "YWJjZA==" },
    { "abcde", "YWJjZGU=" },
    { "any carnal pleasure.", "YW55IGNhcm5hbCBwbGVhc3VyZS4=" },
    { "any carnal pleasure", "YW55IGNhcm5hbCBwbGVhc3VyZQ==" },
    { "any carnal pleasur", "YW55IGNhcm5hbCBwbGVhc3Vy" },
    { "any carnal pleasu", "YW55IGNhcm5hbCBwbGVhc3U=" },
    { "any carnal pleas", "YW55IGNhcm5hbCBwbGVhcw==" },
  };

  const NOF_TESTS = sizeof(TESTS) / sizeof(TESTS[0]);

  const size_t MAX_PLAIN_SZ = 21;
  uint8_t plain[MAX_PLAIN_SZ];
  size_t plain_sz;
  const size_t MAX_ACTUAL_SZ = MAX_PLAIN_SZ;
  uint8_t actual[MAX_ACTUAL_SZ];
  size_t actual_sz;
  char *encoded = alloca(base64_size_string(MAX_PLAIN_SZ));
  ssize_t encoded_sz;
  size_t i;

  for(i = 0; i < NOF_TESTS; i++) {
    /* encoding */
    strcpy(plain, TESTS[i].plain);
    plain_sz = strlen(plain);
    encoded_sz = base64_size_string(plain_sz); 
    fail_unless(strlen(TESTS[i].encoded) + 1 == encoded_sz);
    base64_encode_blob(plain_sz, plain, encoded);
    fail_unless(strcmp(encoded, TESTS[i].encoded) == 0, "actual=%s, expected=%s\n", encoded, TESTS[i].encoded);
    /* decoding */
    encoded_sz = base64_validate_string(encoded);
    fail_if(encoded_sz < 0);
    fail_unless('=' == encoded[encoded_sz] || '\0' == encoded[encoded_sz]);
    actual_sz = base64_size_blob(encoded_sz);
    fail_unless(actual_sz == strlen(TESTS[i].plain));
    fail_if(base64_decode_string(encoded_sz, encoded, actual_sz, actual) < 0, "actual=%s, expected=%s\n", encoded, TESTS[i].plain);
    fail_unless(strcmp(actual, TESTS[i].plain) == 0);
  }
}
END_TEST

Suite*
base64_suite(void)
{
  Suite *s = suite_create("base64");
  TCase *tc_core = tcase_create("base64");
  tcase_add_test(tc_core, test_base64_string_size);
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
