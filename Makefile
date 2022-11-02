compile:
	gcc -Wall -pedantic-errors -o head head.c
run: compile
	./head -n 4 file1.txt file2.txt
clean:
	rm head
