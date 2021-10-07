#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

int strcmp(const char *X, const char *Y) {
  while (*X) {
    if (*X != *Y) {
      break;
    }
    X++;
    Y++;
  }
  return *(const unsigned char *)X - *(const unsigned char *)Y;
}

void uniq(int fd) {
  int i, n;
//   int *start;
//   char *prev;
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
        printf(1, "This is the %dth string: %s",i, buf[i]);
    //   if (buf[i] == '\n') {

    //   } else {
    //   }
    }
  }
  if (n < 0) {
    printf(1, "wc: read error\n");
    exit();
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