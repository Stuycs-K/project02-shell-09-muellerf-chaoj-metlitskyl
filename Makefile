outname = shell
c_files = parse.c utils.c

o_files = $(c_files:.c=.o)
h_files = $(c_files:.c=.h)
.PHONY: compile run clean
$(outname).out compile: main.o $(o_files)
	@gcc -o $(outname).out main.o $(o_files) -Wall
main.o: main.c $(c_files) $(h_files)
	@gcc -c main.c -Wall
%.o: %.c %.h
	@gcc -c $< -Wall
clean:
	@rm -f $(outname).out *.o
run: $(outname).out
	@./$(outname).out
