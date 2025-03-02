#if 1
#include "libu/libu.h"
#include "uoptdata.h"
#include "uoptutil.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "debug.h"

const char *regnames[] = {
    "$zero",
    "$at",
    "$v0", "$v1",
    "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9",
    "$k0", "$k1",
    "$gp",
    "$sp", "$fp",
    "$ra",
    "$f0", "$f1", "$f2", "$f3",
    "$f4", "$f5", "$f6", "$f7", "$f8", "$f9", "$f10", "$f11",
    "$f12", "$f13", "$f14", "$f15",
    "$f16", "$f17", "$f18", "$f19",
    "$f20", "$f21", "$f22", "$f23", "$f24", "$f25",
    "$f26", "$f27", "$f28", "$f29", "$f30", "$f31",
};

const char *regname_print_colors[] = {
    // temp registers
    [r_zero ... r_f31] = C_WHITE,

    // constant registers
    [r_zero ... r_at] = COLOR_RGB(255,160,160),

    // Return registers
    [r_v0] = COLOR_RGB(255,255,96),
    [r_ra] = COLOR_RGB(255,255,96),
    [r_f0 ... r_f1] = COLOR_RGB(255,255,96),

    // saved registers
    [r_s0  ... r_s7]  = COLOR_RGB(255,80,112),
    [r_f20 ... r_f31] = COLOR_RGB(255,128,255),

    // float registers
    [r_f2 ... r_f19] = COLOR_RGB(64, 255, 255),

    // arg registers
    [r_a0  ... r_a3]  = COLOR_RGB(96,255,96),
    [r_f12 ... r_f15] = COLOR_RGB(96,255,96),

    // stack registers
    [r_sp] = COLOR_RGB(32,184,220),
    [r_fp] = COLOR_RGB(32,184,220),

    // special-purpose registers
    [r_k0 ... r_k1] = COLOR_RGB(255,165,0),
    [r_gp]          = COLOR_RGB(255,165,0),

    /* 
     r_t0, r_t1, r_t2, r_t3, r_t4, r_t5, r_t6, r_t7,
     r_s0, r_s1, r_s2, r_s3, r_s4, r_s5, r_s6, r_s7,
     r_t8, r_t9,
     r_k0, r_k1,
     r_gp,
     r_sp, r_fp, r_s8=r_fp,
     r_ra,
     r_f0, r_f1, r_f2, r_f3,
     r_f4, r_f5, r_f6, r_f7, r_f8, r_f9, r_f10, r_f11,
     r_f12, r_f13, r_f14, r_f15,
     r_f16, r_f17, r_f18, r_f19,
     r_f20, r_f21, r_f22, r_f23, r_f24,
     r_f25, r_f26, r_f27, r_f28, r_f29, r_f30, r_f31,
      */
};

const char *regname(int reg) {
    return regnames[reg];
}

const char *regname_color(int reg) {
    return regname_print_colors[reg];
}

const char *opcode_name(Uopcode opc) {
    return utab[opc].opcname;
}

const char *dtype_names[] = {
      "Adt", /* address (pointer)                 */
      "Cdt", /* pointer to readonly data          */
      "Fdt", /* C pointer to function             */
      "Gdt", /* address of label                  */
      "Hdt", /* address that only points to heap  */
      "Idt", /* integer, double word              */
      "Jdt", /* integer, single word              */
      "Kdt", /* unsigned integer, double word     */
      "Ldt", /* non-negative integer, single word */
      "Mdt", /* array or record                   */
      "Ndt", /* non-local labels                  */
      "Pdt", /* procedure, untyped                */
      "Qdt", /* real, double word                 */
      "Rdt", /* real, single word                 */
      "Sdt", /* set                               */
      "Wdt", /* 64 bit wide pointer               */
      "Xdt", /* extended precision                */
      "Zdt", /* undefined                         */
};


const char *dtype_name(enum Datatype type) {
    switch (type) {
        case Idt: return "s64";       break;
        case Jdt: return "s32";       break;
        case Kdt: return "u64";       break;
        case Ldt: return "u32";       break;
        case Qdt: return "f64";       break;
        case Rdt: return "f32";       break;
        default:  return dtype_names[type]; break;
    }
}

const char *exprtype_name(ExpressionType type)
{
    switch (type) {
        case empty:    return "empty";
        case islda:    return "islda";
        case isconst:  return "isconst";
        case isvar:    return "isvar";
        case isop:     return "isop";
        case isilda:   return "isilda";
        case issvar:   return "issvar";
        case dumped:   return "dumped";
        case isrconst: return "isrconst";
        default:       return "unknown";
    }
}

