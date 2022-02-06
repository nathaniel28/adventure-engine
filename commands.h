typedef struct {
  char *name;
  void (*function)(char **, int);
} Command;

extern void do_command(char *cmd);
