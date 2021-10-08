#include "types.h"
#include "stat.h"
#include "user.h"
#define BUFFSIZE 1000

char buf[BUFFSIZE];
char delimiter = '\n';
static int count_group, dups_only, ignore_case = 0;

/* uniq: when fed an input, outputs the input with adjacent identical lines
 * collapsed to one*/
void uniq(int fd) {
  char *cur_line = (char *)malloc(BUFFSIZE);
  char *prev_line;
  int i, n;

  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
      
      // handle overflow
      if (sizeof(cur_line) == BUFFSIZE) {
        char *temp = (char *)malloc(BUFFSIZE * 2);
        strcpy(temp, cur_line);
        free(cur_line);
        cur_line = temp;
      }
      
      *(cur_line + i) = buf[i];
      
      // handle newline
      if (buf[i] == '\n') {
        if (!(strcmp(cur_line, prev_line))) {
          printf(1, "%s", cur_line);
        }
        prev_line = (char *)malloc(sizeof(cur_line));
        strcpy(prev_line, cur_line);
        free(cur_line);
        cur_line = (char *)malloc(sizeof(prev_line));
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

  if (argc <= 1) {
    uniq(0);
    exit();
  }

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

  for (i = 0; i < argc; i++) {
    if ((fd = open(argv[i], 0)) < 0) {
      printf(1, "uniq: cannot open %s\n", argv[i]);
      exit();
    }
    uniq(fd);
    close(fd);
  }
  exit();
}