void print_small_dtype(enum Datatype type, int length) {
    char dtypeSign;
    switch (type) {
        case Idt:
        case Jdt: dtypeSign = 's'; break;
        case Kdt:
        case Ldt: dtypeSign = 'u'; break;

        default: printf("%s", dtype_name(type)); return;
    }

    printf("%c%d", dtypeSign, length);
}

// if a node has higher precedence than its parent, surround with parens
int opc_precedence[Uirsv + 1] = {
    [Uilod] = 2,
    [Ulda] = 2,
    [Ucvt] = 2,
    [Ucvtl] = 2,
    [Unot] = 2,
    [Ulnot] = 2,
    [Uneg] = 2,

    [Udiv] = 3,
    [Urem] = 3, // sometimes generated by loop unrolling
    [Umod] = 3,
    [Umpy] = 3,

    [Uadd] = 4,
    [Usub] = 4,

    [Ushl] = 5,
    [Ushr] = 5,

    [Ules] = 6,
    [Uleq] = 6,
    [Ugrt] = 6,
    [Ugeq] = 6,

    [Uequ] = 7,
    [Uneq] = 7,

    // trap operators have the same precedence as comparisons
    [Utplt] = 6,
    [Utple] = 6,
    [Utpgt] = 6,
    [Utpge] = 6,

    [Utpeq] = 7,
    [Utpne] = 7,

    [Uand] = 8,
    [Uxor] = 9,
    [Uior] = 10,
};

void print_regset64(const char *name, long long set) {
    printf("%s: ", name);
    bool firstPrint = true;
    for (int reg = 1; reg <= 35; reg++) {
        if (SET64_IN(set, reg)) {
            int color = coloroffset(reg);
            if (firstPrint) {
                printf("%s%s", regname_color(color), regname(color));
                firstPrint = false;
            } else {
                printf(" %s%s", regname_color(color), regname(color));
            }
        }
    }
    printf("%s", C_RESET);
}

int higher_precedence_expr(Uopcode opc, struct Expression *op) {
    if (op == NULL) return false;
    switch (op->type) {
        default:
            return false;
        case islda:
            return opc_precedence[Ulda] >= opc_precedence[opc];
        case isop:
            if ((opc == Ucvt || opc == Ucvtl) && (op->data.isop.opc == Ucvt || op->data.isop.opc == Ucvtl)) {
                return false;
            } else {
                return opc_precedence[op->data.isop.opc] != 0 && opc_precedence[op->data.isop.opc] >= opc_precedence[opc];
            }
    }
}

int higher_precedence_image(Uopcode opc, struct IChain *op) {
    if (op == NULL) return false;
    switch (op->type) {
        default:
            return false;
        case islda:
            return opc_precedence[Ulda] >= opc_precedence[opc];
        case isop:
            if ((opc == Ucvt || opc == Ucvtl) && (op->isop.opc == Ucvt || op->isop.opc == Ucvtl)) {
                return false;
            } else {
                return opc_precedence[op->isop.opc] != 0 && opc_precedence[op->isop.opc] >= opc_precedence[opc];
            }
    }
}

void print_opcode(Uopcode opc) {
    fprintf(stdout, "%.4s ", utab[opc].opcname);
}

const char *opc_operator(Uopcode opc) {
    switch (opc) {
        case Udiv:  return "/";
        case Umod:  return "%";
        case Urem:  return "%%"; // maybe there's a better symbol for this ("rem"?)
        case Umpy:  return "*";

        case Uadd:  return "+";
        case Usub:  return "-";

        case Ushr:  return ">>";
        case Ushl:  return "<<";

        case Ules:  return "<";
        case Uleq:  return "<=";
        case Ugrt:  return ">";
        case Ugeq:  return ">=";

        case Uequ:  return "==";
        case Uneq:  return "!=";

        case Uand:  return "&";
        case Uxor:  return "^";
        case Uior:  return "|";

        // unary operators
        case Uilod: return "*";
        case Uistr: return "*";
        case Ulda:  return "&";
        case Unot:  return "~";
        case Ulnot: return "!";
        case Uneg:  return "-";

        // trap operators
        case Utplt: return "<";
        case Utple: return "<=";
        case Utpgt: return ">";
        case Utpge: return ">=";

        case Utpeq: return "==";
        case Utpne: return "!=";

        default:    return opcode_name(opc);
    }
}

