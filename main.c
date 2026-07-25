#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "apc.h"

#define MAX_INPUT 4096

static int is_valid_number(const char *s)
{
    int i = 0;

    if (s[i] == '-' || s[i] == '+')
        i++;

    if (!s[i])
        return 0;

    while (s[i])
    {
        if (!isdigit((unsigned char)s[i]))
            return 0;
        i++;
    }

    return 1;
}

int main()
{
    char num1[MAX_INPUT];
    char num2[MAX_INPUT];
    char op[2];

    printf("Enter expression : ");
    scanf("%4095s %1s %4095s", num1, op, num2);

    if (strlen(op) != 1)
    {
        printf("Invalid operator\n");
        return 1;
    }

    if (!is_valid_number(num1) || !is_valid_number(num2))
    {
        printf("Invalid number\n");
        return 1;
    }

    BigInt *a = bigint_from_string(num1);
    BigInt *b = bigint_from_string(num2);

    if (!a || !b)
    {
        printf("Failed to parse numbers\n");
        bigint_free(a);
        bigint_free(b);
        return 1;
    }

    if (op[0] == '/' && bigint_is_zero(b))
    {
        printf("Division by zero\n");
        bigint_free(a);
        bigint_free(b);
        return 1;
    }

    BigInt *result = NULL;

    switch (op[0])
    {
        case '+': result = bigint_add(a, b); break;
        case '-': result = bigint_sub(a, b); break;
        case '*': result = bigint_mul(a, b); break;
        case '/': result = bigint_div(a, b); break;
        default:
            printf("Invalid operator\n");
            bigint_free(a);
            bigint_free(b);
            return 1;
    }

    if (!result)
    {
        printf("Operation failed\n");
        bigint_free(a);
        bigint_free(b);
        return 1;
    }

    char *res = bigint_to_string(result);
    printf("Result = %s\n", res);

    free(res);
    bigint_free(a);
    bigint_free(b);
    bigint_free(result);

    return 0;
}