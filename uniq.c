#include "types.h"
#include "stat.h"
#include "user.h"

void uniq(int fd) {
  int i, n;
  char buf[512];
  
  for (;;) {
    n = read(0, buf, sizeof buf);
    if (n == 0)
      break;
    if (n < 0) {
      fprintf(2, "read error\n");
      exit();
    }
    if (write(1, buf, n) != n) {
      fprintf(2, "write error\n");
      exit();
    }
  }
  
}

int main(int argc, char *argv[]) {
  int fd, i;

  // read from standard input
  if (argc <= 1) {
    uniq(0);
    exit();
  }
  // read from file passed as arguement
  for (i = 1; i < argc; i++) {
    if ((fd = open(argv[i], 0)) < 0) {
      printf(1, "wc: cannot open %s\n", argv[i]);
      exit();
    }
    uniq(fd);
    close(fd);
  }
  exit();
}