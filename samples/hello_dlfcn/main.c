#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void
hexdump(uint8_t* buf, size_t size) {
  for(int i=0; i<size; i++) {
    if(i%16 == 0 && i!= 0) {
      printf("\n");
    }
    printf("%02x ", buf[i]);
  }
  printf("\n");
}


int main() {
  int (*sceRandomGetRandomNumber)(uint8_t* buf, size_t size) = 0;
  void* libSceRandom = 0;
  uint8_t buf[0x40] = {0};
  Dl_info info;

  if(!(libSceRandom=dlopen("/system/common/lib/libSceRandom.sprx", 0))) {
    puts(dlerror());
    return -1;
  }

  if(!(sceRandomGetRandomNumber=dlsym(libSceRandom, "sceRandomGetRandomNumber"))) {
    puts(dlerror());
    return -1;
  }

  if(sceRandomGetRandomNumber(buf, sizeof(buf)) < 0) {
    perror("sceRandomGetRandomNumber");
    return -1;
  }

  if(dladdr(sceRandomGetRandomNumber, &info) == 0) {
    puts(dlerror());
    return -1;
  }

  printf("%s: 0x%p\n", info.dli_fname, info.dli_fbase);

  hexdump(buf, sizeof(buf));

  return 0;
}

