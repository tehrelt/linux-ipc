#include "shared.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  int fd = open(FIFO_NAME, O_RDONLY);
  while (true) {
  }
}
