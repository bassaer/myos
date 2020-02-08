#include <memory.h>

#include <console.h>
#include <util.h>

void stats(struct MEM_INFO *mm) {
  mm->total = 0;
  mm->used = 0;
  mm->free = 0;
}
