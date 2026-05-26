#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int global_var;

__attribute__((stack_protect))
void explicit_protected(void) {
     int x = 1;
    printf("%d\n", x);
}

struct Data {
    int id;
    char name[16];
};

void char_array_small(void) {
    char buf[4];
    buf[0] = 'A';
}

void char_array_large(void) {
    char buf[16];
    buf[0] = 'A';
}

void int_array(void) {
    int nums[10];
    nums[0] = 1;
}

void pointer_only(void) {
    int x = 10;
    int *p = &x;
    printf("%d\n", *p);
}

void local_struct(void) {
    struct Data d;
    d.id = 1;
    strcpy(d.name, "hello");
}

void uses_strcpy(void) {
    char dst[16];
    strcpy(dst, "hello");
}

void simple_local_int(void){
    int x = 1;
    printf("%d\n", x);
}

void simple_math(void){
    int a = 1;
    int b = 2;
    int c = a + b;
    printf("%d\n", c);
}

void global_only(void){
    global_var++;
    printf("%d\n", global_var);
}

void heap_only(void){
    char *p = malloc(16);
    strcpy(p, "hello");
    printf("%s\n", p);
    free(p);
}

void never_called(void){
    char buf[32];
    buf[0] = 'A';
}

int main(void) {
    char buf[32];

    char_array_small();
    char_array_large();
    int_array();
    pointer_only();
    local_struct();
    uses_strcpy();
    simple_local_int();
    simple_math();
    global_only();
    heap_only();
    return 0;
}
