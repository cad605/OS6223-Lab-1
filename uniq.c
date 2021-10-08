#include "types.h"
#include "stat.h"
#include "user.h"
#define MAXLINE 1000

char buf[512];
char delimiter = '\n';
static int count_group, dups_only, ignore_case = 0;

/* When fed an input, outputs the input with adjacent identical lines collapsed
 * to one*/
void uniq(int fd) {
  int i, n, lines = 0;

  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
      if (buf[i] == '\n') {
        lines++;
      }
    }
    write(1, buf, n);
  }

  if (n < 0) {
    printf(1, "cat: read error\n");
    exit();
  }
}

int main(int argc, char *argv[]) {
  char line[MAXLINE];
  long lineno = 0;
  int fd, i, c, except = 0, number = 0, found = 0;

  while (--argc > 0 && (*++argv)[0] == '-') {
    while (c = *++argv[0]) {
      switch (c) {
      // count, group, and prefix lines by the number of occurrences
      case 'c':
        count_group = 1;
      // only print duplicate lines
      case 'd':
        dups_only = 1;
      // ignore differences in case when comparing
      case 'i':
        ignore_case = 1;
      default:
        printf(1, "uniq: illegal option %c\n", c);
        argc = 0;
        break;
      }
    }
  }
  if (argc <= 1) {
    uniq(0);
    exit();
  }

  for (i = 1; i < argc; i++) {
    if ((fd = open(argv[i], 0)) < 0) {
      printf(1, "uniq: cannot open %s\n", argv[i]);
      exit();
    }
    uniq(fd);
    close(fd);
  }
  exit();
}