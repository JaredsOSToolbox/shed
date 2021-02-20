OBJS = history.o
COMPILER = clang
CFLAGS = -Wall
OUTPUT = shed

all: $(OBJS) 
	$(COMPILER) $(CFLAGS) shed.c $(OBJS) -o $(OUTPUT)

history.o: src/history.c includes/history.h
	$(COMPILER) $(CFLAGS) -c src/history.c

run: all
	./$(OUTPUT)

clean:
	rm $(OBJS) $(OUTPUT)
