EXECS= A B

all: ${EXECS}

A: A.c
	gcc minheap.c A.c -o A

B: A.c
	gcc maxheap.c B.c -o B

clean:
	rm -f ${EXECS}
