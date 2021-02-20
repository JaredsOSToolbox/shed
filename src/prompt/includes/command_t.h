#define MAX_WORDS 100
#define MAX_COMMAND_NAME_SIZ 50

struct command_t {
  char* command_path;
  char** arguments;
  int argc;
};

struct command_t * command_t_constructor(char*);
void command_t_destructor(struct command_t*);
void command_t_invoke(struct command_t*);
void command_t_print(struct command_t*);
