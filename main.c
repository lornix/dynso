#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
    puts("Hello from " __FILE__ " - main program");

    void* ex1=dlopen("ex1.o",RTLD_LAZY);
    if (ex1==NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    char* error=dlerror();

    void (*hello)(void)=(void (*)(void)) dlsym(ex1, "hello");

    error = dlerror();

    if (error != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    (*hello)();

    dlclose(ex1);

    puts("Goodbye from " __FILE__ " - main program");

    exit(EXIT_SUCCESS);
}
