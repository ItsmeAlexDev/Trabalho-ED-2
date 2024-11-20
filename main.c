#include "expressao.c"

void main() {
    Expressao expr;
    
    FILE *arquivo = fopen("teste.txt", "r");

    char *temp;
    fscanf(arquivo, "%[^\n]", temp);

    strcpy(expr.posFixa, temp);
    strcpy(expr.inFixa, getFormaInFixa(expr.posFixa));
    expr.Valor = getValor(expr.posFixa);
    printf("Posfixa: %s\nInfixa: %s\nValor: %.7f\n\n", expr.posFixa, expr.inFixa, expr.Valor);
    
    fclose(arquivo);
}