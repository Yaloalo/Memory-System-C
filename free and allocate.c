#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef int (*func_ptr)(int);

//Struct that maps name to function
typedef struct function_map{
    const char* name;
    func_ptr func;
} f_map;


//Stack Definitions
struct stackframe{

};

typedef struct stack{
    int stack[1000];
    int* base_pointer;
} stack;

typedef struct heap{
    int stack[1000];
    bool is_full;
} heap;


//Stack Operations
int stack_pop(stack* stack, int** base_pointer);
int* stack_push(stack* stack, int** base_pointer, int value);


//Function to work with
int __add__ (int);

int __sub__ (int);

//function map for __add__ and __sub__
f_map map_for_add_sub[] = {
        {"add", __add__},
        {"sub", __sub__},
        {NULL, NULL}
};


int main(){
    //Input, stop being fucking dense scanf need adress and format
    //Also DOuble Pointers for function input when modifyng a pointer
    char input2[100];
    int input;
    printf("Enter number: \n");
    scanf("%d", &input);
    printf("Enter string: \n");
    scanf("%s", &input2);
    //TODO Error catching if input is not add or sub
    for (int i; i<2; i++){
        if (strcmp(map_for_add_sub[i].name, input2) == 0){
            map_for_add_sub[i].func(input);
        }
    }

    struct stack *stack_allocated = (struct stack *)malloc((sizeof(stack)));
    stack_allocated->base_pointer = &stack_allocated->stack[0];
}

int stack_pop(stack* stack, int** base_pointer){
    //Check for the pointer equality and update base pointer location return the value
    for (int i; i < 1000; i++) {
        if (&stack->stack[i] == *base_pointer) {
            *base_pointer = &stack->stack[i-1];
            return stack->stack[i];
        }
    }
}

int* stack_push(stack* stack, int** base_pointer, int value){
    //Typecast wahrschienlich nciht notwendig, heap ubung
    int *adress = (int *)malloc(sizeof(int));
    for (int i; i < 1000; i++) {
        if (&stack->stack[i] == *base_pointer) {
             stack->stack[i=1] = value;
             *base_pointer = &stack->stack[i+1];
            return adress;
        }
    }
    return adress;
}

int __add__ (int a){
    //TODO Overflow Check
    int b = a+1;
    return b;
}

int __sub__ (int a){
    //TODO Underflow Check
    int b = a-1;
    return b;
}