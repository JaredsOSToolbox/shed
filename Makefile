OBJS = command_t.o flag_t.o  pipe_t.o  strings.o  history.o 

COMPILER = clang
CFLAGS = -Wall -g
OUTPUT = shed

all: $(OBJS) shed.c
	$(COMPILER) $(CFLAGS) shed.c $(OBJS) -o $(OUTPUT)

command_t.o: src/command_t.c includes/command_t.h
	$(COMPILER) $(CFLAGS) -c src/command_t.c

flag_t.o: src/flag_t.c includes/flag_t.h
	$(COMPILER) $(CFLAGS) -c src/flag_t.c

history.o: src/history.c includes/history.h
	$(COMPILER) $(CFLAGS) -c src/history.c

pipe_t.o: src/pipe_t.c includes/pipe_t.h
	$(COMPILER) $(CFLAGS) -c src/pipe_t.c

strings.o: src/strings.c includes/strings.h
	$(COMPILER) $(CFLAGS) -c src/strings.c

test: all
	#echo -e "echo --version\n" | ./shed
	echo -e "uname -a" | ./shed -
	echo -e "uname -a | grep -o Linux | wc -l > example" | ./shed -
	#echo -e "uname -a | grep -o Linux | sort < example" | ./shed -
	#echo -e "echo hello | sed" | ./shed -
	#echo -e "echo hello | sed &" | ./shed -
run: all
	./$(OUTPUT)

clean:
	rm $(OBJS) $(OUTPUT)
