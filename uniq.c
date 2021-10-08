#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void uniq(int fd) {
  int n;

  while ((n = read(fd, buf, sizeof(buf))) > 0)
    write(1, buf, n);
  if (n < 0) {
    printf(1, "cat: read error\n");
    exit();
  }
}

int main(int argc, char *argv[]) {
//   char delimiter = '\n';
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