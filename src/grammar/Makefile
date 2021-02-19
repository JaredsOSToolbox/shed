# TODO : this is directly pulled from Diablo/syntaxical_analysis

# OBJS = command.o  DiabloExceptions.o Instruction.o Assembler.o Symbol.o SymbolTable.o # include balancer.o after, needed for synthetic.cpp
COMPILER = clang
CFLAGS = -Wall
OUTPUT = bandit
LIBNAME = banditLib

LIBPATH = -L bin/static/$(LIBNAME)

INC_PATH = -Iincludes

all: scanner parser
	$(COMPILER) $(CFLAGS) main.c -o $(OUTPUT)

#create_static: $(OBJS)
	#ar rcs bin/static/$(LIBNAME).a $(OBJS)

scanner: scanner.l
	flex -o build/scanner.cpp scanner.l

parser: parser.y
	bison -o build/parser.cpp parser.y

run:
	./$(OUTPUT)

clean: 
	rm $(OUTPUT) build/scanner.cpp build/parser.cpp build/parser.hpp build/*.hh

debug: all
	gdb $(OUTPUT)
