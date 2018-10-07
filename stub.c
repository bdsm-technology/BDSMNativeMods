#include <assert.h>

__attribute__((__visibility__("default"))) void *MSHookFunction(void *symbol, void *hook, void **original) {
  assert(0);
  return 0;
}