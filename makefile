ElGamal:
	swig -c++ -python bint.i
	g++ -fPIC -c bint.cpp
	g++ -fPIC -c bint_wrap.cxx -I/usr/include/python2.7
	g++ -shared bint.o bint_wrap.o -o _bint.so
	rm bint.o bint_wrap.cxx bint_wrap.o
