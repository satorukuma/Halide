// LLVM sometimes likes to generate calls to a stack smashing
// protector, but some build environments (e.g. native client), don't
// provide libssp reliably. We define two weak symbols here to help
// things along.

#include "HalideRuntime.h"

#define WEAK __attribute__((weak))

extern "C" {

WEAK char *__stack_chk_guard = (char *)(0xdeadbeef);

WEAK void __stack_chk_fail() {
    halide_error(NULL, "Memory error: stack smashing protector changed!\n");
}

}