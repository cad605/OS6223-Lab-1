#include "types.h"
#include "stat.h"
#include "user.h"
#define MAXLINE 1000

char buf[512];
char delimiter = '\n';
static int count_group, dups_only, ignore_case = 0;

/* uniq: when fed an input, outputs the input with adjacent identical lines
 * collapsed to one*/
void uniq(int fd) {
  printf(1, "uniq: starting\n");
  //   char line[MAXLINE];
  int i, n;

  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
      if (buf[i] == '\n') {
        printf(1, "%s", buf[i]);
      }
    }
  }

  if (n < 0) {
    printf(1, "uniq: read error\n");
    exit();
  }
  printf(1, "\n");
}

int main(int argc, char *argv[]) {
  int fd, i, c;

  while (--argc > 0 && (*++argv)[0] == '-') {
    while ((c = *++argv[0])) {
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
  printf(1, "argc: %d", argc);
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