# dtoa benchmark

This project is a complete rewrite of Milo Yip’s
[dtoa-benchmark](https://github.com/miloyip/dtoa-benchmark), featuring an updated set of algorithms
that reflect the current state of the art and a simplified workflow.

Formatting library from [uxs](https://github.com/gbuzykin/uxs) is also added.

## Introduction

This benchmark evaluates the performance of converting double-precision IEEE-754 floating-point
values (`double`) to ASCII strings.  The function signature is:

```cpp
void dtoa(double value, char* buffer);
```

The resulting string **must** be round-trip convertible: it should parse back to the original value
**exactly** via a correct implementation of `strtod`.

Note: `dtoa` is *not* a standard C or C++ function.

## Procedure

The benchmark consists of two phases:

1. **Correctness verification**  
   All implementations are first validated to ensure round-trip correctness.

2. **Performance measurement**

   The benchmark case is:

   * **RandomDigit**  
     * Generate 100,000 random `double` values (excluding `±inf` and `NaN`).
     * Reduce precision to 1–17 decimal digits in the significand.
     * Convert each value to an ASCII string.

   Each digit group is executed 10 times.  
   For each configuration, 10 trials are run and the **minimum** elapsed time is recorded.

## Build and Run

```bash
cmake .
make run-benchmark
```

Results are written in CSV format to:

```
results/<cpu>_<os>_<compiler>_<commit>.csv
```

They are also automatically converted to HTML with the same base name.

## Results

The following results were measured on Intel(R) Core(TM) i7-14700 linux clang 22.1.8 + libc++:

```
Verifying double-conversion    ... OK. Length Avg = 22.420, Max = 25
Verifying dragonbox            ... 
warning: expected 0.1 but got 1E-1
warning: expected 0.12 but got 1.2E-1
warning: expected 0.123 but got 1.23E-1
warning: expected 0.1234 but got 1.234E-1
warning: expected 1.2345 but got 1.2345E0
OK. Length Avg = 22.027, Max = 24
Verifying fmt                  ... OK. Length Avg = 22.440, Max = 24
Verifying ostringstream        ... 
warning: expected 0.1 but got 0.10000000000000001
warning: expected 1.2345 but got 1.2344999999999999
OK. Length Avg = 22.935, Max = 24
Verifying ryu                  ... 
warning: expected 0.1 but got 1E-1
warning: expected 0.12 but got 1.2E-1
warning: expected 0.123 but got 1.23E-1
warning: expected 0.1234 but got 1.234E-1
warning: expected 1.2345 but got 1.2345E0
OK. Length Avg = 22.027, Max = 24
Verifying schubfach            ... 
warning: expected 0.1 but got 1.e-01
warning: expected 0.12 but got 1.2e-01
warning: expected 0.123 but got 1.23e-01
warning: expected 0.1234 but got 1.234e-01
warning: expected 1.2345 but got 1.2345e+00
OK. Length Avg = 22.561, Max = 24
Verifying sprintf              ... 
warning: expected 0.1 but got 0.10000000000000001
warning: expected 1.2345 but got 1.2344999999999999
OK. Length Avg = 22.935, Max = 24
Verifying to_chars             ... OK. Length Avg = 22.421, Max = 24
Verifying uxs                  ... OK. Length Avg = 22.437, Max = 24
Verifying xjb64                ... OK. Length Avg = 22.444, Max = 24
Verifying yy                   ... OK. Length Avg = 21.974, Max = 25
Verifying zmij                 ... 
warning: expected 0.1 but got 1e-01
warning: expected 0.12 but got 1.2e-01
warning: expected 0.123 but got 1.23e-01
warning: expected 0.1234 but got 1.234e-01
warning: expected 1.2345 but got 1.2345e+00
OK. Length Avg = 22.561, Max = 24
Benchmarking randomdigit double-conversion    ... [  36.363ns,   63.982ns]
Benchmarking randomdigit dragonbox            ... [  14.956ns,   18.910ns]
Benchmarking randomdigit fmt                  ... [  14.053ns,   21.485ns]
Benchmarking randomdigit null                 ... [   0.928ns,    0.934ns]
Benchmarking randomdigit ostringstream        ... [ 367.011ns,  405.353ns]
Benchmarking randomdigit ryu                  ... [  23.340ns,   35.454ns]
Benchmarking randomdigit schubfach            ... [  17.560ns,   27.500ns]
Benchmarking randomdigit sprintf              ... [ 254.717ns,  290.835ns]
Benchmarking randomdigit to_chars             ... [  25.609ns,   38.602ns]
Benchmarking randomdigit uxs                  ... [  29.597ns,   33.770ns]
Benchmarking randomdigit xjb64                ... [   9.698ns,    9.937ns]
Benchmarking randomdigit yy                   ... [  10.331ns,   13.046ns]
Benchmarking randomdigit zmij                 ... [  10.619ns,   10.846ns]
```

### Notes

* `null` performs no conversion and measures loop + call overhead.
* `sprintf` and `ostringstream` do **not** generate shortest representations (e.g.  `0.1` →
  `0.10000000000000001`).
* `ryu`, `dragonbox`, and `schubfach` always emit exponential notation (e.g.  `0.1` → `1E-1`).

## Methods

| Function | Description |
|----------|-------------|
| [asteria](https://github.com/lhmouse/asteria) | `rocket::ascii_numput::put_DD` |
| [double-conversion](https://github.com/google/double-conversion) | `EcmaScriptConverter::ToShortest` which implements Grisu3 with bignum fallback |
| [dragonbox](https://github.com/jk-jeon/dragonbox) | `jkj::dragonbox::to_chars` with full tables |
| [fmt](https://github.com/fmtlib/fmt) e27cc20bd9 Sun Aug 23 19:31:28 2026 +0300 | `fmt::format_to` with compile-time format strings (uses Dragonbox). |
| null | no-op implementation |
| [ostringstream](https://en.cppreference.com/w/cpp/io/basic_ostringstream.html) | `std::ostringstream` with `setprecision(17)` |
| [ryu](https://github.com/ulfjack/ryu) | `d2s_buffered` |
| [schubfach](https://github.com/vitaut/schubfach) | C++ Schubfach implementation |
| [sprintf](https://en.cppreference.com/w/c/io/fprintf.html) | C `sprintf("%.17g", value)` |
| [to_chars](https://en.cppreference.com/w/cpp/utility/to_chars.html) from libc++ | `std::to_chars` |
| [uxs](https://github.com/gbuzykin/uxs) 67536682cc Wed Aug 26 12:17:45 2026 +0300 | `uxs::to_chars` |
| [zmij](https://github.com/vitaut/zmij) | `zmij::write`. |

### Notes

`std::to_string` is excluded because it does **not** guarantee round-trip correctness (until C++26).

## Why is fast `dtoa` important?

Floating-point formatting is ubiquitous in text output.  Standard facilities such as `sprintf` and
`std::stringstream` are often slow.  This benchmark originated from performance work in
[RapidJSON](https://github.com/miloyip/rapidjson/).

## See Also

* [Faster double-to-string conversion](https://vitaut.net/posts/2025/faster-dtoa/)
* [The smallest state-of-the-art double-to-string implementation](
  https://vitaut.net/posts/2025/smallest-dtoa/)