void print_variable(struct VariableLocation loc) {
    switch (loc.memtype) {
        case Mmt:
            fprintf(stdout, "-0x%x($sp)", -loc.addr);
            break;

        case Rmt:
            fprintf(stdout, "%s", regnames[loc.addr]);
            break;

        case Pmt:
            fprintf(stdout, "arg%d", loc.addr / 4);
            break;

        default:
            fprintf(stdout, "var(%cmt, %d, %d)", getmtyname(loc.memtype), loc.addr, loc.blockno);
            break;
    }
}

void print_constant(Datatype dtype, union Constant constval) {
    struct RealstoreData *real;
    switch (dtype) {
        case Adt:
        case Fdt:
        case Gdt:
        case Hdt:
        case Jdt:
        case Ldt:
        case Ndt:
            fprintf(stdout, "%d", constval.intval);
            break;

        case Idt:
        case Kdt:
            fprintf(stdout, "%lld", constval.longval);
            break;

        default:
            real = realstore;
            for (int i = 0; i < (constval.real.disp >> 8); i++) {
                real = real->next;
            }
            fprintf(stdout, "%.*s", constval.real.len, &real->c[constval.real.disp & 0xff]);
            if (dtype == Rdt) {
                fprintf(stdout, "f");
            }
            break;
    }
}

void print_image(struct IChain *image) {
    if (image == NULL) return;
    switch (image->type) {
        case isconst:
            print_constant(image->dtype, image->isconst.number);
            break;

        case isvar:
        case issvar:
            print_variable(image->isvar_issvar.location);
            break;

        case islda:
        case isilda:
            fprintf(stdout, "&");
            print_variable(image->islda_isilda.address);
            break;

        case isop:
            switch (image->isop.opc) {
                case Uadd:
                case Usub:
                case Umpy:
                case Udiv:
                case Umod:
                case Uequ:
                case Uneq:
                case Ules:
                case Uleq:
                case Ugrt:
                case Ugeq:
                case Uand:
                case Uior:
                case Uxor:
                case Ushr:
                case Ushl:
                    if (higher_precedence_image(opc_precedence[image->isop.opc], image->isop.op1)) {
                        fprintf(stdout, "(");
                        print_image(image->isop.op1);
                        fprintf(stdout, ")");
                    } else {
                        print_image(image->isop.op1);
                    }
                    printf(" %s ", opc_operator(image->isop.opc));
                    if (higher_precedence_image(opc_precedence[image->isop.opc], image->isop.op2)) {
                        fprintf(stdout, "(");
                        print_image(image->isop.op2);
                        fprintf(stdout, ")");
                    } else {
                        print_image(image->isop.op2);
                    }
                    break;

                case Ulda:
                case Unot:
                case Ulnot:
                case Uneg:
                    printf("%s", opc_operator(image->isop.opc));
                    if (higher_precedence_image(opc_precedence[image->isop.opc], image->isop.op1))
                    {
                        printf("(");
                        print_image(image->isop.op1);
                        printf(")");
                    } else {
                        print_image(image->isop.op1);
                    }
                    break;

                case Uilod:
                    {
                        bool closeInnerParens = false;
                        bool closeParens = false;
                        bool hasOffset = image->isop.size != 0;

                        // output: *((e1 + e2) + offset)
                        printf("%s", opc_operator(image->isop.opc));
                        if (hasOffset || higher_precedence_image(opc_precedence[image->isop.opc], image->isop.op1)) {
                            closeParens = true;
                            fprintf(stdout, "(");
                        }
                        if (hasOffset && higher_precedence_image(opc_precedence[Uadd], image->isop.op1)) {
                            closeInnerParens = true;
                            fprintf(stdout, "(");
                        }
                        
                        print_image(image->isop.op1);

                        if (closeInnerParens) {
                            fprintf(stdout, ")");
                        }
                        if (hasOffset) {
                            printf(" + %d", image->isop.size);
                        }
                        if (closeParens) {
                            fprintf(stdout, ")");
                        }
                    }
                    break;


                case Uixa:
                    print_image(image->isop.op1);
                    fprintf(stdout, "[");
                    print_image(image->isop.op2);
                    fprintf(stdout, "]");
                    break;

                case Ucvtl:
                    fprintf(stdout, "(");
                    print_small_dtype(image->dtype, image->isop.size);
                    fprintf(stdout, ")");

                    if (higher_precedence_image(opc_precedence[image->isop.opc], image->isop.op1)) {
                        printf("(");
                        print_image(image->isop.op1);
                        printf(")");
                    } else {
                        print_image(image->isop.op1);
                    }
                    break;

                case Ucvt:
                    fprintf(stdout, "(%s)", dtype_name(image->isop.datatype));
                    if (higher_precedence_image(opc_precedence[image->isop.opc], image->isop.op1)) {
                        printf("(");
                        print_image(image->isop.op1);
                        printf(")");
                    } else {
                        print_image(image->isop.op1);
                    }
                    break;

                default:
                    print_opcode(image->isop.opc);
                    print_image(image->isop.op1);
                    if (optab[image->isop.opc].is_binary_op) {
                        fprintf(stdout, ", ");
                        print_image(image->isop.op2);
                    }
                    break;
            }
            break;

        default:
            fprintf(stdout, "unhandled image type");
            break;
    }
}

