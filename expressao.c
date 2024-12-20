#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <float.h>
#include "expressao.h"

#define ERROR_VALUE FLT_MAX
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
                case '/': 
                    if (b == 0) 
                        {
                            printf("ERROR: Divisao por zero.\n\n");
                            return ERROR_VALUE;
                        }
                    else
                        {
                            return a / b;
                        }
                case '^':
                    if (a == 0 && b <= 0)
                        {
                            printf("ERROR: exponenciacao invalida.\n\n");
                            return ERROR_VALUE;
                        }
                    else
                        {
                            return pow(a, b);
                        }
                default:
                    printf("ERROR: Operador desconhecido '%c'.\n\n", op);
                    return ERROR_VALUE;
            }
    }

float performFunction(float a, const char *func)
    {
        if (strcmp(func, "raiz") == 0 || strcmp(func, "root") == 0) 
            {
                if (a < 0) 
                    {
                        printf("ERROR: Raiz de numero negativo.\n\n");
                        return ERROR_VALUE;
                    }
                else
                    {
                        return sqrt(a);
                    }
            }
        if (strcmp(func, "sen") == 0) return sin(a * M_PI / 180);
        if (strcmp(func, "cos") == 0) return cos(a * M_PI / 180);
        if (strcmp(func, "tg") == 0 || strcmp(func, "tan") == 0) 
            {
                if (fmod(a, 180) == 90) 
                    {
                        printf("ERROR: Tangente indefinida em %f graus.\n\n", a);
                        return ERROR_VALUE;
                    }
                else
                    {
                        return tan(a * M_PI / 180);
                    }
            }
        if (strcmp(func, "log") == 0) 
            {
                if (a <= 0) 
                    {
                        printf("ERROR: Logaritmo indefinido para valores nao positivos.\n\n");
                        return ERROR_VALUE;
                    }
                else
                    {
                        return log10(a);
                    }
            }
        
        printf("ERROR: Funcao desconhecida. '%s'.\n\n", func);
        return ERROR_VALUE;
    }

char *getFormaInFixa(char *Str)
    {
        Stack s = {.top = -1};
        char *token = strtok(strdup(Str), " ");
        char buffer[1024];

        while (token != NULL) 
            {
                if (strcmp(token, "**") == 0) token = "^";

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
                                if (a[0] != '(' && b[0]!='(')
                                    {
                                        sprintf(buffer, "(%s %s %s)", a, token, b);
                                    }
                                else
                                    {
                                        sprintf(buffer, "%s %s %s", a, token, b);
                                    }
                                push(&s, buffer);
                                free(a);
                                free(b);
                            }
                        else
                            {
                                char *a = pop(&s);
                                if (a[0] != '(')
                                    {
                                        sprintf(buffer, "%s(%s)", token, a);
                                    }
                                else
                                    {
                                        sprintf(buffer, "%s%s", token, a);
                                    }
                                push(&s, buffer);
                                free(a);
                            }
                    }
                token = strtok(NULL, " ");
            }

        char *result = pop(&s);

        return result;
    }


float getValor(char *Str)
    {    
        Stack s = {.top = -1};
        char *token = strtok(strdup(Str), " ");

        while (token != NULL)
            {
                if (strcmp(token, "**") == 0) token = "^";
                
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