SRC := dynamic_array.c

all: example1 example2

example1: example1.c $(SRC)
	gcc -o example1 example1.c $(SRC)

example2: example2.c $(SRC)
	gcc -o example2 example2.c $(SRC)

debug1: example1.c $(SRC)
	gcc -g -o example1 example1.c $(SRC)

debug2: example1.c $(SRC)
	gcc -g -o example2 example2.c $(SRC)

clean:
	rm example1 example2