void print_expr(struct Expression *expr)
{
    if (expr == NULL) return;
    switch (expr->type)
    {
        case isconst:
            print_constant(expr->datatype, expr->data.isconst.number);
            break;

        case isvar:
        case issvar:
            print_variable(expr->data.isvar_issvar.location);
            break;

        case islda:
        case isilda:
            fprintf(stdout, "&");
            print_variable(expr->data.islda_isilda.address);
            break;

        case isop:
            switch (expr->data.isop.opc) {
                case Uadd:
                case Usub:
                case Umpy:
                case Udiv:
                case Umod:
                case Uequ:
                case Uneq:
                case Ules:
                case Uleq:
                case Ugrt:
                case Ugeq:
                case Uand:
                case Uior:
                case Uxor:
                case Ushr:
                case Ushl:
                    if (higher_precedence_expr(opc_precedence[expr->data.isop.opc], expr->data.isop.op1)) {
                        fprintf(stdout, "(");
                        print_expr(expr->data.isop.op1);
                        fprintf(stdout, ")");
                    } else {
                        print_expr(expr->data.isop.op1);
                    }
                    printf(" %s ", opc_operator(expr->data.isop.opc));
                    if (higher_precedence_expr(opc_precedence[expr->data.isop.opc], expr->data.isop.op2)) {
                        fprintf(stdout, "(");
                        print_expr(expr->data.isop.op2);
                        fprintf(stdout, ")");
                    } else {
                        print_expr(expr->data.isop.op2);
                    }
                    break;

                case Ulda:
                case Unot:
                case Ulnot:
                case Uneg:
                    printf("%s", opc_operator(expr->data.isop.opc));
                    if (higher_precedence_expr(opc_precedence[expr->data.isop.opc], expr->data.isop.op1))
                    {
                        printf("(");
                        print_expr(expr->data.isop.op1);
                        printf(")");
                    } else {
                        print_expr(expr->data.isop.op1);
                    }
                    break;

                case Uilod:
                    {
                        bool closeInnerParens = false;
                        bool closeParens = false;
                        bool hasOffset = expr->data.isop.datasize != 0;

                        // output: *((e1 + e2) + offset)
                        printf("%s", opc_operator(expr->data.isop.opc));
                        if (hasOffset || higher_precedence_expr(opc_precedence[expr->data.isop.opc], expr->data.isop.op1)) {
                            closeParens = true;
                            fprintf(stdout, "(");
                        }
                        if (hasOffset && higher_precedence_expr(opc_precedence[Uadd], expr->data.isop.op1)) {
                            closeInnerParens = true;
                            fprintf(stdout, "(");
                        }
                        
                        print_expr(expr->data.isop.op1);

                        if (closeInnerParens) {
                            fprintf(stdout, ")");
                        }
                        if (hasOffset) {
                            printf(" + %d", expr->data.isop.datasize);
                        }
                        if (closeParens) {
                            fprintf(stdout, ")");
                        }
                    }
                    break;


                case Uixa:
                    print_expr(expr->data.isop.op1);
                    fprintf(stdout, "[");
                    print_expr(expr->data.isop.op2);
                    fprintf(stdout, "]");
                    break;

                case Ucvtl:
                    fprintf(stdout, "(");
                    print_small_dtype(expr->datatype, expr->data.isop.datasize);
                    fprintf(stdout, ")");

                    if (higher_precedence_expr(opc_precedence[expr->data.isop.opc], expr->data.isop.op1)) {
                        printf("(");
                        print_expr(expr->data.isop.op1);
                        printf(")");
                    } else {
                        print_expr(expr->data.isop.op1);
                    }
                    break;

                case Ucvt:
                    fprintf(stdout, "(%s)", dtype_name(expr->data.isop.datatype));
                    if (higher_precedence_expr(opc_precedence[expr->data.isop.opc], expr->data.isop.op1)) {
                        printf("(");
                        print_expr(expr->data.isop.op1);
                        printf(")");
                    } else {
                        print_expr(expr->data.isop.op1);
                    }
                    break;

                default:
                    print_opcode(expr->data.isop.opc);
                    print_expr(expr->data.isop.op1);
                    if (optab[expr->data.isop.opc].is_binary_op) {
                        fprintf(stdout, ", ");
                        print_expr(expr->data.isop.op2);
                    }
                    break;
            }
            break;

        default:
            fprintf(stderr, "unhandled expr type");
            break;
    }
}

