#include "benchmark.h"
#include "uxs/format.h"

static register_method _("uxs", [](double value, char* buffer) {
  *uxs::to_chars(buffer, value) = '\0';
});
