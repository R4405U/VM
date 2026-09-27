SRCS = $(shell find lib/src processor/src -name "*.c" -type f)
FLAGS = -Wall -Wextra -Wpedantic

sm: $(OBJS)
	$(MAKE) clean
	mkdir bin
	gcc -o bin/vm main.c $(SRCS) -Iinclude -Ilib/include -Iprocessor/include  $(FLAGS)

%.o: %.c
	gcc -c $< -Iinclude -o $@

clean:
	rm -rf bin src/*.o *.o vm