void print_assignment(struct Statement *stat)
{
    if (stat->suppressed_iv == 1)
        fprintf(stderr, "\e[95m");
    if (stat->expr->type == isvar || stat->expr->type == issvar)
    {
        if (stat->expr->data.isvar_issvar.assigned_value != NULL) {
            print_expr(stat->expr->data.isvar_issvar.assigned_value);
            fprintf(stderr, ", ");
        }
        print_expr(stat->expr);
    }
}

void print_other_store(struct Statement *stat)
{
    if (stat->u.store.expr != NULL) {
        print_expr(stat->u.store.expr);
        fprintf(stderr, ", ");
    }
    print_expr(stat->expr);
}

int *image_statement_map;
const char **highlight;

void init_image_stmt_map(struct Graphnode *node) {
    image_statement_map = malloc(bittabsize * sizeof(struct Statement *));
    memset(image_statement_map, -1, bittabsize * sizeof(struct Statement *));
    struct IChain *image;

    int i = 0;
    for (struct Statement *stat = node->stat_head; stat != NULL; stat = stat->next) {
        switch (stat->opc) {
                case Unop:
                case Uujp:
                case Ulab:
                case Uclab:
                case Umst:
                case Ubgnb:
                case Uendb:
                case Uret:
                case Uloc:
                case Udef:
                case Ucup:
                case Uicuf:
                case Urcuf:
                case Ucia:
                case Uaent:
                case Ulbgn:
                case Ulend:
                case Ultrm:
                case Ulbdy:
                case Uclbd:
                case Ucubd:
                case Ustep:
                case Uctrl:
                    break;

                case Uchkt:
                case Uistr:
                case Uistv:
                case  Umov:
                case Umovv:
                case Utpeq:
                case Utpge:
                case Utpgt:
                case Utple:
                case Utplt:
                case Utpne:
                case Uirst:
                case Uirsv:
                case Uisst:
                case  Ustr:
                    image = stat->u.store.ichain;
                    if (image != NULL) {
                        if (image_statement_map[image->bitpos] != -1) {
                            //fprintf(stderr, "image already mapped!\n");
                        }
                        image_statement_map[image->bitpos] = i;
                    }
                    image = stat->expr->ichain;
                    if (image != NULL) {
                        if (image_statement_map[image->bitpos] != -1) {
                            //fprintf(stderr, "image already mapped!\n");
                        }
                        image_statement_map[image->bitpos] = i;
                    }
                        break;

                default:
                    image = stat->expr->ichain;
                    if (stat->expr->ichain != NULL) {
                        if (image_statement_map[image->bitpos] != -1) {
                            //fprintf(stderr, "image already mapped!\n");
                        }
                        image_statement_map[image->bitpos] = i;
                    }
                    break;
        }

        i++;
        if (stat->next != NULL && stat->next->graphnode != stat->graphnode) {
            break;
        }
    }

    highlight = calloc(i, sizeof(const char *));
}

