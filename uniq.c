#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];
char delimiter = '\n';

void uniq(int fd, char delimiter) {
  int n;

  while ((n = read(fd, buf, sizeof(buf))) > 0)
    write(1, buf, n);
  if (n < 0) {
    printf(1, "cat: read error\n");
    exit();
  }
}

int main(int argc, char *argv[]) {
  int fd, i;

  if (argc <= 1) {
    uniq(0, delimiter);
    exit();
  }

  for (i = 1; i < argc; i++) {
    if ((fd = open(argv[i], 0)) < 0) {
      printf(1, "cat: cannot open %s\n", argv[i]);
      exit();
    }
    uniq(fd, delimiter);
    close(fd);
  }
  exit();
}