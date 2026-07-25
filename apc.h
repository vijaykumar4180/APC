#ifndef APC_H
#define APC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Node of the doubly linked list ──────────────────────────────────────── */
typedef struct Node {
    int digit;
    struct Node *prev;
    struct Node *next;
} Node;

/* ── BigInt: doubly linked list of digits (most-significant first) ───────── */
typedef struct {
    Node *head;
    Node *tail;
    int   length;
    int   negative;
} BigInt;

/* ── Lifecycle ───────────────────────────────────────────────────────────── */
BigInt *bigint_create(void);
BigInt *bigint_from_string(const char *str);
void    bigint_free(BigInt *n);
void    bigint_print(const BigInt *n);
char   *bigint_to_string(const BigInt *n);

/* ── Arithmetic ──────────────────────────────────────────────────────────── */
BigInt *bigint_add(const BigInt *a, const BigInt *b);
BigInt *bigint_sub(const BigInt *a, const BigInt *b);
BigInt *bigint_mul(const BigInt *a, const BigInt *b);
BigInt *bigint_div(const BigInt *a, const BigInt *b);

/* ── Comparison ──────────────────────────────────────────────────────────── */
int bigint_cmp_abs(const BigInt *a, const BigInt *b);
int bigint_is_zero(const BigInt *n);

#endif /* APC_H */