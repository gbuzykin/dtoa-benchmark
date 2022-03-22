#include "test.h"
#include "util/format.h"

void dtoa_util(double value, char* buffer) {
  //buffer = util::format_to(buffer, "{}",  value);
  buffer = util::to_string_to(buffer, value);
  *buffer = '\0';
}

REGISTER_TEST(util);
