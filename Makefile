compile:
	gcc -lm -Wall -pedantic-errors -o wc wc.c
run: compile
	./wc file2.txt file1.txt -
clean:
	rm wc
