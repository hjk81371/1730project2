compile:
	gcc -Wall -pedantic-errors -o wc wc.c
run: compile
	./wc file1.txt
clean:
	rm wc
