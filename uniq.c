#include "types.h"
#include "stat.h"
#include "user.h"
#define BUFFSIZE 100

char buf[BUFFSIZE];
char delimiter = '\n';
static int count_group, dups_only, ignore_case = 0;

char *toLowerCase(char *str) {
  char *s = (char *)malloc(sizeof(str));
  strcpy(s, str);
  while (*str != '\0') {
    if (*str >= 'A' && *str <= 'Z') {
      *s = *str + 32;
    }
    ++str;
  }
  return s;
}

/* uniq: when fed an input, outputs the input with adjacent identical lines
 * collapsed to one*/
void uniq(int fd) {
  char *cur_line = (char *)malloc(BUFFSIZE);
  char *prev_line = (char *)malloc(BUFFSIZE);
  ;
  int i, n, j, count = 0;

  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    j = 0;
    count = 1;
    for (i = 0; i < n; i++) {

      // handle overflow
      if (j == sizeof(cur_line)) {
        char *temp1 = (char *)malloc(sizeof(cur_line) * 2);
        strcpy(temp1, cur_line);
        free(cur_line);
        cur_line = temp1;

        char *temp2 = (char *)malloc(sizeof(cur_line) * 2);
        strcpy(temp2, prev_line);
        free(prev_line);
        prev_line = temp2;
      }

      *(cur_line + j) = buf[i];
      j = j + 1;

      // handle newline
      if (buf[i] == '\n' || i == n - 1) {
        if (ignore_case) {
          if (count_group) {
            if (i == n - 1) {
              printf(1, "%d %s", count, cur_line);
            } else if (i > 0 && (strcmp(toLowerCase(cur_line),
                                        toLowerCase(prev_line))) != 0) {
              printf(1, "%d %s", count, prev_line);
              count = 1;
            } else {
              count++;
            }
          } else if (dups_only) {
            if (count > 1 && (strcmp(toLowerCase(cur_line), toLowerCase(prev_line))) != 0) {
                printf(1, "%s", prev_line);
                count = 1;
            } else if (count >= 1 && (strcmp(toLowerCase(cur_line), toLowerCase(prev_line))) == 0) {
                count++;
            }
          } else {
            if ((strcmp(toLowerCase(cur_line), toLowerCase(prev_line))) != 0) {
              printf(1, "%s", cur_line);
            }
          }
        } else {
          if (count_group) {
            if (i == n - 1) {
              printf(1, "%d %s", count, cur_line);
            } else if (i > 0 && (strcmp(cur_line, prev_line)) != 0) {
              printf(1, "%d %s", count, prev_line);
              count = 1;
            } else {
              count++;
            }
          } else if (dups_only) {
            if (count > 1 && (strcmp((cur_line), (prev_line))) != 0) {
                printf(1, "%s", prev_line);
                count = 1;
            } else if (count >= 1 && (strcmp((cur_line), (prev_line))) == 0) {
                count++;
            }
          } else {
            if ((strcmp((cur_line), (prev_line))) != 0) {
              printf(1, "%s", cur_line);
            }
          }
        }

        free(prev_line);
        prev_line = (char *)malloc(sizeof(cur_line));
        strcpy(prev_line, cur_line);
        free(cur_line);
        cur_line = (char *)malloc(sizeof(prev_line));
        j = 0;
      }
    }
  }
  free(cur_line);
  free(prev_line);

  if (n < 0) {
    printf(1, "uniq: read error\n");
    exit();
  }
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
        break;
      // ignore differences in case when comparing
      case 'i':
        ignore_case = 1;
        break;
      // only print duplicate lines
      case 'd':
        dups_only = 1;
        break;
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