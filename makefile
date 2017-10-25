CC=g++
CFLAGS=-w -g -o

testSem: token.cpp token.h main.cpp scanner.h scanner.cpp parser.cpp parser.h buildTree.cpp buildTree.h node.h node.cpp semantics.cpp semantics.h stack.cpp stack.h
	$(CC) $(CFLAGS)  comp token.cpp main.cpp scanner.cpp parser.cpp buildTree.cpp node.cpp stack.cpp semantics.cpp

.PHONY: clean

clean:
	rm -f *.o
	rm -f main
	rm -f *.txt
	rm -f *.exe
	rm -f *.rc
	rm -f *.dev
	rm -f *.win
	rm -f *.layout
	rm -f *.postorder
	rm -f *.preorder
	rm -f *.inorder
