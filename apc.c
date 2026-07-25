#include "apc.h"

/* ── Helper Functions ─────────────────────────────────────────────────────── */

static Node *node_new(int digit)
{
    Node *n = malloc(sizeof(Node));
    if (!n) { perror("malloc"); exit(EXIT_FAILURE); }
    n->digit = digit;
    n->prev = n->next = NULL;
    return n;
}

static void bigint_append_tail(BigInt *num, int digit)
{
    Node *n = node_new(digit);
    if (!num->tail)
        num->head = num->tail = n;
    else
    {
        n->prev = num->tail;
        num->tail->next = n;
        num->tail = n;
    }
    num->length++;
}

static void bigint_prepend_head(BigInt *num, int digit)
{
    Node *n = node_new(digit);
    if (!num->head)
        num->head = num->tail = n;
    else
    {
        n->next = num->head;
        num->head->prev = n;
        num->head = n;
    }
    num->length++;
}

static void bigint_trim(BigInt *num)
{
    while (num->head && num->head->next && num->head->digit == 0)
    {
        Node *temp = num->head;
        num->head = temp->next;
        num->head->prev = NULL;
        free(temp);
        num->length--;
    }
}

static void normalize_zero(BigInt *num)
{
    if (num->length == 1 && num->head->digit == 0)
        num->negative = 0;
}

static BigInt *bigint_copy(const BigInt *src)
{
    BigInt *dst = bigint_create();
    dst->negative = src->negative;
    for (Node *cur = src->head; cur; cur = cur->next)
        bigint_append_tail(dst, cur->digit);
    return dst;
}

/* ── Lifecycle ───────────────────────────────────────────────────────────── */

BigInt *bigint_create(void)
{
    BigInt *num = calloc(1, sizeof(BigInt));
    if (!num) { perror("calloc"); exit(EXIT_FAILURE); }
    return num;
}

BigInt *bigint_from_string(const char *str)
{
    if (!str || !*str)
        return NULL;

    BigInt *num = bigint_create();
    int i = 0;

    if (str[i] == '-') { num->negative = 1; i++; }
    else if (str[i] == '+') { i++; }

    /* skip leading zeros but keep at least one digit */
    while (str[i] == '0' && str[i + 1])
        i++;

    for (; str[i]; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            bigint_free(num);
            return NULL;
        }
        bigint_append_tail(num, str[i] - '0');
    }

    if (num->length == 0)
        bigint_append_tail(num, 0);

    normalize_zero(num);
    return num;
}

void bigint_free(BigInt *num)
{
    if (!num) return;
    Node *cur = num->head;
    while (cur)
    {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }
    free(num);
}

char *bigint_to_string(const BigInt *n)
{
    if (!n) return NULL;

    int len = n->length + (n->negative ? 1 : 0) + 1;
    char *str = malloc(len);
    if (!str) { perror("malloc"); exit(EXIT_FAILURE); }

    int i = 0;
    if (n->negative)
        str[i++] = '-';

    for (Node *cur = n->head; cur; cur = cur->next)
        str[i++] = '0' + cur->digit;

    str[i] = '\0';
    return str;
}

void bigint_print(const BigInt *n)
{
    char *s = bigint_to_string(n);
    printf("%s\n", s);
    free(s);
}

/* ── Comparison ──────────────────────────────────────────────────────────── */

int bigint_cmp_abs(const BigInt *a, const BigInt *b)
{
    if (a->length != b->length)
        return a->length > b->length ? 1 : -1;

    Node *ca = a->head, *cb = b->head;
    while (ca)
    {
        if (ca->digit != cb->digit)
            return ca->digit > cb->digit ? 1 : -1;
        ca = ca->next;
        cb = cb->next;
    }
    return 0;
}

int bigint_is_zero(const BigInt *n)
{
    return n->length == 1 && n->head->digit == 0;
}

/* ── Addition of absolute values (result sign set by caller) ─────────────── */

static BigInt *add_abs(const BigInt *a, const BigInt *b)
{
    BigInt *result = bigint_create();
    Node *ta = a->tail, *tb = b->tail;
    int carry = 0;

    while (ta || tb || carry)
    {
        int sum = carry;
        if (ta) { sum += ta->digit; ta = ta->prev; }
        if (tb) { sum += tb->digit; tb = tb->prev; }
        carry = sum / 10;
        bigint_prepend_head(result, sum % 10);
    }
    return result;
}

