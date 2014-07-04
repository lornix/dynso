#include <stdio.h>
#include <stdlib.h>

#include <elf.h>

#pragma GCC visibility push(hidden)

void __attribute__((constructor)) library_constructor()
{
    puts("Welcome from " __FILE__ " - constructor!");
}
void __attribute__((destructor)) library_destructor()
{
    puts("Goodbye from " __FILE__ " - destructor!");
}

#pragma GCC visibility pop

void hello(void)
{
    puts("Hello from " __FILE__ "");
}

/***************************************************************************/

/* given pointer to a char*** array, char** for strings, and max len
 * create an array, returning length of array.  Update *strings to indicate
 * final byte parsed.
 *
 * maxlen: >0, only parse this many items
 *         <=0, ignored, parse until trailing zero found
 */
static int build_char_ptr_array(char*** arr, char** strings, int maxlen)
{
    char* strptr=*strings;
    int arr_count=0;
    *arr=realloc(0,sizeof(char*));
    (*arr)[0]=strptr;

    while (*strptr!=0) {
        if ((maxlen>0)&&(arr_count>=maxlen)) {
            break;
        }
        /* loop while non-zero to skip text of env-var */
        while (*strptr!=0) {
            strptr++;
        }
        /* skip the trailing zero */
        strptr++;
        /* add to array */
        arr_count++;
        *arr=realloc(*arr,sizeof(char*)*(arr_count+1));
        (*arr)[arr_count]=strptr;
    }
    /* update pointer to indicate last byte */
    *strings=strptr;
    /* return count of items found */
    return arr_count;
}

/***************************************************************************/
/***************************************************************************/

/* turns out that you can create executable libraries/objects!
 *
 * Add the following const char string to your library code, editing it as
 * appropriate (/lib/ld-2.19.so or whatever works)
 *
 * Add '-Wl,--entry=MY_ENTRY_POINT` to your CFLAGS, which defines the 'main'
 * function in your code
 *
 * ???
 *
 * profit!
 */

const char interp[] __attribute__((section(".interp"))) = "/lib64/ld-linux-x86-64.so.2";

void ex1_main (
        void* unk0,
        int arg_argc,
        void* unk1, void* unk2, void* unk3, void* unk4,
        char* strptr
        )
{
    /* haven't figured out what unk0-unk4 values represent, but can recreate
     * argc, argv[] and env[] */

    /* rebuild arg array as 'args[]' */
    char** args=0;
    int argc=build_char_ptr_array(&args,&strptr,arg_argc);
    printf("argc: %d ->",argc);
    for (int i=0; i<argc; i++) {
        printf(" %s",args[i]);
    }
    printf("\n");

    puts("");

    /* rebuild environment variable array as 'env[]' */
    char** env=0;
    int envc=build_char_ptr_array(&env,&strptr,0);
    printf("env: %d entries\n",envc);

    puts("");

    printf("unk0: %p - '%s'\n",unk0,(char*)unk0);
    printf("unk1: %p - '%s'\n",unk1,(char*)unk1);
    printf("unk2: %p - '%s'\n",unk2,(char*)unk2);
    printf("unk3: %p\n",unk3);
    printf("unk4: %p\n",unk4);

    exit(0);
}
