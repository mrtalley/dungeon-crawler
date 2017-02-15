#include <stdio.h>
#include <string.h>
#include <machine/endian.h>

// ./fileio -wt <file>
// ./fileio -rt <file>
// ./fileio -wb <file>
// ./fileio -rb <file>

typedef enum action {
  action_read_binary,
  action_write_binary,
  action_read_text,
  action_write_text,
} action_t;

int main(int argc, char *argv[])
{
  FILE *f;
  struct {
    int i;
    int j;
    int k;
  } s = { 3, 1, 4 };

  if(argc != 3) {
    fprintf(stderr, "Wrong number of parameters.\n"); // still printed to terminal, but puts error output in a file
    return -1;
  }

  action_t action;
  // i

  if(!strcmp(argv[1], "-wt")) {
    action = action_write_text;

  }
  else if(!strcmp(argv[1], "-rt")) {
    action = action_read_text;
  }
  else if(!strcmp(argv[1], "-wb")) {
    action = action_write_binary;
  }
  else if(!strcmp(argv[1], "-rb")) {
    action = action_read_binary;
  }
  else {
    fprintf(stderr, "Unrecognized switch.\n");

    return -1;
  }

  if(action == action_write_text || action == action_write_binary) {
    // open for writing
    if((fopen(argv[2], "w"))) {
      fprintf(stderr, "Failed to open %s.\n", argv[2]);

      return -1;
    }
  }
  if(action == action_read_text || action == action_read_binary) {
    // open for reading
    if((fopen(argv[2], "r"))) {
      fprintf(stderr, "Failed to open %s.\n", argv[2]);

      return -1;
    }
  }

  switch(action) {
    case action_read_binary:
      fread(&s.i, sizeof((i), l, f));
      i = be32toh(s.i); // big_endian_32bits
      fread(&s.j, sizeof((i), l, f));
      j = be32toh(s.j); // big_endian_32bits
      fread(&s.k, sizeof((i), l, f));
      k = be32toh(s.k); // big_endian_32bits
      printf("%d %d %d", s.i, s.j, s.k);
      break;

    case action_write_binary:
      i = htobe32(s.i); // host_32bits
      fwrite(&s.i, sizeof((s.i), l, f));
      i = htobe32(s.j); // host_32bits
      fwrite(&s.j, sizeof((s.j), l, f));
      i = htobe32(s.k); // host_32bits
      fwrite(&s.k, sizeof((s.k), l, f));
      printf("%d %d %d", s.i, s.j, s.k);
      break;

    case action_read_text;
      fscanf(f, "%d %d %d", &s.i, &s.j, &s.k); // & becasue you are passing to the var
      break;

    case: action_write_text:
      fprintf(f, "%d %d %d", s.i, s.j, s.k);
      break;

  }

  return 0;
}
