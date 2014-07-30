DynSO
=====

A simple demo/test of a dynamic library.

main.c uses dlopen/dlsym to find the ex1.o object, then execute the 'hello'
function from ex1.c.

Also shows how to create `executable objects` (ex1.o) and some code to rebuild
argv\[] and env\[] from the parameters found on ex1's main entry point function.
