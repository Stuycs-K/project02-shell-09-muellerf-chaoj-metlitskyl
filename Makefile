outname = shell
<<<<<<< HEAD
c_files = parse.c utils.c redirection.c
=======
c_files = parse.c utils.c exec.c
>>>>>>> 89618252669bcfab8ecd2b22c02d16bee40743e6

o_files = $(c_files:.c=.o)
h_files = $(c_files:.c=.h)
.PHONY: compile run clean
$(outname) compile: main.o $(o_files)
	@gcc -o $(outname) main.o $(o_files) -Wall
main.o: main.c $(c_files) $(h_files)
	@gcc -c main.c -Wall
%.o: %.c %.h
	@gcc -c $< -Wall
clean:
	@rm -f $(outname) *.o
run: $(outname)
	@./$(outname)
