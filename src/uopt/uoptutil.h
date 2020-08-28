#ifndef UOPTUTIL_H
#define UOPTUTIL_H

#include "libxmalloc/xmalloc.h"
#include "uoptdata.h"

void *alloc_realloc(void *old_ptr, ssize_t old_size_16_byte_blocks, ssize_t new_size_16_byte_blocks, struct AllocBlock **heap);

bool addreq(struct VariableInner a, struct VariableInner b);
void stackerror(void);
void dbgerror(int code);
void decreasecount(struct Expression *expr);
void increasecount(struct Expression *expr);
int realhash(int len);
int isvarhash(struct VariableInner var);
int blktolev(int blk);
struct Expression *searchvar(unsigned short table_index, struct VariableInner *var);
void getoption(int uopt_option, int n);
void extendstat(Uopcode opc);
void entervregveqv(void);
bool has_ilod(struct Expression *expr);
void skipproc(int reason);
int newbit(struct IChain *ichain, void *unk4);
bool in_indmults(struct IChain *ichain);

#endif
