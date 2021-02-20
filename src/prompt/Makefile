OBJS = history.o strings.o command_t.o
COMPILER = clang
CFLAGS = -Wall
OUTPUT = shed

all: $(OBJS) 
	$(COMPILER) $(CFLAGS) shed.c $(OBJS) -o $(OUTPUT)

command_t.o: src/command_t.c includes/command_t.h
	$(COMPILER) $(CFLAGS) -c src/command_t.c

history.o: src/history.c includes/history.h
	$(COMPILER) $(CFLAGS) -c src/history.c

strings.o: src/strings.c includes/strings.h
	$(COMPILER) $(CFLAGS) -c src/strings.c

run: all
	./$(OUTPUT)

clean:
	rm $(OBJS) $(OUTPUT)
