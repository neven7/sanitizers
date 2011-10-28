//===-- asan_linux.cc -----------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file is a part of AddressSanitizer, an address sanity checker.
//
// Linux-specific details.
//===----------------------------------------------------------------------===//

#include "asan_internal.h"

#include <elf.h>
#include <link.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

extern ElfW(Dyn) _DYNAMIC[];

namespace __asan {

void *__asan_does_not_support_static_linkage() {
  // This will fail to link with -static.
  return &_DYNAMIC;
}

void *__asan_mmap(void *addr, size_t length, int prot, int flags,
                  int fd, uint64_t offset) {
# if __WORDSIZE == 64
  return (void *)syscall(SYS_mmap, addr, length, prot, flags, fd, offset);
# else
  return (void *)syscall(SYS_mmap2, addr, length, prot, flags, fd, offset);
# endif
}

}  // namespace __asan