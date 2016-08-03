CFLAGS = -Wno-deprecated-declarations 

cltk_test: cltk_test.o cltk.o
	$(CC) -o cltk_test cltk_test.o cltk.o -lOpenCL 

cltk_test.o: example/cltk_test.c
	$(CC) -c $(CFLAGS) example/cltk_test.c -o ./cltk_test.o -I./include

cltk.o: src/cltk.c
	$(CC) -c $(CFLAGS) src/cltk.c -o ./cltk.o -I./include

clean:
	rm -f cltk_test *.o
