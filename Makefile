
.SUFFIXES: .cxx .o .x .hxx .c .cpp

NAME1 = FibonacciHeap
CXX1 = $(NAME1).cxx
EXEC1 = $(NAME1).x

CO=g++
FLAGS= -std=c++11

.PHONY: all
all: $(EXEC1)

$(EXEC1):
	$(CO) -o $@ $(CXX1) $(FLAGS)

.PHONY: fib
fib: all

	./$(EXEC1)

.PHONY: clean
clean:                                                     
	rm -f *.o  *~ *.a *.so *.dat *.x core core* a.out; rm -rf ${LIB_DIR}
