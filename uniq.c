#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void uniq(int fd) {
  int i, n;
  //   int *start;
  //   char *prev;
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    int start = 0;
    int end = 0;
    for (i = 0; i < n; i++) {
      if (buf[i] == '\n') {
        end = i;
        printf(1, "This is the %dth string: ", i);
        for (;start <= end; start++){
            printf(1, "%c", buf[i]);
        }
      }
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