/* ── Subtraction of absolute values: assumes |a| >= |b| ─────────────────── */

static BigInt *sub_abs(const BigInt *a, const BigInt *b)
{
    BigInt *result = bigint_create();
    Node *ta = a->tail, *tb = b->tail;
    int borrow = 0;

    while (ta)
    {
        int diff = ta->digit - borrow;
        if (tb) { diff -= tb->digit; tb = tb->prev; }
        if (diff < 0) { diff += 10; borrow = 1; }
        else           borrow = 0;
        bigint_prepend_head(result, diff);
        ta = ta->prev;
    }

    bigint_trim(result);
    return result;
}

/* ── Arithmetic ──────────────────────────────────────────────────────────── */

BigInt *bigint_add(const BigInt *a, const BigInt *b)
{
    BigInt *result;

    if (a->negative == b->negative)
    {
        result = add_abs(a, b);
        result->negative = a->negative;
    }
    else
    {
        int cmp = bigint_cmp_abs(a, b);
        if (cmp == 0)
        {
            result = bigint_create();
            bigint_append_tail(result, 0);
        }
        else if (cmp > 0)
        {
            result = sub_abs(a, b);
            result->negative = a->negative;
        }
        else
        {
            result = sub_abs(b, a);
            result->negative = b->negative;
        }
    }

    normalize_zero(result);
    return result;
}

BigInt *bigint_sub(const BigInt *a, const BigInt *b)
{
    /* a - b == a + (-b) */
    BigInt *neg_b = bigint_copy(b);
    neg_b->negative = !b->negative;
    BigInt *result = bigint_add(a, neg_b);
    bigint_free(neg_b);
    return result;
}

BigInt *bigint_mul(const BigInt *a, const BigInt *b)
{
    /* Result can have at most a->length + b->length digits */
    int len = a->length + b->length;
    int *buf = calloc(len, sizeof(int));
    if (!buf) { perror("calloc"); exit(EXIT_FAILURE); }

    /* Grade-school multiplication, least-significant first */
    Node *ta = a->tail;
    for (int i = 0; ta; ta = ta->prev, i++)
    {
        Node *tb = b->tail;
        for (int j = 0; tb; tb = tb->prev, j++)
        {
            buf[i + j] += ta->digit * tb->digit;
            buf[i + j + 1] += buf[i + j] / 10;
            buf[i + j] %= 10;
        }
    }

    BigInt *result = bigint_create();
    for (int i = len - 1; i >= 0; i--)
        bigint_append_tail(result, buf[i]);

    free(buf);
    bigint_trim(result);

    if (!bigint_is_zero(result))
        result->negative = (a->negative != b->negative);

    normalize_zero(result);
    return result;
}

BigInt *bigint_div(const BigInt *a, const BigInt *b)
{
    if (bigint_is_zero(b))
        return NULL; /* division by zero */

    /* |a| < |b| → quotient is 0 */
    if (bigint_cmp_abs(a, b) < 0)
    {
        BigInt *zero = bigint_create();
        bigint_append_tail(zero, 0);
        return zero;
    }

    /*
     * Long division: walk digits of |a| left-to-right.
     * remainder accumulates one digit at a time; we count
     * how many times |b| fits (always 0-9).
     */
    BigInt *result    = bigint_create();
    BigInt *remainder = bigint_create();
    bigint_append_tail(remainder, 0);   /* start with remainder = 0 */

    for (Node *cur = a->head; cur; cur = cur->next)
    {
        /* remainder = remainder * 10 + cur->digit */
        /* Step 1: append new digit to tail */
        bigint_append_tail(remainder, cur->digit);
        /* Step 2: trim any leading zero (but keep at least one node) */
        bigint_trim(remainder);

        /* Count how many times |b| fits into remainder */
        int q = 0;
        while (bigint_cmp_abs(remainder, b) >= 0)
        {
            BigInt *tmp = sub_abs(remainder, b);
            bigint_free(remainder);
            remainder = tmp;
            q++;
        }
        bigint_append_tail(result, q);
    }

    bigint_trim(result);

    if (!bigint_is_zero(result))
        result->negative = (a->negative != b->negative);

    normalize_zero(result);
    bigint_free(remainder);
    return result;
}