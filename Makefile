# Executable name
BINARY=sp

# Automatically passes flags in -l of gcc
LDLIBS=-lcunit

# Facilitates compiler change
CC=gcc

# Flags to generate dependency files encoding dependencies
DEPFLAGS=-MP -MD

# Flags for gcc options
CFLAGS=-Wall -Werror -Wextra -g -std=gnu99 $(foreach D,$(INCLUDEDIRS),-I$(D)) $(DEPFLAGS)

####################
# Folder variables #
####################

# Folder(s) for .c files
CDIRS=./src # could be a list

# Folder(s) for .h files
INCLUDEDIRS=./include # could be a list

# Folder(s) for test .c files
TESTDIR=./tests

# Folder for graphs
GRAPH=./tests/graph

# Folder for graph output files
GRAPHOUT=./tests/graph_output

##################
# File variables #
##################

# Create a list of all .c files in the CDIRS folder(s)
CFILES=$(foreach D,$(CDIRS),$(wildcard $(D)/*.c))

# Create a .o file for each .c file
OBJECTS=$(patsubst %.c,%.o,$(CFILES))

# Create a .d file (to track dependencies) for each .c file
DEPENFILES=$(patsubst %.c,%.d,$(CFILES))

# Create a list of all test .c files
TESTFILES=$(foreach D,$(TESTDIR),$(wildcard $(D)/*.c))

# Create a binary file for each test .c file
TESTBINS=$(patsubst %.c,%,$(TESTFILES))

# Create a list of graph files
GRAPHFILES=$(foreach F,$(wildcard $(GRAPH)/*.bin),$(notdir $(F)))

#####################
# Makefile commands #
#####################

# The first command is to build the project's executable
sp: $(BINARY)

# To build BINARY, we need OBJECTS. The next line is the compilation
$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ -lpthread

# Run all compiled test file(s)
tests: $(TESTBINS)
	for test in $(TESTBINS) ; do ./$$test ; done

# Run valgrind on the executable
valgrind: $(BINARY)
	$(foreach D,$(GRAPHFILES), \
		valgrind --leak-check=full --show-leak-kinds=all ./$(BINARY) $(GRAPH)/$(D) -f $(GRAPHOUT)/$(patsubst %.bin,%_output.bin, $(D)) ; \
		valgrind --tool=helgrind  --history-level=approx -s ./$(BINARY) $(GRAPH)/$(D) -f $(GRAPHOUT)/$(patsubst %.bin,%_output.bin, $(D)) ; \
	)

# Run cppcheck
cpp: $(BINARY)
	cppcheck --enable=all --force --verbose $(foreach D,$(INCLUDEDIRS),-I$(D)) --check-config .

# Remove all created files (binaries, .o, .d, compiled test files)
clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPENFILES) $(TESTBINS)
	rm -rf ./tests/tests.d

#####################
# Compilation rules #
#####################

# Define how it will compile .o files with .c files
# .h and the include at the bottom of the file allow adding a dependency between .h and .c

%.o:%.c %.h
	$(CC) $(CFLAGS) -c -o $@ $^

# Define how it will compile tests with the rest of the project
$(TESTDIR)/bin/%: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) $< $(OBJECTS) -o $@ $(LDLIBS)

##########
# .Phony #
##########

# If we don't use .Phony and a file has the name of a command, the command might not run if it thinks the file hasn't been modified, despite the fact that the file has nothing to do with the command.

.PHONY: clean tests all

##########
# Others #
##########

# This allows the recompilation of .c files if their .h file is modified

# When a .h file is modified, all .c files that include it will be recompiled

-include $(DEPENFILES)
