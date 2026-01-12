#include "benchmark.h"
#include "uxs/format.h"

static register_method _("uxs", [](double value, char* buffer) {
  buffer = uxs::to_chars(buffer, value);
  *buffer = '\0';
});
