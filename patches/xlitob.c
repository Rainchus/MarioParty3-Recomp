#include "patches.h"
#include "string.h"
#include "xstdio.h"

typedef struct lldiv_t
{
    long long quot;
    long long rem;
} lldiv_t;

extern long long __divdi3_recomp(long long num, long long denom);
extern unsigned long long __udivdi3_recomp(unsigned long long num, unsigned long long denom);
extern unsigned long long __umoddi3_recomp(unsigned long long num, unsigned long long denom);

__attribute__((noinline))
lldiv_t lldiv(long long num, long long denom) {
    lldiv_t ret;

    ret.quot = __divdi3_recomp(num, denom);
    ret.rem = num - denom * ret.quot;

    if (ret.quot < 0 && ret.rem > 0) {
        ret.quot += 1;
        ret.rem -= denom;
    }

    return ret;
}

#define BUFF_LEN 0x18

static char ldigs[] = "0123456789abcdef";
static char udigs[] = "0123456789ABCDEF";

void _Litob(_Pft *px, char code) {
    char buff[BUFF_LEN];
    const char *digs;
    int base;
    int i;
    unsigned long long ullval;

    digs = (code == 'X') ? udigs : ldigs;

    base = (code == 'o') ? 8 : ((code != 'x' && code != 'X') ? 10 : 16);
    i = BUFF_LEN;
    ullval = px->v.ll;

    if ((code == 'd' || code == 'i') && px->v.ll < 0) {
        ullval = -ullval;
    }

    if (ullval != 0 || px->prec != 0) {
        buff[--i] = digs[__umoddi3_recomp(ullval, base)];
    }

    px->v.ll = (long long)__udivdi3_recomp(ullval, base);

    while (px->v.ll > 0 && i > 0) {
        lldiv_t qr = lldiv(px->v.ll, base);

        px->v.ll = qr.quot;
        buff[--i] = digs[qr.rem];
    }

    px->n1 = BUFF_LEN - i;

    // Copying characters without using memcpy
    for (int j = 0; j < px->n1; j++) {
        px->s[j] = buff[i + j];
    }

    if (px->n1 < px->prec) {
        px->nz0 = px->prec - px->n1;
    }

    if (px->prec < 0 && (px->flags & (FLAGS_ZERO | FLAGS_MINUS)) == FLAGS_ZERO) {
        if ((i = px->width - px->n0 - px->nz0 - px->n1) > 0) {
            px->nz0 += i;
        }
    }
}

