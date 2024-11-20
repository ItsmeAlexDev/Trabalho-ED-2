#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "expressao.h"

#define MAX_STACK 512
#define M_PI 3.14159265358979323846

typedef struct
    {
        char *items[MAX_STACK];
        int top;
    } Stack;

void push(Stack *s, char *item)
    {
        if (s->top < MAX_STACK - 1) s->items[++(s->top)] = strdup(item);
    }

char *pop(Stack *s)
    {
        return (s->top >= 0) ? s->items[(s->top)--] : NULL;
    }

float performOperation(float a, float b, char op)
    {
        switch (op)
            {
                case '+': return a + b;
                case '-': return a - b;
                case '*': return a * b;
                case '/': return a / b;
                case '^': return pow(a, b);
                default: return 0;
            }
    }

float performFunction(float a, const char *func)
    {
        if (strcmp(func, "raiz") == 0) return sqrt(a);
        if (strcmp(func, "sen") == 0) return sin(a * M_PI / 180);
        if (strcmp(func, "cos") == 0) return cos(a * M_PI / 180);
        if (strcmp(func, "tg") == 0) return tan(a * M_PI / 180);
        if (strcmp(func, "log") == 0) return log10(a);
        return 0;
    }

char *getFormaInFixa(char *Str)
    {
        Stack s = {.top = -1};
        char *token = strtok(strdup(Str), " ");
        char buffer[1024];

        while (token != NULL) 
            {
                if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])))
                    {
                        push(&s, token);
                    }
                else
                    {
                        if (strlen(token) == 1)
                            {
                                char *b = pop(&s);
                                char *a = pop(&s);
                                sprintf(buffer, "(%s %s %s)", a, token, b);
                                push(&s, buffer);
                                free(a);
                                free(b);
                            }
                        else
                            {
                                char *a = pop(&s);
                                sprintf(buffer, "%s(%s)", token, a);
                                push(&s, buffer);
                                free(a);
                            }
                    }
                token = strtok(NULL, " ");
            }

        char *temp = pop(&s);
        char *result;
        
        sscanf(temp, "(%[^\n]", result);
        result[strlen(result) - 1] = '\0';

        return result;
    }


float getValor(char *Str)
    {    
        Stack s = {.top = -1};
        char *token = strtok(strdup(Str), " ");

        while (token != NULL)
            {
                if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])))
                    {
                        push(&s, token);
                    }
                else
                    {
                        if (strlen(token) == 1)
                            {
                                float b = atof(pop(&s));
                                float a = atof(pop(&s));
                                float result = performOperation(a, b, token[0]);
                                char buffer[50];
                                sprintf(buffer, "%f", result);
                                push(&s, buffer);
                            }
                        else
                            {
                                float a = atof(pop(&s));
                                float result = performFunction(a, token);
                                char buffer[50];
                                sprintf(buffer, "%f", result);
                                push(&s, buffer);
                            }
                    }
                token = strtok(NULL, " ");
            }

        float finalResult = atof(pop(&s));
        return finalResult;
    }