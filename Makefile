EX1 := -o example1 example1.c
EX2 := -o example2 example2.c

all: example1 example2

example1: example1.c
	gcc $(EX1)

example2: example2.c
	gcc $(EX2)

debug1: example1.c
	gcc -g $(EX1)

debug2: example1.c
	gcc -g $(EX2)

clean:
	rm example1 example2