void print_statement(struct Statement *stat) {
    if (stat == NULL) return;

    print_opcode(stat->opc);
    switch (stat->opc) {
        case Unop:
        case Uclab:
        case Umst:
        case Ubgnb:
        case Uendb:
        case Uret:
        case Udef:
        case Ucia:
        case Uaent:
        case Ulbgn:
        case Ulend:
        case Ultrm:
        case Ulbdy:
        case Uclbd:
        case Ucubd:
        case Ustep:
        case Uctrl:
            break;

        case Ucup:
        case Uicuf:
        case Urcuf:
            break;

        case Uloc:
            fprintf(stderr, "%d", stat->u.loc.line);
            break;

        case Uujp:
            fprintf(stderr, "%d", stat->u.jp.target_blockno);
            break;

        case Ufjp:
        case Utjp:
            print_expr(stat->expr);
            fprintf(stderr, " %d", stat->u.jp.target_blockno);
            break;

        case Ulab:
            fprintf(stderr, "%d", stat->u.label.blockno);
            break;

        case Uisst:
        case  Ustr:
            print_assignment(stat);
            break;

        case Uchkt:
        case Uistr:
        case Uistv:
        case  Umov:
        case Umovv:
        case Utpeq:
        case Utpge:
        case Utpgt:
        case Utple:
        case Utplt:
        case Utpne:
        case Uirst:
        case Uirsv:
            print_other_store(stat);
            break;

            // unknown, but expr != NULL
        default:
            print_expr(stat->expr);
            break;
    }
    fprintf(stderr,"\e[m\n");
}
void print_node_statements(struct Graphnode *node)
{
    struct Statement *stat;
    int i;

    //for (i = 0; i < bitposcount; i++)
    //{
    //    if (bittab[i].ichain != NULL)
    //    {
    //        if (image_statement_map[i] != -1) {
    //            if (bvectin0(bittab[i].ichain->bitpos, &node->bvs.stage1.avlocs)) {
    //                highlight[image_statement_map[i]] = "\e[92m";
    //            } else if (bvectin0(bittab[i].ichain->bitpos, &node->bvs.stage1.antlocs)) {
    //                highlight[image_statement_map[i]] = "\e[94m";
    //            }
    //            /* 
    //            else if (bvectin0(bittab[i].ichain->bitpos, &node->bvs.stage1.alters)) {
    //                highlight[image_statement_map[i]] = "\e[91m";
    //            }
    //             */
    //        }

    //    }
    //}

    fprintf(stderr, "\e[94mnode %d", node->num);
    for (struct GraphnodeList *succ = node->predecessors; succ != NULL; succ = succ->next) {
        fprintf(stderr, ", %d", succ->graphnode->num);
    }
    fprintf(stderr, "\e[m\n");

    i = 0;
    for (stat = node->stat_head; stat != NULL; stat = stat->next) {
        /* 
        if ((stat->opc == Uistr ||
             stat->opc == Uistv ||
             stat->opc == Ustr  ||
             stat->opc == Uisst ||
             stat->opc == Umov  ||
             stat->opc == Umovv ||
             stat->opc == Uirst ||
             stat->opc == Uirsv)) {
            if (bvectin0(stat->u.store.ichain->bitpos, &node->bvs.stage1.u.precm.pavlocs))
            {
                fprintf(stderr, "\e[92m");
            }
        }
         */
        /* 
        if (highlight[i] != NULL) {
            fprintf(stderr, "%s", highlight[i]);
        }
         */
        /* 
        switch (stat->opc)
        {
            case Uisst:
            case  Ustr:
            case Uchkt:
            case Uistr:
            case Uistv:
            case  Umov:
            case Umovv:
            case Utpeq:
            case Utpge:
            case Utpgt:
            case Utple:
            case Utplt:
            case Utpne:
            case Uirst:
            case Uirsv:
                if (stat->u.store.unk1F) {
                    fprintf(stderr, "\e[95m");
                }

        }
         */
        print_statement(stat);

        i++;
        if (stat->next != NULL && stat->next->graphnode != stat->graphnode) {
            break;
        }
    }

    //free(highlight);
    //free(image_statement_map);
}

void print_function_statements(const char *desc) {
    fprintf(stderr, "%s", desc);
    struct Graphnode *node = graphhead;

    while (node != NULL)
    {
        print_node_statements(node);
        node = node->next;
    }

}

void print_bitvector(struct BitVector *bv) {
    int blockpos;
    int word = 0;
    int i;
    bool at_first = true;

    printf("(%d) [", bv->num_blocks);

    for (blockpos = 0; blockpos < bv->num_blocks && blockpos <= (bitposcount - 1) >> 7; blockpos++) {
        for (i = word; i < word + 128; i++) {
            if (bv->blocks[blockpos].words[(i & 0x7f) >> 5] & (1U << (31 - (i & 0x1f)))) {
                    if (at_first) {
                        printf("%d", i);
                        at_first = false;
                    } else {
                        printf(", %d", i);
                    }
            }
        }
        word += 128;
    }
    /* 
    if (inside_range && !at_first_element_in_range) {
        printf("..%4d", i - 1);
        //write_string(stdout, "..", 2, 2);
        //write_integer(stdout, i - 1, 4, 10);
    }
     */
    puts("]");
}

#endif
