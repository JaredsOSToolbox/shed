#define PIPE 0
#define OUTPUT 1
#define INPUT 2
#define BACKGROUND 3

#define SIZE 4


struct flag_t {
    bool* container;
    int size;
};

struct flag_t *flag_t_constructor();

void flag_t_destructor(struct flag_t*);

void flag_t_set_flag(struct flag_t*, int);

bool get_flag(struct flag_t*, int);
