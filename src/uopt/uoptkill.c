#include "uoptdata.h"
#include "uoptkill.h"

__asm__(R""(
.macro glabel label
    .global \label
    .balign 4
    \label:
.endm

.rdata
RO_1000CB20:
    # 004494A4 aliaswithptr
    .asciz "uoptkill.p"

    .balign 4
jtbl_1000CB2C:
    # 004494A4 aliaswithptr
    .gpword .L00449524
    .gpword .L00449590
    .gpword .L00449514
    .gpword .L00449514
    .gpword .L00449514
    .gpword .L0044951C
    .gpword .L00449514

RO_1000CB48:
    # 0044B4F4 clkilled
    .asciz "uoptkill.p"

    .balign 4
jtbl_1000CB54:
    # 0044B4F4 clkilled
    .gpword .L0044B984
    .gpword .L0044B778
    .gpword .L0044B698
    .gpword .L0044B888
    .gpword .L0044B5C4
    .gpword .L0044B744
    .gpword .L0044B698

RO_1000CB70:
    # 0044BDFC cskilled
    .asciz "uoptkill.p"

    .balign 4
jtbl_1000CB7C:
    # 0044BDFC cskilled
    .gpword .L0044C254
    .gpword .L0044C094
    .gpword .L0044BFBC
    .gpword .L0044C198
    .gpword .L0044BEF0
    .gpword .L0044C060
    .gpword .L0044BFBC

.data
D_10010A50:
    # 004490B0 findbaseaddr
    .byte 0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04

D_10010A60:
    # 004490B0 findbaseaddr
    .byte 0x00,0x00,0x00,0x20,0x00,0x00,0x01,0x00,0x40,0x00,0x00,0x00

D_10010A6C:
    # 00449250 findbaseaddr_ada
    .byte 0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04

D_10010A7C:
    # 00449250 findbaseaddr_ada
    .byte 0x00,0x00,0x00,0xa0,0x00,0x00,0x01,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00

D_10010A90:
    # 0044962C pointtoheap
    .byte 0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00

D_10010AA0:
    # 004496F0 slkilled
    .byte 0x00,0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x20

D_10010AB0:
    # 004496F0 slkilled
    .byte 0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x10

D_10010ABC:
    # 0044A5C8 smkilled
    .byte 0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x10

D_10010AC8:
    # 0044AAD0 sskilled
    .byte 0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x10

D_10010AD4:
    # 0044BDFC cskilled
    .byte 0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x10

D_10010AE0:
    # 0044C6D4 func_0044C6D4
    .byte 0x00,0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x20

D_10010AF0:
    # 0044D0C4 func_0044D0C4
    .byte 0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x10

D_10010AFC:
    # 0044DBC4 indirectaccessed
    .byte 0x00,0x00,0x00,0x01,0x80,0x00,0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00




.set noat      # allow manual use of $at
.set noreorder # don't insert nops after branches
.text
)"");

/* 
0043CFCC readnxtinst
0046D428 oneloopblockexpr
0046E77C oneloopblockstmt
00473F04 pmov_to_mov
*/
struct Expression *findbaseaddr(struct Expression *expr) {
    while (expr->type == isop) {
        if (expr->data.isop.opc == Udec || expr->data.isop.opc == Uinc || expr->data.isop.opc == Uixa) {
            expr = expr->data.isop.op1;
        } else if (expr->data.isop.opc == Uadd || expr->data.isop.opc == Usub) {
            if ((expr->data.isop.op1->type == islda || expr->data.isop.op1->type == isilda) || 
                    ((expr->data.isop.op1->type == isvar || expr->data.isop.op1->type == issvar) && (expr->data.isop.op1->datatype == Adt || expr->data.isop.op1->datatype == Hdt)) ||
                    (expr->data.isop.op1->type == isop && (expr->data.isop.op1->data.isop.datatype == Adt || expr->data.isop.op1->data.isop.datatype == Hdt))) {
                expr = expr->data.isop.op1;
            } else if ((expr->data.isop.op2->type == islda || expr->data.isop.op2->type == isilda) || 
                    ((expr->data.isop.op2->type == isvar || expr->data.isop.op2->type == issvar) && (expr->data.isop.op2->datatype == Adt || expr->data.isop.op2->datatype == Hdt)) ||
                    (expr->data.isop.op2->type == isop && (expr->data.isop.op2->data.isop.datatype == Adt || expr->data.isop.op2->data.isop.datatype == Hdt))) {
                expr = expr->data.isop.op2;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    return expr;
}
__asm__(R""(
.set noat
.set noreorder

glabel findbaseaddr_ada
    .ent findbaseaddr_ada
    # 0043CFCC readnxtinst
    # 00449250 findbaseaddr_ada
/* 00449250 3C1C0FBD */  .cpload $t9
/* 00449254 279C1040 */  
/* 00449258 0399E021 */  
/* 0044925C 27BDFFD8 */  addiu $sp, $sp, -0x28
/* 00449260 AFB1001C */  sw    $s1, 0x1c($sp)
/* 00449264 AFB00018 */  sw    $s0, 0x18($sp)
/* 00449268 00808025 */  move  $s0, $a0
/* 0044926C 24110004 */  li    $s1, 4
/* 00449270 AFBF0024 */  sw    $ra, 0x24($sp)
/* 00449274 AFBC0020 */  sw    $gp, 0x20($sp)
.L00449278:
/* 00449278 920E0001 */  lbu   $t6, 1($s0)
/* 0044927C 3C018880 */  lui   $at, 0x8880
/* 00449280 2DCF0020 */  sltiu $t7, $t6, 0x20
/* 00449284 000FC023 */  negu  $t8, $t7
/* 00449288 0301C824 */  and   $t9, $t8, $at
/* 0044928C 01D94004 */  sllv  $t0, $t9, $t6
/* 00449290 05020004 */  bltzl $t0, .L004492A4
/* 00449294 92020000 */   lbu   $v0, ($s0)
/* 00449298 10000036 */  b     .L00449374
/* 0044929C 00001825 */   move  $v1, $zero
/* 004492A0 92020000 */  lbu   $v0, ($s0)
.L004492A4:
/* 004492A4 3C014400 */  lui   $at, 0x4400
/* 004492A8 2C490020 */  sltiu $t1, $v0, 0x20
/* 004492AC 00095023 */  negu  $t2, $t1
/* 004492B0 01415824 */  and   $t3, $t2, $at
/* 004492B4 004B6004 */  sllv  $t4, $t3, $v0
/* 004492B8 05810003 */  bgez  $t4, .L004492C8
/* 004492BC 00000000 */   nop   
/* 004492C0 1000002C */  b     .L00449374
/* 004492C4 02001825 */   move  $v1, $s0
.L004492C8:
/* 004492C8 5622002A */  bnel  $s1, $v0, .L00449374
/* 004492CC 00001825 */   move  $v1, $zero
/* 004492D0 92020020 */  lbu   $v0, 0x20($s0)
/* 004492D4 2C4D00A0 */  sltiu $t5, $v0, 0xa0
/* 004492D8 11A00009 */  beqz  $t5, .L00449300
/* 004492DC 2C4B0080 */   sltiu $t3, $v0, 0x80
/* 004492E0 8F998044 */  lw    $t9, %got(D_10010A7C)($gp)
/* 004492E4 00027943 */  sra   $t7, $v0, 5
/* 004492E8 000FC080 */  sll   $t8, $t7, 2
/* 004492EC 27390A7C */  addiu $t9, %lo(D_10010A7C) # addiu $t9, $t9, 0xa7c
/* 004492F0 03387021 */  addu  $t6, $t9, $t8
/* 004492F4 8DC80000 */  lw    $t0, ($t6)
/* 004492F8 00484804 */  sllv  $t1, $t0, $v0
/* 004492FC 292D0000 */  slti  $t5, $t1, 0
.L00449300:
/* 00449300 11A00003 */  beqz  $t5, .L00449310
/* 00449304 00000000 */   nop   
/* 00449308 1000FFDB */  b     .L00449278
/* 0044930C 8E100024 */   lw    $s0, 0x24($s0)
.L00449310:
/* 00449310 11600009 */  beqz  $t3, .L00449338
/* 00449314 00000000 */   nop   
/* 00449318 8F998044 */  lw    $t9, %got(D_10010A6C)($gp)
/* 0044931C 00026143 */  sra   $t4, $v0, 5
/* 00449320 000C7880 */  sll   $t7, $t4, 2
/* 00449324 27390A6C */  addiu $t9, %lo(D_10010A6C) # addiu $t9, $t9, 0xa6c
/* 00449328 032FC021 */  addu  $t8, $t9, $t7
/* 0044932C 8F0E0000 */  lw    $t6, ($t8)
/* 00449330 004E4004 */  sllv  $t0, $t6, $v0
/* 00449334 290B0000 */  slti  $t3, $t0, 0
.L00449338:
/* 00449338 1160000B */  beqz  $t3, .L00449368
/* 0044933C 00000000 */   nop   
/* 00449340 8F99837C */  lw    $t9, %call16(findbaseaddr_ada)($gp)
/* 00449344 8E040024 */  lw    $a0, 0x24($s0)
/* 00449348 0320F809 */  jalr  $t9
/* 0044934C 00000000 */   nop   
/* 00449350 10400003 */  beqz  $v0, .L00449360
/* 00449354 8FBC0020 */   lw    $gp, 0x20($sp)
/* 00449358 10000006 */  b     .L00449374
/* 0044935C 00401825 */   move  $v1, $v0
.L00449360:
/* 00449360 1000FFC5 */  b     .L00449278
/* 00449364 8E100028 */   lw    $s0, 0x28($s0)
.L00449368:
/* 00449368 10000002 */  b     .L00449374
/* 0044936C 00001825 */   move  $v1, $zero
/* 00449370 00001825 */  move  $v1, $zero
.L00449374:
/* 00449374 8FBF0024 */  lw    $ra, 0x24($sp)
/* 00449378 8FB00018 */  lw    $s0, 0x18($sp)
/* 0044937C 8FB1001C */  lw    $s1, 0x1c($sp)
/* 00449380 27BD0028 */  addiu $sp, $sp, 0x28
/* 00449384 03E00008 */  jr    $ra
/* 00449388 00601025 */   move  $v0, $v1
    .type findbaseaddr_ada, @function
    .size findbaseaddr_ada, .-findbaseaddr_ada
    .end findbaseaddr_ada

glabel overlapping
    .ent overlapping
    # 0041F22C base_noalias
    # 004496F0 slkilled
    # 0044A5C8 smkilled
    # 0044AAD0 sskilled
    # 0044C6D4 func_0044C6D4
    # 0044CE80 func_0044CE80
    # 0044D0C4 func_0044D0C4
    # 0044DBC4 indirectaccessed
    # 00458ED0 compareloc
    # 00478FA0 func_00478FA0
/* 0044938C 3C1C0FBD */  .cpload $t9
/* 00449390 279C0F04 */  
/* 00449394 0399E021 */  
/* 00449398 8F8E8A5C */  lw     $t6, %got(lang)($gp)
/* 0044939C 24010003 */  li    $at, 3
/* 004493A0 AFA40000 */  sw    $a0, ($sp)
/* 004493A4 91CE0000 */  lbu   $t6, ($t6)
/* 004493A8 AFA50004 */  sw    $a1, 4($sp)
/* 004493AC AFA60008 */  sw    $a2, 8($sp)
/* 004493B0 15C1001B */  bne   $t6, $at, .L00449420
/* 004493B4 AFA7000C */   sw    $a3, 0xc($sp)
/* 004493B8 8FAF0010 */  lw    $t7, 0x10($sp)
/* 004493BC 8FB80014 */  lw    $t8, 0x14($sp)
/* 004493C0 59E00004 */  blezl $t7, .L004493D4
/* 004493C4 93B90006 */   lbu   $t9, 6($sp)
/* 004493C8 5F000016 */  bgtzl $t8, .L00449424
/* 004493CC 93AB000E */   lbu   $t3, 0xe($sp)
/* 004493D0 93B90006 */  lbu   $t9, 6($sp)
.L004493D4:
/* 004493D4 24030001 */  li    $v1, 1
/* 004493D8 8FA20000 */  lw    $v0, ($sp)
/* 004493DC 33280007 */  andi  $t0, $t9, 7
/* 004493E0 14680003 */  bne   $v1, $t0, .L004493F0
/* 004493E4 2401FFFC */   li    $at, -4
/* 004493E8 10410009 */  beq   $v0, $at, .L00449410
/* 004493EC 00000000 */   nop   
.L004493F0:
/* 004493F0 93A9000E */  lbu   $t1, 0xe($sp)
/* 004493F4 312A0007 */  andi  $t2, $t1, 7
/* 004493F8 146A0007 */  bne   $v1, $t2, .L00449418
/* 004493FC 00000000 */   nop   
/* 00449400 8FA30008 */  lw    $v1, 8($sp)
/* 00449404 2401FFFC */  li    $at, -4
/* 00449408 14610003 */  bne   $v1, $at, .L00449418
/* 0044940C 00000000 */   nop   
.L00449410:
/* 00449410 03E00008 */  jr    $ra
/* 00449414 00001025 */   move  $v0, $zero

.L00449418:
/* 00449418 03E00008 */  jr    $ra
/* 0044941C 24020001 */   li    $v0, 1

.L00449420:
/* 00449420 93AB000E */  lbu   $t3, 0xe($sp)
.L00449424:
/* 00449424 93AD0006 */  lbu   $t5, 6($sp)
/* 00449428 8FAF000C */  lw    $t7, 0xc($sp)
/* 0044942C 316C0007 */  andi  $t4, $t3, 7
/* 00449430 31AE0007 */  andi  $t6, $t5, 7
/* 00449434 158E0018 */  bne   $t4, $t6, .L00449498
/* 00449438 00001825 */   move  $v1, $zero
/* 0044943C 8FB90004 */  lw    $t9, 4($sp)
/* 00449440 000FC2C2 */  srl   $t8, $t7, 0xb
/* 00449444 8FA20000 */  lw    $v0, ($sp)
/* 00449448 001942C2 */  srl   $t0, $t9, 0xb
/* 0044944C 17080012 */  bne   $t8, $t0, .L00449498
/* 00449450 00000000 */   nop   
/* 00449454 8FA30008 */  lw    $v1, 8($sp)
/* 00449458 8FAA0014 */  lw    $t2, 0x14($sp)
/* 0044945C 0043282A */  slt   $a1, $v0, $v1
/* 00449460 38A40001 */  xori  $a0, $a1, 1
/* 00449464 10800003 */  beqz  $a0, .L00449474
/* 00449468 0062282A */   slt   $a1, $v1, $v0
/* 0044946C 00434823 */  subu  $t1, $v0, $v1
/* 00449470 012A202A */  slt   $a0, $t1, $t2
.L00449474:
/* 00449474 14800006 */  bnez  $a0, .L00449490
/* 00449478 00000000 */   nop   
/* 0044947C 38A40001 */  xori  $a0, $a1, 1
/* 00449480 10800003 */  beqz  $a0, .L00449490
/* 00449484 8FAD0010 */   lw    $t5, 0x10($sp)
/* 00449488 00625823 */  subu  $t3, $v1, $v0
/* 0044948C 016D202A */  slt   $a0, $t3, $t5
.L00449490:
/* 00449490 10000001 */  b     .L00449498
/* 00449494 308300FF */   andi  $v1, $a0, 0xff
.L00449498:
/* 00449498 00601025 */  move  $v0, $v1
/* 0044949C 03E00008 */  jr    $ra
/* 004494A0 00000000 */   nop   
    .type overlapping, @function
    .size overlapping, .-overlapping
    .end overlapping

glabel aliaswithptr
    .ent aliaswithptr
    # 0041F22C base_noalias
    # 004496F0 slkilled
    # 0044A5C8 smkilled
    # 0044AAD0 sskilled
    # 0044C6D4 func_0044C6D4
    # 0044CE80 func_0044CE80
    # 0044D0C4 func_0044D0C4
    # 0044DBC4 indirectaccessed
    # 00478FA0 func_00478FA0
/* 004494A4 3C1C0FBD */  .cpload $t9
/* 004494A8 279C0DEC */  
/* 004494AC 0399E021 */  
/* 004494B0 27BDFFD8 */  addiu $sp, $sp, -0x28
/* 004494B4 AFBF001C */  sw    $ra, 0x1c($sp)
/* 004494B8 AFBC0018 */  sw    $gp, 0x18($sp)
/* 004494BC 908E0006 */  lbu   $t6, 6($a0)
/* 004494C0 24010003 */  li    $at, 3
/* 004494C4 00802825 */  move  $a1, $a0
/* 004494C8 31CF0007 */  andi  $t7, $t6, 7
/* 004494CC 15E10003 */  bne   $t7, $at, .L004494DC
/* 004494D0 00000000 */   nop   
/* 004494D4 10000050 */  b     .L00449618
/* 004494D8 00001825 */   move  $v1, $zero
.L004494DC:
/* 004494DC 8F828A5C */  lw     $v0, %got(lang)($gp)
/* 004494E0 24040001 */  li    $a0, 1
/* 004494E4 2407000A */  li    $a3, 10
/* 004494E8 90420000 */  lbu   $v0, ($v0)
/* 004494EC 2C410007 */  sltiu $at, $v0, 7
/* 004494F0 10200042 */  beqz  $at, .L004495FC
/* 004494F4 00000000 */   nop   
/* 004494F8 8F818044 */  lw    $at, %got(jtbl_1000CB2C)($gp)
/* 004494FC 0002C080 */  sll   $t8, $v0, 2
/* 00449500 00380821 */  addu  $at, $at, $t8
/* 00449504 8C38CB2C */  lw    $t8, %lo(jtbl_1000CB2C)($at)
/* 00449508 031CC021 */  addu  $t8, $t8, $gp
/* 0044950C 03000008 */  jr    $t8
/* 00449510 00000000 */   nop   
.L00449514:
/* 00449514 10000040 */  b     .L00449618
/* 00449518 24030001 */   li    $v1, 1
.L0044951C:
/* 0044951C 1000003E */  b     .L00449618
/* 00449520 00001825 */   move  $v1, $zero
.L00449524:
/* 00449524 8F998AE4 */  lw     $t9, %got(nopalias)($gp)
/* 00449528 93390000 */  lbu   $t9, ($t9)
/* 0044952C 13200016 */  beqz  $t9, .L00449588
/* 00449530 00000000 */   nop   
/* 00449534 8F888AE0 */  lw     $t0, %got(nof77alias)($gp)
/* 00449538 91080000 */  lbu   $t0, ($t0)
/* 0044953C 11000003 */  beqz  $t0, .L0044954C
/* 00449540 00000000 */   nop   
/* 00449544 10000034 */  b     .L00449618
/* 00449548 00001825 */   move  $v1, $zero
.L0044954C:
/* 0044954C 8F898980 */  lw     $t1, %got(curblk)($gp)
/* 00449550 8CA40004 */  lw    $a0, 4($a1)
/* 00449554 8D290000 */  lw    $t1, ($t1)
/* 00449558 000422C2 */  srl   $a0, $a0, 0xb
/* 0044955C 15240003 */  bne   $t1, $a0, .L0044956C
/* 00449560 00000000 */   nop   
/* 00449564 1000002C */  b     .L00449618
/* 00449568 00001825 */   move  $v1, $zero
.L0044956C:
/* 0044956C 8F998494 */  lw    $t9, %call16(in_fsym)($gp)
/* 00449570 0320F809 */  jalr  $t9
/* 00449574 00000000 */   nop   
/* 00449578 2C430001 */  sltiu $v1, $v0, 1
/* 0044957C 306300FF */  andi  $v1, $v1, 0xff
/* 00449580 10000025 */  b     .L00449618
/* 00449584 8FBC0018 */   lw    $gp, 0x18($sp)
.L00449588:
/* 00449588 10000023 */  b     .L00449618
/* 0044958C 24030001 */   li    $v1, 1
.L00449590:
/* 00449590 8F8A8B44 */  lw     $t2, %got(use_c_semantics)($gp)
/* 00449594 914A0000 */  lbu   $t2, ($t2)
/* 00449598 11400003 */  beqz  $t2, .L004495A8
/* 0044959C 00000000 */   nop   
/* 004495A0 1000001D */  b     .L00449618
/* 004495A4 24030001 */   li    $v1, 1
.L004495A8:
/* 004495A8 8F8B8AE0 */  lw     $t3, %got(nof77alias)($gp)
/* 004495AC 916B0000 */  lbu   $t3, ($t3)
/* 004495B0 11600003 */  beqz  $t3, .L004495C0
/* 004495B4 00000000 */   nop   
/* 004495B8 10000017 */  b     .L00449618
/* 004495BC 00001825 */   move  $v1, $zero
.L004495C0:
/* 004495C0 8F8C8980 */  lw     $t4, %got(curblk)($gp)
/* 004495C4 8CA40004 */  lw    $a0, 4($a1)
/* 004495C8 8D8C0000 */  lw    $t4, ($t4)
/* 004495CC 000422C2 */  srl   $a0, $a0, 0xb
/* 004495D0 15840003 */  bne   $t4, $a0, .L004495E0
/* 004495D4 00000000 */   nop   
/* 004495D8 1000000F */  b     .L00449618
/* 004495DC 00001825 */   move  $v1, $zero
.L004495E0:
/* 004495E0 8F998494 */  lw    $t9, %call16(in_fsym)($gp)
/* 004495E4 0320F809 */  jalr  $t9
/* 004495E8 00000000 */   nop   
/* 004495EC 2C430001 */  sltiu $v1, $v0, 1
/* 004495F0 306300FF */  andi  $v1, $v1, 0xff
/* 004495F4 10000008 */  b     .L00449618
/* 004495F8 8FBC0018 */   lw    $gp, 0x18($sp)
.L004495FC:
/* 004495FC 8F9988A4 */  lw    $t9, %call16(caseerror)($gp)
/* 00449600 8F868044 */  lw    $a2, %got(RO_1000CB20)($gp)
/* 00449604 24050060 */  li    $a1, 96
/* 00449608 0320F809 */  jalr  $t9
/* 0044960C 24C6CB20 */   addiu $a2, %lo(RO_1000CB20) # addiu $a2, $a2, -0x34e0
/* 00449610 8FBC0018 */  lw    $gp, 0x18($sp)
/* 00449614 93A30027 */  lbu   $v1, 0x27($sp)
.L00449618:
/* 00449618 8FBF001C */  lw    $ra, 0x1c($sp)
/* 0044961C 27BD0028 */  addiu $sp, $sp, 0x28
/* 00449620 00601025 */  move  $v0, $v1
/* 00449624 03E00008 */  jr    $ra
/* 00449628 00000000 */   nop   
    .type aliaswithptr, @function
    .size aliaswithptr, .-aliaswithptr
    .end aliaswithptr

glabel pointtoheap
    .ent pointtoheap
    # 0041F22C base_noalias
    # 0041F530 base_gp_noalias
    # 0041F5E4 base_sp_noalias
    # 004496F0 slkilled
    # 0044A5C8 smkilled
    # 0044AAD0 sskilled
    # 0044C6D4 func_0044C6D4
    # 0044CE80 func_0044CE80
    # 0044D0C4 func_0044D0C4
    # 0044DBC4 indirectaccessed
    # 00458B88 assign_mtag
    # 00478FA0 func_00478FA0
/* 0044962C 3C1C0FBD */  .cpload $t9
/* 00449630 279C0C64 */  
/* 00449634 0399E021 */  
/* 00449638 8F8E8A5C */  lw     $t6, %got(lang)($gp)
/* 0044963C 91CE0000 */  lbu   $t6, ($t6)
/* 00449640 51C00004 */  beql  $t6, $zero, .L00449654
/* 00449644 90820000 */   lbu   $v0, ($a0)
/* 00449648 03E00008 */  jr    $ra
/* 0044964C 00001025 */   move  $v0, $zero

/* 00449650 90820000 */  lbu   $v0, ($a0)
.L00449654:
/* 00449654 3C011200 */  lui   $at, 0x1200
/* 00449658 24050004 */  li    $a1, 4
/* 0044965C 2C4F0020 */  sltiu $t7, $v0, 0x20
/* 00449660 000FC023 */  negu  $t8, $t7
/* 00449664 0301C824 */  and   $t9, $t8, $at
/* 00449668 00594004 */  sllv  $t0, $t9, $v0
/* 0044966C 05010007 */  bgez  $t0, .L0044968C
/* 00449670 00000000 */   nop   
/* 00449674 90890001 */  lbu   $t1, 1($a0)
/* 00449678 24050004 */  li    $a1, 4
/* 0044967C 00A91826 */  xor   $v1, $a1, $t1
/* 00449680 2C630001 */  sltiu $v1, $v1, 1
/* 00449684 03E00008 */  jr    $ra
/* 00449688 306200FF */   andi  $v0, $v1, 0xff

.L0044968C:
/* 0044968C 54A20016 */  bnel  $a1, $v0, .L004496E8
/* 00449690 00001825 */   move  $v1, $zero
/* 00449694 908A0020 */  lbu   $t2, 0x20($a0)
/* 00449698 254BFFE0 */  addiu $t3, $t2, -0x20
/* 0044969C 2D6C0080 */  sltiu $t4, $t3, 0x80
/* 004496A0 11800009 */  beqz  $t4, .L004496C8
/* 004496A4 00000000 */   nop   
/* 004496A8 8F8F8044 */  lw    $t7, %got(D_10010A90)($gp)
/* 004496AC 000B6943 */  sra   $t5, $t3, 5
/* 004496B0 000D7080 */  sll   $t6, $t5, 2
/* 004496B4 25EF0A90 */  addiu $t7, %lo(D_10010A90) # addiu $t7, $t7, 0xa90
/* 004496B8 01EEC021 */  addu  $t8, $t7, $t6
/* 004496BC 8F190000 */  lw    $t9, ($t8)
/* 004496C0 01794004 */  sllv  $t0, $t9, $t3
/* 004496C4 290C0000 */  slti  $t4, $t0, 0
.L004496C8:
/* 004496C8 51800007 */  beql  $t4, $zero, .L004496E8
/* 004496CC 00001825 */   move  $v1, $zero
/* 004496D0 908A0001 */  lbu   $t2, 1($a0)
/* 004496D4 00AA1826 */  xor   $v1, $a1, $t2
/* 004496D8 2C630001 */  sltiu $v1, $v1, 1
/* 004496DC 03E00008 */  jr    $ra
/* 004496E0 306200FF */   andi  $v0, $v1, 0xff

/* 004496E4 00001825 */  move  $v1, $zero
.L004496E8:
/* 004496E8 03E00008 */  jr    $ra
/* 004496EC 00601025 */   move  $v0, $v1
    .type pointtoheap, @function
    .size pointtoheap, .-pointtoheap
    .end pointtoheap

glabel slkilled
    .ent slkilled
    # 004123C4 varinsert
    # 00431268 func_00431268
    # 004324F4 findinduct
    # 0044B308 strkillprev
    # 0044B448 lodkillprev
    # 0044D7F0 varkilled
    # 0044D8C4 strlkilled
    # 0044DADC strlant
/* 004496F0 3C1C0FBD */  .cpload $t9
/* 004496F4 279C0BA0 */  
/* 004496F8 0399E021 */  
/* 004496FC 27BDFFB8 */  addiu $sp, $sp, -0x48
/* 00449700 AFBF0024 */  sw    $ra, 0x24($sp)
/* 00449704 AFBC0020 */  sw    $gp, 0x20($sp)
/* 00449708 AFB0001C */  sw    $s0, 0x1c($sp)
/* 0044970C AFA40048 */  sw    $a0, 0x48($sp)
/* 00449710 90820000 */  lbu   $v0, ($a0)
/* 00449714 00A08025 */  move  $s0, $a1
/* 00449718 244FFF80 */  addiu $t7, $v0, -0x80
/* 0044971C 2DF80020 */  sltiu $t8, $t7, 0x20
/* 00449720 0018C823 */  negu  $t9, $t8
/* 00449724 332A0810 */  andi  $t2, $t9, 0x810
/* 00449728 01EA5804 */  sllv  $t3, $t2, $t7
/* 0044972C 0560000E */  bltz  $t3, .L00449768
/* 00449730 00000000 */   nop   
/* 00449734 90A30000 */  lbu   $v1, ($a1)
/* 00449738 24010004 */  li    $at, 4
/* 0044973C 244FFFE0 */  addiu $t7, $v0, -0x20
/* 00449740 1461000B */  bne   $v1, $at, .L00449770
/* 00449744 2DEB0060 */   sltiu $t3, $t7, 0x60
/* 00449748 90AC0020 */  lbu   $t4, 0x20($a1)
/* 0044974C 258DFF80 */  addiu $t5, $t4, -0x80
/* 00449750 2DAE0020 */  sltiu $t6, $t5, 0x20
/* 00449754 000EC023 */  negu  $t8, $t6
/* 00449758 33191020 */  andi  $t9, $t8, 0x1020
/* 0044975C 01B95004 */  sllv  $t2, $t9, $t5
/* 00449760 05410003 */  bgez  $t2, .L00449770
/* 00449764 00000000 */   nop   
.L00449768:
/* 00449768 10000392 */  b     .L0044A5B4
/* 0044976C 24020001 */   li    $v0, 1
.L00449770:
/* 00449770 11600009 */  beqz  $t3, .L00449798
/* 00449774 00000000 */   nop   
/* 00449778 8F988044 */  lw    $t8, %got(D_10010AB0)($gp)
/* 0044977C 000F6143 */  sra   $t4, $t7, 5
/* 00449780 000C7080 */  sll   $t6, $t4, 2
/* 00449784 27180AB0 */  addiu $t8, %lo(D_10010AB0) # addiu $t8, $t8, 0xab0
/* 00449788 030EC821 */  addu  $t9, $t8, $t6
/* 0044978C 8F2D0000 */  lw    $t5, ($t9)
/* 00449790 01ED5004 */  sllv  $t2, $t5, $t7
/* 00449794 294B0000 */  slti  $t3, $t2, 0
.L00449798:
/* 00449798 1560000C */  bnez  $t3, .L004497CC
/* 0044979C 8FB80048 */   lw    $t8, 0x48($sp)
/* 004497A0 8F0E0024 */  lw    $t6, 0x24($t8)
/* 004497A4 24010007 */  li    $at, 7
/* 004497A8 91D90000 */  lbu   $t9, ($t6)
/* 004497AC 57210008 */  bnel  $t9, $at, .L004497D0
/* 004497B0 2C6D0020 */   sltiu $t5, $v1, 0x20
/* 004497B4 8F99862C */  lw    $t9, %call16(fix_sbase)($gp)
/* 004497B8 03002025 */  move  $a0, $t8
/* 004497BC 0320F809 */  jalr  $t9
/* 004497C0 00000000 */   nop   
/* 004497C4 8FBC0020 */  lw    $gp, 0x20($sp)
/* 004497C8 92030000 */  lbu   $v1, ($s0)
.L004497CC:
/* 004497CC 2C6D0020 */  sltiu $t5, $v1, 0x20
.L004497D0:
/* 004497D0 000D7823 */  negu  $t7, $t5
/* 004497D4 3C011200 */  lui   $at, 0x1200
/* 004497D8 01E15024 */  and   $t2, $t7, $at
/* 004497DC 006A6004 */  sllv  $t4, $t2, $v1
/* 004497E0 05810067 */  bgez  $t4, .L00449980
/* 004497E4 8FAB0048 */   lw    $t3, 0x48($sp)
/* 004497E8 916E0000 */  lbu   $t6, ($t3)
/* 004497EC 25D9FFE0 */  addiu $t9, $t6, -0x20
/* 004497F0 2F380060 */  sltiu $t8, $t9, 0x60
/* 004497F4 13000009 */  beqz  $t8, .L0044981C
/* 004497F8 00000000 */   nop   
/* 004497FC 8F8A8044 */  lw    $t2, %got(D_10010AB0)($gp)
/* 00449800 00196943 */  sra   $t5, $t9, 5
/* 00449804 000D7880 */  sll   $t7, $t5, 2
/* 00449808 254A0AB0 */  addiu $t2, %lo(D_10010AB0) # addiu $t2, $t2, 0xab0
/* 0044980C 014F6021 */  addu  $t4, $t2, $t7
/* 00449810 8D8E0000 */  lw    $t6, ($t4)
/* 00449814 032E6804 */  sllv  $t5, $t6, $t9
/* 00449818 29B80000 */  slti  $t8, $t5, 0
.L0044981C:
/* 0044981C 53000014 */  beql  $t8, $zero, .L00449870
/* 00449820 8FB80048 */   lw    $t8, 0x48($sp)
/* 00449824 8E040028 */  lw    $a0, 0x28($s0)
/* 00449828 8E05002C */  lw    $a1, 0x2c($s0)
/* 0044982C 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 00449830 AFA40000 */  sw    $a0, ($sp)
/* 00449834 AFA50004 */  sw    $a1, 4($sp)
/* 00449838 8D620004 */  lw    $v0, 4($t3)
/* 0044983C 920D0020 */  lbu   $t5, 0x20($s0)
/* 00449840 8C460028 */  lw    $a2, 0x28($v0)
/* 00449844 904A0020 */  lbu   $t2, 0x20($v0)
/* 00449848 AFA60008 */  sw    $a2, 8($sp)
/* 0044984C 8C47002C */  lw    $a3, 0x2c($v0)
/* 00449850 AFAD0010 */  sw    $t5, 0x10($sp)
/* 00449854 AFAA0014 */  sw    $t2, 0x14($sp)
/* 00449858 0320F809 */  jalr  $t9
/* 0044985C AFA7000C */   sw    $a3, 0xc($sp)
/* 00449860 8FBC0020 */  lw    $gp, 0x20($sp)
/* 00449864 10000352 */  b     .L0044A5B0
/* 00449868 304400FF */   andi  $a0, $v0, 0xff
/* 0044986C 8FB80048 */  lw    $t8, 0x48($sp)
.L00449870:
/* 00449870 3C014400 */  lui   $at, 0x4400
/* 00449874 8F0F0024 */  lw    $t7, 0x24($t8)
/* 00449878 AFAF0038 */  sw    $t7, 0x38($sp)
/* 0044987C 91E60000 */  lbu   $a2, ($t7)
/* 00449880 2CCC0020 */  sltiu $t4, $a2, 0x20
/* 00449884 000C5823 */  negu  $t3, $t4
/* 00449888 01617024 */  and   $t6, $t3, $at
/* 0044988C 00CEC804 */  sllv  $t9, $t6, $a2
/* 00449890 07210012 */  bgez  $t9, .L004498DC
/* 00449894 3C011B00 */   lui   $at, 0x1b00
/* 00449898 8E040028 */  lw    $a0, 0x28($s0)
/* 0044989C 920B0020 */  lbu   $t3, 0x20($s0)
/* 004498A0 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 004498A4 AFA40000 */  sw    $a0, ($sp)
/* 004498A8 8E05002C */  lw    $a1, 0x2c($s0)
/* 004498AC AFA50004 */  sw    $a1, 4($sp)
/* 004498B0 8DE6002C */  lw    $a2, 0x2c($t7)
/* 004498B4 AFA60008 */  sw    $a2, 8($sp)
/* 004498B8 8DE70030 */  lw    $a3, 0x30($t7)
/* 004498BC AFAB0010 */  sw    $t3, 0x10($sp)
/* 004498C0 AFA7000C */  sw    $a3, 0xc($sp)
/* 004498C4 8DEE0024 */  lw    $t6, 0x24($t7)
/* 004498C8 0320F809 */  jalr  $t9
/* 004498CC AFAE0014 */   sw    $t6, 0x14($sp)
/* 004498D0 8FBC0020 */  lw    $gp, 0x20($sp)
/* 004498D4 10000336 */  b     .L0044A5B0
/* 004498D8 304400FF */   andi  $a0, $v0, 0xff
.L004498DC:
/* 004498DC 2CD90020 */  sltiu $t9, $a2, 0x20
/* 004498E0 00196823 */  negu  $t5, $t9
/* 004498E4 01A15024 */  and   $t2, $t5, $at
/* 004498E8 00CAC004 */  sllv  $t8, $t2, $a2
/* 004498EC 07010007 */  bgez  $t8, .L0044990C
/* 004498F0 00000000 */   nop   
/* 004498F4 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 004498F8 8FA40038 */  lw    $a0, 0x38($sp)
/* 004498FC 0320F809 */  jalr  $t9
/* 00449900 00000000 */   nop   
/* 00449904 10400003 */  beqz  $v0, .L00449914
/* 00449908 8FBC0020 */   lw    $gp, 0x20($sp)
.L0044990C:
/* 0044990C 10000328 */  b     .L0044A5B0
/* 00449910 00002025 */   move  $a0, $zero
.L00449914:
/* 00449914 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 00449918 26040028 */  addiu $a0, $s0, 0x28
/* 0044991C 0320F809 */  jalr  $t9
/* 00449920 00000000 */   nop   
/* 00449924 8FBC0020 */  lw    $gp, 0x20($sp)
/* 00449928 14400013 */  bnez  $v0, .L00449978
/* 0044992C 00401825 */   move  $v1, $v0
/* 00449930 8F8C8A5C */  lw     $t4, %got(lang)($gp)
/* 00449934 24090001 */  li    $t1, 1
/* 00449938 8FAB0048 */  lw    $t3, 0x48($sp)
/* 0044993C 918C0000 */  lbu   $t4, ($t4)
/* 00449940 012C1826 */  xor   $v1, $t1, $t4
/* 00449944 2C630001 */  sltiu $v1, $v1, 1
/* 00449948 1060000B */  beqz  $v1, .L00449978
/* 0044994C 00000000 */   nop   
/* 00449950 8D620024 */  lw    $v0, 0x24($t3)
/* 00449954 90430000 */  lbu   $v1, ($v0)
/* 00449958 38630003 */  xori  $v1, $v1, 3
/* 0044995C 2C630001 */  sltiu $v1, $v1, 1
/* 00449960 10600005 */  beqz  $v1, .L00449978
/* 00449964 00000000 */   nop   
/* 00449968 904F002E */  lbu   $t7, 0x2e($v0)
/* 0044996C 31EE0007 */  andi  $t6, $t7, 7
/* 00449970 012E1826 */  xor   $v1, $t1, $t6
/* 00449974 2C630001 */  sltiu $v1, $v1, 1
.L00449978:
/* 00449978 1000030D */  b     .L0044A5B0
/* 0044997C 306400FF */   andi  $a0, $v1, 0xff
.L00449980:
/* 00449980 8E190034 */  lw    $t9, 0x34($s0)
/* 00449984 24010007 */  li    $at, 7
/* 00449988 AFB9002C */  sw    $t9, 0x2c($sp)
/* 0044998C 93240000 */  lbu   $a0, ($t9)
/* 00449990 5481000A */  bnel  $a0, $at, .L004499BC
/* 00449994 2C8A0020 */   sltiu $t2, $a0, 0x20
/* 00449998 8F998630 */  lw    $t9, %call16(fix_base)($gp)
/* 0044999C 02002025 */  move  $a0, $s0
/* 004499A0 0320F809 */  jalr  $t9
/* 004499A4 00000000 */   nop   
/* 004499A8 8E020034 */  lw    $v0, 0x34($s0)
/* 004499AC 8FBC0020 */  lw    $gp, 0x20($sp)
/* 004499B0 90440000 */  lbu   $a0, ($v0)
/* 004499B4 AFA2002C */  sw    $v0, 0x2c($sp)
/* 004499B8 2C8A0020 */  sltiu $t2, $a0, 0x20
.L004499BC:
/* 004499BC 000AC023 */  negu  $t8, $t2
/* 004499C0 3C014400 */  lui   $at, 0x4400
/* 004499C4 03016024 */  and   $t4, $t8, $at
/* 004499C8 008C5804 */  sllv  $t3, $t4, $a0
/* 004499CC 05610071 */  bgez  $t3, .L00449B94
/* 004499D0 8FAF0048 */   lw    $t7, 0x48($sp)
/* 004499D4 91EE0000 */  lbu   $t6, ($t7)
/* 004499D8 25D9FFE0 */  addiu $t9, $t6, -0x20
/* 004499DC 2F2D0060 */  sltiu $t5, $t9, 0x60
/* 004499E0 11A00009 */  beqz  $t5, .L00449A08
/* 004499E4 00000000 */   nop   
/* 004499E8 8F8C8044 */  lw    $t4, %got(D_10010AB0)($gp)
/* 004499EC 00195143 */  sra   $t2, $t9, 5
/* 004499F0 000AC080 */  sll   $t8, $t2, 2
/* 004499F4 258C0AB0 */  addiu $t4, %lo(D_10010AB0) # addiu $t4, $t4, 0xab0
/* 004499F8 01985821 */  addu  $t3, $t4, $t8
/* 004499FC 8D6E0000 */  lw    $t6, ($t3)
/* 00449A00 032E5004 */  sllv  $t2, $t6, $t9
/* 00449A04 294D0000 */  slti  $t5, $t2, 0
.L00449A08:
/* 00449A08 11A00015 */  beqz  $t5, .L00449A60
/* 00449A0C 8FAB0048 */   lw    $t3, 0x48($sp)
/* 00449A10 8FB8002C */  lw    $t8, 0x2c($sp)
/* 00449A14 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 00449A18 8F04002C */  lw    $a0, 0x2c($t8)
/* 00449A1C AFA40000 */  sw    $a0, ($sp)
/* 00449A20 8F050030 */  lw    $a1, 0x30($t8)
/* 00449A24 AFA50004 */  sw    $a1, 4($sp)
/* 00449A28 8DE20004 */  lw    $v0, 4($t7)
/* 00449A2C 8C460028 */  lw    $a2, 0x28($v0)
/* 00449A30 904D0020 */  lbu   $t5, 0x20($v0)
/* 00449A34 AFA60008 */  sw    $a2, 8($sp)
/* 00449A38 8C47002C */  lw    $a3, 0x2c($v0)
/* 00449A3C AFA7000C */  sw    $a3, 0xc($sp)
/* 00449A40 8F0C0024 */  lw    $t4, 0x24($t8)
/* 00449A44 AFAD0014 */  sw    $t5, 0x14($sp)
/* 00449A48 0320F809 */  jalr  $t9
/* 00449A4C AFAC0010 */   sw    $t4, 0x10($sp)
/* 00449A50 8FBC0020 */  lw    $gp, 0x20($sp)
/* 00449A54 304400FF */  andi  $a0, $v0, 0xff
/* 00449A58 10000150 */  b     .L00449F9C
/* 00449A5C 92050020 */   lbu   $a1, 0x20($s0)
.L00449A60:
/* 00449A60 8D6E0024 */  lw    $t6, 0x24($t3)
/* 00449A64 3C014400 */  lui   $at, 0x4400
/* 00449A68 8FAC002C */  lw    $t4, 0x2c($sp)
/* 00449A6C AFAE0038 */  sw    $t6, 0x38($sp)
/* 00449A70 91C60000 */  lbu   $a2, ($t6)
/* 00449A74 2CCF0020 */  sltiu $t7, $a2, 0x20
/* 00449A78 000FC823 */  negu  $t9, $t7
/* 00449A7C 03215024 */  and   $t2, $t9, $at
/* 00449A80 00CAC004 */  sllv  $t8, $t2, $a2
/* 00449A84 07010013 */  bgez  $t8, .L00449AD4
/* 00449A88 00000000 */   nop   
/* 00449A8C 8D84002C */  lw    $a0, 0x2c($t4)
/* 00449A90 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 00449A94 AFA40000 */  sw    $a0, ($sp)
/* 00449A98 8D850030 */  lw    $a1, 0x30($t4)
/* 00449A9C AFA50004 */  sw    $a1, 4($sp)
/* 00449AA0 8DC6002C */  lw    $a2, 0x2c($t6)
/* 00449AA4 AFA60008 */  sw    $a2, 8($sp)
/* 00449AA8 8DC70030 */  lw    $a3, 0x30($t6)
/* 00449AAC AFA7000C */  sw    $a3, 0xc($sp)
/* 00449AB0 8D8A0024 */  lw    $t2, 0x24($t4)
/* 00449AB4 AFAA0010 */  sw    $t2, 0x10($sp)
/* 00449AB8 8DD80024 */  lw    $t8, 0x24($t6)
/* 00449ABC 0320F809 */  jalr  $t9
/* 00449AC0 AFB80014 */   sw    $t8, 0x14($sp)
/* 00449AC4 8FBC0020 */  lw    $gp, 0x20($sp)
/* 00449AC8 304400FF */  andi  $a0, $v0, 0xff
/* 00449ACC 10000133 */  b     .L00449F9C
/* 00449AD0 92050020 */   lbu   $a1, 0x20($s0)
.L00449AD4:
/* 00449AD4 8F8D8A5C */  lw     $t5, %got(lang)($gp)
/* 00449AD8 24010002 */  li    $at, 2
/* 00449ADC 2CCB0020 */  sltiu $t3, $a2, 0x20
/* 00449AE0 91AD0000 */  lbu   $t5, ($t5)
/* 00449AE4 000B7823 */  negu  $t7, $t3
/* 00449AE8 11A10005 */  beq   $t5, $at, .L00449B00
/* 00449AEC 3C011B00 */   lui   $at, 0x1b00
/* 00449AF0 01E1C824 */  and   $t9, $t7, $at
/* 00449AF4 00D96004 */  sllv  $t4, $t9, $a2
/* 00449AF8 05830008 */  bgezl $t4, .L00449B1C
/* 00449AFC 00002025 */   move  $a0, $zero
.L00449B00:
/* 00449B00 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 00449B04 8FA40038 */  lw    $a0, 0x38($sp)
/* 00449B08 0320F809 */  jalr  $t9
/* 00449B0C 00000000 */   nop   
/* 00449B10 10400004 */  beqz  $v0, .L00449B24
/* 00449B14 8FBC0020 */   lw    $gp, 0x20($sp)
/* 00449B18 00002025 */  move  $a0, $zero
.L00449B1C:
/* 00449B1C 1000011F */  b     .L00449F9C
/* 00449B20 92050020 */   lbu   $a1, 0x20($s0)
.L00449B24:
/* 00449B24 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 00449B28 8E040034 */  lw    $a0, 0x34($s0)
/* 00449B2C 0320F809 */  jalr  $t9
/* 00449B30 2484002C */   addiu $a0, $a0, 0x2c
/* 00449B34 8FBC0020 */  lw    $gp, 0x20($sp)
/* 00449B38 14400013 */  bnez  $v0, .L00449B88
/* 00449B3C 00401825 */   move  $v1, $v0
/* 00449B40 8F8A8A5C */  lw     $t2, %got(lang)($gp)
/* 00449B44 24090001 */  li    $t1, 1
/* 00449B48 8FAE0048 */  lw    $t6, 0x48($sp)
/* 00449B4C 914A0000 */  lbu   $t2, ($t2)
/* 00449B50 012A1826 */  xor   $v1, $t1, $t2
/* 00449B54 2C630001 */  sltiu $v1, $v1, 1
/* 00449B58 5060000C */  beql  $v1, $zero, .L00449B8C
/* 00449B5C 306400FF */   andi  $a0, $v1, 0xff
/* 00449B60 8DC80024 */  lw    $t0, 0x24($t6)
/* 00449B64 91030000 */  lbu   $v1, ($t0)
/* 00449B68 38630003 */  xori  $v1, $v1, 3
/* 00449B6C 2C630001 */  sltiu $v1, $v1, 1
/* 00449B70 50600006 */  beql  $v1, $zero, .L00449B8C
/* 00449B74 306400FF */   andi  $a0, $v1, 0xff
/* 00449B78 9118002E */  lbu   $t8, 0x2e($t0)
/* 00449B7C 330D0007 */  andi  $t5, $t8, 7
/* 00449B80 012D1826 */  xor   $v1, $t1, $t5
/* 00449B84 2C630001 */  sltiu $v1, $v1, 1
.L00449B88:
/* 00449B88 306400FF */  andi  $a0, $v1, 0xff
.L00449B8C:
/* 00449B8C 10000103 */  b     .L00449F9C
/* 00449B90 92050020 */   lbu   $a1, 0x20($s0)
.L00449B94:
/* 00449B94 8F8B8A5C */  lw     $t3, %got(lang)($gp)
/* 00449B98 24010002 */  li    $at, 2
/* 00449B9C 2C8F0020 */  sltiu $t7, $a0, 0x20
/* 00449BA0 916B0000 */  lbu   $t3, ($t3)
/* 00449BA4 000FC823 */  negu  $t9, $t7
/* 00449BA8 11610007 */  beq   $t3, $at, .L00449BC8
/* 00449BAC 3C011B00 */   lui   $at, 0x1b00
/* 00449BB0 03216024 */  and   $t4, $t9, $at
/* 00449BB4 008C5004 */  sllv  $t2, $t4, $a0
/* 00449BB8 05400003 */  bltz  $t2, .L00449BC8
/* 00449BBC 00002025 */   move  $a0, $zero
/* 00449BC0 100000F6 */  b     .L00449F9C
/* 00449BC4 92050020 */   lbu   $a1, 0x20($s0)
.L00449BC8:
/* 00449BC8 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 00449BCC 8FA4002C */  lw    $a0, 0x2c($sp)
/* 00449BD0 0320F809 */  jalr  $t9
/* 00449BD4 00000000 */   nop   
/* 00449BD8 1040001B */  beqz  $v0, .L00449C48
/* 00449BDC 8FBC0020 */   lw    $gp, 0x20($sp)
/* 00449BE0 8FAE0048 */  lw    $t6, 0x48($sp)
/* 00449BE4 91D80000 */  lbu   $t8, ($t6)
/* 00449BE8 270DFFE0 */  addiu $t5, $t8, -0x20
/* 00449BEC 2DAB0060 */  sltiu $t3, $t5, 0x60
/* 00449BF0 11600009 */  beqz  $t3, .L00449C18
/* 00449BF4 00000000 */   nop   
/* 00449BF8 8F8C8044 */  lw    $t4, %got(D_10010AB0)($gp)
/* 00449BFC 000D7943 */  sra   $t7, $t5, 5
/* 00449C00 000FC880 */  sll   $t9, $t7, 2
/* 00449C04 258C0AB0 */  addiu $t4, %lo(D_10010AB0) # addiu $t4, $t4, 0xab0
/* 00449C08 01995021 */  addu  $t2, $t4, $t9
/* 00449C0C 8D4E0000 */  lw    $t6, ($t2)
/* 00449C10 01AEC004 */  sllv  $t8, $t6, $t5
/* 00449C14 2B0B0000 */  slti  $t3, $t8, 0
.L00449C18:
/* 00449C18 11600003 */  beqz  $t3, .L00449C28
/* 00449C1C 00000000 */   nop   
/* 00449C20 10000007 */  b     .L00449C40
/* 00449C24 00002025 */   move  $a0, $zero
.L00449C28:
/* 00449C28 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 00449C2C 8FAC0048 */  lw    $t4, 0x48($sp)
/* 00449C30 0320F809 */  jalr  $t9
/* 00449C34 8D840024 */   lw    $a0, 0x24($t4)
/* 00449C38 8FBC0020 */  lw    $gp, 0x20($sp)
/* 00449C3C 304400FF */  andi  $a0, $v0, 0xff
.L00449C40:
/* 00449C40 100000D6 */  b     .L00449F9C
/* 00449C44 92050020 */   lbu   $a1, 0x20($s0)
.L00449C48:
/* 00449C48 8FB90048 */  lw    $t9, 0x48($sp)
/* 00449C4C 932A0000 */  lbu   $t2, ($t9)
/* 00449C50 254EFFE0 */  addiu $t6, $t2, -0x20
/* 00449C54 2DCD0060 */  sltiu $t5, $t6, 0x60
/* 00449C58 11A00009 */  beqz  $t5, .L00449C80
/* 00449C5C 00000000 */   nop   
/* 00449C60 8F8B8044 */  lw    $t3, %got(D_10010AB0)($gp)
/* 00449C64 000EC143 */  sra   $t8, $t6, 5
/* 00449C68 00187880 */  sll   $t7, $t8, 2
/* 00449C6C 256B0AB0 */  addiu $t3, %lo(D_10010AB0) # addiu $t3, $t3, 0xab0
/* 00449C70 016F6021 */  addu  $t4, $t3, $t7
/* 00449C74 8D8A0000 */  lw    $t2, ($t4)
/* 00449C78 01CAC004 */  sllv  $t8, $t2, $t6
/* 00449C7C 2B0D0000 */  slti  $t5, $t8, 0
.L00449C80:
/* 00449C80 11A0001D */  beqz  $t5, .L00449CF8
/* 00449C84 8FAE0048 */   lw    $t6, 0x48($sp)
/* 00449C88 8F240004 */  lw    $a0, 4($t9)
/* 00449C8C 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 00449C90 24840028 */  addiu $a0, $a0, 0x28
/* 00449C94 0320F809 */  jalr  $t9
/* 00449C98 00000000 */   nop   
/* 00449C9C 8FBC0020 */  lw    $gp, 0x20($sp)
/* 00449CA0 14400012 */  bnez  $v0, .L00449CEC
/* 00449CA4 00401825 */   move  $v1, $v0
/* 00449CA8 8F8F8A5C */  lw     $t7, %got(lang)($gp)
/* 00449CAC 24090001 */  li    $t1, 1
/* 00449CB0 91EF0000 */  lbu   $t7, ($t7)
/* 00449CB4 012F1826 */  xor   $v1, $t1, $t7
/* 00449CB8 2C630001 */  sltiu $v1, $v1, 1
/* 00449CBC 5060000C */  beql  $v1, $zero, .L00449CF0
/* 00449CC0 306400FF */   andi  $a0, $v1, 0xff
/* 00449CC4 8E020034 */  lw    $v0, 0x34($s0)
/* 00449CC8 90430000 */  lbu   $v1, ($v0)
/* 00449CCC 38630003 */  xori  $v1, $v1, 3
/* 00449CD0 2C630001 */  sltiu $v1, $v1, 1
/* 00449CD4 50600006 */  beql  $v1, $zero, .L00449CF0
/* 00449CD8 306400FF */   andi  $a0, $v1, 0xff
/* 00449CDC 904C002E */  lbu   $t4, 0x2e($v0)
/* 00449CE0 318A0007 */  andi  $t2, $t4, 7
/* 00449CE4 012A1826 */  xor   $v1, $t1, $t2
/* 00449CE8 2C630001 */  sltiu $v1, $v1, 1
.L00449CEC:
/* 00449CEC 306400FF */  andi  $a0, $v1, 0xff
.L00449CF0:
/* 00449CF0 100000AA */  b     .L00449F9C
/* 00449CF4 92050020 */   lbu   $a1, 0x20($s0)
.L00449CF8:
/* 00449CF8 8DC80024 */  lw    $t0, 0x24($t6)
/* 00449CFC 3C014400 */  lui   $at, 0x4400
/* 00449D00 91060000 */  lbu   $a2, ($t0)
/* 00449D04 2CD80020 */  sltiu $t8, $a2, 0x20
/* 00449D08 00185823 */  negu  $t3, $t8
/* 00449D0C 01616824 */  and   $t5, $t3, $at
/* 00449D10 00CDC804 */  sllv  $t9, $t5, $a2
/* 00449D14 0721001C */  bgez  $t9, .L00449D88
/* 00449D18 00000000 */   nop   
/* 00449D1C 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 00449D20 2504002C */  addiu $a0, $t0, 0x2c
/* 00449D24 0320F809 */  jalr  $t9
/* 00449D28 00000000 */   nop   
/* 00449D2C 8FBC0020 */  lw    $gp, 0x20($sp)
/* 00449D30 14400012 */  bnez  $v0, .L00449D7C
/* 00449D34 00401825 */   move  $v1, $v0
/* 00449D38 8F8F8A5C */  lw     $t7, %got(lang)($gp)
/* 00449D3C 24090001 */  li    $t1, 1
/* 00449D40 91EF0000 */  lbu   $t7, ($t7)
/* 00449D44 012F1826 */  xor   $v1, $t1, $t7
/* 00449D48 2C630001 */  sltiu $v1, $v1, 1
/* 00449D4C 5060000C */  beql  $v1, $zero, .L00449D80
/* 00449D50 306400FF */   andi  $a0, $v1, 0xff
/* 00449D54 8E020034 */  lw    $v0, 0x34($s0)
/* 00449D58 90430000 */  lbu   $v1, ($v0)
/* 00449D5C 38630003 */  xori  $v1, $v1, 3
/* 00449D60 2C630001 */  sltiu $v1, $v1, 1
/* 00449D64 50600006 */  beql  $v1, $zero, .L00449D80
/* 00449D68 306400FF */   andi  $a0, $v1, 0xff
/* 00449D6C 904C002E */  lbu   $t4, 0x2e($v0)
/* 00449D70 318A0007 */  andi  $t2, $t4, 7
/* 00449D74 012A1826 */  xor   $v1, $t1, $t2
/* 00449D78 2C630001 */  sltiu $v1, $v1, 1
.L00449D7C:
/* 00449D7C 306400FF */  andi  $a0, $v1, 0xff
.L00449D80:
/* 00449D80 10000086 */  b     .L00449F9C
/* 00449D84 92050020 */   lbu   $a1, 0x20($s0)
.L00449D88:
/* 00449D88 8F858A5C */  lw     $a1, %got(lang)($gp)
/* 00449D8C 24010002 */  li    $at, 2
/* 00449D90 2CCE0020 */  sltiu $t6, $a2, 0x20
/* 00449D94 90A50000 */  lbu   $a1, ($a1)
/* 00449D98 000EC023 */  negu  $t8, $t6
/* 00449D9C 10A10007 */  beq   $a1, $at, .L00449DBC
/* 00449DA0 3C011B00 */   lui   $at, 0x1b00
/* 00449DA4 03015824 */  and   $t3, $t8, $at
/* 00449DA8 00CB6804 */  sllv  $t5, $t3, $a2
/* 00449DAC 05A00003 */  bltz  $t5, .L00449DBC
/* 00449DB0 00002025 */   move  $a0, $zero
/* 00449DB4 10000079 */  b     .L00449F9C
/* 00449DB8 92050020 */   lbu   $a1, 0x20($s0)
.L00449DBC:
/* 00449DBC 2CB90020 */  sltiu $t9, $a1, 0x20
/* 00449DC0 00197823 */  negu  $t7, $t9
/* 00449DC4 3C01C400 */  lui   $at, 0xc400
/* 00449DC8 01E16024 */  and   $t4, $t7, $at
/* 00449DCC 00AC1804 */  sllv  $v1, $t4, $a1
/* 00449DD0 28630000 */  slti  $v1, $v1, 0
/* 00449DD4 2C630001 */  sltiu $v1, $v1, 1
/* 00449DD8 1460006E */  bnez  $v1, .L00449F94
/* 00449DDC 24090001 */   li    $t1, 1
/* 00449DE0 01251826 */  xor   $v1, $t1, $a1
/* 00449DE4 2C630001 */  sltiu $v1, $v1, 1
/* 00449DE8 1060002E */  beqz  $v1, .L00449EA4
/* 00449DEC 00000000 */   nop   
/* 00449DF0 8F838AE0 */  lw     $v1, %got(nof77alias)($gp)
/* 00449DF4 90630000 */  lbu   $v1, ($v1)
/* 00449DF8 2C630001 */  sltiu $v1, $v1, 1
/* 00449DFC 54600066 */  bnezl $v1, .L00449F98
/* 00449E00 306400FF */   andi  $a0, $v1, 0xff
/* 00449E04 8F838B44 */  lw     $v1, %got(use_c_semantics)($gp)
/* 00449E08 90630000 */  lbu   $v1, ($v1)
/* 00449E0C 54600062 */  bnezl $v1, .L00449F98
/* 00449E10 306400FF */   andi  $a0, $v1, 0xff
/* 00449E14 8E020034 */  lw    $v0, 0x34($s0)
/* 00449E18 90430000 */  lbu   $v1, ($v0)
/* 00449E1C 38630003 */  xori  $v1, $v1, 3
/* 00449E20 2C630001 */  sltiu $v1, $v1, 1
/* 00449E24 1060000D */  beqz  $v1, .L00449E5C
/* 00449E28 00000000 */   nop   
/* 00449E2C 9044002E */  lbu   $a0, 0x2e($v0)
/* 00449E30 30840007 */  andi  $a0, $a0, 7
/* 00449E34 01241826 */  xor   $v1, $t1, $a0
/* 00449E38 2C630001 */  sltiu $v1, $v1, 1
/* 00449E3C 54600056 */  bnezl $v1, .L00449F98
/* 00449E40 306400FF */   andi  $a0, $v1, 0xff
/* 00449E44 8F838B14 */  lw     $v1, %got(f77_static_flag)($gp)
/* 00449E48 90630000 */  lbu   $v1, ($v1)
/* 00449E4C 10600003 */  beqz  $v1, .L00449E5C
/* 00449E50 00000000 */   nop   
/* 00449E54 38830004 */  xori  $v1, $a0, 4
/* 00449E58 2C630001 */  sltiu $v1, $v1, 1
.L00449E5C:
/* 00449E5C 5460004E */  bnezl $v1, .L00449F98
/* 00449E60 306400FF */   andi  $a0, $v1, 0xff
/* 00449E64 38C30003 */  xori  $v1, $a2, 3
/* 00449E68 2C630001 */  sltiu $v1, $v1, 1
/* 00449E6C 1060000D */  beqz  $v1, .L00449EA4
/* 00449E70 00000000 */   nop   
/* 00449E74 9102002E */  lbu   $v0, 0x2e($t0)
/* 00449E78 30420007 */  andi  $v0, $v0, 7
/* 00449E7C 01221826 */  xor   $v1, $t1, $v0
/* 00449E80 2C630001 */  sltiu $v1, $v1, 1
/* 00449E84 54600044 */  bnezl $v1, .L00449F98
/* 00449E88 306400FF */   andi  $a0, $v1, 0xff
/* 00449E8C 8F838B14 */  lw     $v1, %got(f77_static_flag)($gp)
/* 00449E90 90630000 */  lbu   $v1, ($v1)
/* 00449E94 10600003 */  beqz  $v1, .L00449EA4
/* 00449E98 00000000 */   nop   
/* 00449E9C 38430004 */  xori  $v1, $v0, 4
/* 00449EA0 2C630001 */  sltiu $v1, $v1, 1
.L00449EA4:
/* 00449EA4 1460003B */  bnez  $v1, .L00449F94
/* 00449EA8 2CA20001 */   sltiu $v0, $a1, 1
/* 00449EAC 10400008 */  beqz  $v0, .L00449ED0
/* 00449EB0 00401825 */   move  $v1, $v0
/* 00449EB4 8F838AE4 */  lw     $v1, %got(nopalias)($gp)
/* 00449EB8 90630000 */  lbu   $v1, ($v1)
/* 00449EBC 10600004 */  beqz  $v1, .L00449ED0
/* 00449EC0 00000000 */   nop   
/* 00449EC4 8F838AE0 */  lw     $v1, %got(nof77alias)($gp)
/* 00449EC8 90630000 */  lbu   $v1, ($v1)
/* 00449ECC 2C630001 */  sltiu $v1, $v1, 1
.L00449ED0:
/* 00449ED0 54600031 */  bnezl $v1, .L00449F98
/* 00449ED4 306400FF */   andi  $a0, $v1, 0xff
/* 00449ED8 10400004 */  beqz  $v0, .L00449EEC
/* 00449EDC 00401825 */   move  $v1, $v0
/* 00449EE0 8F838AE4 */  lw     $v1, %got(nopalias)($gp)
/* 00449EE4 90630000 */  lbu   $v1, ($v1)
/* 00449EE8 2C630001 */  sltiu $v1, $v1, 1
.L00449EEC:
/* 00449EEC 5460002A */  bnezl $v1, .L00449F98
/* 00449EF0 306400FF */   andi  $a0, $v1, 0xff
/* 00449EF4 8E020034 */  lw    $v0, 0x34($s0)
/* 00449EF8 90440000 */  lbu   $a0, ($v0)
/* 00449EFC 38830007 */  xori  $v1, $a0, 7
/* 00449F00 2C630001 */  sltiu $v1, $v1, 1
/* 00449F04 54600024 */  bnezl $v1, .L00449F98
/* 00449F08 306400FF */   andi  $a0, $v1, 0xff
/* 00449F0C 38C30007 */  xori  $v1, $a2, 7
/* 00449F10 2C630001 */  sltiu $v1, $v1, 1
/* 00449F14 54600020 */  bnezl $v1, .L00449F98
/* 00449F18 306400FF */   andi  $a0, $v1, 0xff
/* 00449F1C 01021826 */  xor   $v1, $t0, $v0
/* 00449F20 2C630001 */  sltiu $v1, $v1, 1
/* 00449F24 1460001B */  bnez  $v1, .L00449F94
/* 00449F28 2C8A0020 */   sltiu $t2, $a0, 0x20
/* 00449F2C 000A7023 */  negu  $t6, $t2
/* 00449F30 3C011200 */  lui   $at, 0x1200
/* 00449F34 01C1C024 */  and   $t8, $t6, $at
/* 00449F38 00981804 */  sllv  $v1, $t8, $a0
/* 00449F3C 28630000 */  slti  $v1, $v1, 0
/* 00449F40 10600014 */  beqz  $v1, .L00449F94
/* 00449F44 2CCB0020 */   sltiu $t3, $a2, 0x20
/* 00449F48 000B6823 */  negu  $t5, $t3
/* 00449F4C 3C011200 */  lui   $at, 0x1200
/* 00449F50 01A1C824 */  and   $t9, $t5, $at
/* 00449F54 00D91804 */  sllv  $v1, $t9, $a2
/* 00449F58 28630000 */  slti  $v1, $v1, 0
/* 00449F5C 5060000E */  beql  $v1, $zero, .L00449F98
/* 00449F60 306400FF */   andi  $a0, $v1, 0xff
/* 00449F64 8F99860C */  lw    $t9, %call16(addreq)($gp)
/* 00449F68 8C440028 */  lw    $a0, 0x28($v0)
/* 00449F6C 8C45002C */  lw    $a1, 0x2c($v0)
/* 00449F70 8D060028 */  lw    $a2, 0x28($t0)
/* 00449F74 8D07002C */  lw    $a3, 0x2c($t0)
/* 00449F78 AFA40000 */  sw    $a0, ($sp)
/* 00449F7C AFA50004 */  sw    $a1, 4($sp)
/* 00449F80 AFA60008 */  sw    $a2, 8($sp)
/* 00449F84 0320F809 */  jalr  $t9
/* 00449F88 AFA7000C */   sw    $a3, 0xc($sp)
/* 00449F8C 8FBC0020 */  lw    $gp, 0x20($sp)
/* 00449F90 00401825 */  move  $v1, $v0
.L00449F94:
/* 00449F94 306400FF */  andi  $a0, $v1, 0xff
.L00449F98:
/* 00449F98 92050020 */  lbu   $a1, 0x20($s0)
.L00449F9C:
/* 00449F9C 24B8FFE0 */  addiu $t8, $a1, -0x20
/* 00449FA0 2F0B0080 */  sltiu $t3, $t8, 0x80
/* 00449FA4 11600009 */  beqz  $t3, .L00449FCC
/* 00449FA8 00000000 */   nop   
/* 00449FAC 8F8F8044 */  lw    $t7, %got(D_10010AA0)($gp)
/* 00449FB0 00186943 */  sra   $t5, $t8, 5
/* 00449FB4 000DC880 */  sll   $t9, $t5, 2
/* 00449FB8 25EF0AA0 */  addiu $t7, %lo(D_10010AA0) # addiu $t7, $t7, 0xaa0
/* 00449FBC 01F96021 */  addu  $t4, $t7, $t9
/* 00449FC0 8D8A0000 */  lw    $t2, ($t4)
/* 00449FC4 030A7004 */  sllv  $t6, $t2, $t8
/* 00449FC8 29CB0000 */  slti  $t3, $t6, 0
.L00449FCC:
/* 00449FCC 15600150 */  bnez  $t3, .L0044A510
/* 00449FD0 00000000 */   nop   
/* 00449FD4 1480014E */  bnez  $a0, .L0044A510
/* 00449FD8 00000000 */   nop   
/* 00449FDC 8E0F0038 */  lw    $t7, 0x38($s0)
/* 00449FE0 3C014400 */  lui   $at, 0x4400
/* 00449FE4 8FAD0048 */  lw    $t5, 0x48($sp)
/* 00449FE8 AFAF0030 */  sw    $t7, 0x30($sp)
/* 00449FEC 91E40000 */  lbu   $a0, ($t7)
/* 00449FF0 2C8C0020 */  sltiu $t4, $a0, 0x20
/* 00449FF4 000C5023 */  negu  $t2, $t4
/* 00449FF8 0141C024 */  and   $t8, $t2, $at
/* 00449FFC 00987004 */  sllv  $t6, $t8, $a0
/* 0044A000 05C10069 */  bgez  $t6, .L0044A1A8
/* 0044A004 00000000 */   nop   
/* 0044A008 91AB0000 */  lbu   $t3, ($t5)
/* 0044A00C 256FFFE0 */  addiu $t7, $t3, -0x20
/* 0044A010 2DF90060 */  sltiu $t9, $t7, 0x60
/* 0044A014 13200009 */  beqz  $t9, .L0044A03C
/* 0044A018 00000000 */   nop   
/* 0044A01C 8F988044 */  lw    $t8, %got(D_10010AB0)($gp)
/* 0044A020 000F6143 */  sra   $t4, $t7, 5
/* 0044A024 000C5080 */  sll   $t2, $t4, 2
/* 0044A028 27180AB0 */  addiu $t8, %lo(D_10010AB0) # addiu $t8, $t8, 0xab0
/* 0044A02C 030A7021 */  addu  $t6, $t8, $t2
/* 0044A030 8DCB0000 */  lw    $t3, ($t6)
/* 0044A034 01EB6004 */  sllv  $t4, $t3, $t7
/* 0044A038 29990000 */  slti  $t9, $t4, 0
.L0044A03C:
/* 0044A03C 13200015 */  beqz  $t9, .L0044A094
/* 0044A040 8FAE0048 */   lw    $t6, 0x48($sp)
/* 0044A044 8FAA0030 */  lw    $t2, 0x30($sp)
/* 0044A048 8D44002C */  lw    $a0, 0x2c($t2)
/* 0044A04C AFA40000 */  sw    $a0, ($sp)
/* 0044A050 8D450030 */  lw    $a1, 0x30($t2)
/* 0044A054 AFA50004 */  sw    $a1, 4($sp)
/* 0044A058 8DA20004 */  lw    $v0, 4($t5)
/* 0044A05C 8C460028 */  lw    $a2, 0x28($v0)
/* 0044A060 90590020 */  lbu   $t9, 0x20($v0)
/* 0044A064 AFA60008 */  sw    $a2, 8($sp)
/* 0044A068 8C47002C */  lw    $a3, 0x2c($v0)
/* 0044A06C AFA7000C */  sw    $a3, 0xc($sp)
/* 0044A070 8D580024 */  lw    $t8, 0x24($t2)
/* 0044A074 AFB90014 */  sw    $t9, 0x14($sp)
/* 0044A078 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044A07C AFB80010 */  sw    $t8, 0x10($sp)
/* 0044A080 0320F809 */  jalr  $t9
/* 0044A084 00000000 */   nop   
/* 0044A088 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A08C 10000148 */  b     .L0044A5B0
/* 0044A090 304400FF */   andi  $a0, $v0, 0xff
.L0044A094:
/* 0044A094 8DCB0024 */  lw    $t3, 0x24($t6)
/* 0044A098 3C014400 */  lui   $at, 0x4400
/* 0044A09C 8FB80030 */  lw    $t8, 0x30($sp)
/* 0044A0A0 AFAB0038 */  sw    $t3, 0x38($sp)
/* 0044A0A4 91660000 */  lbu   $a2, ($t3)
/* 0044A0A8 2CCD0020 */  sltiu $t5, $a2, 0x20
/* 0044A0AC 000D7823 */  negu  $t7, $t5
/* 0044A0B0 01E16024 */  and   $t4, $t7, $at
/* 0044A0B4 00CC5004 */  sllv  $t2, $t4, $a2
/* 0044A0B8 05410012 */  bgez  $t2, .L0044A104
/* 0044A0BC 2CD90020 */   sltiu $t9, $a2, 0x20
/* 0044A0C0 8F04002C */  lw    $a0, 0x2c($t8)
/* 0044A0C4 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044A0C8 AFA40000 */  sw    $a0, ($sp)
/* 0044A0CC 8F050030 */  lw    $a1, 0x30($t8)
/* 0044A0D0 AFA50004 */  sw    $a1, 4($sp)
/* 0044A0D4 8D66002C */  lw    $a2, 0x2c($t3)
/* 0044A0D8 AFA60008 */  sw    $a2, 8($sp)
/* 0044A0DC 8D670030 */  lw    $a3, 0x30($t3)
/* 0044A0E0 AFA7000C */  sw    $a3, 0xc($sp)
/* 0044A0E4 8F0C0024 */  lw    $t4, 0x24($t8)
/* 0044A0E8 AFAC0010 */  sw    $t4, 0x10($sp)
/* 0044A0EC 8D6A0024 */  lw    $t2, 0x24($t3)
/* 0044A0F0 0320F809 */  jalr  $t9
/* 0044A0F4 AFAA0014 */   sw    $t2, 0x14($sp)
/* 0044A0F8 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A0FC 1000012C */  b     .L0044A5B0
/* 0044A100 304400FF */   andi  $a0, $v0, 0xff
.L0044A104:
/* 0044A104 00197023 */  negu  $t6, $t9
/* 0044A108 3C011B00 */  lui   $at, 0x1b00
/* 0044A10C 01C16824 */  and   $t5, $t6, $at
/* 0044A110 00CD7804 */  sllv  $t7, $t5, $a2
/* 0044A114 05E10007 */  bgez  $t7, .L0044A134
/* 0044A118 00000000 */   nop   
/* 0044A11C 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044A120 8FA40038 */  lw    $a0, 0x38($sp)
/* 0044A124 0320F809 */  jalr  $t9
/* 0044A128 00000000 */   nop   
/* 0044A12C 10400003 */  beqz  $v0, .L0044A13C
/* 0044A130 8FBC0020 */   lw    $gp, 0x20($sp)
.L0044A134:
/* 0044A134 1000011E */  b     .L0044A5B0
/* 0044A138 00002025 */   move  $a0, $zero
.L0044A13C:
/* 0044A13C 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044A140 8E040038 */  lw    $a0, 0x38($s0)
/* 0044A144 0320F809 */  jalr  $t9
/* 0044A148 2484002C */   addiu $a0, $a0, 0x2c
/* 0044A14C 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A150 24090001 */  li    $t1, 1
/* 0044A154 14400012 */  bnez  $v0, .L0044A1A0
/* 0044A158 00401825 */   move  $v1, $v0
/* 0044A15C 8F988A5C */  lw     $t8, %got(lang)($gp)
/* 0044A160 8FAC0048 */  lw    $t4, 0x48($sp)
/* 0044A164 93180000 */  lbu   $t8, ($t8)
/* 0044A168 01381826 */  xor   $v1, $t1, $t8
/* 0044A16C 2C630001 */  sltiu $v1, $v1, 1
/* 0044A170 1060000B */  beqz  $v1, .L0044A1A0
/* 0044A174 00000000 */   nop   
/* 0044A178 8D820024 */  lw    $v0, 0x24($t4)
/* 0044A17C 90430000 */  lbu   $v1, ($v0)
/* 0044A180 38630003 */  xori  $v1, $v1, 3
/* 0044A184 2C630001 */  sltiu $v1, $v1, 1
/* 0044A188 10600005 */  beqz  $v1, .L0044A1A0
/* 0044A18C 00000000 */   nop   
/* 0044A190 904B002E */  lbu   $t3, 0x2e($v0)
/* 0044A194 316A0007 */  andi  $t2, $t3, 7
/* 0044A198 012A1826 */  xor   $v1, $t1, $t2
/* 0044A19C 2C630001 */  sltiu $v1, $v1, 1
.L0044A1A0:
/* 0044A1A0 10000103 */  b     .L0044A5B0
/* 0044A1A4 306400FF */   andi  $a0, $v1, 0xff
.L0044A1A8:
/* 0044A1A8 8F998A5C */  lw     $t9, %got(lang)($gp)
/* 0044A1AC 24010002 */  li    $at, 2
/* 0044A1B0 2C8E0020 */  sltiu $t6, $a0, 0x20
/* 0044A1B4 93390000 */  lbu   $t9, ($t9)
/* 0044A1B8 000E6823 */  negu  $t5, $t6
/* 0044A1BC 13210007 */  beq   $t9, $at, .L0044A1DC
/* 0044A1C0 3C011B00 */   lui   $at, 0x1b00
/* 0044A1C4 01A17824 */  and   $t7, $t5, $at
/* 0044A1C8 008FC004 */  sllv  $t8, $t7, $a0
/* 0044A1CC 07000003 */  bltz  $t8, .L0044A1DC
/* 0044A1D0 00000000 */   nop   
/* 0044A1D4 100000F6 */  b     .L0044A5B0
/* 0044A1D8 00002025 */   move  $a0, $zero
.L0044A1DC:
/* 0044A1DC 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044A1E0 8FA40030 */  lw    $a0, 0x30($sp)
/* 0044A1E4 0320F809 */  jalr  $t9
/* 0044A1E8 00000000 */   nop   
/* 0044A1EC 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A1F0 1040001B */  beqz  $v0, .L0044A260
/* 0044A1F4 24090001 */   li    $t1, 1
/* 0044A1F8 8FA20048 */  lw    $v0, 0x48($sp)
/* 0044A1FC 904C0000 */  lbu   $t4, ($v0)
/* 0044A200 258BFFE0 */  addiu $t3, $t4, -0x20
/* 0044A204 2D6A0060 */  sltiu $t2, $t3, 0x60
/* 0044A208 1140000A */  beqz  $t2, .L0044A234
/* 0044A20C 00000000 */   nop   
/* 0044A210 8F8D8044 */  lw    $t5, %got(D_10010AB0)($gp)
/* 0044A214 000BC943 */  sra   $t9, $t3, 5
/* 0044A218 00197080 */  sll   $t6, $t9, 2
/* 0044A21C 25AD0AB0 */  addiu $t5, %lo(D_10010AB0) # addiu $t5, $t5, 0xab0
/* 0044A220 01AE7821 */  addu  $t7, $t5, $t6
/* 0044A224 8DF80000 */  lw    $t8, ($t7)
/* 0044A228 01786004 */  sllv  $t4, $t8, $t3
/* 0044A22C 29990000 */  slti  $t9, $t4, 0
/* 0044A230 03205025 */  move  $t2, $t9
.L0044A234:
/* 0044A234 11400003 */  beqz  $t2, .L0044A244
/* 0044A238 00000000 */   nop   
/* 0044A23C 100000DC */  b     .L0044A5B0
/* 0044A240 00002025 */   move  $a0, $zero
.L0044A244:
/* 0044A244 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044A248 8C440024 */  lw    $a0, 0x24($v0)
/* 0044A24C 0320F809 */  jalr  $t9
/* 0044A250 00000000 */   nop   
/* 0044A254 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A258 100000D5 */  b     .L0044A5B0
/* 0044A25C 304400FF */   andi  $a0, $v0, 0xff
.L0044A260:
/* 0044A260 8FAD0048 */  lw    $t5, 0x48($sp)
/* 0044A264 91AE0000 */  lbu   $t6, ($t5)
/* 0044A268 25CFFFE0 */  addiu $t7, $t6, -0x20
/* 0044A26C 2DF80060 */  sltiu $t8, $t7, 0x60
/* 0044A270 13000009 */  beqz  $t8, .L0044A298
/* 0044A274 00000000 */   nop   
/* 0044A278 8F998044 */  lw    $t9, %got(D_10010AB0)($gp)
/* 0044A27C 000F5943 */  sra   $t3, $t7, 5
/* 0044A280 000B6080 */  sll   $t4, $t3, 2
/* 0044A284 27390AB0 */  addiu $t9, %lo(D_10010AB0) # addiu $t9, $t9, 0xab0
/* 0044A288 032C5021 */  addu  $t2, $t9, $t4
/* 0044A28C 8D4E0000 */  lw    $t6, ($t2)
/* 0044A290 01EE5804 */  sllv  $t3, $t6, $t7
/* 0044A294 29780000 */  slti  $t8, $t3, 0
.L0044A298:
/* 0044A298 1300001B */  beqz  $t8, .L0044A308
/* 0044A29C 8FAF0048 */   lw    $t7, 0x48($sp)
/* 0044A2A0 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044A2A4 8DA40004 */  lw    $a0, 4($t5)
/* 0044A2A8 0320F809 */  jalr  $t9
/* 0044A2AC 24840028 */   addiu $a0, $a0, 0x28
/* 0044A2B0 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A2B4 24090001 */  li    $t1, 1
/* 0044A2B8 14400011 */  bnez  $v0, .L0044A300
/* 0044A2BC 00401825 */   move  $v1, $v0
/* 0044A2C0 8F8C8A5C */  lw     $t4, %got(lang)($gp)
/* 0044A2C4 918C0000 */  lbu   $t4, ($t4)
/* 0044A2C8 012C1826 */  xor   $v1, $t1, $t4
/* 0044A2CC 2C630001 */  sltiu $v1, $v1, 1
/* 0044A2D0 1060000B */  beqz  $v1, .L0044A300
/* 0044A2D4 00000000 */   nop   
/* 0044A2D8 8E020038 */  lw    $v0, 0x38($s0)
/* 0044A2DC 90430000 */  lbu   $v1, ($v0)
/* 0044A2E0 38630003 */  xori  $v1, $v1, 3
/* 0044A2E4 2C630001 */  sltiu $v1, $v1, 1
/* 0044A2E8 10600005 */  beqz  $v1, .L0044A300
/* 0044A2EC 00000000 */   nop   
/* 0044A2F0 904A002E */  lbu   $t2, 0x2e($v0)
/* 0044A2F4 314E0007 */  andi  $t6, $t2, 7
/* 0044A2F8 012E1826 */  xor   $v1, $t1, $t6
/* 0044A2FC 2C630001 */  sltiu $v1, $v1, 1
.L0044A300:
/* 0044A300 100000AB */  b     .L0044A5B0
/* 0044A304 306400FF */   andi  $a0, $v1, 0xff
.L0044A308:
/* 0044A308 8DE80024 */  lw    $t0, 0x24($t7)
/* 0044A30C 3C014400 */  lui   $at, 0x4400
/* 0044A310 91060000 */  lbu   $a2, ($t0)
/* 0044A314 2CCB0020 */  sltiu $t3, $a2, 0x20
/* 0044A318 000BC823 */  negu  $t9, $t3
/* 0044A31C 0321C024 */  and   $t8, $t9, $at
/* 0044A320 00D86804 */  sllv  $t5, $t8, $a2
/* 0044A324 05A1001B */  bgez  $t5, .L0044A394
/* 0044A328 00000000 */   nop   
/* 0044A32C 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044A330 2504002C */  addiu $a0, $t0, 0x2c
/* 0044A334 0320F809 */  jalr  $t9
/* 0044A338 00000000 */   nop   
/* 0044A33C 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A340 24090001 */  li    $t1, 1
/* 0044A344 14400011 */  bnez  $v0, .L0044A38C
/* 0044A348 00401825 */   move  $v1, $v0
/* 0044A34C 8F8C8A5C */  lw     $t4, %got(lang)($gp)
/* 0044A350 918C0000 */  lbu   $t4, ($t4)
/* 0044A354 012C1826 */  xor   $v1, $t1, $t4
/* 0044A358 2C630001 */  sltiu $v1, $v1, 1
/* 0044A35C 1060000B */  beqz  $v1, .L0044A38C
/* 0044A360 00000000 */   nop   
/* 0044A364 8E020038 */  lw    $v0, 0x38($s0)
/* 0044A368 90430000 */  lbu   $v1, ($v0)
/* 0044A36C 38630003 */  xori  $v1, $v1, 3
/* 0044A370 2C630001 */  sltiu $v1, $v1, 1
/* 0044A374 10600005 */  beqz  $v1, .L0044A38C
/* 0044A378 00000000 */   nop   
/* 0044A37C 904A002E */  lbu   $t2, 0x2e($v0)
/* 0044A380 314E0007 */  andi  $t6, $t2, 7
/* 0044A384 012E1826 */  xor   $v1, $t1, $t6
/* 0044A388 2C630001 */  sltiu $v1, $v1, 1
.L0044A38C:
/* 0044A38C 10000088 */  b     .L0044A5B0
/* 0044A390 306400FF */   andi  $a0, $v1, 0xff
.L0044A394:
/* 0044A394 8F858A5C */  lw     $a1, %got(lang)($gp)
/* 0044A398 24010002 */  li    $at, 2
/* 0044A39C 2CCF0020 */  sltiu $t7, $a2, 0x20
/* 0044A3A0 90A50000 */  lbu   $a1, ($a1)
/* 0044A3A4 000F5823 */  negu  $t3, $t7
/* 0044A3A8 10A10008 */  beq   $a1, $at, .L0044A3CC
/* 0044A3AC 2CAD0020 */   sltiu $t5, $a1, 0x20
/* 0044A3B0 3C011B00 */  lui   $at, 0x1b00
/* 0044A3B4 0161C824 */  and   $t9, $t3, $at
/* 0044A3B8 00D9C004 */  sllv  $t8, $t9, $a2
/* 0044A3BC 07020004 */  bltzl $t8, .L0044A3D0
/* 0044A3C0 000D6023 */   negu  $t4, $t5
/* 0044A3C4 1000007A */  b     .L0044A5B0
/* 0044A3C8 00002025 */   move  $a0, $zero
.L0044A3CC:
/* 0044A3CC 000D6023 */  negu  $t4, $t5
.L0044A3D0:
/* 0044A3D0 3C01C400 */  lui   $at, 0xc400
/* 0044A3D4 01815024 */  and   $t2, $t4, $at
/* 0044A3D8 00AA1804 */  sllv  $v1, $t2, $a1
/* 0044A3DC 28630000 */  slti  $v1, $v1, 0
/* 0044A3E0 2C630001 */  sltiu $v1, $v1, 1
/* 0044A3E4 14600048 */  bnez  $v1, .L0044A508
/* 0044A3E8 00000000 */   nop   
/* 0044A3EC 01251826 */  xor   $v1, $t1, $a1
/* 0044A3F0 2C630001 */  sltiu $v1, $v1, 1
/* 0044A3F4 10600008 */  beqz  $v1, .L0044A418
/* 0044A3F8 00000000 */   nop   
/* 0044A3FC 8F838AE0 */  lw     $v1, %got(nof77alias)($gp)
/* 0044A400 90630000 */  lbu   $v1, ($v1)
/* 0044A404 2C630001 */  sltiu $v1, $v1, 1
/* 0044A408 1460003F */  bnez  $v1, .L0044A508
/* 0044A40C 00000000 */   nop   
/* 0044A410 8F838B44 */  lw     $v1, %got(use_c_semantics)($gp)
/* 0044A414 90630000 */  lbu   $v1, ($v1)
.L0044A418:
/* 0044A418 1460003B */  bnez  $v1, .L0044A508
/* 0044A41C 2CA20001 */   sltiu $v0, $a1, 1
/* 0044A420 10400008 */  beqz  $v0, .L0044A444
/* 0044A424 00401825 */   move  $v1, $v0
/* 0044A428 8F838AE4 */  lw     $v1, %got(nopalias)($gp)
/* 0044A42C 90630000 */  lbu   $v1, ($v1)
/* 0044A430 10600004 */  beqz  $v1, .L0044A444
/* 0044A434 00000000 */   nop   
/* 0044A438 8F838AE0 */  lw     $v1, %got(nof77alias)($gp)
/* 0044A43C 90630000 */  lbu   $v1, ($v1)
/* 0044A440 2C630001 */  sltiu $v1, $v1, 1
.L0044A444:
/* 0044A444 14600030 */  bnez  $v1, .L0044A508
/* 0044A448 00000000 */   nop   
/* 0044A44C 10400004 */  beqz  $v0, .L0044A460
/* 0044A450 00401825 */   move  $v1, $v0
/* 0044A454 8F838AE4 */  lw     $v1, %got(nopalias)($gp)
/* 0044A458 90630000 */  lbu   $v1, ($v1)
/* 0044A45C 2C630001 */  sltiu $v1, $v1, 1
.L0044A460:
/* 0044A460 14600029 */  bnez  $v1, .L0044A508
/* 0044A464 00000000 */   nop   
/* 0044A468 8E020038 */  lw    $v0, 0x38($s0)
/* 0044A46C 90440000 */  lbu   $a0, ($v0)
/* 0044A470 38830007 */  xori  $v1, $a0, 7
/* 0044A474 2C630001 */  sltiu $v1, $v1, 1
/* 0044A478 14600023 */  bnez  $v1, .L0044A508
/* 0044A47C 00000000 */   nop   
/* 0044A480 38C30007 */  xori  $v1, $a2, 7
/* 0044A484 2C630001 */  sltiu $v1, $v1, 1
/* 0044A488 1460001F */  bnez  $v1, .L0044A508
/* 0044A48C 00000000 */   nop   
/* 0044A490 01021826 */  xor   $v1, $t0, $v0
/* 0044A494 2C630001 */  sltiu $v1, $v1, 1
/* 0044A498 1460001B */  bnez  $v1, .L0044A508
/* 0044A49C 2C8E0020 */   sltiu $t6, $a0, 0x20
/* 0044A4A0 000E7823 */  negu  $t7, $t6
/* 0044A4A4 3C011200 */  lui   $at, 0x1200
/* 0044A4A8 01E15824 */  and   $t3, $t7, $at
/* 0044A4AC 008B1804 */  sllv  $v1, $t3, $a0
/* 0044A4B0 28630000 */  slti  $v1, $v1, 0
/* 0044A4B4 10600014 */  beqz  $v1, .L0044A508
/* 0044A4B8 3C011200 */   lui   $at, 0x1200
/* 0044A4BC 2CD90020 */  sltiu $t9, $a2, 0x20
/* 0044A4C0 0019C023 */  negu  $t8, $t9
/* 0044A4C4 03016824 */  and   $t5, $t8, $at
/* 0044A4C8 00CD1804 */  sllv  $v1, $t5, $a2
/* 0044A4CC 28630000 */  slti  $v1, $v1, 0
/* 0044A4D0 1060000D */  beqz  $v1, .L0044A508
/* 0044A4D4 00000000 */   nop   
/* 0044A4D8 8C440028 */  lw    $a0, 0x28($v0)
/* 0044A4DC 8C45002C */  lw    $a1, 0x2c($v0)
/* 0044A4E0 8F99860C */  lw    $t9, %call16(addreq)($gp)
/* 0044A4E4 AFA40000 */  sw    $a0, ($sp)
/* 0044A4E8 AFA50004 */  sw    $a1, 4($sp)
/* 0044A4EC 8D060028 */  lw    $a2, 0x28($t0)
/* 0044A4F0 AFA60008 */  sw    $a2, 8($sp)
/* 0044A4F4 8D07002C */  lw    $a3, 0x2c($t0)
/* 0044A4F8 0320F809 */  jalr  $t9
/* 0044A4FC AFA7000C */   sw    $a3, 0xc($sp)
/* 0044A500 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A504 00401825 */  move  $v1, $v0
.L0044A508:
/* 0044A508 10000029 */  b     .L0044A5B0
/* 0044A50C 306400FF */   andi  $a0, $v1, 0xff
.L0044A510:
/* 0044A510 8F8B8A60 */  lw     $t3, %got(inlopt)($gp)
/* 0044A514 916B0000 */  lbu   $t3, ($t3)
/* 0044A518 51600026 */  beql  $t3, $zero, .L0044A5B4
/* 0044A51C 00801025 */   move  $v0, $a0
/* 0044A520 10800023 */  beqz  $a0, .L0044A5B0
/* 0044A524 24010036 */   li    $at, 54
/* 0044A528 54A10022 */  bnel  $a1, $at, .L0044A5B4
/* 0044A52C 00801025 */   move  $v0, $a0
/* 0044A530 8FB90048 */  lw    $t9, 0x48($sp)
/* 0044A534 2401003F */  li    $at, 63
/* 0044A538 93380000 */  lbu   $t8, ($t9)
/* 0044A53C 5701001D */  bnel  $t8, $at, .L0044A5B4
/* 0044A540 00801025 */   move  $v0, $a0
/* 0044A544 8E0D0034 */  lw    $t5, 0x34($s0)
/* 0044A548 8F2C0024 */  lw    $t4, 0x24($t9)
/* 0044A54C 55AC0019 */  bnel  $t5, $t4, .L0044A5B4
/* 0044A550 00801025 */   move  $v0, $a0
/* 0044A554 8E0A0024 */  lw    $t2, 0x24($s0)
/* 0044A558 8F2E0004 */  lw    $t6, 4($t9)
/* 0044A55C 554E0015 */  bnel  $t2, $t6, .L0044A5B4
/* 0044A560 00801025 */   move  $v0, $a0
/* 0044A564 8F220030 */  lw    $v0, 0x30($t9)
/* 0044A568 8E04002C */  lw    $a0, 0x2c($s0)
/* 0044A56C 0044182A */  slt   $v1, $v0, $a0
/* 0044A570 38630001 */  xori  $v1, $v1, 1
/* 0044A574 10600004 */  beqz  $v1, .L0044A588
/* 0044A578 00000000 */   nop   
/* 0044A57C 960B003C */  lhu   $t3, 0x3c($s0)
/* 0044A580 00447823 */  subu  $t7, $v0, $a0
/* 0044A584 01EB182A */  slt   $v1, $t7, $t3
.L0044A588:
/* 0044A588 54600009 */  bnezl $v1, .L0044A5B0
/* 0044A58C 306400FF */   andi  $a0, $v1, 0xff
/* 0044A590 0082182A */  slt   $v1, $a0, $v0
/* 0044A594 38630001 */  xori  $v1, $v1, 1
/* 0044A598 10600004 */  beqz  $v1, .L0044A5AC
/* 0044A59C 8FAD0048 */   lw    $t5, 0x48($sp)
/* 0044A5A0 8DAC0020 */  lw    $t4, 0x20($t5)
/* 0044A5A4 0082C023 */  subu  $t8, $a0, $v0
/* 0044A5A8 030C182A */  slt   $v1, $t8, $t4
.L0044A5AC:
/* 0044A5AC 306400FF */  andi  $a0, $v1, 0xff
.L0044A5B0:
/* 0044A5B0 00801025 */  move  $v0, $a0
.L0044A5B4:
/* 0044A5B4 8FBF0024 */  lw    $ra, 0x24($sp)
/* 0044A5B8 8FB0001C */  lw    $s0, 0x1c($sp)
/* 0044A5BC 27BD0048 */  addiu $sp, $sp, 0x48
/* 0044A5C0 03E00008 */  jr    $ra
/* 0044A5C4 00000000 */   nop   
    .type slkilled, @function
    .size slkilled, .-slkilled
    .end slkilled

glabel smkilled
    .ent smkilled
    # 0044B25C movkillprev
    # 0044B308 strkillprev
    # 0044D8C4 strlkilled
    # 0044D9E4 strskilled
    # 0044DADC strlant
/* 0044A5C8 3C1C0FBD */  .cpload $t9
/* 0044A5CC 279CFCC8 */  
/* 0044A5D0 0399E021 */  
/* 0044A5D4 27BDFFC0 */  addiu $sp, $sp, -0x40
/* 0044A5D8 AFBF0024 */  sw    $ra, 0x24($sp)
/* 0044A5DC AFBC0020 */  sw    $gp, 0x20($sp)
/* 0044A5E0 AFA40040 */  sw    $a0, 0x40($sp)
/* 0044A5E4 AFA50044 */  sw    $a1, 0x44($sp)
/* 0044A5E8 90820000 */  lbu   $v0, ($a0)
/* 0044A5EC 244FFF80 */  addiu $t7, $v0, -0x80
/* 0044A5F0 2DF80020 */  sltiu $t8, $t7, 0x20
/* 0044A5F4 0018C823 */  negu  $t9, $t8
/* 0044A5F8 332B0810 */  andi  $t3, $t9, 0x810
/* 0044A5FC 01EB6004 */  sllv  $t4, $t3, $t7
/* 0044A600 05810003 */  bgez  $t4, .L0044A610
/* 0044A604 244DFFE0 */   addiu $t5, $v0, -0x20
/* 0044A608 1000012D */  b     .L0044AAC0
/* 0044A60C 24020001 */   li    $v0, 1
.L0044A610:
/* 0044A610 2DAE0060 */  sltiu $t6, $t5, 0x60
/* 0044A614 11C00009 */  beqz  $t6, .L0044A63C
/* 0044A618 00000000 */   nop   
/* 0044A61C 8F8B8044 */  lw    $t3, %got(D_10010ABC)($gp)
/* 0044A620 000DC143 */  sra   $t8, $t5, 5
/* 0044A624 0018C880 */  sll   $t9, $t8, 2
/* 0044A628 256B0ABC */  addiu $t3, %lo(D_10010ABC) # addiu $t3, $t3, 0xabc
/* 0044A62C 01797821 */  addu  $t7, $t3, $t9
/* 0044A630 8DEC0000 */  lw    $t4, ($t7)
/* 0044A634 01ACC004 */  sllv  $t8, $t4, $t5
/* 0044A638 2B0E0000 */  slti  $t6, $t8, 0
.L0044A63C:
/* 0044A63C 15C0000B */  bnez  $t6, .L0044A66C
/* 0044A640 8FB90040 */   lw    $t9, 0x40($sp)
/* 0044A644 8F2F0024 */  lw    $t7, 0x24($t9)
/* 0044A648 24010007 */  li    $at, 7
/* 0044A64C 03202025 */  move  $a0, $t9
/* 0044A650 91EC0000 */  lbu   $t4, ($t7)
/* 0044A654 55810006 */  bnel  $t4, $at, .L0044A670
/* 0044A658 8FAD0044 */   lw    $t5, 0x44($sp)
/* 0044A65C 8F99862C */  lw    $t9, %call16(fix_sbase)($gp)
/* 0044A660 0320F809 */  jalr  $t9
/* 0044A664 00000000 */   nop   
/* 0044A668 8FBC0020 */  lw    $gp, 0x20($sp)
.L0044A66C:
/* 0044A66C 8FAD0044 */  lw    $t5, 0x44($sp)
.L0044A670:
/* 0044A670 3C014400 */  lui   $at, 0x4400
/* 0044A674 8FB90040 */  lw    $t9, 0x40($sp)
/* 0044A678 8DB8002C */  lw    $t8, 0x2c($t5)
/* 0044A67C AFB80028 */  sw    $t8, 0x28($sp)
/* 0044A680 93020000 */  lbu   $v0, ($t8)
/* 0044A684 2C4B0020 */  sltiu $t3, $v0, 0x20
/* 0044A688 000B7023 */  negu  $t6, $t3
/* 0044A68C 01C17824 */  and   $t7, $t6, $at
/* 0044A690 004F6004 */  sllv  $t4, $t7, $v0
/* 0044A694 05810058 */  bgez  $t4, .L0044A7F8
/* 0044A698 00000000 */   nop   
/* 0044A69C 932D0000 */  lbu   $t5, ($t9)
/* 0044A6A0 25B8FFE0 */  addiu $t8, $t5, -0x20
/* 0044A6A4 2F0B0060 */  sltiu $t3, $t8, 0x60
/* 0044A6A8 11600009 */  beqz  $t3, .L0044A6D0
/* 0044A6AC 00000000 */   nop   
/* 0044A6B0 8F8C8044 */  lw    $t4, %got(D_10010ABC)($gp)
/* 0044A6B4 00187143 */  sra   $t6, $t8, 5
/* 0044A6B8 000E7880 */  sll   $t7, $t6, 2
/* 0044A6BC 258C0ABC */  addiu $t4, %lo(D_10010ABC) # addiu $t4, $t4, 0xabc
/* 0044A6C0 018F6821 */  addu  $t5, $t4, $t7
/* 0044A6C4 8DAE0000 */  lw    $t6, ($t5)
/* 0044A6C8 030E6004 */  sllv  $t4, $t6, $t8
/* 0044A6CC 298B0000 */  slti  $t3, $t4, 0
.L0044A6D0:
/* 0044A6D0 11600015 */  beqz  $t3, .L0044A728
/* 0044A6D4 8FB80040 */   lw    $t8, 0x40($sp)
/* 0044A6D8 8FAD0028 */  lw    $t5, 0x28($sp)
/* 0044A6DC 8DA4002C */  lw    $a0, 0x2c($t5)
/* 0044A6E0 AFA40000 */  sw    $a0, ($sp)
/* 0044A6E4 8DAE0030 */  lw    $t6, 0x30($t5)
/* 0044A6E8 AFAE0004 */  sw    $t6, 4($sp)
/* 0044A6EC 8F220004 */  lw    $v0, 4($t9)
/* 0044A6F0 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044A6F4 8FA50004 */  lw    $a1, 4($sp)
/* 0044A6F8 8C460028 */  lw    $a2, 0x28($v0)
/* 0044A6FC 904E0020 */  lbu   $t6, 0x20($v0)
/* 0044A700 AFA60008 */  sw    $a2, 8($sp)
/* 0044A704 8C47002C */  lw    $a3, 0x2c($v0)
/* 0044A708 AFA7000C */  sw    $a3, 0xc($sp)
/* 0044A70C 8DAB0024 */  lw    $t3, 0x24($t5)
/* 0044A710 AFAE0014 */  sw    $t6, 0x14($sp)
/* 0044A714 0320F809 */  jalr  $t9
/* 0044A718 AFAB0010 */   sw    $t3, 0x10($sp)
/* 0044A71C 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A720 100000E6 */  b     .L0044AABC
/* 0044A724 304400FF */   andi  $a0, $v0, 0xff
.L0044A728:
/* 0044A728 8F190024 */  lw    $t9, 0x24($t8)
/* 0044A72C 3C014400 */  lui   $at, 0x4400
/* 0044A730 8FAE0028 */  lw    $t6, 0x28($sp)
/* 0044A734 AFB90030 */  sw    $t9, 0x30($sp)
/* 0044A738 93250000 */  lbu   $a1, ($t9)
/* 0044A73C 2CAC0020 */  sltiu $t4, $a1, 0x20
/* 0044A740 000C7823 */  negu  $t7, $t4
/* 0044A744 01E16824 */  and   $t5, $t7, $at
/* 0044A748 00AD5804 */  sllv  $t3, $t5, $a1
/* 0044A74C 05610014 */  bgez  $t3, .L0044A7A0
/* 0044A750 2CAC0020 */   sltiu $t4, $a1, 0x20
/* 0044A754 8DC4002C */  lw    $a0, 0x2c($t6)
/* 0044A758 AFA40000 */  sw    $a0, ($sp)
/* 0044A75C 8DD80030 */  lw    $t8, 0x30($t6)
/* 0044A760 AFB80004 */  sw    $t8, 4($sp)
/* 0044A764 8F26002C */  lw    $a2, 0x2c($t9)
/* 0044A768 8FA50004 */  lw    $a1, 4($sp)
/* 0044A76C AFA60008 */  sw    $a2, 8($sp)
/* 0044A770 8F270030 */  lw    $a3, 0x30($t9)
/* 0044A774 AFA7000C */  sw    $a3, 0xc($sp)
/* 0044A778 8DCB0024 */  lw    $t3, 0x24($t6)
/* 0044A77C AFAB0010 */  sw    $t3, 0x10($sp)
/* 0044A780 8F380024 */  lw    $t8, 0x24($t9)
/* 0044A784 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044A788 AFB80014 */  sw    $t8, 0x14($sp)
/* 0044A78C 0320F809 */  jalr  $t9
/* 0044A790 00000000 */   nop   
/* 0044A794 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A798 100000C8 */  b     .L0044AABC
/* 0044A79C 304400FF */   andi  $a0, $v0, 0xff
.L0044A7A0:
/* 0044A7A0 000C7823 */  negu  $t7, $t4
/* 0044A7A4 3C011B00 */  lui   $at, 0x1b00
/* 0044A7A8 01E16824 */  and   $t5, $t7, $at
/* 0044A7AC 00AD7004 */  sllv  $t6, $t5, $a1
/* 0044A7B0 05C10008 */  bgez  $t6, .L0044A7D4
/* 0044A7B4 00000000 */   nop   
/* 0044A7B8 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044A7BC 8FA40030 */  lw    $a0, 0x30($sp)
/* 0044A7C0 0320F809 */  jalr  $t9
/* 0044A7C4 00000000 */   nop   
/* 0044A7C8 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A7CC 10400003 */  beqz  $v0, .L0044A7DC
/* 0044A7D0 8FAA0044 */   lw    $t2, 0x44($sp)
.L0044A7D4:
/* 0044A7D4 100000B9 */  b     .L0044AABC
/* 0044A7D8 00002025 */   move  $a0, $zero
.L0044A7DC:
/* 0044A7DC 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044A7E0 8D44002C */  lw    $a0, 0x2c($t2)
/* 0044A7E4 0320F809 */  jalr  $t9
/* 0044A7E8 2484002C */   addiu $a0, $a0, 0x2c
/* 0044A7EC 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A7F0 100000B2 */  b     .L0044AABC
/* 0044A7F4 304400FF */   andi  $a0, $v0, 0xff
.L0044A7F8:
/* 0044A7F8 8F8B8A5C */  lw     $t3, %got(lang)($gp)
/* 0044A7FC 24010002 */  li    $at, 2
/* 0044A800 2C590020 */  sltiu $t9, $v0, 0x20
/* 0044A804 916B0000 */  lbu   $t3, ($t3)
/* 0044A808 0019C023 */  negu  $t8, $t9
/* 0044A80C 11610007 */  beq   $t3, $at, .L0044A82C
/* 0044A810 3C011B00 */   lui   $at, 0x1b00
/* 0044A814 03016024 */  and   $t4, $t8, $at
/* 0044A818 004C7804 */  sllv  $t7, $t4, $v0
/* 0044A81C 05E00003 */  bltz  $t7, .L0044A82C
/* 0044A820 00000000 */   nop   
/* 0044A824 100000A5 */  b     .L0044AABC
/* 0044A828 00002025 */   move  $a0, $zero
.L0044A82C:
/* 0044A82C 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044A830 8FA40028 */  lw    $a0, 0x28($sp)
/* 0044A834 0320F809 */  jalr  $t9
/* 0044A838 00000000 */   nop   
/* 0044A83C 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A840 8FA30040 */  lw    $v1, 0x40($sp)
/* 0044A844 10400019 */  beqz  $v0, .L0044A8AC
/* 0044A848 8FAA0044 */   lw    $t2, 0x44($sp)
/* 0044A84C 906D0000 */  lbu   $t5, ($v1)
/* 0044A850 25AEFFE0 */  addiu $t6, $t5, -0x20
/* 0044A854 2DCB0060 */  sltiu $t3, $t6, 0x60
/* 0044A858 11600009 */  beqz  $t3, .L0044A880
/* 0044A85C 00000000 */   nop   
/* 0044A860 8F8C8044 */  lw    $t4, %got(D_10010ABC)($gp)
/* 0044A864 000EC943 */  sra   $t9, $t6, 5
/* 0044A868 0019C080 */  sll   $t8, $t9, 2
/* 0044A86C 258C0ABC */  addiu $t4, %lo(D_10010ABC) # addiu $t4, $t4, 0xabc
/* 0044A870 01987821 */  addu  $t7, $t4, $t8
/* 0044A874 8DED0000 */  lw    $t5, ($t7)
/* 0044A878 01CDC804 */  sllv  $t9, $t5, $t6
/* 0044A87C 2B2B0000 */  slti  $t3, $t9, 0
.L0044A880:
/* 0044A880 11600003 */  beqz  $t3, .L0044A890
/* 0044A884 00000000 */   nop   
/* 0044A888 1000008C */  b     .L0044AABC
/* 0044A88C 00002025 */   move  $a0, $zero
.L0044A890:
/* 0044A890 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044A894 8C640024 */  lw    $a0, 0x24($v1)
/* 0044A898 0320F809 */  jalr  $t9
/* 0044A89C 00000000 */   nop   
/* 0044A8A0 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A8A4 10000085 */  b     .L0044AABC
/* 0044A8A8 304400FF */   andi  $a0, $v0, 0xff
.L0044A8AC:
/* 0044A8AC 90780000 */  lbu   $t8, ($v1)
/* 0044A8B0 270FFFE0 */  addiu $t7, $t8, -0x20
/* 0044A8B4 2DED0060 */  sltiu $t5, $t7, 0x60
/* 0044A8B8 11A00009 */  beqz  $t5, .L0044A8E0
/* 0044A8BC 00000000 */   nop   
/* 0044A8C0 8F8C8044 */  lw    $t4, %got(D_10010ABC)($gp)
/* 0044A8C4 000F7143 */  sra   $t6, $t7, 5
/* 0044A8C8 000EC880 */  sll   $t9, $t6, 2
/* 0044A8CC 258C0ABC */  addiu $t4, %lo(D_10010ABC) # addiu $t4, $t4, 0xabc
/* 0044A8D0 01995821 */  addu  $t3, $t4, $t9
/* 0044A8D4 8D780000 */  lw    $t8, ($t3)
/* 0044A8D8 01F87004 */  sllv  $t6, $t8, $t7
/* 0044A8DC 29CD0000 */  slti  $t5, $t6, 0
.L0044A8E0:
/* 0044A8E0 51A00009 */  beql  $t5, $zero, .L0044A908
/* 0044A8E4 8C690024 */   lw    $t1, 0x24($v1)
/* 0044A8E8 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044A8EC 8C640004 */  lw    $a0, 4($v1)
/* 0044A8F0 0320F809 */  jalr  $t9
/* 0044A8F4 24840028 */   addiu $a0, $a0, 0x28
/* 0044A8F8 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A8FC 1000006F */  b     .L0044AABC
/* 0044A900 304400FF */   andi  $a0, $v0, 0xff
/* 0044A904 8C690024 */  lw    $t1, 0x24($v1)
.L0044A908:
/* 0044A908 3C014400 */  lui   $at, 0x4400
/* 0044A90C 91250000 */  lbu   $a1, ($t1)
/* 0044A910 2CB90020 */  sltiu $t9, $a1, 0x20
/* 0044A914 00195823 */  negu  $t3, $t9
/* 0044A918 0161C024 */  and   $t8, $t3, $at
/* 0044A91C 00B87804 */  sllv  $t7, $t8, $a1
/* 0044A920 05E10008 */  bgez  $t7, .L0044A944
/* 0044A924 00000000 */   nop   
/* 0044A928 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044A92C 2524002C */  addiu $a0, $t1, 0x2c
/* 0044A930 0320F809 */  jalr  $t9
/* 0044A934 00000000 */   nop   
/* 0044A938 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044A93C 1000005F */  b     .L0044AABC
/* 0044A940 304400FF */   andi  $a0, $v0, 0xff
.L0044A944:
/* 0044A944 8F828A5C */  lw     $v0, %got(lang)($gp)
/* 0044A948 24010002 */  li    $at, 2
/* 0044A94C 2CAE0020 */  sltiu $t6, $a1, 0x20
/* 0044A950 90420000 */  lbu   $v0, ($v0)
/* 0044A954 000E6023 */  negu  $t4, $t6
/* 0044A958 10410008 */  beq   $v0, $at, .L0044A97C
/* 0044A95C 2C4B0020 */   sltiu $t3, $v0, 0x20
/* 0044A960 3C011B00 */  lui   $at, 0x1b00
/* 0044A964 01816824 */  and   $t5, $t4, $at
/* 0044A968 00ADC804 */  sllv  $t9, $t5, $a1
/* 0044A96C 07220004 */  bltzl $t9, .L0044A980
/* 0044A970 000BC023 */   negu  $t8, $t3
/* 0044A974 10000051 */  b     .L0044AABC
/* 0044A978 00002025 */   move  $a0, $zero
.L0044A97C:
/* 0044A97C 000BC023 */  negu  $t8, $t3
.L0044A980:
/* 0044A980 3C01C400 */  lui   $at, 0xc400
/* 0044A984 03017824 */  and   $t7, $t8, $at
/* 0044A988 004F1804 */  sllv  $v1, $t7, $v0
/* 0044A98C 28630000 */  slti  $v1, $v1, 0
/* 0044A990 2C630001 */  sltiu $v1, $v1, 1
/* 0044A994 54600049 */  bnezl $v1, .L0044AABC
/* 0044A998 306400FF */   andi  $a0, $v1, 0xff
/* 0044A99C 38430001 */  xori  $v1, $v0, 1
/* 0044A9A0 2C630001 */  sltiu $v1, $v1, 1
/* 0044A9A4 10600008 */  beqz  $v1, .L0044A9C8
/* 0044A9A8 00000000 */   nop   
/* 0044A9AC 8F838AE0 */  lw     $v1, %got(nof77alias)($gp)
/* 0044A9B0 90630000 */  lbu   $v1, ($v1)
/* 0044A9B4 2C630001 */  sltiu $v1, $v1, 1
/* 0044A9B8 54600040 */  bnezl $v1, .L0044AABC
/* 0044A9BC 306400FF */   andi  $a0, $v1, 0xff
/* 0044A9C0 8F838B44 */  lw     $v1, %got(use_c_semantics)($gp)
/* 0044A9C4 90630000 */  lbu   $v1, ($v1)
.L0044A9C8:
/* 0044A9C8 1460003B */  bnez  $v1, .L0044AAB8
/* 0044A9CC 2C440001 */   sltiu $a0, $v0, 1
/* 0044A9D0 10800008 */  beqz  $a0, .L0044A9F4
/* 0044A9D4 00801825 */   move  $v1, $a0
/* 0044A9D8 8F838AE4 */  lw     $v1, %got(nopalias)($gp)
/* 0044A9DC 90630000 */  lbu   $v1, ($v1)
/* 0044A9E0 10600004 */  beqz  $v1, .L0044A9F4
/* 0044A9E4 00000000 */   nop   
/* 0044A9E8 8F838AE0 */  lw     $v1, %got(nof77alias)($gp)
/* 0044A9EC 90630000 */  lbu   $v1, ($v1)
/* 0044A9F0 2C630001 */  sltiu $v1, $v1, 1
.L0044A9F4:
/* 0044A9F4 54600031 */  bnezl $v1, .L0044AABC
/* 0044A9F8 306400FF */   andi  $a0, $v1, 0xff
/* 0044A9FC 10800004 */  beqz  $a0, .L0044AA10
/* 0044AA00 00801825 */   move  $v1, $a0
/* 0044AA04 8F838AE4 */  lw     $v1, %got(nopalias)($gp)
/* 0044AA08 90630000 */  lbu   $v1, ($v1)
/* 0044AA0C 2C630001 */  sltiu $v1, $v1, 1
.L0044AA10:
/* 0044AA10 5460002A */  bnezl $v1, .L0044AABC
/* 0044AA14 306400FF */   andi  $a0, $v1, 0xff
/* 0044AA18 8D48002C */  lw    $t0, 0x2c($t2)
/* 0044AA1C 91020000 */  lbu   $v0, ($t0)
/* 0044AA20 38430007 */  xori  $v1, $v0, 7
/* 0044AA24 2C630001 */  sltiu $v1, $v1, 1
/* 0044AA28 54600024 */  bnezl $v1, .L0044AABC
/* 0044AA2C 306400FF */   andi  $a0, $v1, 0xff
/* 0044AA30 38A30007 */  xori  $v1, $a1, 7
/* 0044AA34 2C630001 */  sltiu $v1, $v1, 1
/* 0044AA38 54600020 */  bnezl $v1, .L0044AABC
/* 0044AA3C 306400FF */   andi  $a0, $v1, 0xff
/* 0044AA40 01281826 */  xor   $v1, $t1, $t0
/* 0044AA44 2C630001 */  sltiu $v1, $v1, 1
/* 0044AA48 1460001B */  bnez  $v1, .L0044AAB8
/* 0044AA4C 2C4E0020 */   sltiu $t6, $v0, 0x20
/* 0044AA50 000E6023 */  negu  $t4, $t6
/* 0044AA54 3C011200 */  lui   $at, 0x1200
/* 0044AA58 01816824 */  and   $t5, $t4, $at
/* 0044AA5C 004D1804 */  sllv  $v1, $t5, $v0
/* 0044AA60 28630000 */  slti  $v1, $v1, 0
/* 0044AA64 10600014 */  beqz  $v1, .L0044AAB8
/* 0044AA68 3C011200 */   lui   $at, 0x1200
/* 0044AA6C 2CB90020 */  sltiu $t9, $a1, 0x20
/* 0044AA70 00195823 */  negu  $t3, $t9
/* 0044AA74 0161C024 */  and   $t8, $t3, $at
/* 0044AA78 00B81804 */  sllv  $v1, $t8, $a1
/* 0044AA7C 28630000 */  slti  $v1, $v1, 0
/* 0044AA80 5060000E */  beql  $v1, $zero, .L0044AABC
/* 0044AA84 306400FF */   andi  $a0, $v1, 0xff
/* 0044AA88 8D040028 */  lw    $a0, 0x28($t0)
/* 0044AA8C 8D05002C */  lw    $a1, 0x2c($t0)
/* 0044AA90 8F99860C */  lw    $t9, %call16(addreq)($gp)
/* 0044AA94 AFA40000 */  sw    $a0, ($sp)
/* 0044AA98 AFA50004 */  sw    $a1, 4($sp)
/* 0044AA9C 8D260028 */  lw    $a2, 0x28($t1)
/* 0044AAA0 AFA60008 */  sw    $a2, 8($sp)
/* 0044AAA4 8D27002C */  lw    $a3, 0x2c($t1)
/* 0044AAA8 0320F809 */  jalr  $t9
/* 0044AAAC AFA7000C */   sw    $a3, 0xc($sp)
/* 0044AAB0 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044AAB4 00401825 */  move  $v1, $v0
.L0044AAB8:
/* 0044AAB8 306400FF */  andi  $a0, $v1, 0xff
.L0044AABC:
/* 0044AABC 00801025 */  move  $v0, $a0
.L0044AAC0:
/* 0044AAC0 8FBF0024 */  lw    $ra, 0x24($sp)
/* 0044AAC4 27BD0040 */  addiu $sp, $sp, 0x40
/* 0044AAC8 03E00008 */  jr    $ra
/* 0044AACC 00000000 */   nop   
    .type smkilled, @function
    .size smkilled, .-smkilled
    .end smkilled

glabel sskilled
    .ent sskilled
    # 0044B308 strkillprev
    # 0044D9E4 strskilled
/* 0044AAD0 3C1C0FBD */  .cpload $t9
/* 0044AAD4 279CF7C0 */  
/* 0044AAD8 0399E021 */  
/* 0044AADC 27BDFFC0 */  addiu $sp, $sp, -0x40
/* 0044AAE0 AFBF0024 */  sw    $ra, 0x24($sp)
/* 0044AAE4 AFBC0020 */  sw    $gp, 0x20($sp)
/* 0044AAE8 AFA40040 */  sw    $a0, 0x40($sp)
/* 0044AAEC AFA50044 */  sw    $a1, 0x44($sp)
/* 0044AAF0 90830000 */  lbu   $v1, ($a0)
/* 0044AAF4 246FFF80 */  addiu $t7, $v1, -0x80
/* 0044AAF8 2DF80020 */  sltiu $t8, $t7, 0x20
/* 0044AAFC 0018C823 */  negu  $t9, $t8
/* 0044AB00 332A0810 */  andi  $t2, $t9, 0x810
/* 0044AB04 01EA5804 */  sllv  $t3, $t2, $t7
/* 0044AB08 0560000B */  bltz  $t3, .L0044AB38
/* 0044AB0C 00000000 */   nop   
/* 0044AB10 90A20000 */  lbu   $v0, ($a1)
/* 0044AB14 246FFFE0 */  addiu $t7, $v1, -0x20
/* 0044AB18 2DEB0060 */  sltiu $t3, $t7, 0x60
/* 0044AB1C 244DFF80 */  addiu $t5, $v0, -0x80
/* 0044AB20 2DAE0020 */  sltiu $t6, $t5, 0x20
/* 0044AB24 000EC023 */  negu  $t8, $t6
/* 0044AB28 33190810 */  andi  $t9, $t8, 0x810
/* 0044AB2C 01B95004 */  sllv  $t2, $t9, $t5
/* 0044AB30 05410003 */  bgez  $t2, .L0044AB40
/* 0044AB34 00000000 */   nop   
.L0044AB38:
/* 0044AB38 100001C4 */  b     .L0044B24C
/* 0044AB3C 24020001 */   li    $v0, 1
.L0044AB40:
/* 0044AB40 11600009 */  beqz  $t3, .L0044AB68
/* 0044AB44 00000000 */   nop   
/* 0044AB48 8F988044 */  lw    $t8, %got(D_10010AC8)($gp)
/* 0044AB4C 000F6143 */  sra   $t4, $t7, 5
/* 0044AB50 000C7080 */  sll   $t6, $t4, 2
/* 0044AB54 27180AC8 */  addiu $t8, %lo(D_10010AC8) # addiu $t8, $t8, 0xac8
/* 0044AB58 030EC821 */  addu  $t9, $t8, $t6
/* 0044AB5C 8F2D0000 */  lw    $t5, ($t9)
/* 0044AB60 01ED5004 */  sllv  $t2, $t5, $t7
/* 0044AB64 294B0000 */  slti  $t3, $t2, 0
.L0044AB68:
/* 0044AB68 1560000D */  bnez  $t3, .L0044ABA0
/* 0044AB6C 8FB80040 */   lw    $t8, 0x40($sp)
/* 0044AB70 8F0E0024 */  lw    $t6, 0x24($t8)
/* 0044AB74 24010007 */  li    $at, 7
/* 0044AB78 91D90000 */  lbu   $t9, ($t6)
/* 0044AB7C 57210009 */  bnel  $t9, $at, .L0044ABA4
/* 0044AB80 244FFFE0 */   addiu $t7, $v0, -0x20
/* 0044AB84 8F99862C */  lw    $t9, %call16(fix_sbase)($gp)
/* 0044AB88 03002025 */  move  $a0, $t8
/* 0044AB8C 0320F809 */  jalr  $t9
/* 0044AB90 00000000 */   nop   
/* 0044AB94 8FAD0044 */  lw    $t5, 0x44($sp)
/* 0044AB98 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044AB9C 91A20000 */  lbu   $v0, ($t5)
.L0044ABA0:
/* 0044ABA0 244FFFE0 */  addiu $t7, $v0, -0x20
.L0044ABA4:
/* 0044ABA4 2DEA0060 */  sltiu $t2, $t7, 0x60
/* 0044ABA8 11400009 */  beqz  $t2, .L0044ABD0
/* 0044ABAC 00000000 */   nop   
/* 0044ABB0 8F8E8044 */  lw    $t6, %got(D_10010AC8)($gp)
/* 0044ABB4 000F6143 */  sra   $t4, $t7, 5
/* 0044ABB8 000C5880 */  sll   $t3, $t4, 2
/* 0044ABBC 25CE0AC8 */  addiu $t6, %lo(D_10010AC8) # addiu $t6, $t6, 0xac8
/* 0044ABC0 01CBC821 */  addu  $t9, $t6, $t3
/* 0044ABC4 8F380000 */  lw    $t8, ($t9)
/* 0044ABC8 01F86804 */  sllv  $t5, $t8, $t7
/* 0044ABCC 29AA0000 */  slti  $t2, $t5, 0
.L0044ABD0:
/* 0044ABD0 11400058 */  beqz  $t2, .L0044AD34
/* 0044ABD4 8FB90044 */   lw    $t9, 0x44($sp)
/* 0044ABD8 8FAE0040 */  lw    $t6, 0x40($sp)
/* 0044ABDC 91CB0000 */  lbu   $t3, ($t6)
/* 0044ABE0 2579FFE0 */  addiu $t9, $t3, -0x20
/* 0044ABE4 2F380060 */  sltiu $t8, $t9, 0x60
/* 0044ABE8 13000009 */  beqz  $t8, .L0044AC10
/* 0044ABEC 00000000 */   nop   
/* 0044ABF0 8F8C8044 */  lw    $t4, %got(D_10010AC8)($gp)
/* 0044ABF4 00197943 */  sra   $t7, $t9, 5
/* 0044ABF8 000F6880 */  sll   $t5, $t7, 2
/* 0044ABFC 258C0AC8 */  addiu $t4, %lo(D_10010AC8) # addiu $t4, $t4, 0xac8
/* 0044AC00 018D5021 */  addu  $t2, $t4, $t5
/* 0044AC04 8D4B0000 */  lw    $t3, ($t2)
/* 0044AC08 032B7804 */  sllv  $t7, $t3, $t9
/* 0044AC0C 29F80000 */  slti  $t8, $t7, 0
.L0044AC10:
/* 0044AC10 13000015 */  beqz  $t8, .L0044AC68
/* 0044AC14 8FAD0040 */   lw    $t5, 0x40($sp)
/* 0044AC18 8FAD0044 */  lw    $t5, 0x44($sp)
/* 0044AC1C 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044AC20 8DA20004 */  lw    $v0, 4($t5)
/* 0044AC24 8C440028 */  lw    $a0, 0x28($v0)
/* 0044AC28 904C0020 */  lbu   $t4, 0x20($v0)
/* 0044AC2C AFA40000 */  sw    $a0, ($sp)
/* 0044AC30 8C45002C */  lw    $a1, 0x2c($v0)
/* 0044AC34 AFA50004 */  sw    $a1, 4($sp)
/* 0044AC38 8DC30004 */  lw    $v1, 4($t6)
/* 0044AC3C 8C660028 */  lw    $a2, 0x28($v1)
/* 0044AC40 90780020 */  lbu   $t8, 0x20($v1)
/* 0044AC44 AFA60008 */  sw    $a2, 8($sp)
/* 0044AC48 8C67002C */  lw    $a3, 0x2c($v1)
/* 0044AC4C AFAC0010 */  sw    $t4, 0x10($sp)
/* 0044AC50 AFB80014 */  sw    $t8, 0x14($sp)
/* 0044AC54 0320F809 */  jalr  $t9
/* 0044AC58 AFA7000C */   sw    $a3, 0xc($sp)
/* 0044AC5C 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044AC60 10000150 */  b     .L0044B1A4
/* 0044AC64 304400FF */   andi  $a0, $v0, 0xff
.L0044AC68:
/* 0044AC68 8DAA0024 */  lw    $t2, 0x24($t5)
/* 0044AC6C 3C014400 */  lui   $at, 0x4400
/* 0044AC70 AFAA0030 */  sw    $t2, 0x30($sp)
/* 0044AC74 91440000 */  lbu   $a0, ($t2)
/* 0044AC78 2C8B0020 */  sltiu $t3, $a0, 0x20
/* 0044AC7C 000B7023 */  negu  $t6, $t3
/* 0044AC80 01C1C824 */  and   $t9, $t6, $at
/* 0044AC84 00997804 */  sllv  $t7, $t9, $a0
/* 0044AC88 05E10014 */  bgez  $t7, .L0044ACDC
/* 0044AC8C 2C8C0020 */   sltiu $t4, $a0, 0x20
/* 0044AC90 8FAC0044 */  lw    $t4, 0x44($sp)
/* 0044AC94 8D820004 */  lw    $v0, 4($t4)
/* 0044AC98 8C440028 */  lw    $a0, 0x28($v0)
/* 0044AC9C 90590020 */  lbu   $t9, 0x20($v0)
/* 0044ACA0 AFA40000 */  sw    $a0, ($sp)
/* 0044ACA4 8C45002C */  lw    $a1, 0x2c($v0)
/* 0044ACA8 AFA50004 */  sw    $a1, 4($sp)
/* 0044ACAC 8D46002C */  lw    $a2, 0x2c($t2)
/* 0044ACB0 AFA60008 */  sw    $a2, 8($sp)
/* 0044ACB4 8D470030 */  lw    $a3, 0x30($t2)
/* 0044ACB8 AFB90010 */  sw    $t9, 0x10($sp)
/* 0044ACBC 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044ACC0 AFA7000C */  sw    $a3, 0xc($sp)
/* 0044ACC4 8D4F0024 */  lw    $t7, 0x24($t2)
/* 0044ACC8 0320F809 */  jalr  $t9
/* 0044ACCC AFAF0014 */   sw    $t7, 0x14($sp)
/* 0044ACD0 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044ACD4 10000133 */  b     .L0044B1A4
/* 0044ACD8 304400FF */   andi  $a0, $v0, 0xff
.L0044ACDC:
/* 0044ACDC 000CC023 */  negu  $t8, $t4
/* 0044ACE0 3C011B00 */  lui   $at, 0x1b00
/* 0044ACE4 03016824 */  and   $t5, $t8, $at
/* 0044ACE8 008D5804 */  sllv  $t3, $t5, $a0
/* 0044ACEC 05610007 */  bgez  $t3, .L0044AD0C
/* 0044ACF0 00000000 */   nop   
/* 0044ACF4 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044ACF8 8FA40030 */  lw    $a0, 0x30($sp)
/* 0044ACFC 0320F809 */  jalr  $t9
/* 0044AD00 00000000 */   nop   
/* 0044AD04 10400003 */  beqz  $v0, .L0044AD14
/* 0044AD08 8FBC0020 */   lw    $gp, 0x20($sp)
.L0044AD0C:
/* 0044AD0C 10000125 */  b     .L0044B1A4
/* 0044AD10 00002025 */   move  $a0, $zero
.L0044AD14:
/* 0044AD14 8FAE0044 */  lw    $t6, 0x44($sp)
/* 0044AD18 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044AD1C 8DC40004 */  lw    $a0, 4($t6)
/* 0044AD20 0320F809 */  jalr  $t9
/* 0044AD24 24840028 */   addiu $a0, $a0, 0x28
/* 0044AD28 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044AD2C 1000011D */  b     .L0044B1A4
/* 0044AD30 304400FF */   andi  $a0, $v0, 0xff
.L0044AD34:
/* 0044AD34 8F2A0024 */  lw    $t2, 0x24($t9)
/* 0044AD38 24010007 */  li    $at, 7
/* 0044AD3C 03202025 */  move  $a0, $t9
/* 0044AD40 AFAA0028 */  sw    $t2, 0x28($sp)
/* 0044AD44 91420000 */  lbu   $v0, ($t2)
/* 0044AD48 5441000A */  bnel  $v0, $at, .L0044AD74
/* 0044AD4C 2C4C0020 */   sltiu $t4, $v0, 0x20
/* 0044AD50 8F99862C */  lw    $t9, %call16(fix_sbase)($gp)
/* 0044AD54 0320F809 */  jalr  $t9
/* 0044AD58 00000000 */   nop   
/* 0044AD5C 8FAF0044 */  lw    $t7, 0x44($sp)
/* 0044AD60 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044AD64 8DE80024 */  lw    $t0, 0x24($t7)
/* 0044AD68 91020000 */  lbu   $v0, ($t0)
/* 0044AD6C AFA80028 */  sw    $t0, 0x28($sp)
/* 0044AD70 2C4C0020 */  sltiu $t4, $v0, 0x20
.L0044AD74:
/* 0044AD74 000CC023 */  negu  $t8, $t4
/* 0044AD78 3C014400 */  lui   $at, 0x4400
/* 0044AD7C 03016824 */  and   $t5, $t8, $at
/* 0044AD80 004D5804 */  sllv  $t3, $t5, $v0
/* 0044AD84 05610055 */  bgez  $t3, .L0044AEDC
/* 0044AD88 8FAE0040 */   lw    $t6, 0x40($sp)
/* 0044AD8C 91CA0000 */  lbu   $t2, ($t6)
/* 0044AD90 2559FFE0 */  addiu $t9, $t2, -0x20
/* 0044AD94 2F2F0060 */  sltiu $t7, $t9, 0x60
/* 0044AD98 11E00009 */  beqz  $t7, .L0044ADC0
/* 0044AD9C 00000000 */   nop   
/* 0044ADA0 8F8D8044 */  lw    $t5, %got(D_10010AC8)($gp)
/* 0044ADA4 00196143 */  sra   $t4, $t9, 5
/* 0044ADA8 000CC080 */  sll   $t8, $t4, 2
/* 0044ADAC 25AD0AC8 */  addiu $t5, %lo(D_10010AC8) # addiu $t5, $t5, 0xac8
/* 0044ADB0 01B85821 */  addu  $t3, $t5, $t8
/* 0044ADB4 8D6A0000 */  lw    $t2, ($t3)
/* 0044ADB8 032A6004 */  sllv  $t4, $t2, $t9
/* 0044ADBC 298F0000 */  slti  $t7, $t4, 0
.L0044ADC0:
/* 0044ADC0 11E00014 */  beqz  $t7, .L0044AE14
/* 0044ADC4 8FAB0040 */   lw    $t3, 0x40($sp)
/* 0044ADC8 8FB80028 */  lw    $t8, 0x28($sp)
/* 0044ADCC 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044ADD0 8F04002C */  lw    $a0, 0x2c($t8)
/* 0044ADD4 AFA40000 */  sw    $a0, ($sp)
/* 0044ADD8 8F050030 */  lw    $a1, 0x30($t8)
/* 0044ADDC AFA50004 */  sw    $a1, 4($sp)
/* 0044ADE0 8DC30004 */  lw    $v1, 4($t6)
/* 0044ADE4 8C660028 */  lw    $a2, 0x28($v1)
/* 0044ADE8 906F0020 */  lbu   $t7, 0x20($v1)
/* 0044ADEC AFA60008 */  sw    $a2, 8($sp)
/* 0044ADF0 8C67002C */  lw    $a3, 0x2c($v1)
/* 0044ADF4 AFA7000C */  sw    $a3, 0xc($sp)
/* 0044ADF8 8F0D0024 */  lw    $t5, 0x24($t8)
/* 0044ADFC AFAF0014 */  sw    $t7, 0x14($sp)
/* 0044AE00 0320F809 */  jalr  $t9
/* 0044AE04 AFAD0010 */   sw    $t5, 0x10($sp)
/* 0044AE08 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044AE0C 100000E5 */  b     .L0044B1A4
/* 0044AE10 304400FF */   andi  $a0, $v0, 0xff
.L0044AE14:
/* 0044AE14 8D6A0024 */  lw    $t2, 0x24($t3)
/* 0044AE18 3C014400 */  lui   $at, 0x4400
/* 0044AE1C 8FAD0028 */  lw    $t5, 0x28($sp)
/* 0044AE20 AFAA0030 */  sw    $t2, 0x30($sp)
/* 0044AE24 91440000 */  lbu   $a0, ($t2)
/* 0044AE28 2C8E0020 */  sltiu $t6, $a0, 0x20
/* 0044AE2C 000EC823 */  negu  $t9, $t6
/* 0044AE30 03216024 */  and   $t4, $t9, $at
/* 0044AE34 008CC004 */  sllv  $t8, $t4, $a0
/* 0044AE38 07010012 */  bgez  $t8, .L0044AE84
/* 0044AE3C 2C8F0020 */   sltiu $t7, $a0, 0x20
/* 0044AE40 8DA4002C */  lw    $a0, 0x2c($t5)
/* 0044AE44 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044AE48 AFA40000 */  sw    $a0, ($sp)
/* 0044AE4C 8DA50030 */  lw    $a1, 0x30($t5)
/* 0044AE50 AFA50004 */  sw    $a1, 4($sp)
/* 0044AE54 8D46002C */  lw    $a2, 0x2c($t2)
/* 0044AE58 AFA60008 */  sw    $a2, 8($sp)
/* 0044AE5C 8D470030 */  lw    $a3, 0x30($t2)
/* 0044AE60 AFA7000C */  sw    $a3, 0xc($sp)
/* 0044AE64 8DAC0024 */  lw    $t4, 0x24($t5)
/* 0044AE68 AFAC0010 */  sw    $t4, 0x10($sp)
/* 0044AE6C 8D580024 */  lw    $t8, 0x24($t2)
/* 0044AE70 0320F809 */  jalr  $t9
/* 0044AE74 AFB80014 */   sw    $t8, 0x14($sp)
/* 0044AE78 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044AE7C 100000C9 */  b     .L0044B1A4
/* 0044AE80 304400FF */   andi  $a0, $v0, 0xff
.L0044AE84:
/* 0044AE84 000F5823 */  negu  $t3, $t7
/* 0044AE88 3C011B00 */  lui   $at, 0x1b00
/* 0044AE8C 01617024 */  and   $t6, $t3, $at
/* 0044AE90 008EC804 */  sllv  $t9, $t6, $a0
/* 0044AE94 07210007 */  bgez  $t9, .L0044AEB4
/* 0044AE98 00000000 */   nop   
/* 0044AE9C 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044AEA0 8FA40030 */  lw    $a0, 0x30($sp)
/* 0044AEA4 0320F809 */  jalr  $t9
/* 0044AEA8 00000000 */   nop   
/* 0044AEAC 10400003 */  beqz  $v0, .L0044AEBC
/* 0044AEB0 8FBC0020 */   lw    $gp, 0x20($sp)
.L0044AEB4:
/* 0044AEB4 100000BB */  b     .L0044B1A4
/* 0044AEB8 00002025 */   move  $a0, $zero
.L0044AEBC:
/* 0044AEBC 8FAD0044 */  lw    $t5, 0x44($sp)
/* 0044AEC0 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044AEC4 8DA40024 */  lw    $a0, 0x24($t5)
/* 0044AEC8 0320F809 */  jalr  $t9
/* 0044AECC 2484002C */   addiu $a0, $a0, 0x2c
/* 0044AED0 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044AED4 100000B3 */  b     .L0044B1A4
/* 0044AED8 304400FF */   andi  $a0, $v0, 0xff
.L0044AEDC:
/* 0044AEDC 8F8C8A5C */  lw     $t4, %got(lang)($gp)
/* 0044AEE0 24010002 */  li    $at, 2
/* 0044AEE4 2C4A0020 */  sltiu $t2, $v0, 0x20
/* 0044AEE8 918C0000 */  lbu   $t4, ($t4)
/* 0044AEEC 000AC023 */  negu  $t8, $t2
/* 0044AEF0 11810007 */  beq   $t4, $at, .L0044AF10
/* 0044AEF4 3C011B00 */   lui   $at, 0x1b00
/* 0044AEF8 03017824 */  and   $t7, $t8, $at
/* 0044AEFC 004F5804 */  sllv  $t3, $t7, $v0
/* 0044AF00 05600003 */  bltz  $t3, .L0044AF10
/* 0044AF04 00000000 */   nop   
/* 0044AF08 100000A6 */  b     .L0044B1A4
/* 0044AF0C 00002025 */   move  $a0, $zero
.L0044AF10:
/* 0044AF10 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044AF14 8FA40028 */  lw    $a0, 0x28($sp)
/* 0044AF18 0320F809 */  jalr  $t9
/* 0044AF1C 00000000 */   nop   
/* 0044AF20 1040001A */  beqz  $v0, .L0044AF8C
/* 0044AF24 8FBC0020 */   lw    $gp, 0x20($sp)
/* 0044AF28 8FAE0040 */  lw    $t6, 0x40($sp)
/* 0044AF2C 91D90000 */  lbu   $t9, ($t6)
/* 0044AF30 272DFFE0 */  addiu $t5, $t9, -0x20
/* 0044AF34 2DAC0060 */  sltiu $t4, $t5, 0x60
/* 0044AF38 11800009 */  beqz  $t4, .L0044AF60
/* 0044AF3C 00000000 */   nop   
/* 0044AF40 8F8F8044 */  lw    $t7, %got(D_10010AC8)($gp)
/* 0044AF44 000D5143 */  sra   $t2, $t5, 5
/* 0044AF48 000AC080 */  sll   $t8, $t2, 2
/* 0044AF4C 25EF0AC8 */  addiu $t7, %lo(D_10010AC8) # addiu $t7, $t7, 0xac8
/* 0044AF50 01F85821 */  addu  $t3, $t7, $t8
/* 0044AF54 8D6E0000 */  lw    $t6, ($t3)
/* 0044AF58 01AEC804 */  sllv  $t9, $t6, $t5
/* 0044AF5C 2B2C0000 */  slti  $t4, $t9, 0
.L0044AF60:
/* 0044AF60 11800003 */  beqz  $t4, .L0044AF70
/* 0044AF64 00000000 */   nop   
/* 0044AF68 1000008E */  b     .L0044B1A4
/* 0044AF6C 00002025 */   move  $a0, $zero
.L0044AF70:
/* 0044AF70 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044AF74 8FAF0040 */  lw    $t7, 0x40($sp)
/* 0044AF78 0320F809 */  jalr  $t9
/* 0044AF7C 8DE40024 */   lw    $a0, 0x24($t7)
/* 0044AF80 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044AF84 10000087 */  b     .L0044B1A4
/* 0044AF88 304400FF */   andi  $a0, $v0, 0xff
.L0044AF8C:
/* 0044AF8C 8FB80040 */  lw    $t8, 0x40($sp)
/* 0044AF90 930B0000 */  lbu   $t3, ($t8)
/* 0044AF94 256EFFE0 */  addiu $t6, $t3, -0x20
/* 0044AF98 2DCD0060 */  sltiu $t5, $t6, 0x60
/* 0044AF9C 11A00009 */  beqz  $t5, .L0044AFC4
/* 0044AFA0 00000000 */   nop   
/* 0044AFA4 8F8C8044 */  lw    $t4, %got(D_10010AC8)($gp)
/* 0044AFA8 000EC943 */  sra   $t9, $t6, 5
/* 0044AFAC 00195080 */  sll   $t2, $t9, 2
/* 0044AFB0 258C0AC8 */  addiu $t4, %lo(D_10010AC8) # addiu $t4, $t4, 0xac8
/* 0044AFB4 018A7821 */  addu  $t7, $t4, $t2
/* 0044AFB8 8DEB0000 */  lw    $t3, ($t7)
/* 0044AFBC 01CBC804 */  sllv  $t9, $t3, $t6
/* 0044AFC0 2B2D0000 */  slti  $t5, $t9, 0
.L0044AFC4:
/* 0044AFC4 11A00008 */  beqz  $t5, .L0044AFE8
/* 0044AFC8 8FAA0040 */   lw    $t2, 0x40($sp)
/* 0044AFCC 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044AFD0 8F040004 */  lw    $a0, 4($t8)
/* 0044AFD4 0320F809 */  jalr  $t9
/* 0044AFD8 24840028 */   addiu $a0, $a0, 0x28
/* 0044AFDC 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044AFE0 10000070 */  b     .L0044B1A4
/* 0044AFE4 304400FF */   andi  $a0, $v0, 0xff
.L0044AFE8:
/* 0044AFE8 8D490024 */  lw    $t1, 0x24($t2)
/* 0044AFEC 3C014400 */  lui   $at, 0x4400
/* 0044AFF0 91240000 */  lbu   $a0, ($t1)
/* 0044AFF4 2C8F0020 */  sltiu $t7, $a0, 0x20
/* 0044AFF8 000F5823 */  negu  $t3, $t7
/* 0044AFFC 01617024 */  and   $t6, $t3, $at
/* 0044B000 008EC804 */  sllv  $t9, $t6, $a0
/* 0044B004 07210008 */  bgez  $t9, .L0044B028
/* 0044B008 00000000 */   nop   
/* 0044B00C 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044B010 2524002C */  addiu $a0, $t1, 0x2c
/* 0044B014 0320F809 */  jalr  $t9
/* 0044B018 00000000 */   nop   
/* 0044B01C 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044B020 10000060 */  b     .L0044B1A4
/* 0044B024 304400FF */   andi  $a0, $v0, 0xff
.L0044B028:
/* 0044B028 8F828A5C */  lw     $v0, %got(lang)($gp)
/* 0044B02C 24010002 */  li    $at, 2
/* 0044B030 2C8C0020 */  sltiu $t4, $a0, 0x20
/* 0044B034 90420000 */  lbu   $v0, ($v0)
/* 0044B038 000C6823 */  negu  $t5, $t4
/* 0044B03C 10410008 */  beq   $v0, $at, .L0044B060
/* 0044B040 2C4F0020 */   sltiu $t7, $v0, 0x20
/* 0044B044 3C011B00 */  lui   $at, 0x1b00
/* 0044B048 01A1C024 */  and   $t8, $t5, $at
/* 0044B04C 00985004 */  sllv  $t2, $t8, $a0
/* 0044B050 05420004 */  bltzl $t2, .L0044B064
/* 0044B054 000F5823 */   negu  $t3, $t7
/* 0044B058 10000052 */  b     .L0044B1A4
/* 0044B05C 00002025 */   move  $a0, $zero
.L0044B060:
/* 0044B060 000F5823 */  negu  $t3, $t7
.L0044B064:
/* 0044B064 3C01C400 */  lui   $at, 0xc400
/* 0044B068 01617024 */  and   $t6, $t3, $at
/* 0044B06C 004E1804 */  sllv  $v1, $t6, $v0
/* 0044B070 28630000 */  slti  $v1, $v1, 0
/* 0044B074 2C630001 */  sltiu $v1, $v1, 1
/* 0044B078 5460004A */  bnezl $v1, .L0044B1A4
/* 0044B07C 306400FF */   andi  $a0, $v1, 0xff
/* 0044B080 38430001 */  xori  $v1, $v0, 1
/* 0044B084 2C630001 */  sltiu $v1, $v1, 1
/* 0044B088 10600008 */  beqz  $v1, .L0044B0AC
/* 0044B08C 00000000 */   nop   
/* 0044B090 8F838AE0 */  lw     $v1, %got(nof77alias)($gp)
/* 0044B094 90630000 */  lbu   $v1, ($v1)
/* 0044B098 2C630001 */  sltiu $v1, $v1, 1
/* 0044B09C 54600041 */  bnezl $v1, .L0044B1A4
/* 0044B0A0 306400FF */   andi  $a0, $v1, 0xff
/* 0044B0A4 8F838B44 */  lw     $v1, %got(use_c_semantics)($gp)
/* 0044B0A8 90630000 */  lbu   $v1, ($v1)
.L0044B0AC:
/* 0044B0AC 1460003C */  bnez  $v1, .L0044B1A0
/* 0044B0B0 2C450001 */   sltiu $a1, $v0, 1
/* 0044B0B4 10A00008 */  beqz  $a1, .L0044B0D8
/* 0044B0B8 00A01825 */   move  $v1, $a1
/* 0044B0BC 8F838AE4 */  lw     $v1, %got(nopalias)($gp)
/* 0044B0C0 90630000 */  lbu   $v1, ($v1)
/* 0044B0C4 10600004 */  beqz  $v1, .L0044B0D8
/* 0044B0C8 00000000 */   nop   
/* 0044B0CC 8F838AE0 */  lw     $v1, %got(nof77alias)($gp)
/* 0044B0D0 90630000 */  lbu   $v1, ($v1)
/* 0044B0D4 2C630001 */  sltiu $v1, $v1, 1
.L0044B0D8:
/* 0044B0D8 54600032 */  bnezl $v1, .L0044B1A4
/* 0044B0DC 306400FF */   andi  $a0, $v1, 0xff
/* 0044B0E0 10A00004 */  beqz  $a1, .L0044B0F4
/* 0044B0E4 00A01825 */   move  $v1, $a1
/* 0044B0E8 8F838AE4 */  lw     $v1, %got(nopalias)($gp)
/* 0044B0EC 90630000 */  lbu   $v1, ($v1)
/* 0044B0F0 2C630001 */  sltiu $v1, $v1, 1
.L0044B0F4:
/* 0044B0F4 5460002B */  bnezl $v1, .L0044B1A4
/* 0044B0F8 306400FF */   andi  $a0, $v1, 0xff
/* 0044B0FC 8FB90044 */  lw    $t9, 0x44($sp)
/* 0044B100 8F280024 */  lw    $t0, 0x24($t9)
/* 0044B104 91020000 */  lbu   $v0, ($t0)
/* 0044B108 38430007 */  xori  $v1, $v0, 7
/* 0044B10C 2C630001 */  sltiu $v1, $v1, 1
/* 0044B110 54600024 */  bnezl $v1, .L0044B1A4
/* 0044B114 306400FF */   andi  $a0, $v1, 0xff
/* 0044B118 38830007 */  xori  $v1, $a0, 7
/* 0044B11C 2C630001 */  sltiu $v1, $v1, 1
/* 0044B120 54600020 */  bnezl $v1, .L0044B1A4
/* 0044B124 306400FF */   andi  $a0, $v1, 0xff
/* 0044B128 01281826 */  xor   $v1, $t1, $t0
/* 0044B12C 2C630001 */  sltiu $v1, $v1, 1
/* 0044B130 1460001B */  bnez  $v1, .L0044B1A0
/* 0044B134 2C4C0020 */   sltiu $t4, $v0, 0x20
/* 0044B138 000C6823 */  negu  $t5, $t4
/* 0044B13C 3C011200 */  lui   $at, 0x1200
/* 0044B140 01A1C024 */  and   $t8, $t5, $at
/* 0044B144 00581804 */  sllv  $v1, $t8, $v0
/* 0044B148 28630000 */  slti  $v1, $v1, 0
/* 0044B14C 10600014 */  beqz  $v1, .L0044B1A0
/* 0044B150 2C8A0020 */   sltiu $t2, $a0, 0x20
/* 0044B154 000A7823 */  negu  $t7, $t2
/* 0044B158 3C011200 */  lui   $at, 0x1200
/* 0044B15C 01E15824 */  and   $t3, $t7, $at
/* 0044B160 008B1804 */  sllv  $v1, $t3, $a0
/* 0044B164 28630000 */  slti  $v1, $v1, 0
/* 0044B168 5060000E */  beql  $v1, $zero, .L0044B1A4
/* 0044B16C 306400FF */   andi  $a0, $v1, 0xff
/* 0044B170 8D040028 */  lw    $a0, 0x28($t0)
/* 0044B174 8D05002C */  lw    $a1, 0x2c($t0)
/* 0044B178 8F99860C */  lw    $t9, %call16(addreq)($gp)
/* 0044B17C AFA40000 */  sw    $a0, ($sp)
/* 0044B180 AFA50004 */  sw    $a1, 4($sp)
/* 0044B184 8D260028 */  lw    $a2, 0x28($t1)
/* 0044B188 AFA60008 */  sw    $a2, 8($sp)
/* 0044B18C 8D27002C */  lw    $a3, 0x2c($t1)
/* 0044B190 0320F809 */  jalr  $t9
/* 0044B194 AFA7000C */   sw    $a3, 0xc($sp)
/* 0044B198 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044B19C 00401825 */  move  $v1, $v0
.L0044B1A0:
/* 0044B1A0 306400FF */  andi  $a0, $v1, 0xff
.L0044B1A4:
/* 0044B1A4 8F988A60 */  lw     $t8, %got(inlopt)($gp)
/* 0044B1A8 93180000 */  lbu   $t8, ($t8)
/* 0044B1AC 53000027 */  beql  $t8, $zero, .L0044B24C
/* 0044B1B0 00801025 */   move  $v0, $a0
/* 0044B1B4 10800024 */  beqz  $a0, .L0044B248
/* 0044B1B8 8FAA0044 */   lw    $t2, 0x44($sp)
/* 0044B1BC 914F0000 */  lbu   $t7, ($t2)
/* 0044B1C0 2402003F */  li    $v0, 63
/* 0044B1C4 8FAB0040 */  lw    $t3, 0x40($sp)
/* 0044B1C8 544F0020 */  bnel  $v0, $t7, .L0044B24C
/* 0044B1CC 00801025 */   move  $v0, $a0
/* 0044B1D0 916E0000 */  lbu   $t6, ($t3)
/* 0044B1D4 544E001D */  bnel  $v0, $t6, .L0044B24C
/* 0044B1D8 00801025 */   move  $v0, $a0
/* 0044B1DC 8D590024 */  lw    $t9, 0x24($t2)
/* 0044B1E0 8D6C0024 */  lw    $t4, 0x24($t3)
/* 0044B1E4 572C0019 */  bnel  $t9, $t4, .L0044B24C
/* 0044B1E8 00801025 */   move  $v0, $a0
/* 0044B1EC 8D4D0004 */  lw    $t5, 4($t2)
/* 0044B1F0 8D780004 */  lw    $t8, 4($t3)
/* 0044B1F4 55B80015 */  bnel  $t5, $t8, .L0044B24C
/* 0044B1F8 00801025 */   move  $v0, $a0
/* 0044B1FC 8D620030 */  lw    $v0, 0x30($t3)
/* 0044B200 8D440030 */  lw    $a0, 0x30($t2)
/* 0044B204 0044182A */  slt   $v1, $v0, $a0
/* 0044B208 38630001 */  xori  $v1, $v1, 1
/* 0044B20C 10600004 */  beqz  $v1, .L0044B220
/* 0044B210 00000000 */   nop   
/* 0044B214 8D4E0020 */  lw    $t6, 0x20($t2)
/* 0044B218 00447823 */  subu  $t7, $v0, $a0
/* 0044B21C 01EE182A */  slt   $v1, $t7, $t6
.L0044B220:
/* 0044B220 54600009 */  bnezl $v1, .L0044B248
/* 0044B224 306400FF */   andi  $a0, $v1, 0xff
/* 0044B228 0082182A */  slt   $v1, $a0, $v0
/* 0044B22C 38630001 */  xori  $v1, $v1, 1
/* 0044B230 10600004 */  beqz  $v1, .L0044B244
/* 0044B234 8FAC0040 */   lw    $t4, 0x40($sp)
/* 0044B238 8D8D0020 */  lw    $t5, 0x20($t4)
/* 0044B23C 0082C823 */  subu  $t9, $a0, $v0
/* 0044B240 032D182A */  slt   $v1, $t9, $t5
.L0044B244:
/* 0044B244 306400FF */  andi  $a0, $v1, 0xff
.L0044B248:
/* 0044B248 00801025 */  move  $v0, $a0
.L0044B24C:
/* 0044B24C 8FBF0024 */  lw    $ra, 0x24($sp)
/* 0044B250 27BD0040 */  addiu $sp, $sp, 0x40
/* 0044B254 03E00008 */  jr    $ra
/* 0044B258 00000000 */   nop   
    .type sskilled, @function
    .size sskilled, .-sskilled
    .end sskilled
)"");

/* 
0043CFCC readnxtinst
0046E77C oneloopblockstmt
00473F04 pmov_to_mov
*/
void movkillprev(struct Statement *stmt) {
    struct VarAccessList *item;

    for (item = curgraphnode->varlisthead; item != NULL; item = item->next) {
        if (item->type == 1 && !item->unk8) {
            if (item->data.store->u.store.unk1D) {
                item->data.store->u.store.unk1D = !smkilled(item->data.store, stmt);
            }
        }
    }
}

/* 
0043CFCC readnxtinst
0046E77C oneloopblockstmt
004737E0 par_to_str
00473F04 pmov_to_mov
*/
void strkillprev(struct Statement *stmt) {
    struct VarAccessList *item;

    for (item = curgraphnode->varlisthead; item != NULL; item = item->next) {
        if (item->type == 2 && !item->unk8) {
            if (!item->data.var->unk2) {
                item->data.var->unk2 = slkilled(stmt, item->data.var);
            }
        } else if (item->type == 3) { // mov?
            if (item->data.store->u.store.unk1F) {
                item->data.store->u.store.unk1F = !smkilled(stmt, item->data.store);
            }
        } else if (item->type == 1 && !item->unk8) {
            if (item->data.store->u.store.unk1F) {
                item->data.store->u.store.unk1F = !sskilled(stmt, item->data.store);
            }
        }
    }
    
}
/*
0043ABD0 incroccurrence
0043CFCC readnxtinst
0046D428 oneloopblockexpr
*/
void lodkillprev(struct Expression *expr) {
    struct VarAccessList *list;

    for (list = curgraphnode->varlisthead; list != NULL; list = list->next) {
        if (list->type == 1 && !list->unk8 && list->data.store->u.store.unk1D) {
            list->data.store->u.store.unk1D = !slkilled(list->data.store, expr);
        }
    }
}

__asm__(R""(
.set noat
.set noreorder

glabel clkilled
    .ent clkilled
    # 0044C4D8 cupkillprev
    # 0044CD14 listplkilled
    # 00454D08 func_00454D08
    # 0045C8A0 lvalaltered
    # 0045CBDC cupaltered
    # 004638C0 regdataflow
/* 0044B4F4 3C1C0FBD */  .cpload $t9
/* 0044B4F8 279CED9C */  
/* 0044B4FC 0399E021 */  
/* 0044B500 27BDFFD8 */  addiu $sp, $sp, -0x28
/* 0044B504 AFBF001C */  sw    $ra, 0x1c($sp)
/* 0044B508 AFBC0018 */  sw    $gp, 0x18($sp)
/* 0044B50C AFA40028 */  sw    $a0, 0x28($sp)
/* 0044B510 AFA5002C */  sw    $a1, 0x2c($sp)
/* 0044B514 90CE0000 */  lbu   $t6, ($a2)
/* 0044B518 3C011200 */  lui   $at, 0x1200
/* 0044B51C 00C03825 */  move  $a3, $a2
/* 0044B520 2DCF0020 */  sltiu $t7, $t6, 0x20
/* 0044B524 000FC023 */  negu  $t8, $t7
/* 0044B528 0301C824 */  and   $t9, $t8, $at
/* 0044B52C 01D94004 */  sllv  $t0, $t9, $t6
/* 0044B530 05010156 */  bgez  $t0, .L0044BA8C
/* 0044B534 00000000 */   nop   
/* 0044B538 90C4002E */  lbu   $a0, 0x2e($a2)
/* 0044B53C 24010003 */  li    $at, 3
/* 0044B540 8FA9002C */  lw    $t1, 0x2c($sp)
/* 0044B544 30840007 */  andi  $a0, $a0, 7
/* 0044B548 54810004 */  bnel  $a0, $at, .L0044B55C
/* 0044B54C 912A000E */   lbu   $t2, 0xe($t1)
/* 0044B550 100001E6 */  b     .L0044BCEC
/* 0044B554 24040001 */   li    $a0, 1
/* 0044B558 912A000E */  lbu   $t2, 0xe($t1)
.L0044B55C:
/* 0044B55C 1140000C */  beqz  $t2, .L0044B590
/* 0044B560 00000000 */   nop   
/* 0044B564 8F8B8A30 */  lw     $t3, %got(aentptr)($gp)
/* 0044B568 8D6B0000 */  lw    $t3, ($t3)
/* 0044B56C 11600006 */  beqz  $t3, .L0044B588
/* 0044B570 00000000 */   nop   
/* 0044B574 8F8C8A5C */  lw     $t4, %got(lang)($gp)
/* 0044B578 24010001 */  li    $at, 1
/* 0044B57C 918C0000 */  lbu   $t4, ($t4)
/* 0044B580 11810003 */  beq   $t4, $at, .L0044B590
/* 0044B584 00000000 */   nop   
.L0044B588:
/* 0044B588 100001D8 */  b     .L0044BCEC
/* 0044B58C 00002025 */   move  $a0, $zero
.L0044B590:
/* 0044B590 8F828A5C */  lw     $v0, %got(lang)($gp)
/* 0044B594 24050239 */  li    $a1, 569
/* 0044B598 90420000 */  lbu   $v0, ($v0)
/* 0044B59C 2C410007 */  sltiu $at, $v0, 7
/* 0044B5A0 10200131 */  beqz  $at, .L0044BA68
/* 0044B5A4 00000000 */   nop   
/* 0044B5A8 8F818044 */  lw    $at, %got(jtbl_1000CB54)($gp)
/* 0044B5AC 00026880 */  sll   $t5, $v0, 2
/* 0044B5B0 002D0821 */  addu  $at, $at, $t5
/* 0044B5B4 8C2DCB54 */  lw    $t5, %lo(jtbl_1000CB54)($at)
/* 0044B5B8 01BC6821 */  addu  $t5, $t5, $gp
/* 0044B5BC 01A00008 */  jr    $t5
/* 0044B5C0 00000000 */   nop   
.L0044B5C4:
/* 0044B5C4 90EF0022 */  lbu   $t7, 0x22($a3)
/* 0044B5C8 11E00006 */  beqz  $t7, .L0044B5E4
/* 0044B5CC 00000000 */   nop   
/* 0044B5D0 8F988B6C */  lw     $t8, %got(curproc)($gp)
/* 0044B5D4 8F180000 */  lw    $t8, ($t8)
/* 0044B5D8 93190014 */  lbu   $t9, 0x14($t8)
/* 0044B5DC 13200003 */  beqz  $t9, .L0044B5EC
/* 0044B5E0 00000000 */   nop   
.L0044B5E4:
/* 0044B5E4 100001C1 */  b     .L0044BCEC
/* 0044B5E8 24040001 */   li    $a0, 1
.L0044B5EC:
/* 0044B5EC 8F888B70 */  lw     $t0, %got(indirprocs)($gp)
/* 0044B5F0 8FAE002C */  lw    $t6, 0x2c($sp)
/* 0044B5F4 8FA90028 */  lw    $t1, 0x28($sp)
/* 0044B5F8 8D080000 */  lw    $t0, ($t0)
/* 0044B5FC 55C80004 */  bnel  $t6, $t0, .L0044B610
/* 0044B600 90E2002F */   lbu   $v0, 0x2f($a3)
/* 0044B604 100001B9 */  b     .L0044BCEC
/* 0044B608 24040001 */   li    $a0, 1
/* 0044B60C 90E2002F */  lbu   $v0, 0x2f($a3)
.L0044B610:
/* 0044B610 8FAA002C */  lw    $t2, 0x2c($sp)
/* 0044B614 0049082A */  slt   $at, $v0, $t1
/* 0044B618 54200004 */  bnezl $at, .L0044B62C
/* 0044B61C 91430009 */   lbu   $v1, 9($t2)
/* 0044B620 100001B2 */  b     .L0044BCEC
/* 0044B624 00002025 */   move  $a0, $zero
/* 0044B628 91430009 */  lbu   $v1, 9($t2)
.L0044B62C:
/* 0044B62C 10600002 */  beqz  $v1, .L0044B638
/* 0044B630 00000000 */   nop   
/* 0044B634 2C430002 */  sltiu $v1, $v0, 2
.L0044B638:
/* 0044B638 14600015 */  bnez  $v1, .L0044B690
/* 0044B63C 00000000 */   nop   
/* 0044B640 8CE40028 */  lw    $a0, 0x28($a3)
/* 0044B644 8FAD002C */  lw    $t5, 0x2c($sp)
/* 0044B648 8F99848C */  lw    $t9, %call16(varintree)($gp)
/* 0044B64C AFA40000 */  sw    $a0, ($sp)
/* 0044B650 8CE5002C */  lw    $a1, 0x2c($a3)
/* 0044B654 AFA50004 */  sw    $a1, 4($sp)
/* 0044B658 8DA60004 */  lw    $a2, 4($t5)
/* 0044B65C 0320F809 */  jalr  $t9
/* 0044B660 AFA70030 */   sw    $a3, 0x30($sp)
/* 0044B664 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044B668 8FA70030 */  lw    $a3, 0x30($sp)
/* 0044B66C 14400008 */  bnez  $v0, .L0044B690
/* 0044B670 00401825 */   move  $v1, $v0
/* 0044B674 8F998490 */  lw    $t9, %call16(furthervarintree)($gp)
/* 0044B678 00E02025 */  move  $a0, $a3
/* 0044B67C 8FA5002C */  lw    $a1, 0x2c($sp)
/* 0044B680 0320F809 */  jalr  $t9
/* 0044B684 00000000 */   nop   
/* 0044B688 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044B68C 00401825 */  move  $v1, $v0
.L0044B690:
/* 0044B690 10000196 */  b     .L0044BCEC
/* 0044B694 306400FF */   andi  $a0, $v1, 0xff
.L0044B698:
/* 0044B698 90EF0022 */  lbu   $t7, 0x22($a3)
/* 0044B69C 55E00004 */  bnezl $t7, .L0044B6B0
/* 0044B6A0 90F8002F */   lbu   $t8, 0x2f($a3)
/* 0044B6A4 10000191 */  b     .L0044BCEC
/* 0044B6A8 24040001 */   li    $a0, 1
/* 0044B6AC 90F8002F */  lbu   $t8, 0x2f($a3)
.L0044B6B0:
/* 0044B6B0 8FB90028 */  lw    $t9, 0x28($sp)
/* 0044B6B4 0319082A */  slt   $at, $t8, $t9
/* 0044B6B8 14200003 */  bnez  $at, .L0044B6C8
/* 0044B6BC 00000000 */   nop   
/* 0044B6C0 1000018A */  b     .L0044BCEC
/* 0044B6C4 00002025 */   move  $a0, $zero
.L0044B6C8:
/* 0044B6C8 8F888B70 */  lw     $t0, %got(indirprocs)($gp)
/* 0044B6CC 8FAE002C */  lw    $t6, 0x2c($sp)
/* 0044B6D0 8D080000 */  lw    $t0, ($t0)
/* 0044B6D4 01C81826 */  xor   $v1, $t6, $t0
/* 0044B6D8 2C630001 */  sltiu $v1, $v1, 1
/* 0044B6DC 14600017 */  bnez  $v1, .L0044B73C
/* 0044B6E0 00000000 */   nop   
/* 0044B6E4 91C30009 */  lbu   $v1, 9($t6)
/* 0044B6E8 14600014 */  bnez  $v1, .L0044B73C
/* 0044B6EC 00000000 */   nop   
/* 0044B6F0 8CE40028 */  lw    $a0, 0x28($a3)
/* 0044B6F4 8F99848C */  lw    $t9, %call16(varintree)($gp)
/* 0044B6F8 AFA40000 */  sw    $a0, ($sp)
/* 0044B6FC 8CE5002C */  lw    $a1, 0x2c($a3)
/* 0044B700 AFA50004 */  sw    $a1, 4($sp)
/* 0044B704 8DC60004 */  lw    $a2, 4($t6)
/* 0044B708 0320F809 */  jalr  $t9
/* 0044B70C AFA70030 */   sw    $a3, 0x30($sp)
/* 0044B710 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044B714 8FA70030 */  lw    $a3, 0x30($sp)
/* 0044B718 14400008 */  bnez  $v0, .L0044B73C
/* 0044B71C 00401825 */   move  $v1, $v0
/* 0044B720 8F998490 */  lw    $t9, %call16(furthervarintree)($gp)
/* 0044B724 00E02025 */  move  $a0, $a3
/* 0044B728 8FA5002C */  lw    $a1, 0x2c($sp)
/* 0044B72C 0320F809 */  jalr  $t9
/* 0044B730 00000000 */   nop   
/* 0044B734 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044B738 00401825 */  move  $v1, $v0
.L0044B73C:
/* 0044B73C 1000016B */  b     .L0044BCEC
/* 0044B740 306400FF */   andi  $a0, $v1, 0xff
.L0044B744:
/* 0044B744 24010004 */  li    $at, 4
/* 0044B748 10810003 */  beq   $a0, $at, .L0044B758
/* 0044B74C 00000000 */   nop   
/* 0044B750 10000166 */  b     .L0044BCEC
/* 0044B754 00002025 */   move  $a0, $zero
.L0044B758:
/* 0044B758 8F998494 */  lw    $t9, %call16(in_fsym)($gp)
/* 0044B75C 8CE4002C */  lw    $a0, 0x2c($a3)
/* 0044B760 0320F809 */  jalr  $t9
/* 0044B764 000422C2 */   srl   $a0, $a0, 0xb
/* 0044B768 2C440001 */  sltiu $a0, $v0, 1
/* 0044B76C 308400FF */  andi  $a0, $a0, 0xff
/* 0044B770 1000015E */  b     .L0044BCEC
/* 0044B774 8FBC0018 */   lw    $gp, 0x18($sp)
.L0044B778:
/* 0044B778 8F8B8B44 */  lw     $t3, %got(use_c_semantics)($gp)
/* 0044B77C 8FAF0028 */  lw    $t7, 0x28($sp)
/* 0044B780 916B0000 */  lbu   $t3, ($t3)
/* 0044B784 51600007 */  beql  $t3, $zero, .L0044B7A4
/* 0044B788 90ED002F */   lbu   $t5, 0x2f($a3)
/* 0044B78C 90EC0022 */  lbu   $t4, 0x22($a3)
/* 0044B790 55800004 */  bnezl $t4, .L0044B7A4
/* 0044B794 90ED002F */   lbu   $t5, 0x2f($a3)
/* 0044B798 10000154 */  b     .L0044BCEC
/* 0044B79C 24040001 */   li    $a0, 1
/* 0044B7A0 90ED002F */  lbu   $t5, 0x2f($a3)
.L0044B7A4:
/* 0044B7A4 01AF082A */  slt   $at, $t5, $t7
/* 0044B7A8 54200004 */  bnezl $at, .L0044B7BC
/* 0044B7AC 24010004 */   li    $at, 4
/* 0044B7B0 1000014E */  b     .L0044BCEC
/* 0044B7B4 00002025 */   move  $a0, $zero
/* 0044B7B8 24010004 */  li    $at, 4
.L0044B7BC:
/* 0044B7BC 10810003 */  beq   $a0, $at, .L0044B7CC
/* 0044B7C0 00000000 */   nop   
/* 0044B7C4 10000149 */  b     .L0044BCEC
/* 0044B7C8 00002025 */   move  $a0, $zero
.L0044B7CC:
/* 0044B7CC 8F998494 */  lw    $t9, %call16(in_fsym)($gp)
/* 0044B7D0 8CE4002C */  lw    $a0, 0x2c($a3)
/* 0044B7D4 AFA70030 */  sw    $a3, 0x30($sp)
/* 0044B7D8 0320F809 */  jalr  $t9
/* 0044B7DC 000422C2 */   srl   $a0, $a0, 0xb
/* 0044B7E0 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044B7E4 10400003 */  beqz  $v0, .L0044B7F4
/* 0044B7E8 8FA70030 */   lw    $a3, 0x30($sp)
/* 0044B7EC 1000013F */  b     .L0044BCEC
/* 0044B7F0 00002025 */   move  $a0, $zero
.L0044B7F4:
/* 0044B7F4 90F80022 */  lbu   $t8, 0x22($a3)
/* 0044B7F8 17000003 */  bnez  $t8, .L0044B808
/* 0044B7FC 00000000 */   nop   
/* 0044B800 1000013A */  b     .L0044BCEC
/* 0044B804 24040001 */   li    $a0, 1
.L0044B808:
/* 0044B808 8F888B70 */  lw     $t0, %got(indirprocs)($gp)
/* 0044B80C 8FB9002C */  lw    $t9, 0x2c($sp)
/* 0044B810 8D080000 */  lw    $t0, ($t0)
/* 0044B814 03281826 */  xor   $v1, $t9, $t0
/* 0044B818 2C630001 */  sltiu $v1, $v1, 1
/* 0044B81C 14600018 */  bnez  $v1, .L0044B880
/* 0044B820 00000000 */   nop   
/* 0044B824 93230009 */  lbu   $v1, 9($t9)
/* 0044B828 14600015 */  bnez  $v1, .L0044B880
/* 0044B82C 00000000 */   nop   
/* 0044B830 8CE40028 */  lw    $a0, 0x28($a3)
/* 0044B834 AFA40000 */  sw    $a0, ($sp)
/* 0044B838 8CE5002C */  lw    $a1, 0x2c($a3)
/* 0044B83C AFA50004 */  sw    $a1, 4($sp)
/* 0044B840 8F260004 */  lw    $a2, 4($t9)
/* 0044B844 8F99848C */  lw    $t9, %call16(varintree)($gp)
/* 0044B848 AFA70030 */  sw    $a3, 0x30($sp)
/* 0044B84C 0320F809 */  jalr  $t9
/* 0044B850 00000000 */   nop   
/* 0044B854 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044B858 8FA70030 */  lw    $a3, 0x30($sp)
/* 0044B85C 14400008 */  bnez  $v0, .L0044B880
/* 0044B860 00401825 */   move  $v1, $v0
/* 0044B864 8F998490 */  lw    $t9, %call16(furthervarintree)($gp)
/* 0044B868 00E02025 */  move  $a0, $a3
/* 0044B86C 8FA5002C */  lw    $a1, 0x2c($sp)
/* 0044B870 0320F809 */  jalr  $t9
/* 0044B874 00000000 */   nop   
/* 0044B878 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044B87C 00401825 */  move  $v1, $v0
.L0044B880:
/* 0044B880 1000011A */  b     .L0044BCEC
/* 0044B884 306400FF */   andi  $a0, $v1, 0xff
.L0044B888:
/* 0044B888 90E20022 */  lbu   $v0, 0x22($a3)
/* 0044B88C 8FAC0028 */  lw    $t4, 0x28($sp)
/* 0044B890 54400008 */  bnezl $v0, .L0044B8B4
/* 0044B894 90EB002F */   lbu   $t3, 0x2f($a3)
/* 0044B898 8F8E8B44 */  lw     $t6, %got(use_c_semantics)($gp)
/* 0044B89C 91CE0000 */  lbu   $t6, ($t6)
/* 0044B8A0 51C00004 */  beql  $t6, $zero, .L0044B8B4
/* 0044B8A4 90EB002F */   lbu   $t3, 0x2f($a3)
/* 0044B8A8 10000110 */  b     .L0044BCEC
/* 0044B8AC 24040001 */   li    $a0, 1
/* 0044B8B0 90EB002F */  lbu   $t3, 0x2f($a3)
.L0044B8B4:
/* 0044B8B4 016C082A */  slt   $at, $t3, $t4
/* 0044B8B8 14200003 */  bnez  $at, .L0044B8C8
/* 0044B8BC 00000000 */   nop   
/* 0044B8C0 1000010A */  b     .L0044BCEC
/* 0044B8C4 00002025 */   move  $a0, $zero
.L0044B8C8:
/* 0044B8C8 14400003 */  bnez  $v0, .L0044B8D8
/* 0044B8CC 8FAD002C */   lw    $t5, 0x2c($sp)
/* 0044B8D0 10000106 */  b     .L0044BCEC
/* 0044B8D4 24040001 */   li    $a0, 1
.L0044B8D8:
/* 0044B8D8 91A30009 */  lbu   $v1, 9($t5)
/* 0044B8DC 8FAF002C */  lw    $t7, 0x2c($sp)
/* 0044B8E0 10600003 */  beqz  $v1, .L0044B8F0
/* 0044B8E4 00000000 */   nop   
/* 0044B8E8 38830004 */  xori  $v1, $a0, 4
/* 0044B8EC 2C630001 */  sltiu $v1, $v1, 1
.L0044B8F0:
/* 0044B8F0 14600022 */  bnez  $v1, .L0044B97C
/* 0044B8F4 00000000 */   nop   
/* 0044B8F8 91E30008 */  lbu   $v1, 8($t7)
/* 0044B8FC 8FA9002C */  lw    $t1, 0x2c($sp)
/* 0044B900 2C630001 */  sltiu $v1, $v1, 1
/* 0044B904 10600005 */  beqz  $v1, .L0044B91C
/* 0044B908 00000000 */   nop   
/* 0044B90C 8F98897C */  lw     $t8, %got(curlevel)($gp)
/* 0044B910 91E80010 */  lbu   $t0, 0x10($t7)
/* 0044B914 8F180000 */  lw    $t8, ($t8)
/* 0044B918 0308182A */  slt   $v1, $t8, $t0
.L0044B91C:
/* 0044B91C 14600017 */  bnez  $v1, .L0044B97C
/* 0044B920 00000000 */   nop   
/* 0044B924 9123000A */  lbu   $v1, 0xa($t1)
/* 0044B928 14600014 */  bnez  $v1, .L0044B97C
/* 0044B92C 00000000 */   nop   
/* 0044B930 8CE40028 */  lw    $a0, 0x28($a3)
/* 0044B934 8F99848C */  lw    $t9, %call16(varintree)($gp)
/* 0044B938 AFA40000 */  sw    $a0, ($sp)
/* 0044B93C 8CE5002C */  lw    $a1, 0x2c($a3)
/* 0044B940 AFA50004 */  sw    $a1, 4($sp)
/* 0044B944 8D260004 */  lw    $a2, 4($t1)
/* 0044B948 0320F809 */  jalr  $t9
/* 0044B94C AFA70030 */   sw    $a3, 0x30($sp)
/* 0044B950 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044B954 8FA70030 */  lw    $a3, 0x30($sp)
/* 0044B958 14400008 */  bnez  $v0, .L0044B97C
/* 0044B95C 00401825 */   move  $v1, $v0
/* 0044B960 8F998490 */  lw    $t9, %call16(furthervarintree)($gp)
/* 0044B964 00E02025 */  move  $a0, $a3
/* 0044B968 8FA5002C */  lw    $a1, 0x2c($sp)
/* 0044B96C 0320F809 */  jalr  $t9
/* 0044B970 00000000 */   nop   
/* 0044B974 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044B978 00401825 */  move  $v1, $v0
.L0044B97C:
/* 0044B97C 100000DB */  b     .L0044BCEC
/* 0044B980 306400FF */   andi  $a0, $v1, 0xff
.L0044B984:
/* 0044B984 90EE002F */  lbu   $t6, 0x2f($a3)
/* 0044B988 8FAB0028 */  lw    $t3, 0x28($sp)
/* 0044B98C 01CB082A */  slt   $at, $t6, $t3
/* 0044B990 54200004 */  bnezl $at, .L0044B9A4
/* 0044B994 90EC0022 */   lbu   $t4, 0x22($a3)
/* 0044B998 100000D4 */  b     .L0044BCEC
/* 0044B99C 00002025 */   move  $a0, $zero
/* 0044B9A0 90EC0022 */  lbu   $t4, 0x22($a3)
.L0044B9A4:
/* 0044B9A4 8FAD002C */  lw    $t5, 0x2c($sp)
/* 0044B9A8 11800009 */  beqz  $t4, .L0044B9D0
/* 0044B9AC 00000000 */   nop   
/* 0044B9B0 91AF000F */  lbu   $t7, 0xf($t5)
/* 0044B9B4 11E00008 */  beqz  $t7, .L0044B9D8
/* 0044B9B8 00000000 */   nop   
/* 0044B9BC 8F988B6C */  lw     $t8, %got(curproc)($gp)
/* 0044B9C0 8F180000 */  lw    $t8, ($t8)
/* 0044B9C4 93080014 */  lbu   $t0, 0x14($t8)
/* 0044B9C8 11000003 */  beqz  $t0, .L0044B9D8
/* 0044B9CC 00000000 */   nop   
.L0044B9D0:
/* 0044B9D0 100000C6 */  b     .L0044BCEC
/* 0044B9D4 24040001 */   li    $a0, 1
.L0044B9D8:
/* 0044B9D8 8F998B70 */  lw     $t9, %got(indirprocs)($gp)
/* 0044B9DC 8FAA002C */  lw    $t2, 0x2c($sp)
/* 0044B9E0 8F390000 */  lw    $t9, ($t9)
/* 0044B9E4 01591826 */  xor   $v1, $t2, $t9
/* 0044B9E8 2C630001 */  sltiu $v1, $v1, 1
/* 0044B9EC 1460001C */  bnez  $v1, .L0044BA60
/* 0044B9F0 00000000 */   nop   
/* 0044B9F4 91430009 */  lbu   $v1, 9($t2)
/* 0044B9F8 10600003 */  beqz  $v1, .L0044BA08
/* 0044B9FC 00000000 */   nop   
/* 0044BA00 38830004 */  xori  $v1, $a0, 4
/* 0044BA04 2C630001 */  sltiu $v1, $v1, 1
.L0044BA08:
/* 0044BA08 14600015 */  bnez  $v1, .L0044BA60
/* 0044BA0C 00000000 */   nop   
/* 0044BA10 8CE40028 */  lw    $a0, 0x28($a3)
/* 0044BA14 8FAB002C */  lw    $t3, 0x2c($sp)
/* 0044BA18 8F99848C */  lw    $t9, %call16(varintree)($gp)
/* 0044BA1C AFA40000 */  sw    $a0, ($sp)
/* 0044BA20 8CE5002C */  lw    $a1, 0x2c($a3)
/* 0044BA24 AFA50004 */  sw    $a1, 4($sp)
/* 0044BA28 8D660004 */  lw    $a2, 4($t3)
/* 0044BA2C 0320F809 */  jalr  $t9
/* 0044BA30 AFA70030 */   sw    $a3, 0x30($sp)
/* 0044BA34 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044BA38 8FA70030 */  lw    $a3, 0x30($sp)
/* 0044BA3C 14400008 */  bnez  $v0, .L0044BA60
/* 0044BA40 00401825 */   move  $v1, $v0
/* 0044BA44 8F998490 */  lw    $t9, %call16(furthervarintree)($gp)
/* 0044BA48 00E02025 */  move  $a0, $a3
/* 0044BA4C 8FA5002C */  lw    $a1, 0x2c($sp)
/* 0044BA50 0320F809 */  jalr  $t9
/* 0044BA54 00000000 */   nop   
/* 0044BA58 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044BA5C 00401825 */  move  $v1, $v0
.L0044BA60:
/* 0044BA60 100000A2 */  b     .L0044BCEC
/* 0044BA64 306400FF */   andi  $a0, $v1, 0xff
.L0044BA68:
/* 0044BA68 8F9988A4 */  lw    $t9, %call16(caseerror)($gp)
/* 0044BA6C 8F868044 */  lw    $a2, %got(RO_1000CB48)($gp)
/* 0044BA70 24040001 */  li    $a0, 1
/* 0044BA74 2407000A */  li    $a3, 10
/* 0044BA78 0320F809 */  jalr  $t9
/* 0044BA7C 24C6CB48 */   addiu $a2, %lo(RO_1000CB48) # addiu $a2, $a2, -0x34b8
/* 0044BA80 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044BA84 10000099 */  b     .L0044BCEC
/* 0044BA88 93A40026 */   lbu   $a0, 0x26($sp)
.L0044BA8C:
/* 0044BA8C 8F828A5C */  lw     $v0, %got(lang)($gp)
/* 0044BA90 3C012A00 */  lui   $at, 0x2a00
/* 0044BA94 90420000 */  lbu   $v0, ($v0)
/* 0044BA98 2C4C0020 */  sltiu $t4, $v0, 0x20
/* 0044BA9C 000C6823 */  negu  $t5, $t4
/* 0044BAA0 01A17824 */  and   $t7, $t5, $at
/* 0044BAA4 004FC004 */  sllv  $t8, $t7, $v0
/* 0044BAA8 07000007 */  bltz  $t8, .L0044BAC8
/* 0044BAAC 24010003 */   li    $at, 3
/* 0044BAB0 54410008 */  bnel  $v0, $at, .L0044BAD4
/* 0044BAB4 90F90020 */   lbu   $t9, 0x20($a3)
/* 0044BAB8 8F888B44 */  lw     $t0, %got(use_c_semantics)($gp)
/* 0044BABC 91080000 */  lbu   $t0, ($t0)
/* 0044BAC0 51000004 */  beql  $t0, $zero, .L0044BAD4
/* 0044BAC4 90F90020 */   lbu   $t9, 0x20($a3)
.L0044BAC8:
/* 0044BAC8 10000088 */  b     .L0044BCEC
/* 0044BACC 24040001 */   li    $a0, 1
/* 0044BAD0 90F90020 */  lbu   $t9, 0x20($a3)
.L0044BAD4:
/* 0044BAD4 8FAD002C */  lw    $t5, 0x2c($sp)
/* 0044BAD8 272AFF80 */  addiu $t2, $t9, -0x80
/* 0044BADC 2D490020 */  sltiu $t1, $t2, 0x20
/* 0044BAE0 00097023 */  negu  $t6, $t1
/* 0044BAE4 31CB1020 */  andi  $t3, $t6, 0x1020
/* 0044BAE8 014B6004 */  sllv  $t4, $t3, $t2
/* 0044BAEC 05830004 */  bgezl $t4, .L0044BB00
/* 0044BAF0 91AF000E */   lbu   $t7, 0xe($t5)
/* 0044BAF4 1000007E */  b     .L0044BCF0
/* 0044BAF8 24020001 */   li    $v0, 1
/* 0044BAFC 91AF000E */  lbu   $t7, 0xe($t5)
.L0044BB00:
/* 0044BB00 51E0000B */  beql  $t7, $zero, .L0044BB30
/* 0044BB04 8CE50034 */   lw    $a1, 0x34($a3)
/* 0044BB08 8F988A30 */  lw     $t8, %got(aentptr)($gp)
/* 0044BB0C 24010001 */  li    $at, 1
/* 0044BB10 8F180000 */  lw    $t8, ($t8)
/* 0044BB14 13000003 */  beqz  $t8, .L0044BB24
/* 0044BB18 00000000 */   nop   
/* 0044BB1C 50410004 */  beql  $v0, $at, .L0044BB30
/* 0044BB20 8CE50034 */   lw    $a1, 0x34($a3)
.L0044BB24:
/* 0044BB24 10000072 */  b     .L0044BCF0
/* 0044BB28 00001025 */   move  $v0, $zero
/* 0044BB2C 8CE50034 */  lw    $a1, 0x34($a3)
.L0044BB30:
/* 0044BB30 24010007 */  li    $at, 7
/* 0044BB34 90A20000 */  lbu   $v0, ($a1)
/* 0044BB38 5441000B */  bnel  $v0, $at, .L0044BB68
/* 0044BB3C 2C480020 */   sltiu $t0, $v0, 0x20
/* 0044BB40 8F998630 */  lw    $t9, %call16(fix_base)($gp)
/* 0044BB44 00E02025 */  move  $a0, $a3
/* 0044BB48 AFA70030 */  sw    $a3, 0x30($sp)
/* 0044BB4C 0320F809 */  jalr  $t9
/* 0044BB50 00000000 */   nop   
/* 0044BB54 8FA70030 */  lw    $a3, 0x30($sp)
/* 0044BB58 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044BB5C 8CE50034 */  lw    $a1, 0x34($a3)
/* 0044BB60 90A20000 */  lbu   $v0, ($a1)
/* 0044BB64 2C480020 */  sltiu $t0, $v0, 0x20
.L0044BB68:
/* 0044BB68 0008C823 */  negu  $t9, $t0
/* 0044BB6C 3C014400 */  lui   $at, 0x4400
/* 0044BB70 03214824 */  and   $t1, $t9, $at
/* 0044BB74 00497004 */  sllv  $t6, $t1, $v0
/* 0044BB78 05C1001D */  bgez  $t6, .L0044BBF0
/* 0044BB7C 2C4D0020 */   sltiu $t5, $v0, 0x20
/* 0044BB80 8F8B8A5C */  lw     $t3, %got(lang)($gp)
/* 0044BB84 24010005 */  li    $at, 5
/* 0044BB88 8FAC0028 */  lw    $t4, 0x28($sp)
/* 0044BB8C 916B0000 */  lbu   $t3, ($t3)
/* 0044BB90 51610007 */  beql  $t3, $at, .L0044BBB0
/* 0044BB94 90A30032 */   lbu   $v1, 0x32($a1)
/* 0044BB98 8CAA0028 */  lw    $t2, 0x28($a1)
/* 0044BB9C 90E60020 */  lbu   $a2, 0x20($a3)
/* 0044BBA0 014C202A */  slt   $a0, $t2, $t4
/* 0044BBA4 1000001D */  b     .L0044BC1C
/* 0044BBA8 308400FF */   andi  $a0, $a0, 0xff
/* 0044BBAC 90A30032 */  lbu   $v1, 0x32($a1)
.L0044BBB0:
/* 0044BBB0 30630007 */  andi  $v1, $v1, 7
/* 0044BBB4 38630004 */  xori  $v1, $v1, 4
/* 0044BBB8 2C630001 */  sltiu $v1, $v1, 1
/* 0044BBBC 5060000A */  beql  $v1, $zero, .L0044BBE8
/* 0044BBC0 306400FF */   andi  $a0, $v1, 0xff
/* 0044BBC4 8F998494 */  lw    $t9, %call16(in_fsym)($gp)
/* 0044BBC8 8CA40030 */  lw    $a0, 0x30($a1)
/* 0044BBCC AFA70030 */  sw    $a3, 0x30($sp)
/* 0044BBD0 0320F809 */  jalr  $t9
/* 0044BBD4 000422C2 */   srl   $a0, $a0, 0xb
/* 0044BBD8 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044BBDC 8FA70030 */  lw    $a3, 0x30($sp)
/* 0044BBE0 2C430001 */  sltiu $v1, $v0, 1
/* 0044BBE4 306400FF */  andi  $a0, $v1, 0xff
.L0044BBE8:
/* 0044BBE8 1000000C */  b     .L0044BC1C
/* 0044BBEC 90E60020 */   lbu   $a2, 0x20($a3)
.L0044BBF0:
/* 0044BBF0 000D7823 */  negu  $t7, $t5
/* 0044BBF4 3C011B00 */  lui   $at, 0x1b00
/* 0044BBF8 01E1C024 */  and   $t8, $t7, $at
/* 0044BBFC 00584004 */  sllv  $t0, $t8, $v0
/* 0044BC00 05000003 */  bltz  $t0, .L0044BC10
/* 0044BC04 90E60020 */   lbu   $a2, 0x20($a3)
/* 0044BC08 10000004 */  b     .L0044BC1C
/* 0044BC0C 00002025 */   move  $a0, $zero
.L0044BC10:
/* 0044BC10 8FA40028 */  lw    $a0, 0x28($sp)
/* 0044BC14 0004202A */  slt   $a0, $zero, $a0
/* 0044BC18 308400FF */  andi  $a0, $a0, 0xff
.L0044BC1C:
/* 0044BC1C 24D9FFE0 */  addiu $t9, $a2, -0x20
/* 0044BC20 2F290020 */  sltiu $t1, $t9, 0x20
/* 0044BC24 00097023 */  negu  $t6, $t1
/* 0044BC28 31CB1200 */  andi  $t3, $t6, 0x1200
/* 0044BC2C 032B5004 */  sllv  $t2, $t3, $t9
/* 0044BC30 0542002F */  bltzl $t2, .L0044BCF0
/* 0044BC34 00801025 */   move  $v0, $a0
/* 0044BC38 5480002D */  bnezl $a0, .L0044BCF0
/* 0044BC3C 00801025 */   move  $v0, $a0
/* 0044BC40 8CE20038 */  lw    $v0, 0x38($a3)
/* 0044BC44 3C014400 */  lui   $at, 0x4400
/* 0044BC48 90430000 */  lbu   $v1, ($v0)
/* 0044BC4C 2C6C0020 */  sltiu $t4, $v1, 0x20
/* 0044BC50 000C6823 */  negu  $t5, $t4
/* 0044BC54 01A17824 */  and   $t7, $t5, $at
/* 0044BC58 006FC004 */  sllv  $t8, $t7, $v1
/* 0044BC5C 07010019 */  bgez  $t8, .L0044BCC4
/* 0044BC60 2C6B0020 */   sltiu $t3, $v1, 0x20
/* 0044BC64 8F888A5C */  lw     $t0, %got(lang)($gp)
/* 0044BC68 24010005 */  li    $at, 5
/* 0044BC6C 8FAE0028 */  lw    $t6, 0x28($sp)
/* 0044BC70 91080000 */  lbu   $t0, ($t0)
/* 0044BC74 51010006 */  beql  $t0, $at, .L0044BC90
/* 0044BC78 90430032 */   lbu   $v1, 0x32($v0)
/* 0044BC7C 8C490028 */  lw    $t1, 0x28($v0)
/* 0044BC80 012E202A */  slt   $a0, $t1, $t6
/* 0044BC84 10000019 */  b     .L0044BCEC
/* 0044BC88 308400FF */   andi  $a0, $a0, 0xff
/* 0044BC8C 90430032 */  lbu   $v1, 0x32($v0)
.L0044BC90:
/* 0044BC90 30630007 */  andi  $v1, $v1, 7
/* 0044BC94 38630004 */  xori  $v1, $v1, 4
/* 0044BC98 2C630001 */  sltiu $v1, $v1, 1
/* 0044BC9C 10600007 */  beqz  $v1, .L0044BCBC
/* 0044BCA0 00000000 */   nop   
/* 0044BCA4 8F998494 */  lw    $t9, %call16(in_fsym)($gp)
/* 0044BCA8 8C440030 */  lw    $a0, 0x30($v0)
/* 0044BCAC 0320F809 */  jalr  $t9
/* 0044BCB0 000422C2 */   srl   $a0, $a0, 0xb
/* 0044BCB4 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044BCB8 2C430001 */  sltiu $v1, $v0, 1
.L0044BCBC:
/* 0044BCBC 1000000B */  b     .L0044BCEC
/* 0044BCC0 306400FF */   andi  $a0, $v1, 0xff
.L0044BCC4:
/* 0044BCC4 000BC823 */  negu  $t9, $t3
/* 0044BCC8 3C011B00 */  lui   $at, 0x1b00
/* 0044BCCC 03215024 */  and   $t2, $t9, $at
/* 0044BCD0 006A6004 */  sllv  $t4, $t2, $v1
/* 0044BCD4 05800003 */  bltz  $t4, .L0044BCE4
/* 0044BCD8 8FA40028 */   lw    $a0, 0x28($sp)
/* 0044BCDC 10000003 */  b     .L0044BCEC
/* 0044BCE0 00002025 */   move  $a0, $zero
.L0044BCE4:
/* 0044BCE4 0004202A */  slt   $a0, $zero, $a0
/* 0044BCE8 308400FF */  andi  $a0, $a0, 0xff
.L0044BCEC:
/* 0044BCEC 00801025 */  move  $v0, $a0
.L0044BCF0:
/* 0044BCF0 8FBF001C */  lw    $ra, 0x1c($sp)
/* 0044BCF4 27BD0028 */  addiu $sp, $sp, 0x28
/* 0044BCF8 03E00008 */  jr    $ra
/* 0044BCFC 00000000 */   nop   
    .type clkilled, @function
    .size clkilled, .-clkilled
    .end clkilled

glabel cmkilled
    .ent cmkilled
    # 004175BC copypropagate
    # 0044C4D8 cupkillprev
    # 0044D4B4 listpskilled
    # 0045D208 patchvectors
/* 0044BD00 3C1C0FBD */  .cpload $t9
/* 0044BD04 279CE590 */  
/* 0044BD08 0399E021 */  
/* 0044BD0C 8F838A5C */  lw     $v1, %got(lang)($gp)
/* 0044BD10 27BDFFE0 */  addiu $sp, $sp, -0x20
/* 0044BD14 3C012A00 */  lui   $at, 0x2a00
/* 0044BD18 90630000 */  lbu   $v1, ($v1)
/* 0044BD1C AFA50024 */  sw    $a1, 0x24($sp)
/* 0044BD20 00802825 */  move  $a1, $a0
/* 0044BD24 2C6E0020 */  sltiu $t6, $v1, 0x20
/* 0044BD28 000E7823 */  negu  $t7, $t6
/* 0044BD2C 01E1C024 */  and   $t8, $t7, $at
/* 0044BD30 0078C804 */  sllv  $t9, $t8, $v1
/* 0044BD34 AFBF001C */  sw    $ra, 0x1c($sp)
/* 0044BD38 AFBC0018 */  sw    $gp, 0x18($sp)
/* 0044BD3C 07210003 */  bgez  $t9, .L0044BD4C
/* 0044BD40 AFA40020 */   sw    $a0, 0x20($sp)
/* 0044BD44 10000028 */  b     .L0044BDE8
/* 0044BD48 24030001 */   li    $v1, 1
.L0044BD4C:
/* 0044BD4C 8CC2002C */  lw    $v0, 0x2c($a2)
/* 0044BD50 3C014400 */  lui   $at, 0x4400
/* 0044BD54 90440000 */  lbu   $a0, ($v0)
/* 0044BD58 2C880020 */  sltiu $t0, $a0, 0x20
/* 0044BD5C 00084823 */  negu  $t1, $t0
/* 0044BD60 01215024 */  and   $t2, $t1, $at
/* 0044BD64 008A5804 */  sllv  $t3, $t2, $a0
/* 0044BD68 05610016 */  bgez  $t3, .L0044BDC4
/* 0044BD6C 2C8D0020 */   sltiu $t5, $a0, 0x20
/* 0044BD70 24010005 */  li    $at, 5
/* 0044BD74 50610006 */  beql  $v1, $at, .L0044BD90
/* 0044BD78 90440032 */   lbu   $a0, 0x32($v0)
/* 0044BD7C 8C4C0028 */  lw    $t4, 0x28($v0)
/* 0044BD80 0185182A */  slt   $v1, $t4, $a1
/* 0044BD84 10000018 */  b     .L0044BDE8
/* 0044BD88 306300FF */   andi  $v1, $v1, 0xff
/* 0044BD8C 90440032 */  lbu   $a0, 0x32($v0)
.L0044BD90:
/* 0044BD90 30840007 */  andi  $a0, $a0, 7
/* 0044BD94 38840004 */  xori  $a0, $a0, 4
/* 0044BD98 2C840001 */  sltiu $a0, $a0, 1
/* 0044BD9C 10800007 */  beqz  $a0, .L0044BDBC
/* 0044BDA0 00000000 */   nop   
/* 0044BDA4 8F998494 */  lw    $t9, %call16(in_fsym)($gp)
/* 0044BDA8 8C440030 */  lw    $a0, 0x30($v0)
/* 0044BDAC 0320F809 */  jalr  $t9
/* 0044BDB0 000422C2 */   srl   $a0, $a0, 0xb
/* 0044BDB4 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044BDB8 2C440001 */  sltiu $a0, $v0, 1
.L0044BDBC:
/* 0044BDBC 1000000A */  b     .L0044BDE8
/* 0044BDC0 308300FF */   andi  $v1, $a0, 0xff
.L0044BDC4:
/* 0044BDC4 000D7023 */  negu  $t6, $t5
/* 0044BDC8 3C011B00 */  lui   $at, 0x1b00
/* 0044BDCC 01C17824 */  and   $t7, $t6, $at
/* 0044BDD0 008FC004 */  sllv  $t8, $t7, $a0
/* 0044BDD4 07000003 */  bltz  $t8, .L0044BDE4
/* 0044BDD8 0005182A */   slt   $v1, $zero, $a1
/* 0044BDDC 10000002 */  b     .L0044BDE8
/* 0044BDE0 00001825 */   move  $v1, $zero
.L0044BDE4:
/* 0044BDE4 306300FF */  andi  $v1, $v1, 0xff
.L0044BDE8:
/* 0044BDE8 8FBF001C */  lw    $ra, 0x1c($sp)
/* 0044BDEC 27BD0020 */  addiu $sp, $sp, 0x20
/* 0044BDF0 00601025 */  move  $v0, $v1
/* 0044BDF4 03E00008 */  jr    $ra
/* 0044BDF8 00000000 */   nop   
    .type cmkilled, @function
    .size cmkilled, .-cmkilled
    .end cmkilled

glabel cskilled
    .ent cskilled
    # 004175BC copypropagate
    # 004471AC codeimage
    # 0044C4D8 cupkillprev
    # 0044D4B4 listpskilled
    # 0045D208 patchvectors
/* 0044BDFC 3C1C0FBD */  .cpload $t9
/* 0044BE00 279CE494 */  
/* 0044BE04 0399E021 */  
/* 0044BE08 27BDFFD8 */  addiu $sp, $sp, -0x28
/* 0044BE0C AFBF001C */  sw    $ra, 0x1c($sp)
/* 0044BE10 AFBC0018 */  sw    $gp, 0x18($sp)
/* 0044BE14 AFA40028 */  sw    $a0, 0x28($sp)
/* 0044BE18 AFA5002C */  sw    $a1, 0x2c($sp)
/* 0044BE1C AFA60030 */  sw    $a2, 0x30($sp)
/* 0044BE20 90C20000 */  lbu   $v0, ($a2)
/* 0044BE24 8FAE0030 */  lw    $t6, 0x30($sp)
/* 0044BE28 244FFFE0 */  addiu $t7, $v0, -0x20
/* 0044BE2C 2DF80060 */  sltiu $t8, $t7, 0x60
/* 0044BE30 13000009 */  beqz  $t8, .L0044BE58
/* 0044BE34 00000000 */   nop   
/* 0044BE38 8F898044 */  lw    $t1, %got(D_10010AD4)($gp)
/* 0044BE3C 000FC943 */  sra   $t9, $t7, 5
/* 0044BE40 00194080 */  sll   $t0, $t9, 2
/* 0044BE44 25290AD4 */  addiu $t1, %lo(D_10010AD4) # addiu $t1, $t1, 0xad4
/* 0044BE48 01285021 */  addu  $t2, $t1, $t0
/* 0044BE4C 8D4B0000 */  lw    $t3, ($t2)
/* 0044BE50 01EB6004 */  sllv  $t4, $t3, $t7
/* 0044BE54 29980000 */  slti  $t8, $t4, 0
.L0044BE58:
/* 0044BE58 1300013E */  beqz  $t8, .L0044C354
/* 0044BE5C 00000000 */   nop   
/* 0044BE60 8DC70004 */  lw    $a3, 4($t6)
/* 0044BE64 24010003 */  li    $at, 3
/* 0044BE68 90E4002E */  lbu   $a0, 0x2e($a3)
/* 0044BE6C 30840007 */  andi  $a0, $a0, 7
/* 0044BE70 54810004 */  bnel  $a0, $at, .L0044BE84
/* 0044BE74 8FB9002C */   lw    $t9, 0x2c($sp)
/* 0044BE78 10000192 */  b     .L0044C4C4
/* 0044BE7C 24040001 */   li    $a0, 1
/* 0044BE80 8FB9002C */  lw    $t9, 0x2c($sp)
.L0044BE84:
/* 0044BE84 9329000E */  lbu   $t1, 0xe($t9)
/* 0044BE88 1120000C */  beqz  $t1, .L0044BEBC
/* 0044BE8C 00000000 */   nop   
/* 0044BE90 8F888A30 */  lw     $t0, %got(aentptr)($gp)
/* 0044BE94 8D080000 */  lw    $t0, ($t0)
/* 0044BE98 11000006 */  beqz  $t0, .L0044BEB4
/* 0044BE9C 00000000 */   nop   
/* 0044BEA0 8F8A8A5C */  lw     $t2, %got(lang)($gp)
/* 0044BEA4 24010001 */  li    $at, 1
/* 0044BEA8 914A0000 */  lbu   $t2, ($t2)
/* 0044BEAC 11410003 */  beq   $t2, $at, .L0044BEBC
/* 0044BEB0 00000000 */   nop   
.L0044BEB4:
/* 0044BEB4 10000183 */  b     .L0044C4C4
/* 0044BEB8 00002025 */   move  $a0, $zero
.L0044BEBC:
/* 0044BEBC 8F828A5C */  lw     $v0, %got(lang)($gp)
/* 0044BEC0 240502B9 */  li    $a1, 697
/* 0044BEC4 90420000 */  lbu   $v0, ($v0)
/* 0044BEC8 2C410007 */  sltiu $at, $v0, 7
/* 0044BECC 10200118 */  beqz  $at, .L0044C330
/* 0044BED0 00000000 */   nop   
/* 0044BED4 8F818044 */  lw    $at, %got(jtbl_1000CB7C)($gp)
/* 0044BED8 00025880 */  sll   $t3, $v0, 2
/* 0044BEDC 002B0821 */  addu  $at, $at, $t3
/* 0044BEE0 8C2BCB7C */  lw    $t3, %lo(jtbl_1000CB7C)($at)
/* 0044BEE4 017C5821 */  addu  $t3, $t3, $gp
/* 0044BEE8 01600008 */  jr    $t3
/* 0044BEEC 00000000 */   nop   
.L0044BEF0:
/* 0044BEF0 90EF0022 */  lbu   $t7, 0x22($a3)
/* 0044BEF4 11E00006 */  beqz  $t7, .L0044BF10
/* 0044BEF8 00000000 */   nop   
/* 0044BEFC 8F8C8B6C */  lw     $t4, %got(curproc)($gp)
/* 0044BF00 8D8C0000 */  lw    $t4, ($t4)
/* 0044BF04 918D0014 */  lbu   $t5, 0x14($t4)
/* 0044BF08 11A00003 */  beqz  $t5, .L0044BF18
/* 0044BF0C 00000000 */   nop   
.L0044BF10:
/* 0044BF10 1000016C */  b     .L0044C4C4
/* 0044BF14 24040001 */   li    $a0, 1
.L0044BF18:
/* 0044BF18 8F8E8B70 */  lw     $t6, %got(indirprocs)($gp)
/* 0044BF1C 8FB8002C */  lw    $t8, 0x2c($sp)
/* 0044BF20 8DCE0000 */  lw    $t6, ($t6)
/* 0044BF24 570E0004 */  bnel  $t8, $t6, .L0044BF38
/* 0044BF28 90E2002F */   lbu   $v0, 0x2f($a3)
/* 0044BF2C 10000165 */  b     .L0044C4C4
/* 0044BF30 24040001 */   li    $a0, 1
/* 0044BF34 90E2002F */  lbu   $v0, 0x2f($a3)
.L0044BF38:
/* 0044BF38 8FB90028 */  lw    $t9, 0x28($sp)
/* 0044BF3C 8FA9002C */  lw    $t1, 0x2c($sp)
/* 0044BF40 0059082A */  slt   $at, $v0, $t9
/* 0044BF44 54200004 */  bnezl $at, .L0044BF58
/* 0044BF48 91230009 */   lbu   $v1, 9($t1)
/* 0044BF4C 1000015D */  b     .L0044C4C4
/* 0044BF50 00002025 */   move  $a0, $zero
/* 0044BF54 91230009 */  lbu   $v1, 9($t1)
.L0044BF58:
/* 0044BF58 10600002 */  beqz  $v1, .L0044BF64
/* 0044BF5C 00000000 */   nop   
/* 0044BF60 2C430002 */  sltiu $v1, $v0, 2
.L0044BF64:
/* 0044BF64 14600013 */  bnez  $v1, .L0044BFB4
/* 0044BF68 00000000 */   nop   
/* 0044BF6C 8CE40028 */  lw    $a0, 0x28($a3)
/* 0044BF70 8F99848C */  lw    $t9, %call16(varintree)($gp)
/* 0044BF74 8FAB002C */  lw    $t3, 0x2c($sp)
/* 0044BF78 AFA40000 */  sw    $a0, ($sp)
/* 0044BF7C 8CE5002C */  lw    $a1, 0x2c($a3)
/* 0044BF80 AFA50004 */  sw    $a1, 4($sp)
/* 0044BF84 0320F809 */  jalr  $t9
/* 0044BF88 8D660004 */   lw    $a2, 4($t3)
/* 0044BF8C 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044BF90 14400008 */  bnez  $v0, .L0044BFB4
/* 0044BF94 00401825 */   move  $v1, $v0
/* 0044BF98 8F998490 */  lw    $t9, %call16(furthervarintree)($gp)
/* 0044BF9C 8FAF0030 */  lw    $t7, 0x30($sp)
/* 0044BFA0 8FA5002C */  lw    $a1, 0x2c($sp)
/* 0044BFA4 0320F809 */  jalr  $t9
/* 0044BFA8 8DE40004 */   lw    $a0, 4($t7)
/* 0044BFAC 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044BFB0 00401825 */  move  $v1, $v0
.L0044BFB4:
/* 0044BFB4 10000143 */  b     .L0044C4C4
/* 0044BFB8 306400FF */   andi  $a0, $v1, 0xff
.L0044BFBC:
/* 0044BFBC 90EC0022 */  lbu   $t4, 0x22($a3)
/* 0044BFC0 8FB80028 */  lw    $t8, 0x28($sp)
/* 0044BFC4 55800004 */  bnezl $t4, .L0044BFD8
/* 0044BFC8 90ED002F */   lbu   $t5, 0x2f($a3)
/* 0044BFCC 1000013D */  b     .L0044C4C4
/* 0044BFD0 24040001 */   li    $a0, 1
/* 0044BFD4 90ED002F */  lbu   $t5, 0x2f($a3)
.L0044BFD8:
/* 0044BFD8 01B8082A */  slt   $at, $t5, $t8
/* 0044BFDC 14200003 */  bnez  $at, .L0044BFEC
/* 0044BFE0 00000000 */   nop   
/* 0044BFE4 10000137 */  b     .L0044C4C4
/* 0044BFE8 00002025 */   move  $a0, $zero
.L0044BFEC:
/* 0044BFEC 8F998B70 */  lw     $t9, %got(indirprocs)($gp)
/* 0044BFF0 8FAE002C */  lw    $t6, 0x2c($sp)
/* 0044BFF4 8F390000 */  lw    $t9, ($t9)
/* 0044BFF8 01D91826 */  xor   $v1, $t6, $t9
/* 0044BFFC 2C630001 */  sltiu $v1, $v1, 1
/* 0044C000 14600015 */  bnez  $v1, .L0044C058
/* 0044C004 00000000 */   nop   
/* 0044C008 91C30009 */  lbu   $v1, 9($t6)
/* 0044C00C 14600012 */  bnez  $v1, .L0044C058
/* 0044C010 00000000 */   nop   
/* 0044C014 8CE40028 */  lw    $a0, 0x28($a3)
/* 0044C018 8F99848C */  lw    $t9, %call16(varintree)($gp)
/* 0044C01C AFA40000 */  sw    $a0, ($sp)
/* 0044C020 8CE5002C */  lw    $a1, 0x2c($a3)
/* 0044C024 AFA50004 */  sw    $a1, 4($sp)
/* 0044C028 0320F809 */  jalr  $t9
/* 0044C02C 8DC60004 */   lw    $a2, 4($t6)
/* 0044C030 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044C034 14400008 */  bnez  $v0, .L0044C058
/* 0044C038 00401825 */   move  $v1, $v0
/* 0044C03C 8F998490 */  lw    $t9, %call16(furthervarintree)($gp)
/* 0044C040 8FAA0030 */  lw    $t2, 0x30($sp)
/* 0044C044 8FA5002C */  lw    $a1, 0x2c($sp)
/* 0044C048 0320F809 */  jalr  $t9
/* 0044C04C 8D440004 */   lw    $a0, 4($t2)
/* 0044C050 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044C054 00401825 */  move  $v1, $v0
.L0044C058:
/* 0044C058 1000011A */  b     .L0044C4C4
/* 0044C05C 306400FF */   andi  $a0, $v1, 0xff
.L0044C060:
/* 0044C060 24010004 */  li    $at, 4
/* 0044C064 10810003 */  beq   $a0, $at, .L0044C074
/* 0044C068 00000000 */   nop   
/* 0044C06C 10000115 */  b     .L0044C4C4
/* 0044C070 00002025 */   move  $a0, $zero
.L0044C074:
/* 0044C074 8F998494 */  lw    $t9, %call16(in_fsym)($gp)
/* 0044C078 8CE4002C */  lw    $a0, 0x2c($a3)
/* 0044C07C 0320F809 */  jalr  $t9
/* 0044C080 000422C2 */   srl   $a0, $a0, 0xb
/* 0044C084 2C440001 */  sltiu $a0, $v0, 1
/* 0044C088 308400FF */  andi  $a0, $a0, 0xff
/* 0044C08C 1000010D */  b     .L0044C4C4
/* 0044C090 8FBC0018 */   lw    $gp, 0x18($sp)
.L0044C094:
/* 0044C094 8F8B8B44 */  lw     $t3, %got(use_c_semantics)($gp)
/* 0044C098 8FAD0028 */  lw    $t5, 0x28($sp)
/* 0044C09C 916B0000 */  lbu   $t3, ($t3)
/* 0044C0A0 51600007 */  beql  $t3, $zero, .L0044C0C0
/* 0044C0A4 90EC002F */   lbu   $t4, 0x2f($a3)
/* 0044C0A8 90EF0022 */  lbu   $t7, 0x22($a3)
/* 0044C0AC 55E00004 */  bnezl $t7, .L0044C0C0
/* 0044C0B0 90EC002F */   lbu   $t4, 0x2f($a3)
/* 0044C0B4 10000103 */  b     .L0044C4C4
/* 0044C0B8 24040001 */   li    $a0, 1
/* 0044C0BC 90EC002F */  lbu   $t4, 0x2f($a3)
.L0044C0C0:
/* 0044C0C0 018D082A */  slt   $at, $t4, $t5
/* 0044C0C4 54200004 */  bnezl $at, .L0044C0D8
/* 0044C0C8 24010004 */   li    $at, 4
/* 0044C0CC 100000FD */  b     .L0044C4C4
/* 0044C0D0 00002025 */   move  $a0, $zero
/* 0044C0D4 24010004 */  li    $at, 4
.L0044C0D8:
/* 0044C0D8 10810003 */  beq   $a0, $at, .L0044C0E8
/* 0044C0DC 00000000 */   nop   
/* 0044C0E0 100000F8 */  b     .L0044C4C4
/* 0044C0E4 00002025 */   move  $a0, $zero
.L0044C0E8:
/* 0044C0E8 8F998494 */  lw    $t9, %call16(in_fsym)($gp)
/* 0044C0EC 8CE4002C */  lw    $a0, 0x2c($a3)
/* 0044C0F0 0320F809 */  jalr  $t9
/* 0044C0F4 000422C2 */   srl   $a0, $a0, 0xb
/* 0044C0F8 10400003 */  beqz  $v0, .L0044C108
/* 0044C0FC 8FBC0018 */   lw    $gp, 0x18($sp)
/* 0044C100 100000F0 */  b     .L0044C4C4
/* 0044C104 00002025 */   move  $a0, $zero
.L0044C108:
/* 0044C108 8FB80030 */  lw    $t8, 0x30($sp)
/* 0044C10C 8F070004 */  lw    $a3, 4($t8)
/* 0044C110 90F90022 */  lbu   $t9, 0x22($a3)
/* 0044C114 17200003 */  bnez  $t9, .L0044C124
/* 0044C118 00000000 */   nop   
/* 0044C11C 100000E9 */  b     .L0044C4C4
/* 0044C120 24040001 */   li    $a0, 1
.L0044C124:
/* 0044C124 8F888B70 */  lw     $t0, %got(indirprocs)($gp)
/* 0044C128 8FA9002C */  lw    $t1, 0x2c($sp)
/* 0044C12C 8D080000 */  lw    $t0, ($t0)
/* 0044C130 01281826 */  xor   $v1, $t1, $t0
/* 0044C134 2C630001 */  sltiu $v1, $v1, 1
/* 0044C138 14600015 */  bnez  $v1, .L0044C190
/* 0044C13C 00000000 */   nop   
/* 0044C140 91230009 */  lbu   $v1, 9($t1)
/* 0044C144 14600012 */  bnez  $v1, .L0044C190
/* 0044C148 00000000 */   nop   
/* 0044C14C 8CE40028 */  lw    $a0, 0x28($a3)
/* 0044C150 8F99848C */  lw    $t9, %call16(varintree)($gp)
/* 0044C154 AFA40000 */  sw    $a0, ($sp)
/* 0044C158 8CE5002C */  lw    $a1, 0x2c($a3)
/* 0044C15C AFA50004 */  sw    $a1, 4($sp)
/* 0044C160 0320F809 */  jalr  $t9
/* 0044C164 8D260004 */   lw    $a2, 4($t1)
/* 0044C168 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044C16C 14400008 */  bnez  $v0, .L0044C190
/* 0044C170 00401825 */   move  $v1, $v0
/* 0044C174 8F998490 */  lw    $t9, %call16(furthervarintree)($gp)
/* 0044C178 8FAB0030 */  lw    $t3, 0x30($sp)
/* 0044C17C 8FA5002C */  lw    $a1, 0x2c($sp)
/* 0044C180 0320F809 */  jalr  $t9
/* 0044C184 8D640004 */   lw    $a0, 4($t3)
/* 0044C188 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044C18C 00401825 */  move  $v1, $v0
.L0044C190:
/* 0044C190 100000CC */  b     .L0044C4C4
/* 0044C194 306400FF */   andi  $a0, $v1, 0xff
.L0044C198:
/* 0044C198 90E20022 */  lbu   $v0, 0x22($a3)
/* 0044C19C 8FAD0028 */  lw    $t5, 0x28($sp)
/* 0044C1A0 54400008 */  bnezl $v0, .L0044C1C4
/* 0044C1A4 90EC002F */   lbu   $t4, 0x2f($a3)
/* 0044C1A8 8F8F8B44 */  lw     $t7, %got(use_c_semantics)($gp)
/* 0044C1AC 91EF0000 */  lbu   $t7, ($t7)
/* 0044C1B0 51E00004 */  beql  $t7, $zero, .L0044C1C4
/* 0044C1B4 90EC002F */   lbu   $t4, 0x2f($a3)
/* 0044C1B8 100000C2 */  b     .L0044C4C4
/* 0044C1BC 24040001 */   li    $a0, 1
/* 0044C1C0 90EC002F */  lbu   $t4, 0x2f($a3)
.L0044C1C4:
/* 0044C1C4 018D082A */  slt   $at, $t4, $t5
/* 0044C1C8 14200003 */  bnez  $at, .L0044C1D8
/* 0044C1CC 00000000 */   nop   
/* 0044C1D0 100000BC */  b     .L0044C4C4
/* 0044C1D4 00002025 */   move  $a0, $zero
.L0044C1D8:
/* 0044C1D8 14400003 */  bnez  $v0, .L0044C1E8
/* 0044C1DC 8FB8002C */   lw    $t8, 0x2c($sp)
/* 0044C1E0 100000B8 */  b     .L0044C4C4
/* 0044C1E4 24040001 */   li    $a0, 1
.L0044C1E8:
/* 0044C1E8 93030009 */  lbu   $v1, 9($t8)
/* 0044C1EC 10600003 */  beqz  $v1, .L0044C1FC
/* 0044C1F0 00000000 */   nop   
/* 0044C1F4 38830004 */  xori  $v1, $a0, 4
/* 0044C1F8 2C630001 */  sltiu $v1, $v1, 1
.L0044C1FC:
/* 0044C1FC 14600013 */  bnez  $v1, .L0044C24C
/* 0044C200 00000000 */   nop   
/* 0044C204 8CE40028 */  lw    $a0, 0x28($a3)
/* 0044C208 8F99848C */  lw    $t9, %call16(varintree)($gp)
/* 0044C20C 8FAE002C */  lw    $t6, 0x2c($sp)
/* 0044C210 AFA40000 */  sw    $a0, ($sp)
/* 0044C214 8CE5002C */  lw    $a1, 0x2c($a3)
/* 0044C218 AFA50004 */  sw    $a1, 4($sp)
/* 0044C21C 0320F809 */  jalr  $t9
/* 0044C220 8DC60004 */   lw    $a2, 4($t6)
/* 0044C224 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044C228 14400008 */  bnez  $v0, .L0044C24C
/* 0044C22C 00401825 */   move  $v1, $v0
/* 0044C230 8F998490 */  lw    $t9, %call16(furthervarintree)($gp)
/* 0044C234 8FAA0030 */  lw    $t2, 0x30($sp)
/* 0044C238 8FA5002C */  lw    $a1, 0x2c($sp)
/* 0044C23C 0320F809 */  jalr  $t9
/* 0044C240 8D440004 */   lw    $a0, 4($t2)
/* 0044C244 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044C248 00401825 */  move  $v1, $v0
.L0044C24C:
/* 0044C24C 1000009D */  b     .L0044C4C4
/* 0044C250 306400FF */   andi  $a0, $v1, 0xff
.L0044C254:
/* 0044C254 90E9002F */  lbu   $t1, 0x2f($a3)
/* 0044C258 8FAB0028 */  lw    $t3, 0x28($sp)
/* 0044C25C 012B082A */  slt   $at, $t1, $t3
/* 0044C260 54200004 */  bnezl $at, .L0044C274
/* 0044C264 90EF0022 */   lbu   $t7, 0x22($a3)
/* 0044C268 10000096 */  b     .L0044C4C4
/* 0044C26C 00002025 */   move  $a0, $zero
/* 0044C270 90EF0022 */  lbu   $t7, 0x22($a3)
.L0044C274:
/* 0044C274 8FAC002C */  lw    $t4, 0x2c($sp)
/* 0044C278 11E00009 */  beqz  $t7, .L0044C2A0
/* 0044C27C 00000000 */   nop   
/* 0044C280 918D000F */  lbu   $t5, 0xf($t4)
/* 0044C284 11A00008 */  beqz  $t5, .L0044C2A8
/* 0044C288 00000000 */   nop   
/* 0044C28C 8F988B6C */  lw     $t8, %got(curproc)($gp)
/* 0044C290 8F180000 */  lw    $t8, ($t8)
/* 0044C294 93190014 */  lbu   $t9, 0x14($t8)
/* 0044C298 13200003 */  beqz  $t9, .L0044C2A8
/* 0044C29C 00000000 */   nop   
.L0044C2A0:
/* 0044C2A0 10000088 */  b     .L0044C4C4
/* 0044C2A4 24040001 */   li    $a0, 1
.L0044C2A8:
/* 0044C2A8 8F8E8B70 */  lw     $t6, %got(indirprocs)($gp)
/* 0044C2AC 8FA8002C */  lw    $t0, 0x2c($sp)
/* 0044C2B0 8DCE0000 */  lw    $t6, ($t6)
/* 0044C2B4 010E1826 */  xor   $v1, $t0, $t6
/* 0044C2B8 2C630001 */  sltiu $v1, $v1, 1
/* 0044C2BC 1460001A */  bnez  $v1, .L0044C328
/* 0044C2C0 00000000 */   nop   
/* 0044C2C4 91030009 */  lbu   $v1, 9($t0)
/* 0044C2C8 10600003 */  beqz  $v1, .L0044C2D8
/* 0044C2CC 00000000 */   nop   
/* 0044C2D0 38830004 */  xori  $v1, $a0, 4
/* 0044C2D4 2C630001 */  sltiu $v1, $v1, 1
.L0044C2D8:
/* 0044C2D8 14600013 */  bnez  $v1, .L0044C328
/* 0044C2DC 00000000 */   nop   
/* 0044C2E0 8CE40028 */  lw    $a0, 0x28($a3)
/* 0044C2E4 8F99848C */  lw    $t9, %call16(varintree)($gp)
/* 0044C2E8 8FAB002C */  lw    $t3, 0x2c($sp)
/* 0044C2EC AFA40000 */  sw    $a0, ($sp)
/* 0044C2F0 8CE5002C */  lw    $a1, 0x2c($a3)
/* 0044C2F4 AFA50004 */  sw    $a1, 4($sp)
/* 0044C2F8 0320F809 */  jalr  $t9
/* 0044C2FC 8D660004 */   lw    $a2, 4($t3)
/* 0044C300 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044C304 14400008 */  bnez  $v0, .L0044C328
/* 0044C308 00401825 */   move  $v1, $v0
/* 0044C30C 8F998490 */  lw    $t9, %call16(furthervarintree)($gp)
/* 0044C310 8FAF0030 */  lw    $t7, 0x30($sp)
/* 0044C314 8FA5002C */  lw    $a1, 0x2c($sp)
/* 0044C318 0320F809 */  jalr  $t9
/* 0044C31C 8DE40004 */   lw    $a0, 4($t7)
/* 0044C320 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044C324 00401825 */  move  $v1, $v0
.L0044C328:
/* 0044C328 10000066 */  b     .L0044C4C4
/* 0044C32C 306400FF */   andi  $a0, $v1, 0xff
.L0044C330:
/* 0044C330 8F9988A4 */  lw    $t9, %call16(caseerror)($gp)
/* 0044C334 8F868044 */  lw    $a2, %got(RO_1000CB70)($gp)
/* 0044C338 24040001 */  li    $a0, 1
/* 0044C33C 2407000A */  li    $a3, 10
/* 0044C340 0320F809 */  jalr  $t9
/* 0044C344 24C6CB70 */   addiu $a2, %lo(RO_1000CB70) # addiu $a2, $a2, -0x3490
/* 0044C348 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044C34C 1000005D */  b     .L0044C4C4
/* 0044C350 93A40027 */   lbu   $a0, 0x27($sp)
.L0044C354:
/* 0044C354 8F838A5C */  lw     $v1, %got(lang)($gp)
/* 0044C358 3C012A00 */  lui   $at, 0x2a00
/* 0044C35C 90630000 */  lbu   $v1, ($v1)
/* 0044C360 2C6C0020 */  sltiu $t4, $v1, 0x20
/* 0044C364 000C6823 */  negu  $t5, $t4
/* 0044C368 01A1C024 */  and   $t8, $t5, $at
/* 0044C36C 0078C804 */  sllv  $t9, $t8, $v1
/* 0044C370 07200007 */  bltz  $t9, .L0044C390
/* 0044C374 24010003 */   li    $at, 3
/* 0044C378 14610007 */  bne   $v1, $at, .L0044C398
/* 0044C37C 2448FF80 */   addiu $t0, $v0, -0x80
/* 0044C380 8F8E8B44 */  lw     $t6, %got(use_c_semantics)($gp)
/* 0044C384 91CE0000 */  lbu   $t6, ($t6)
/* 0044C388 51C00004 */  beql  $t6, $zero, .L0044C39C
/* 0044C38C 2D0A0020 */   sltiu $t2, $t0, 0x20
.L0044C390:
/* 0044C390 1000004C */  b     .L0044C4C4
/* 0044C394 24040001 */   li    $a0, 1
.L0044C398:
/* 0044C398 2D0A0020 */  sltiu $t2, $t0, 0x20
.L0044C39C:
/* 0044C39C 000A4823 */  negu  $t1, $t2
/* 0044C3A0 312B0810 */  andi  $t3, $t1, 0x810
/* 0044C3A4 010B7804 */  sllv  $t7, $t3, $t0
/* 0044C3A8 05E10003 */  bgez  $t7, .L0044C3B8
/* 0044C3AC 8FAC002C */   lw    $t4, 0x2c($sp)
/* 0044C3B0 10000045 */  b     .L0044C4C8
/* 0044C3B4 24020001 */   li    $v0, 1
.L0044C3B8:
/* 0044C3B8 918D000E */  lbu   $t5, 0xe($t4)
/* 0044C3BC 51A0000B */  beql  $t5, $zero, .L0044C3EC
/* 0044C3C0 8FB90030 */   lw    $t9, 0x30($sp)
/* 0044C3C4 8F988A30 */  lw     $t8, %got(aentptr)($gp)
/* 0044C3C8 24010001 */  li    $at, 1
/* 0044C3CC 8F180000 */  lw    $t8, ($t8)
/* 0044C3D0 13000003 */  beqz  $t8, .L0044C3E0
/* 0044C3D4 00000000 */   nop   
/* 0044C3D8 50610004 */  beql  $v1, $at, .L0044C3EC
/* 0044C3DC 8FB90030 */   lw    $t9, 0x30($sp)
.L0044C3E0:
/* 0044C3E0 10000039 */  b     .L0044C4C8
/* 0044C3E4 00001025 */   move  $v0, $zero
/* 0044C3E8 8FB90030 */  lw    $t9, 0x30($sp)
.L0044C3EC:
/* 0044C3EC 24010007 */  li    $at, 7
/* 0044C3F0 8F250024 */  lw    $a1, 0x24($t9)
/* 0044C3F4 03202025 */  move  $a0, $t9
/* 0044C3F8 90A20000 */  lbu   $v0, ($a1)
/* 0044C3FC 54410009 */  bnel  $v0, $at, .L0044C424
/* 0044C400 2C4A0020 */   sltiu $t2, $v0, 0x20
/* 0044C404 8F99862C */  lw    $t9, %call16(fix_sbase)($gp)
/* 0044C408 0320F809 */  jalr  $t9
/* 0044C40C 00000000 */   nop   
/* 0044C410 8FAE0030 */  lw    $t6, 0x30($sp)
/* 0044C414 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044C418 8DC50024 */  lw    $a1, 0x24($t6)
/* 0044C41C 90A20000 */  lbu   $v0, ($a1)
/* 0044C420 2C4A0020 */  sltiu $t2, $v0, 0x20
.L0044C424:
/* 0044C424 000A4823 */  negu  $t1, $t2
/* 0044C428 3C014400 */  lui   $at, 0x4400
/* 0044C42C 01215824 */  and   $t3, $t1, $at
/* 0044C430 004B4004 */  sllv  $t0, $t3, $v0
/* 0044C434 05010019 */  bgez  $t0, .L0044C49C
/* 0044C438 2C580020 */   sltiu $t8, $v0, 0x20
/* 0044C43C 8F8F8A5C */  lw     $t7, %got(lang)($gp)
/* 0044C440 24010005 */  li    $at, 5
/* 0044C444 8FAD0028 */  lw    $t5, 0x28($sp)
/* 0044C448 91EF0000 */  lbu   $t7, ($t7)
/* 0044C44C 51E10006 */  beql  $t7, $at, .L0044C468
/* 0044C450 90A30032 */   lbu   $v1, 0x32($a1)
/* 0044C454 8CAC0028 */  lw    $t4, 0x28($a1)
/* 0044C458 018D202A */  slt   $a0, $t4, $t5
/* 0044C45C 10000019 */  b     .L0044C4C4
/* 0044C460 308400FF */   andi  $a0, $a0, 0xff
/* 0044C464 90A30032 */  lbu   $v1, 0x32($a1)
.L0044C468:
/* 0044C468 30630007 */  andi  $v1, $v1, 7
/* 0044C46C 38630004 */  xori  $v1, $v1, 4
/* 0044C470 2C630001 */  sltiu $v1, $v1, 1
/* 0044C474 10600007 */  beqz  $v1, .L0044C494
/* 0044C478 00000000 */   nop   
/* 0044C47C 8F998494 */  lw    $t9, %call16(in_fsym)($gp)
/* 0044C480 8CA40030 */  lw    $a0, 0x30($a1)
/* 0044C484 0320F809 */  jalr  $t9
/* 0044C488 000422C2 */   srl   $a0, $a0, 0xb
/* 0044C48C 8FBC0018 */  lw    $gp, 0x18($sp)
/* 0044C490 2C430001 */  sltiu $v1, $v0, 1
.L0044C494:
/* 0044C494 1000000B */  b     .L0044C4C4
/* 0044C498 306400FF */   andi  $a0, $v1, 0xff
.L0044C49C:
/* 0044C49C 0018C823 */  negu  $t9, $t8
/* 0044C4A0 3C011B00 */  lui   $at, 0x1b00
/* 0044C4A4 03217024 */  and   $t6, $t9, $at
/* 0044C4A8 004E5004 */  sllv  $t2, $t6, $v0
/* 0044C4AC 05400003 */  bltz  $t2, .L0044C4BC
/* 0044C4B0 8FA40028 */   lw    $a0, 0x28($sp)
/* 0044C4B4 10000003 */  b     .L0044C4C4
/* 0044C4B8 00002025 */   move  $a0, $zero
.L0044C4BC:
/* 0044C4BC 0004202A */  slt   $a0, $zero, $a0
/* 0044C4C0 308400FF */  andi  $a0, $a0, 0xff
.L0044C4C4:
/* 0044C4C4 00801025 */  move  $v0, $a0
.L0044C4C8:
/* 0044C4C8 8FBF001C */  lw    $ra, 0x1c($sp)
/* 0044C4CC 27BD0028 */  addiu $sp, $sp, 0x28
/* 0044C4D0 03E00008 */  jr    $ra
/* 0044C4D4 00000000 */   nop   
    .type cskilled, @function
    .size cskilled, .-cskilled
    .end cskilled
)"");

/* 
0043CFCC readnxtinst
*/
void cupkillprev(int level, struct Proc *proc) {
    struct VarAccessList *item;

    for (item = curgraphnode->varlisthead; item != NULL; item = item->next) {
        if (item->type == 2) {
            if (item->data.var->unk2 == 0) {
                item->data.var->unk2 = clkilled(level, proc, item->data.var);
            }
        } else if (item->type == 3) {
            if (item->data.store->u.store.unk1F) {
                item->data.store->u.store.unk1F = !cmkilled(level, proc, item->data.store->u.store);
            }
            if (!item->data.store->u.store.unk1F) {
                item->data.store->u.store.unk1D = false;
            }
        } else if (item->type == 1) {
            if (item->data.store->u.store.unk1F) {
                item->data.store->u.store.unk1F = !cskilled(level, proc, item->data.store);
            }
            if (!item->data.store->u.store.unk1F) {
                item->data.store->u.store.unk1D = false;
            }
        }
    }
}

__asm__(R""(
.set noat
.set noreorder

glabel ciakillprev
    .ent ciakillprev
    # 0043CFCC readnxtinst
/* 0044C648 3C1C0FBD */  .cpload $t9
/* 0044C64C 279CDC48 */  
/* 0044C650 0399E021 */  
/* 0044C654 8F8E89B4 */  lw     $t6, %got(curgraphnode)($gp)
/* 0044C658 24060003 */  li    $a2, 3
/* 0044C65C 24050001 */  li    $a1, 1
/* 0044C660 8DCE0000 */  lw    $t6, ($t6)
/* 0044C664 24040002 */  li    $a0, 2
/* 0044C668 8DC20024 */  lw    $v0, 0x24($t6)
/* 0044C66C 10400017 */  beqz  $v0, .L0044C6CC
/* 0044C670 00000000 */   nop   
/* 0044C674 90430009 */  lbu   $v1, 9($v0)
.L0044C678:
/* 0044C678 14830004 */  bne   $a0, $v1, .L0044C68C
/* 0044C67C 00000000 */   nop   
/* 0044C680 8C4F000C */  lw    $t7, 0xc($v0)
/* 0044C684 1000000E */  b     .L0044C6C0
/* 0044C688 A1E50002 */   sb    $a1, 2($t7)
.L0044C68C:
/* 0044C68C 14C30006 */  bne   $a2, $v1, .L0044C6A8
/* 0044C690 00000000 */   nop   
/* 0044C694 8C58000C */  lw    $t8, 0xc($v0)
/* 0044C698 A300001F */  sb    $zero, 0x1f($t8)
/* 0044C69C 8C59000C */  lw    $t9, 0xc($v0)
/* 0044C6A0 10000007 */  b     .L0044C6C0
/* 0044C6A4 A320001D */   sb    $zero, 0x1d($t9)
.L0044C6A8:
/* 0044C6A8 54A30006 */  bnel  $a1, $v1, .L0044C6C4
/* 0044C6AC 8C420004 */   lw    $v0, 4($v0)
/* 0044C6B0 8C48000C */  lw    $t0, 0xc($v0)
/* 0044C6B4 A100001F */  sb    $zero, 0x1f($t0)
/* 0044C6B8 8C49000C */  lw    $t1, 0xc($v0)
/* 0044C6BC A120001D */  sb    $zero, 0x1d($t1)
.L0044C6C0:
/* 0044C6C0 8C420004 */  lw    $v0, 4($v0)
.L0044C6C4:
/* 0044C6C4 5440FFEC */  bnezl $v0, .L0044C678
/* 0044C6C8 90430009 */   lbu   $v1, 9($v0)
.L0044C6CC:
/* 0044C6CC 03E00008 */  jr    $ra
/* 0044C6D0 00000000 */   nop   
    .type ciakillprev, @function
    .size ciakillprev, .-ciakillprev
    .end ciakillprev

    .type func_0044C6D4, @function
func_0044C6D4:
    # 0044CD14 listplkilled
/* 0044C6D4 3C1C0FBD */  .cpload $t9
/* 0044C6D8 279CDBBC */  
/* 0044C6DC 0399E021 */  
/* 0044C6E0 27BDFFB8 */  addiu $sp, $sp, -0x48
/* 0044C6E4 AFBF002C */  sw    $ra, 0x2c($sp)
/* 0044C6E8 AFBC0028 */  sw    $gp, 0x28($sp)
/* 0044C6EC AFB10024 */  sw    $s1, 0x24($sp)
/* 0044C6F0 AFB00020 */  sw    $s0, 0x20($sp)
/* 0044C6F4 90A20000 */  lbu   $v0, ($a1)
/* 0044C6F8 24010004 */  li    $at, 4
/* 0044C6FC 00A08025 */  move  $s0, $a1
/* 0044C700 1441000B */  bne   $v0, $at, .L0044C730
/* 0044C704 00808825 */   move  $s1, $a0
/* 0044C708 90AE0020 */  lbu   $t6, 0x20($a1)
/* 0044C70C 25CFFF80 */  addiu $t7, $t6, -0x80
/* 0044C710 2DF80020 */  sltiu $t8, $t7, 0x20
/* 0044C714 0018C823 */  negu  $t9, $t8
/* 0044C718 33281020 */  andi  $t0, $t9, 0x1020
/* 0044C71C 01E84804 */  sllv  $t1, $t0, $t7
/* 0044C720 05230004 */  bgezl $t1, .L0044C734
/* 0044C724 2C4A0020 */   sltiu $t2, $v0, 0x20
/* 0044C728 10000175 */  b     .L0044CD00
/* 0044C72C 24020001 */   li    $v0, 1
.L0044C730:
/* 0044C730 2C4A0020 */  sltiu $t2, $v0, 0x20
.L0044C734:
/* 0044C734 000A5823 */  negu  $t3, $t2
/* 0044C738 3C011200 */  lui   $at, 0x1200
/* 0044C73C 01616024 */  and   $t4, $t3, $at
/* 0044C740 004C6804 */  sllv  $t5, $t4, $v0
/* 0044C744 05A30042 */  bgezl $t5, .L0044C850
/* 0044C748 8E0B0034 */   lw    $t3, 0x34($s0)
/* 0044C74C 92220000 */  lbu   $v0, ($s1)
/* 0044C750 3C014400 */  lui   $at, 0x4400
/* 0044C754 2C4E0020 */  sltiu $t6, $v0, 0x20
/* 0044C758 000EC023 */  negu  $t8, $t6
/* 0044C75C 0301C824 */  and   $t9, $t8, $at
/* 0044C760 00594004 */  sllv  $t0, $t9, $v0
/* 0044C764 05010012 */  bgez  $t0, .L0044C7B0
/* 0044C768 2C4E0020 */   sltiu $t6, $v0, 0x20
/* 0044C76C 8E040028 */  lw    $a0, 0x28($s0)
/* 0044C770 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044C774 8E26002C */  lw    $a2, 0x2c($s1)
/* 0044C778 AFA40000 */  sw    $a0, ($sp)
/* 0044C77C 8E05002C */  lw    $a1, 0x2c($s0)
/* 0044C780 8E270030 */  lw    $a3, 0x30($s1)
/* 0044C784 920C0020 */  lbu   $t4, 0x20($s0)
/* 0044C788 8E2D0024 */  lw    $t5, 0x24($s1)
/* 0044C78C AFA60008 */  sw    $a2, 8($sp)
/* 0044C790 AFA50004 */  sw    $a1, 4($sp)
/* 0044C794 AFA7000C */  sw    $a3, 0xc($sp)
/* 0044C798 AFAC0010 */  sw    $t4, 0x10($sp)
/* 0044C79C 0320F809 */  jalr  $t9
/* 0044C7A0 AFAD0014 */   sw    $t5, 0x14($sp)
/* 0044C7A4 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044C7A8 10000154 */  b     .L0044CCFC
/* 0044C7AC 304400FF */   andi  $a0, $v0, 0xff
.L0044C7B0:
/* 0044C7B0 000EC023 */  negu  $t8, $t6
/* 0044C7B4 3C011B00 */  lui   $at, 0x1b00
/* 0044C7B8 0301C824 */  and   $t9, $t8, $at
/* 0044C7BC 00594004 */  sllv  $t0, $t9, $v0
/* 0044C7C0 05010007 */  bgez  $t0, .L0044C7E0
/* 0044C7C4 00000000 */   nop   
/* 0044C7C8 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044C7CC 02202025 */  move  $a0, $s1
/* 0044C7D0 0320F809 */  jalr  $t9
/* 0044C7D4 00000000 */   nop   
/* 0044C7D8 10400003 */  beqz  $v0, .L0044C7E8
/* 0044C7DC 8FBC0028 */   lw    $gp, 0x28($sp)
.L0044C7E0:
/* 0044C7E0 10000146 */  b     .L0044CCFC
/* 0044C7E4 00002025 */   move  $a0, $zero
.L0044C7E8:
/* 0044C7E8 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044C7EC 26040028 */  addiu $a0, $s0, 0x28
/* 0044C7F0 0320F809 */  jalr  $t9
/* 0044C7F4 00000000 */   nop   
/* 0044C7F8 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044C7FC 14400011 */  bnez  $v0, .L0044C844
/* 0044C800 00401825 */   move  $v1, $v0
/* 0044C804 8F8F8A5C */  lw     $t7, %got(lang)($gp)
/* 0044C808 24020001 */  li    $v0, 1
/* 0044C80C 91EF0000 */  lbu   $t7, ($t7)
/* 0044C810 004F1826 */  xor   $v1, $v0, $t7
/* 0044C814 2C630001 */  sltiu $v1, $v1, 1
/* 0044C818 1060000A */  beqz  $v1, .L0044C844
/* 0044C81C 00000000 */   nop   
/* 0044C820 92230000 */  lbu   $v1, ($s1)
/* 0044C824 38630003 */  xori  $v1, $v1, 3
/* 0044C828 2C630001 */  sltiu $v1, $v1, 1
/* 0044C82C 10600005 */  beqz  $v1, .L0044C844
/* 0044C830 00000000 */   nop   
/* 0044C834 9229002E */  lbu   $t1, 0x2e($s1)
/* 0044C838 312A0007 */  andi  $t2, $t1, 7
/* 0044C83C 004A1826 */  xor   $v1, $v0, $t2
/* 0044C840 2C630001 */  sltiu $v1, $v1, 1
.L0044C844:
/* 0044C844 1000012D */  b     .L0044CCFC
/* 0044C848 306400FF */   andi  $a0, $v1, 0xff
/* 0044C84C 8E0B0034 */  lw    $t3, 0x34($s0)
.L0044C850:
/* 0044C850 24010007 */  li    $at, 7
/* 0044C854 AFAB0030 */  sw    $t3, 0x30($sp)
/* 0044C858 91620000 */  lbu   $v0, ($t3)
/* 0044C85C 5441000A */  bnel  $v0, $at, .L0044C888
/* 0044C860 2C4D0020 */   sltiu $t5, $v0, 0x20
/* 0044C864 8F998630 */  lw    $t9, %call16(fix_base)($gp)
/* 0044C868 02002025 */  move  $a0, $s0
/* 0044C86C 0320F809 */  jalr  $t9
/* 0044C870 00000000 */   nop   
/* 0044C874 8E030034 */  lw    $v1, 0x34($s0)
/* 0044C878 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044C87C 90620000 */  lbu   $v0, ($v1)
/* 0044C880 AFA30030 */  sw    $v1, 0x30($sp)
/* 0044C884 2C4D0020 */  sltiu $t5, $v0, 0x20
.L0044C888:
/* 0044C888 000D7023 */  negu  $t6, $t5
/* 0044C88C 3C014400 */  lui   $at, 0x4400
/* 0044C890 01C1C024 */  and   $t8, $t6, $at
/* 0044C894 0058C804 */  sllv  $t9, $t8, $v0
/* 0044C898 07210045 */  bgez  $t9, .L0044C9B0
/* 0044C89C 00000000 */   nop   
/* 0044C8A0 92220000 */  lbu   $v0, ($s1)
/* 0044C8A4 3C014400 */  lui   $at, 0x4400
/* 0044C8A8 8FAB0030 */  lw    $t3, 0x30($sp)
/* 0044C8AC 2C480020 */  sltiu $t0, $v0, 0x20
/* 0044C8B0 00087823 */  negu  $t7, $t0
/* 0044C8B4 01E14824 */  and   $t1, $t7, $at
/* 0044C8B8 00495004 */  sllv  $t2, $t1, $v0
/* 0044C8BC 05410014 */  bgez  $t2, .L0044C910
/* 0044C8C0 2C4F0020 */   sltiu $t7, $v0, 0x20
/* 0044C8C4 8D64002C */  lw    $a0, 0x2c($t3)
/* 0044C8C8 8E26002C */  lw    $a2, 0x2c($s1)
/* 0044C8CC 8E270030 */  lw    $a3, 0x30($s1)
/* 0044C8D0 AFA40000 */  sw    $a0, ($sp)
/* 0044C8D4 8D650030 */  lw    $a1, 0x30($t3)
/* 0044C8D8 AFA60008 */  sw    $a2, 8($sp)
/* 0044C8DC AFA7000C */  sw    $a3, 0xc($sp)
/* 0044C8E0 AFA50004 */  sw    $a1, 4($sp)
/* 0044C8E4 8D790024 */  lw    $t9, 0x24($t3)
/* 0044C8E8 8E280024 */  lw    $t0, 0x24($s1)
/* 0044C8EC AFB90010 */  sw    $t9, 0x10($sp)
/* 0044C8F0 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044C8F4 AFA80014 */  sw    $t0, 0x14($sp)
/* 0044C8F8 0320F809 */  jalr  $t9
/* 0044C8FC 00000000 */   nop   
/* 0044C900 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044C904 304400FF */  andi  $a0, $v0, 0xff
/* 0044C908 10000064 */  b     .L0044CA9C
/* 0044C90C 92050020 */   lbu   $a1, 0x20($s0)
.L0044C910:
/* 0044C910 000F4823 */  negu  $t1, $t7
/* 0044C914 3C011B00 */  lui   $at, 0x1b00
/* 0044C918 01215024 */  and   $t2, $t1, $at
/* 0044C91C 004A6004 */  sllv  $t4, $t2, $v0
/* 0044C920 05830008 */  bgezl $t4, .L0044C944
/* 0044C924 00002025 */   move  $a0, $zero
/* 0044C928 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044C92C 02202025 */  move  $a0, $s1
/* 0044C930 0320F809 */  jalr  $t9
/* 0044C934 00000000 */   nop   
/* 0044C938 10400004 */  beqz  $v0, .L0044C94C
/* 0044C93C 8FBC0028 */   lw    $gp, 0x28($sp)
/* 0044C940 00002025 */  move  $a0, $zero
.L0044C944:
/* 0044C944 10000055 */  b     .L0044CA9C
/* 0044C948 92050020 */   lbu   $a1, 0x20($s0)
.L0044C94C:
/* 0044C94C 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044C950 8E040034 */  lw    $a0, 0x34($s0)
/* 0044C954 0320F809 */  jalr  $t9
/* 0044C958 2484002C */   addiu $a0, $a0, 0x2c
/* 0044C95C 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044C960 14400010 */  bnez  $v0, .L0044C9A4
/* 0044C964 00401825 */   move  $v1, $v0
/* 0044C968 8F838A5C */  lw     $v1, %got(lang)($gp)
/* 0044C96C 90630000 */  lbu   $v1, ($v1)
/* 0044C970 38630001 */  xori  $v1, $v1, 1
/* 0044C974 2C630001 */  sltiu $v1, $v1, 1
/* 0044C978 5060000B */  beql  $v1, $zero, .L0044C9A8
/* 0044C97C 306400FF */   andi  $a0, $v1, 0xff
/* 0044C980 92230000 */  lbu   $v1, ($s1)
/* 0044C984 38630003 */  xori  $v1, $v1, 3
/* 0044C988 2C630001 */  sltiu $v1, $v1, 1
/* 0044C98C 50600006 */  beql  $v1, $zero, .L0044C9A8
/* 0044C990 306400FF */   andi  $a0, $v1, 0xff
/* 0044C994 9223002E */  lbu   $v1, 0x2e($s1)
/* 0044C998 30630007 */  andi  $v1, $v1, 7
/* 0044C99C 38630001 */  xori  $v1, $v1, 1
/* 0044C9A0 2C630001 */  sltiu $v1, $v1, 1
.L0044C9A4:
/* 0044C9A4 306400FF */  andi  $a0, $v1, 0xff
.L0044C9A8:
/* 0044C9A8 1000003C */  b     .L0044CA9C
/* 0044C9AC 92050020 */   lbu   $a1, 0x20($s0)
.L0044C9B0:
/* 0044C9B0 8F8D8A5C */  lw     $t5, %got(lang)($gp)
/* 0044C9B4 24010002 */  li    $at, 2
/* 0044C9B8 2C4E0020 */  sltiu $t6, $v0, 0x20
/* 0044C9BC 91AD0000 */  lbu   $t5, ($t5)
/* 0044C9C0 000EC023 */  negu  $t8, $t6
/* 0044C9C4 11A10007 */  beq   $t5, $at, .L0044C9E4
/* 0044C9C8 3C011B00 */   lui   $at, 0x1b00
/* 0044C9CC 03015824 */  and   $t3, $t8, $at
/* 0044C9D0 004BC804 */  sllv  $t9, $t3, $v0
/* 0044C9D4 07200003 */  bltz  $t9, .L0044C9E4
/* 0044C9D8 00002025 */   move  $a0, $zero
/* 0044C9DC 1000002F */  b     .L0044CA9C
/* 0044C9E0 92050020 */   lbu   $a1, 0x20($s0)
.L0044C9E4:
/* 0044C9E4 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044C9E8 8FA40030 */  lw    $a0, 0x30($sp)
/* 0044C9EC 0320F809 */  jalr  $t9
/* 0044C9F0 00000000 */   nop   
/* 0044C9F4 10400009 */  beqz  $v0, .L0044CA1C
/* 0044C9F8 8FBC0028 */   lw    $gp, 0x28($sp)
/* 0044C9FC 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044CA00 02202025 */  move  $a0, $s1
/* 0044CA04 0320F809 */  jalr  $t9
/* 0044CA08 00000000 */   nop   
/* 0044CA0C 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044CA10 304400FF */  andi  $a0, $v0, 0xff
/* 0044CA14 10000021 */  b     .L0044CA9C
/* 0044CA18 92050020 */   lbu   $a1, 0x20($s0)
.L0044CA1C:
/* 0044CA1C 92220000 */  lbu   $v0, ($s1)
/* 0044CA20 3C014400 */  lui   $at, 0x4400
/* 0044CA24 2C480020 */  sltiu $t0, $v0, 0x20
/* 0044CA28 00087823 */  negu  $t7, $t0
/* 0044CA2C 01E14824 */  and   $t1, $t7, $at
/* 0044CA30 00495004 */  sllv  $t2, $t1, $v0
/* 0044CA34 05410009 */  bgez  $t2, .L0044CA5C
/* 0044CA38 00000000 */   nop   
/* 0044CA3C 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044CA40 2624002C */  addiu $a0, $s1, 0x2c
/* 0044CA44 0320F809 */  jalr  $t9
/* 0044CA48 00000000 */   nop   
/* 0044CA4C 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044CA50 304400FF */  andi  $a0, $v0, 0xff
/* 0044CA54 10000011 */  b     .L0044CA9C
/* 0044CA58 92050020 */   lbu   $a1, 0x20($s0)
.L0044CA5C:
/* 0044CA5C 8F8C8A5C */  lw     $t4, %got(lang)($gp)
/* 0044CA60 24010002 */  li    $at, 2
/* 0044CA64 2C4D0020 */  sltiu $t5, $v0, 0x20
/* 0044CA68 918C0000 */  lbu   $t4, ($t4)
/* 0044CA6C 000D7023 */  negu  $t6, $t5
/* 0044CA70 24040001 */  li    $a0, 1
/* 0044CA74 11810008 */  beq   $t4, $at, .L0044CA98
/* 0044CA78 3C011B00 */   lui   $at, 0x1b00
/* 0044CA7C 01C1C024 */  and   $t8, $t6, $at
/* 0044CA80 00585804 */  sllv  $t3, $t8, $v0
/* 0044CA84 05620005 */  bltzl $t3, .L0044CA9C
/* 0044CA88 92050020 */   lbu   $a1, 0x20($s0)
/* 0044CA8C 00002025 */  move  $a0, $zero
/* 0044CA90 10000002 */  b     .L0044CA9C
/* 0044CA94 92050020 */   lbu   $a1, 0x20($s0)
.L0044CA98:
/* 0044CA98 92050020 */  lbu   $a1, 0x20($s0)
.L0044CA9C:
/* 0044CA9C 24B9FFE0 */  addiu $t9, $a1, -0x20
/* 0044CAA0 2F280080 */  sltiu $t0, $t9, 0x80
/* 0044CAA4 11000009 */  beqz  $t0, .L0044CACC
/* 0044CAA8 00000000 */   nop   
/* 0044CAAC 8F8A8044 */  lw    $t2, %got(D_10010AE0)($gp)
/* 0044CAB0 00197943 */  sra   $t7, $t9, 5
/* 0044CAB4 000F4880 */  sll   $t1, $t7, 2
/* 0044CAB8 254A0AE0 */  addiu $t2, %lo(D_10010AE0) # addiu $t2, $t2, 0xae0
/* 0044CABC 01496021 */  addu  $t4, $t2, $t1
/* 0044CAC0 8D8D0000 */  lw    $t5, ($t4)
/* 0044CAC4 032D7004 */  sllv  $t6, $t5, $t9
/* 0044CAC8 29C80000 */  slti  $t0, $t6, 0
.L0044CACC:
/* 0044CACC 5500008C */  bnezl $t0, .L0044CD00
/* 0044CAD0 00801025 */   move  $v0, $a0
/* 0044CAD4 5480008A */  bnezl $a0, .L0044CD00
/* 0044CAD8 00801025 */   move  $v0, $a0
/* 0044CADC 8E0B0038 */  lw    $t3, 0x38($s0)
/* 0044CAE0 3C014400 */  lui   $at, 0x4400
/* 0044CAE4 AFAB0030 */  sw    $t3, 0x30($sp)
/* 0044CAE8 91620000 */  lbu   $v0, ($t3)
/* 0044CAEC 01607825 */  move  $t7, $t3
/* 0044CAF0 2C4A0020 */  sltiu $t2, $v0, 0x20
/* 0044CAF4 000A4823 */  negu  $t1, $t2
/* 0044CAF8 01216024 */  and   $t4, $t1, $at
/* 0044CAFC 004C6804 */  sllv  $t5, $t4, $v0
/* 0044CB00 05A10041 */  bgez  $t5, .L0044CC08
/* 0044CB04 00000000 */   nop   
/* 0044CB08 92220000 */  lbu   $v0, ($s1)
/* 0044CB0C 3C014400 */  lui   $at, 0x4400
/* 0044CB10 2C590020 */  sltiu $t9, $v0, 0x20
/* 0044CB14 00197023 */  negu  $t6, $t9
/* 0044CB18 01C1C024 */  and   $t8, $t6, $at
/* 0044CB1C 00584004 */  sllv  $t0, $t8, $v0
/* 0044CB20 05010013 */  bgez  $t0, .L0044CB70
/* 0044CB24 2C4E0020 */   sltiu $t6, $v0, 0x20
/* 0044CB28 8D64002C */  lw    $a0, 0x2c($t3)
/* 0044CB2C 8E26002C */  lw    $a2, 0x2c($s1)
/* 0044CB30 8E270030 */  lw    $a3, 0x30($s1)
/* 0044CB34 AFA40000 */  sw    $a0, ($sp)
/* 0044CB38 8D650030 */  lw    $a1, 0x30($t3)
/* 0044CB3C 8E390024 */  lw    $t9, 0x24($s1)
/* 0044CB40 AFA60008 */  sw    $a2, 8($sp)
/* 0044CB44 AFA7000C */  sw    $a3, 0xc($sp)
/* 0044CB48 AFA50004 */  sw    $a1, 4($sp)
/* 0044CB4C 8DED0024 */  lw    $t5, 0x24($t7)
/* 0044CB50 AFB90014 */  sw    $t9, 0x14($sp)
/* 0044CB54 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044CB58 AFAD0010 */  sw    $t5, 0x10($sp)
/* 0044CB5C 0320F809 */  jalr  $t9
/* 0044CB60 00000000 */   nop   
/* 0044CB64 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044CB68 10000064 */  b     .L0044CCFC
/* 0044CB6C 304400FF */   andi  $a0, $v0, 0xff
.L0044CB70:
/* 0044CB70 000EC023 */  negu  $t8, $t6
/* 0044CB74 3C011B00 */  lui   $at, 0x1b00
/* 0044CB78 03014024 */  and   $t0, $t8, $at
/* 0044CB7C 00485804 */  sllv  $t3, $t0, $v0
/* 0044CB80 05610007 */  bgez  $t3, .L0044CBA0
/* 0044CB84 00000000 */   nop   
/* 0044CB88 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044CB8C 02202025 */  move  $a0, $s1
/* 0044CB90 0320F809 */  jalr  $t9
/* 0044CB94 00000000 */   nop   
/* 0044CB98 10400003 */  beqz  $v0, .L0044CBA8
/* 0044CB9C 8FBC0028 */   lw    $gp, 0x28($sp)
.L0044CBA0:
/* 0044CBA0 10000056 */  b     .L0044CCFC
/* 0044CBA4 00002025 */   move  $a0, $zero
.L0044CBA8:
/* 0044CBA8 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044CBAC 8E040038 */  lw    $a0, 0x38($s0)
/* 0044CBB0 0320F809 */  jalr  $t9
/* 0044CBB4 2484002C */   addiu $a0, $a0, 0x2c
/* 0044CBB8 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044CBBC 14400010 */  bnez  $v0, .L0044CC00
/* 0044CBC0 00401825 */   move  $v1, $v0
/* 0044CBC4 8F838A5C */  lw     $v1, %got(lang)($gp)
/* 0044CBC8 90630000 */  lbu   $v1, ($v1)
/* 0044CBCC 38630001 */  xori  $v1, $v1, 1
/* 0044CBD0 2C630001 */  sltiu $v1, $v1, 1
/* 0044CBD4 1060000A */  beqz  $v1, .L0044CC00
/* 0044CBD8 00000000 */   nop   
/* 0044CBDC 92230000 */  lbu   $v1, ($s1)
/* 0044CBE0 38630003 */  xori  $v1, $v1, 3
/* 0044CBE4 2C630001 */  sltiu $v1, $v1, 1
/* 0044CBE8 10600005 */  beqz  $v1, .L0044CC00
/* 0044CBEC 00000000 */   nop   
/* 0044CBF0 9223002E */  lbu   $v1, 0x2e($s1)
/* 0044CBF4 30630007 */  andi  $v1, $v1, 7
/* 0044CBF8 38630001 */  xori  $v1, $v1, 1
/* 0044CBFC 2C630001 */  sltiu $v1, $v1, 1
.L0044CC00:
/* 0044CC00 1000003E */  b     .L0044CCFC
/* 0044CC04 306400FF */   andi  $a0, $v1, 0xff
.L0044CC08:
/* 0044CC08 8F8A8A5C */  lw     $t2, %got(lang)($gp)
/* 0044CC0C 24010002 */  li    $at, 2
/* 0044CC10 2C490020 */  sltiu $t1, $v0, 0x20
/* 0044CC14 914A0000 */  lbu   $t2, ($t2)
/* 0044CC18 00096023 */  negu  $t4, $t1
/* 0044CC1C 11410007 */  beq   $t2, $at, .L0044CC3C
/* 0044CC20 3C011B00 */   lui   $at, 0x1b00
/* 0044CC24 01817824 */  and   $t7, $t4, $at
/* 0044CC28 004F6804 */  sllv  $t5, $t7, $v0
/* 0044CC2C 05A00003 */  bltz  $t5, .L0044CC3C
/* 0044CC30 00000000 */   nop   
/* 0044CC34 10000031 */  b     .L0044CCFC
/* 0044CC38 00002025 */   move  $a0, $zero
.L0044CC3C:
/* 0044CC3C 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044CC40 8FA40030 */  lw    $a0, 0x30($sp)
/* 0044CC44 0320F809 */  jalr  $t9
/* 0044CC48 00000000 */   nop   
/* 0044CC4C 10400008 */  beqz  $v0, .L0044CC70
/* 0044CC50 8FBC0028 */   lw    $gp, 0x28($sp)
/* 0044CC54 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044CC58 02202025 */  move  $a0, $s1
/* 0044CC5C 0320F809 */  jalr  $t9
/* 0044CC60 00000000 */   nop   
/* 0044CC64 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044CC68 10000024 */  b     .L0044CCFC
/* 0044CC6C 304400FF */   andi  $a0, $v0, 0xff
.L0044CC70:
/* 0044CC70 92220000 */  lbu   $v0, ($s1)
/* 0044CC74 3C014400 */  lui   $at, 0x4400
/* 0044CC78 2C590020 */  sltiu $t9, $v0, 0x20
/* 0044CC7C 00197023 */  negu  $t6, $t9
/* 0044CC80 01C1C024 */  and   $t8, $t6, $at
/* 0044CC84 00584004 */  sllv  $t0, $t8, $v0
/* 0044CC88 05010008 */  bgez  $t0, .L0044CCAC
/* 0044CC8C 00000000 */   nop   
/* 0044CC90 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044CC94 2624002C */  addiu $a0, $s1, 0x2c
/* 0044CC98 0320F809 */  jalr  $t9
/* 0044CC9C 00000000 */   nop   
/* 0044CCA0 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044CCA4 10000015 */  b     .L0044CCFC
/* 0044CCA8 304400FF */   andi  $a0, $v0, 0xff
.L0044CCAC:
/* 0044CCAC 8F8B8A5C */  lw     $t3, %got(lang)($gp)
/* 0044CCB0 24010002 */  li    $at, 2
/* 0044CCB4 2C4A0020 */  sltiu $t2, $v0, 0x20
/* 0044CCB8 916B0000 */  lbu   $t3, ($t3)
/* 0044CCBC 000A4823 */  negu  $t1, $t2
/* 0044CCC0 11610005 */  beq   $t3, $at, .L0044CCD8
/* 0044CCC4 3C011B00 */   lui   $at, 0x1b00
/* 0044CCC8 01216024 */  and   $t4, $t1, $at
/* 0044CCCC 004C7804 */  sllv  $t7, $t4, $v0
/* 0044CCD0 05E10007 */  bgez  $t7, .L0044CCF0
/* 0044CCD4 00000000 */   nop   
.L0044CCD8:
/* 0044CCD8 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044CCDC 02202025 */  move  $a0, $s1
/* 0044CCE0 0320F809 */  jalr  $t9
/* 0044CCE4 00000000 */   nop   
/* 0044CCE8 10400003 */  beqz  $v0, .L0044CCF8
/* 0044CCEC 8FBC0028 */   lw    $gp, 0x28($sp)
.L0044CCF0:
/* 0044CCF0 10000002 */  b     .L0044CCFC
/* 0044CCF4 00002025 */   move  $a0, $zero
.L0044CCF8:
/* 0044CCF8 24040001 */  li    $a0, 1
.L0044CCFC:
/* 0044CCFC 00801025 */  move  $v0, $a0
.L0044CD00:
/* 0044CD00 8FBF002C */  lw    $ra, 0x2c($sp)
/* 0044CD04 8FB00020 */  lw    $s0, 0x20($sp)
/* 0044CD08 8FB10024 */  lw    $s1, 0x24($sp)
/* 0044CD0C 03E00008 */  jr    $ra
/* 0044CD10 27BD0048 */   addiu $sp, $sp, 0x48

glabel listplkilled
    .ent listplkilled
    # 00431508 func_00431508
    # 004324F4 findinduct
    # 0044D6EC parkillprev
    # 00454D08 func_00454D08
    # 00455060 func_00455060
    # 0045C8A0 lvalaltered
    # 0045CD78 expaltered
    # 004638C0 regdataflow
/* 0044CD14 3C1C0FBD */  .cpload $t9
/* 0044CD18 279CD57C */  
/* 0044CD1C 0399E021 */  
/* 0044CD20 27BDFFA8 */  addiu $sp, $sp, -0x58
/* 0044CD24 AFBE0038 */  sw    $fp, 0x38($sp)
/* 0044CD28 AFB2001C */  sw    $s2, 0x1c($sp)
/* 0044CD2C AFB00014 */  sw    $s0, 0x14($sp)
/* 0044CD30 00A09025 */  move  $s2, $a1
/* 0044CD34 30DE00FF */  andi  $fp, $a2, 0xff
/* 0044CD38 AFBF003C */  sw    $ra, 0x3c($sp)
/* 0044CD3C AFBC0034 */  sw    $gp, 0x34($sp)
/* 0044CD40 AFB70030 */  sw    $s7, 0x30($sp)
/* 0044CD44 AFB6002C */  sw    $s6, 0x2c($sp)
/* 0044CD48 AFB50028 */  sw    $s5, 0x28($sp)
/* 0044CD4C AFB40024 */  sw    $s4, 0x24($sp)
/* 0044CD50 AFB30020 */  sw    $s3, 0x20($sp)
/* 0044CD54 AFB10018 */  sw    $s1, 0x18($sp)
/* 0044CD58 AFA60060 */  sw    $a2, 0x60($sp)
/* 0044CD5C 00001825 */  move  $v1, $zero
/* 0044CD60 1080003A */  beqz  $a0, .L0044CE4C
/* 0044CD64 00808025 */   move  $s0, $a0
/* 0044CD68 8F978B70 */  lw     $s7, %got(indirprocs)($gp)
/* 0044CD6C 8F96897C */  lw     $s6, %got(curlevel)($gp)
/* 0044CD70 27B50058 */  addiu $s5, $sp, 0x58
/* 0044CD74 8F948A5C */  lw     $s4, %got(lang)($gp)
/* 0044CD78 24130003 */  li    $s3, 3
/* 0044CD7C 24110002 */  li    $s1, 2
/* 0044CD80 92020014 */  lbu   $v0, 0x14($s0)
.L0044CD84:
/* 0044CD84 10400006 */  beqz  $v0, .L0044CDA0
/* 0044CD88 00000000 */   nop   
/* 0044CD8C 928E0000 */  lbu   $t6, ($s4)
/* 0044CD90 166E000E */  bne   $s3, $t6, .L0044CDCC
/* 0044CD94 00000000 */   nop   
/* 0044CD98 1222000C */  beq   $s1, $v0, .L0044CDCC
/* 0044CD9C 00000000 */   nop   
.L0044CDA0:
/* 0044CDA0 17C0000A */  bnez  $fp, .L0044CDCC
/* 0044CDA4 00000000 */   nop   
/* 0044CDA8 8F998028 */  lw    $t9, %got(func_0044C6D4)($gp)
/* 0044CDAC 8E040020 */  lw    $a0, 0x20($s0)
/* 0044CDB0 02402825 */  move  $a1, $s2
/* 0044CDB4 2739C6D4 */  addiu $t9, %lo(func_0044C6D4) # addiu $t9, $t9, -0x392c
/* 0044CDB8 0320F809 */  jalr  $t9
/* 0044CDBC 02A01025 */   move  $v0, $s5
/* 0044CDC0 8FBC0034 */  lw    $gp, 0x34($sp)
/* 0044CDC4 1000001C */  b     .L0044CE38
/* 0044CDC8 304300FF */   andi  $v1, $v0, 0xff
.L0044CDCC:
/* 0044CDCC 5622001B */  bnel  $s1, $v0, .L0044CE3C
/* 0044CDD0 8E100024 */   lw    $s0, 0x24($s0)
/* 0044CDD4 8E020020 */  lw    $v0, 0x20($s0)
/* 0044CDD8 904F0000 */  lbu   $t7, ($v0)
/* 0044CDDC 162F000F */  bne   $s1, $t7, .L0044CE1C
/* 0044CDE0 00000000 */   nop   
/* 0044CDE4 8F99843C */  lw    $t9, %call16(getproc)($gp)
/* 0044CDE8 8C440020 */  lw    $a0, 0x20($v0)
/* 0044CDEC 0320F809 */  jalr  $t9
/* 0044CDF0 00000000 */   nop   
/* 0044CDF4 8FBC0034 */  lw    $gp, 0x34($sp)
/* 0044CDF8 90440010 */  lbu   $a0, 0x10($v0)
/* 0044CDFC 00402825 */  move  $a1, $v0
/* 0044CE00 8F9983A4 */  lw    $t9, %call16(clkilled)($gp)
/* 0044CE04 02403025 */  move  $a2, $s2
/* 0044CE08 0320F809 */  jalr  $t9
/* 0044CE0C 00000000 */   nop   
/* 0044CE10 8FBC0034 */  lw    $gp, 0x34($sp)
/* 0044CE14 10000008 */  b     .L0044CE38
/* 0044CE18 304300FF */   andi  $v1, $v0, 0xff
.L0044CE1C:
/* 0044CE1C 8F9983A4 */  lw    $t9, %call16(clkilled)($gp)
/* 0044CE20 8EC40000 */  lw    $a0, ($s6)
/* 0044CE24 8EE50000 */  lw    $a1, ($s7)
/* 0044CE28 0320F809 */  jalr  $t9
/* 0044CE2C 02403025 */   move  $a2, $s2
/* 0044CE30 8FBC0034 */  lw    $gp, 0x34($sp)
/* 0044CE34 304300FF */  andi  $v1, $v0, 0xff
.L0044CE38:
/* 0044CE38 8E100024 */  lw    $s0, 0x24($s0)
.L0044CE3C:
/* 0044CE3C 52000004 */  beql  $s0, $zero, .L0044CE50
/* 0044CE40 8FBF003C */   lw    $ra, 0x3c($sp)
/* 0044CE44 5060FFCF */  beql  $v1, $zero, .L0044CD84
/* 0044CE48 92020014 */   lbu   $v0, 0x14($s0)
.L0044CE4C:
/* 0044CE4C 8FBF003C */  lw    $ra, 0x3c($sp)
.L0044CE50:
/* 0044CE50 8FB00014 */  lw    $s0, 0x14($sp)
/* 0044CE54 8FB10018 */  lw    $s1, 0x18($sp)
/* 0044CE58 8FB2001C */  lw    $s2, 0x1c($sp)
/* 0044CE5C 8FB30020 */  lw    $s3, 0x20($sp)
/* 0044CE60 8FB40024 */  lw    $s4, 0x24($sp)
/* 0044CE64 8FB50028 */  lw    $s5, 0x28($sp)
/* 0044CE68 8FB6002C */  lw    $s6, 0x2c($sp)
/* 0044CE6C 8FB70030 */  lw    $s7, 0x30($sp)
/* 0044CE70 8FBE0038 */  lw    $fp, 0x38($sp)
/* 0044CE74 27BD0058 */  addiu $sp, $sp, 0x58
/* 0044CE78 03E00008 */  jr    $ra
/* 0044CE7C 00601025 */   move  $v0, $v1
    .type listplkilled, @function
    .size listplkilled, .-listplkilled
    .end listplkilled

    .type func_0044CE80, @function
func_0044CE80:
    # 0044D4B4 listpskilled
/* 0044CE80 3C1C0FBD */  .cpload $t9
/* 0044CE84 279CD410 */  
/* 0044CE88 0399E021 */  
/* 0044CE8C 27BDFFC0 */  addiu $sp, $sp, -0x40
/* 0044CE90 AFBF0024 */  sw    $ra, 0x24($sp)
/* 0044CE94 AFBC0020 */  sw    $gp, 0x20($sp)
/* 0044CE98 AFB0001C */  sw    $s0, 0x1c($sp)
/* 0044CE9C AFA50044 */  sw    $a1, 0x44($sp)
/* 0044CEA0 8CAF002C */  lw    $t7, 0x2c($a1)
/* 0044CEA4 3C014400 */  lui   $at, 0x4400
/* 0044CEA8 00808025 */  move  $s0, $a0
/* 0044CEAC AFAF002C */  sw    $t7, 0x2c($sp)
/* 0044CEB0 91E20000 */  lbu   $v0, ($t7)
/* 0044CEB4 2C580020 */  sltiu $t8, $v0, 0x20
/* 0044CEB8 0018C823 */  negu  $t9, $t8
/* 0044CEBC 03214024 */  and   $t0, $t9, $at
/* 0044CEC0 00484804 */  sllv  $t1, $t0, $v0
/* 0044CEC4 05210043 */  bgez  $t1, .L0044CFD4
/* 0044CEC8 00000000 */   nop   
/* 0044CECC 90820000 */  lbu   $v0, ($a0)
/* 0044CED0 3C014400 */  lui   $at, 0x4400
/* 0044CED4 2C4A0020 */  sltiu $t2, $v0, 0x20
/* 0044CED8 000A5823 */  negu  $t3, $t2
/* 0044CEDC 01616024 */  and   $t4, $t3, $at
/* 0044CEE0 004C6804 */  sllv  $t5, $t4, $v0
/* 0044CEE4 05A10013 */  bgez  $t5, .L0044CF34
/* 0044CEE8 2C4B0020 */   sltiu $t3, $v0, 0x20
/* 0044CEEC 8DF8002C */  lw    $t8, 0x2c($t7)
/* 0044CEF0 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044CEF4 AFB80000 */  sw    $t8, ($sp)
/* 0044CEF8 8DE50030 */  lw    $a1, 0x30($t7)
/* 0044CEFC AFA50004 */  sw    $a1, 4($sp)
/* 0044CF00 8C86002C */  lw    $a2, 0x2c($a0)
/* 0044CF04 AFA60008 */  sw    $a2, 8($sp)
/* 0044CF08 8C870030 */  lw    $a3, 0x30($a0)
/* 0044CF0C AFA7000C */  sw    $a3, 0xc($sp)
/* 0044CF10 8DE90024 */  lw    $t1, 0x24($t7)
/* 0044CF14 AFA90010 */  sw    $t1, 0x10($sp)
/* 0044CF18 8C8A0024 */  lw    $t2, 0x24($a0)
/* 0044CF1C 03002025 */  move  $a0, $t8
/* 0044CF20 0320F809 */  jalr  $t9
/* 0044CF24 AFAA0014 */   sw    $t2, 0x14($sp)
/* 0044CF28 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044CF2C 10000060 */  b     .L0044D0B0
/* 0044CF30 304400FF */   andi  $a0, $v0, 0xff
.L0044CF34:
/* 0044CF34 000B6023 */  negu  $t4, $t3
/* 0044CF38 3C011B00 */  lui   $at, 0x1b00
/* 0044CF3C 01816824 */  and   $t5, $t4, $at
/* 0044CF40 004D7004 */  sllv  $t6, $t5, $v0
/* 0044CF44 05C10007 */  bgez  $t6, .L0044CF64
/* 0044CF48 00000000 */   nop   
/* 0044CF4C 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044CF50 02002025 */  move  $a0, $s0
/* 0044CF54 0320F809 */  jalr  $t9
/* 0044CF58 00000000 */   nop   
/* 0044CF5C 10400003 */  beqz  $v0, .L0044CF6C
/* 0044CF60 8FBC0020 */   lw    $gp, 0x20($sp)
.L0044CF64:
/* 0044CF64 10000052 */  b     .L0044D0B0
/* 0044CF68 00002025 */   move  $a0, $zero
.L0044CF6C:
/* 0044CF6C 8FB80044 */  lw    $t8, 0x44($sp)
/* 0044CF70 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044CF74 8F04002C */  lw    $a0, 0x2c($t8)
/* 0044CF78 0320F809 */  jalr  $t9
/* 0044CF7C 2484002C */   addiu $a0, $a0, 0x2c
/* 0044CF80 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044CF84 14400011 */  bnez  $v0, .L0044CFCC
/* 0044CF88 00401825 */   move  $v1, $v0
/* 0044CF8C 8F998A5C */  lw     $t9, %got(lang)($gp)
/* 0044CF90 24020001 */  li    $v0, 1
/* 0044CF94 93390000 */  lbu   $t9, ($t9)
/* 0044CF98 00591826 */  xor   $v1, $v0, $t9
/* 0044CF9C 2C630001 */  sltiu $v1, $v1, 1
/* 0044CFA0 1060000A */  beqz  $v1, .L0044CFCC
/* 0044CFA4 00000000 */   nop   
/* 0044CFA8 92030000 */  lbu   $v1, ($s0)
/* 0044CFAC 38630003 */  xori  $v1, $v1, 3
/* 0044CFB0 2C630001 */  sltiu $v1, $v1, 1
/* 0044CFB4 10600005 */  beqz  $v1, .L0044CFCC
/* 0044CFB8 00000000 */   nop   
/* 0044CFBC 9208002E */  lbu   $t0, 0x2e($s0)
/* 0044CFC0 310F0007 */  andi  $t7, $t0, 7
/* 0044CFC4 004F1826 */  xor   $v1, $v0, $t7
/* 0044CFC8 2C630001 */  sltiu $v1, $v1, 1
.L0044CFCC:
/* 0044CFCC 10000038 */  b     .L0044D0B0
/* 0044CFD0 306400FF */   andi  $a0, $v1, 0xff
.L0044CFD4:
/* 0044CFD4 8F898A5C */  lw     $t1, %got(lang)($gp)
/* 0044CFD8 24010002 */  li    $at, 2
/* 0044CFDC 2C4A0020 */  sltiu $t2, $v0, 0x20
/* 0044CFE0 91290000 */  lbu   $t1, ($t1)
/* 0044CFE4 000A5823 */  negu  $t3, $t2
/* 0044CFE8 11210007 */  beq   $t1, $at, .L0044D008
/* 0044CFEC 3C011B00 */   lui   $at, 0x1b00
/* 0044CFF0 01616024 */  and   $t4, $t3, $at
/* 0044CFF4 004C6804 */  sllv  $t5, $t4, $v0
/* 0044CFF8 05A00003 */  bltz  $t5, .L0044D008
/* 0044CFFC 00000000 */   nop   
/* 0044D000 1000002B */  b     .L0044D0B0
/* 0044D004 00002025 */   move  $a0, $zero
.L0044D008:
/* 0044D008 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044D00C 8FA4002C */  lw    $a0, 0x2c($sp)
/* 0044D010 0320F809 */  jalr  $t9
/* 0044D014 00000000 */   nop   
/* 0044D018 10400008 */  beqz  $v0, .L0044D03C
/* 0044D01C 8FBC0020 */   lw    $gp, 0x20($sp)
/* 0044D020 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044D024 02002025 */  move  $a0, $s0
/* 0044D028 0320F809 */  jalr  $t9
/* 0044D02C 00000000 */   nop   
/* 0044D030 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044D034 1000001E */  b     .L0044D0B0
/* 0044D038 304400FF */   andi  $a0, $v0, 0xff
.L0044D03C:
/* 0044D03C 92020000 */  lbu   $v0, ($s0)
/* 0044D040 3C014400 */  lui   $at, 0x4400
/* 0044D044 2C4E0020 */  sltiu $t6, $v0, 0x20
/* 0044D048 000EC023 */  negu  $t8, $t6
/* 0044D04C 0301C824 */  and   $t9, $t8, $at
/* 0044D050 00594004 */  sllv  $t0, $t9, $v0
/* 0044D054 05010008 */  bgez  $t0, .L0044D078
/* 0044D058 00000000 */   nop   
/* 0044D05C 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044D060 2604002C */  addiu $a0, $s0, 0x2c
/* 0044D064 0320F809 */  jalr  $t9
/* 0044D068 00000000 */   nop   
/* 0044D06C 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044D070 1000000F */  b     .L0044D0B0
/* 0044D074 304400FF */   andi  $a0, $v0, 0xff
.L0044D078:
/* 0044D078 8F8F8A5C */  lw     $t7, %got(lang)($gp)
/* 0044D07C 24010002 */  li    $at, 2
/* 0044D080 2C490020 */  sltiu $t1, $v0, 0x20
/* 0044D084 91EF0000 */  lbu   $t7, ($t7)
/* 0044D088 00095023 */  negu  $t2, $t1
/* 0044D08C 24040001 */  li    $a0, 1
/* 0044D090 11E10007 */  beq   $t7, $at, .L0044D0B0
/* 0044D094 3C011B00 */   lui   $at, 0x1b00
/* 0044D098 01415824 */  and   $t3, $t2, $at
/* 0044D09C 004B6004 */  sllv  $t4, $t3, $v0
/* 0044D0A0 05800003 */  bltz  $t4, .L0044D0B0
/* 0044D0A4 00000000 */   nop   
/* 0044D0A8 10000001 */  b     .L0044D0B0
/* 0044D0AC 00002025 */   move  $a0, $zero
.L0044D0B0:
/* 0044D0B0 8FBF0024 */  lw    $ra, 0x24($sp)
/* 0044D0B4 8FB0001C */  lw    $s0, 0x1c($sp)
/* 0044D0B8 27BD0040 */  addiu $sp, $sp, 0x40
/* 0044D0BC 03E00008 */  jr    $ra
/* 0044D0C0 00801025 */   move  $v0, $a0

    .type func_0044D0C4, @function
func_0044D0C4:
    # 0044D4B4 listpskilled
/* 0044D0C4 3C1C0FBD */  .cpload $t9
/* 0044D0C8 279CD1CC */  
/* 0044D0CC 0399E021 */  
/* 0044D0D0 27BDFFC0 */  addiu $sp, $sp, -0x40
/* 0044D0D4 AFBF0024 */  sw    $ra, 0x24($sp)
/* 0044D0D8 AFBC0020 */  sw    $gp, 0x20($sp)
/* 0044D0DC AFB0001C */  sw    $s0, 0x1c($sp)
/* 0044D0E0 AFA50044 */  sw    $a1, 0x44($sp)
/* 0044D0E4 90A20000 */  lbu   $v0, ($a1)
/* 0044D0E8 00808025 */  move  $s0, $a0
/* 0044D0EC 244FFF80 */  addiu $t7, $v0, -0x80
/* 0044D0F0 2DF80020 */  sltiu $t8, $t7, 0x20
/* 0044D0F4 0018C823 */  negu  $t9, $t8
/* 0044D0F8 33280810 */  andi  $t0, $t9, 0x810
/* 0044D0FC 01E84804 */  sllv  $t1, $t0, $t7
/* 0044D100 05210003 */  bgez  $t1, .L0044D110
/* 0044D104 244AFFE0 */   addiu $t2, $v0, -0x20
/* 0044D108 100000E5 */  b     .L0044D4A0
/* 0044D10C 24020001 */   li    $v0, 1
.L0044D110:
/* 0044D110 2D4B0060 */  sltiu $t3, $t2, 0x60
/* 0044D114 11600009 */  beqz  $t3, .L0044D13C
/* 0044D118 00000000 */   nop   
/* 0044D11C 8F8E8044 */  lw    $t6, %got(D_10010AF0)($gp)
/* 0044D120 000A6143 */  sra   $t4, $t2, 5
/* 0044D124 000C6880 */  sll   $t5, $t4, 2
/* 0044D128 25CE0AF0 */  addiu $t6, %lo(D_10010AF0) # addiu $t6, $t6, 0xaf0
/* 0044D12C 01CDC021 */  addu  $t8, $t6, $t5
/* 0044D130 8F190000 */  lw    $t9, ($t8)
/* 0044D134 01594004 */  sllv  $t0, $t9, $t2
/* 0044D138 290B0000 */  slti  $t3, $t0, 0
.L0044D13C:
/* 0044D13C 51600046 */  beql  $t3, $zero, .L0044D258
/* 0044D140 8FAB0044 */   lw    $t3, 0x44($sp)
/* 0044D144 92020000 */  lbu   $v0, ($s0)
/* 0044D148 3C014400 */  lui   $at, 0x4400
/* 0044D14C 8FB80044 */  lw    $t8, 0x44($sp)
/* 0044D150 2C490020 */  sltiu $t1, $v0, 0x20
/* 0044D154 00096023 */  negu  $t4, $t1
/* 0044D158 01817024 */  and   $t6, $t4, $at
/* 0044D15C 004E6804 */  sllv  $t5, $t6, $v0
/* 0044D160 05A10013 */  bgez  $t5, .L0044D1B0
/* 0044D164 2C4C0020 */   sltiu $t4, $v0, 0x20
/* 0044D168 8F020004 */  lw    $v0, 4($t8)
/* 0044D16C 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044D170 8E06002C */  lw    $a2, 0x2c($s0)
/* 0044D174 8C440028 */  lw    $a0, 0x28($v0)
/* 0044D178 8E070030 */  lw    $a3, 0x30($s0)
/* 0044D17C 8E090024 */  lw    $t1, 0x24($s0)
/* 0044D180 AFA40000 */  sw    $a0, ($sp)
/* 0044D184 8C45002C */  lw    $a1, 0x2c($v0)
/* 0044D188 904B0020 */  lbu   $t3, 0x20($v0)
/* 0044D18C AFA60008 */  sw    $a2, 8($sp)
/* 0044D190 AFA7000C */  sw    $a3, 0xc($sp)
/* 0044D194 AFA90014 */  sw    $t1, 0x14($sp)
/* 0044D198 AFA50004 */  sw    $a1, 4($sp)
/* 0044D19C 0320F809 */  jalr  $t9
/* 0044D1A0 AFAB0010 */   sw    $t3, 0x10($sp)
/* 0044D1A4 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044D1A8 100000BC */  b     .L0044D49C
/* 0044D1AC 304400FF */   andi  $a0, $v0, 0xff
.L0044D1B0:
/* 0044D1B0 000C7023 */  negu  $t6, $t4
/* 0044D1B4 3C011B00 */  lui   $at, 0x1b00
/* 0044D1B8 01C16824 */  and   $t5, $t6, $at
/* 0044D1BC 004DC004 */  sllv  $t8, $t5, $v0
/* 0044D1C0 07010007 */  bgez  $t8, .L0044D1E0
/* 0044D1C4 00000000 */   nop   
/* 0044D1C8 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044D1CC 02002025 */  move  $a0, $s0
/* 0044D1D0 0320F809 */  jalr  $t9
/* 0044D1D4 00000000 */   nop   
/* 0044D1D8 10400003 */  beqz  $v0, .L0044D1E8
/* 0044D1DC 8FBC0020 */   lw    $gp, 0x20($sp)
.L0044D1E0:
/* 0044D1E0 100000AE */  b     .L0044D49C
/* 0044D1E4 00002025 */   move  $a0, $zero
.L0044D1E8:
/* 0044D1E8 8FB90044 */  lw    $t9, 0x44($sp)
/* 0044D1EC 8F240004 */  lw    $a0, 4($t9)
/* 0044D1F0 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044D1F4 24840028 */  addiu $a0, $a0, 0x28
/* 0044D1F8 0320F809 */  jalr  $t9
/* 0044D1FC 00000000 */   nop   
/* 0044D200 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044D204 14400011 */  bnez  $v0, .L0044D24C
/* 0044D208 00401825 */   move  $v1, $v0
/* 0044D20C 8F8A8A5C */  lw     $t2, %got(lang)($gp)
/* 0044D210 24020001 */  li    $v0, 1
/* 0044D214 914A0000 */  lbu   $t2, ($t2)
/* 0044D218 004A1826 */  xor   $v1, $v0, $t2
/* 0044D21C 2C630001 */  sltiu $v1, $v1, 1
/* 0044D220 1060000A */  beqz  $v1, .L0044D24C
/* 0044D224 00000000 */   nop   
/* 0044D228 92030000 */  lbu   $v1, ($s0)
/* 0044D22C 38630003 */  xori  $v1, $v1, 3
/* 0044D230 2C630001 */  sltiu $v1, $v1, 1
/* 0044D234 10600005 */  beqz  $v1, .L0044D24C
/* 0044D238 00000000 */   nop   
/* 0044D23C 9208002E */  lbu   $t0, 0x2e($s0)
/* 0044D240 310F0007 */  andi  $t7, $t0, 7
/* 0044D244 004F1826 */  xor   $v1, $v0, $t7
/* 0044D248 2C630001 */  sltiu $v1, $v1, 1
.L0044D24C:
/* 0044D24C 10000093 */  b     .L0044D49C
/* 0044D250 306400FF */   andi  $a0, $v1, 0xff
/* 0044D254 8FAB0044 */  lw    $t3, 0x44($sp)
.L0044D258:
/* 0044D258 24010007 */  li    $at, 7
/* 0044D25C 8D690024 */  lw    $t1, 0x24($t3)
/* 0044D260 AFA90028 */  sw    $t1, 0x28($sp)
/* 0044D264 91220000 */  lbu   $v0, ($t1)
/* 0044D268 5441000B */  bnel  $v0, $at, .L0044D298
/* 0044D26C 2C4E0020 */   sltiu $t6, $v0, 0x20
/* 0044D270 8F99862C */  lw    $t9, %call16(fix_sbase)($gp)
/* 0044D274 01602025 */  move  $a0, $t3
/* 0044D278 0320F809 */  jalr  $t9
/* 0044D27C 00000000 */   nop   
/* 0044D280 8FAC0044 */  lw    $t4, 0x44($sp)
/* 0044D284 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044D288 8D830024 */  lw    $v1, 0x24($t4)
/* 0044D28C 90620000 */  lbu   $v0, ($v1)
/* 0044D290 AFA30028 */  sw    $v1, 0x28($sp)
/* 0044D294 2C4E0020 */  sltiu $t6, $v0, 0x20
.L0044D298:
/* 0044D298 000E6823 */  negu  $t5, $t6
/* 0044D29C 3C014400 */  lui   $at, 0x4400
/* 0044D2A0 01A1C024 */  and   $t8, $t5, $at
/* 0044D2A4 0058C804 */  sllv  $t9, $t8, $v0
/* 0044D2A8 07210044 */  bgez  $t9, .L0044D3BC
/* 0044D2AC 00000000 */   nop   
/* 0044D2B0 92020000 */  lbu   $v0, ($s0)
/* 0044D2B4 3C014400 */  lui   $at, 0x4400
/* 0044D2B8 8FAB0028 */  lw    $t3, 0x28($sp)
/* 0044D2BC 2C4A0020 */  sltiu $t2, $v0, 0x20
/* 0044D2C0 000A4023 */  negu  $t0, $t2
/* 0044D2C4 01017824 */  and   $t7, $t0, $at
/* 0044D2C8 004F4804 */  sllv  $t1, $t7, $v0
/* 0044D2CC 05210013 */  bgez  $t1, .L0044D31C
/* 0044D2D0 2C480020 */   sltiu $t0, $v0, 0x20
/* 0044D2D4 8D64002C */  lw    $a0, 0x2c($t3)
/* 0044D2D8 8E06002C */  lw    $a2, 0x2c($s0)
/* 0044D2DC 8E070030 */  lw    $a3, 0x30($s0)
/* 0044D2E0 AFA40000 */  sw    $a0, ($sp)
/* 0044D2E4 8D650030 */  lw    $a1, 0x30($t3)
/* 0044D2E8 AFA60008 */  sw    $a2, 8($sp)
/* 0044D2EC AFA7000C */  sw    $a3, 0xc($sp)
/* 0044D2F0 AFA50004 */  sw    $a1, 4($sp)
/* 0044D2F4 8D790024 */  lw    $t9, 0x24($t3)
/* 0044D2F8 8E0A0024 */  lw    $t2, 0x24($s0)
/* 0044D2FC AFB90010 */  sw    $t9, 0x10($sp)
/* 0044D300 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044D304 AFAA0014 */  sw    $t2, 0x14($sp)
/* 0044D308 0320F809 */  jalr  $t9
/* 0044D30C 00000000 */   nop   
/* 0044D310 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044D314 10000061 */  b     .L0044D49C
/* 0044D318 304400FF */   andi  $a0, $v0, 0xff
.L0044D31C:
/* 0044D31C 00087823 */  negu  $t7, $t0
/* 0044D320 3C011B00 */  lui   $at, 0x1b00
/* 0044D324 01E14824 */  and   $t1, $t7, $at
/* 0044D328 00496004 */  sllv  $t4, $t1, $v0
/* 0044D32C 05810007 */  bgez  $t4, .L0044D34C
/* 0044D330 00000000 */   nop   
/* 0044D334 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044D338 02002025 */  move  $a0, $s0
/* 0044D33C 0320F809 */  jalr  $t9
/* 0044D340 00000000 */   nop   
/* 0044D344 10400003 */  beqz  $v0, .L0044D354
/* 0044D348 8FBC0020 */   lw    $gp, 0x20($sp)
.L0044D34C:
/* 0044D34C 10000053 */  b     .L0044D49C
/* 0044D350 00002025 */   move  $a0, $zero
.L0044D354:
/* 0044D354 8FAE0044 */  lw    $t6, 0x44($sp)
/* 0044D358 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044D35C 8DC40024 */  lw    $a0, 0x24($t6)
/* 0044D360 0320F809 */  jalr  $t9
/* 0044D364 2484002C */   addiu $a0, $a0, 0x2c
/* 0044D368 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044D36C 14400011 */  bnez  $v0, .L0044D3B4
/* 0044D370 00401825 */   move  $v1, $v0
/* 0044D374 8F8D8A5C */  lw     $t5, %got(lang)($gp)
/* 0044D378 24020001 */  li    $v0, 1
/* 0044D37C 91AD0000 */  lbu   $t5, ($t5)
/* 0044D380 004D1826 */  xor   $v1, $v0, $t5
/* 0044D384 2C630001 */  sltiu $v1, $v1, 1
/* 0044D388 1060000A */  beqz  $v1, .L0044D3B4
/* 0044D38C 00000000 */   nop   
/* 0044D390 92030000 */  lbu   $v1, ($s0)
/* 0044D394 38630003 */  xori  $v1, $v1, 3
/* 0044D398 2C630001 */  sltiu $v1, $v1, 1
/* 0044D39C 10600005 */  beqz  $v1, .L0044D3B4
/* 0044D3A0 00000000 */   nop   
/* 0044D3A4 9218002E */  lbu   $t8, 0x2e($s0)
/* 0044D3A8 330B0007 */  andi  $t3, $t8, 7
/* 0044D3AC 004B1826 */  xor   $v1, $v0, $t3
/* 0044D3B0 2C630001 */  sltiu $v1, $v1, 1
.L0044D3B4:
/* 0044D3B4 10000039 */  b     .L0044D49C
/* 0044D3B8 306400FF */   andi  $a0, $v1, 0xff
.L0044D3BC:
/* 0044D3BC 8F998A5C */  lw     $t9, %got(lang)($gp)
/* 0044D3C0 24010002 */  li    $at, 2
/* 0044D3C4 2C4A0020 */  sltiu $t2, $v0, 0x20
/* 0044D3C8 93390000 */  lbu   $t9, ($t9)
/* 0044D3CC 000A4023 */  negu  $t0, $t2
/* 0044D3D0 13210007 */  beq   $t9, $at, .L0044D3F0
/* 0044D3D4 3C011B00 */   lui   $at, 0x1b00
/* 0044D3D8 01017824 */  and   $t7, $t0, $at
/* 0044D3DC 004F4804 */  sllv  $t1, $t7, $v0
/* 0044D3E0 05200003 */  bltz  $t1, .L0044D3F0
/* 0044D3E4 00000000 */   nop   
/* 0044D3E8 1000002C */  b     .L0044D49C
/* 0044D3EC 00002025 */   move  $a0, $zero
.L0044D3F0:
/* 0044D3F0 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044D3F4 8FA40028 */  lw    $a0, 0x28($sp)
/* 0044D3F8 0320F809 */  jalr  $t9
/* 0044D3FC 00000000 */   nop   
/* 0044D400 10400008 */  beqz  $v0, .L0044D424
/* 0044D404 8FBC0020 */   lw    $gp, 0x20($sp)
/* 0044D408 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044D40C 02002025 */  move  $a0, $s0
/* 0044D410 0320F809 */  jalr  $t9
/* 0044D414 00000000 */   nop   
/* 0044D418 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044D41C 1000001F */  b     .L0044D49C
/* 0044D420 304400FF */   andi  $a0, $v0, 0xff
.L0044D424:
/* 0044D424 92020000 */  lbu   $v0, ($s0)
/* 0044D428 3C014400 */  lui   $at, 0x4400
/* 0044D42C 2C4C0020 */  sltiu $t4, $v0, 0x20
/* 0044D430 000C7023 */  negu  $t6, $t4
/* 0044D434 01C16824 */  and   $t5, $t6, $at
/* 0044D438 004DC004 */  sllv  $t8, $t5, $v0
/* 0044D43C 07010008 */  bgez  $t8, .L0044D460
/* 0044D440 00000000 */   nop   
/* 0044D444 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044D448 2604002C */  addiu $a0, $s0, 0x2c
/* 0044D44C 0320F809 */  jalr  $t9
/* 0044D450 00000000 */   nop   
/* 0044D454 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044D458 10000010 */  b     .L0044D49C
/* 0044D45C 304400FF */   andi  $a0, $v0, 0xff
.L0044D460:
/* 0044D460 8F8B8A5C */  lw     $t3, %got(lang)($gp)
/* 0044D464 24010002 */  li    $at, 2
/* 0044D468 24040001 */  li    $a0, 1
/* 0044D46C 916B0000 */  lbu   $t3, ($t3)
/* 0044D470 1161000A */  beq   $t3, $at, .L0044D49C
/* 0044D474 00000000 */   nop   
/* 0044D478 2C590020 */  sltiu $t9, $v0, 0x20
/* 0044D47C 00195023 */  negu  $t2, $t9
/* 0044D480 3C011B00 */  lui   $at, 0x1b00
/* 0044D484 01414024 */  and   $t0, $t2, $at
/* 0044D488 00487804 */  sllv  $t7, $t0, $v0
/* 0044D48C 05E00003 */  bltz  $t7, .L0044D49C
/* 0044D490 00000000 */   nop   
/* 0044D494 10000001 */  b     .L0044D49C
/* 0044D498 00002025 */   move  $a0, $zero
.L0044D49C:
/* 0044D49C 00801025 */  move  $v0, $a0
.L0044D4A0:
/* 0044D4A0 8FBF0024 */  lw    $ra, 0x24($sp)
/* 0044D4A4 8FB0001C */  lw    $s0, 0x1c($sp)
/* 0044D4A8 27BD0040 */  addiu $sp, $sp, 0x40
/* 0044D4AC 03E00008 */  jr    $ra
/* 0044D4B0 00000000 */   nop   

glabel listpskilled
    .ent listpskilled
    # 004175BC copypropagate
    # 004471AC codeimage
    # 0044D6EC parkillprev
    # 0045D208 patchvectors
/* 0044D4B4 3C1C0FBD */  .cpload $t9
/* 0044D4B8 279CCDDC */  
/* 0044D4BC 0399E021 */  
/* 0044D4C0 27BDFFA8 */  addiu $sp, $sp, -0x58
/* 0044D4C4 AFB10018 */  sw    $s1, 0x18($sp)
/* 0044D4C8 AFB00014 */  sw    $s0, 0x14($sp)
/* 0044D4CC 00A08825 */  move  $s1, $a1
/* 0044D4D0 AFBF003C */  sw    $ra, 0x3c($sp)
/* 0044D4D4 AFBE0038 */  sw    $fp, 0x38($sp)
/* 0044D4D8 AFBC0034 */  sw    $gp, 0x34($sp)
/* 0044D4DC AFB70030 */  sw    $s7, 0x30($sp)
/* 0044D4E0 AFB6002C */  sw    $s6, 0x2c($sp)
/* 0044D4E4 AFB50028 */  sw    $s5, 0x28($sp)
/* 0044D4E8 AFB40024 */  sw    $s4, 0x24($sp)
/* 0044D4EC AFB30020 */  sw    $s3, 0x20($sp)
/* 0044D4F0 AFB2001C */  sw    $s2, 0x1c($sp)
/* 0044D4F4 AFA60060 */  sw    $a2, 0x60($sp)
/* 0044D4F8 00001825 */  move  $v1, $zero
/* 0044D4FC 1080006E */  beqz  $a0, .L0044D6B8
/* 0044D500 00808025 */   move  $s0, $a0
/* 0044D504 8F9E8A5C */  lw     $fp, %got(lang)($gp)
/* 0044D508 24170003 */  li    $s7, 3
/* 0044D50C 8F968B70 */  lw     $s6, %got(indirprocs)($gp)
/* 0044D510 8F95897C */  lw     $s5, %got(curlevel)($gp)
/* 0044D514 27B40058 */  addiu $s4, $sp, 0x58
/* 0044D518 8FB30048 */  lw    $s3, 0x48($sp)
/* 0044D51C 24120002 */  li    $s2, 2
/* 0044D520 92020014 */  lbu   $v0, 0x14($s0)
.L0044D524:
/* 0044D524 93AF0063 */  lbu   $t7, 0x63($sp)
/* 0044D528 10400006 */  beqz  $v0, .L0044D544
/* 0044D52C 00000000 */   nop   
/* 0044D530 93CE0000 */  lbu   $t6, ($fp)
/* 0044D534 16EE0020 */  bne   $s7, $t6, .L0044D5B8
/* 0044D538 00000000 */   nop   
/* 0044D53C 1242001E */  beq   $s2, $v0, .L0044D5B8
/* 0044D540 00000000 */   nop   
.L0044D544:
/* 0044D544 15E0001C */  bnez  $t7, .L0044D5B8
/* 0044D548 00000000 */   nop   
/* 0044D54C 8F998028 */  lw    $t9, %got(func_0044D0C4)($gp)
/* 0044D550 8E040020 */  lw    $a0, 0x20($s0)
/* 0044D554 02202825 */  move  $a1, $s1
/* 0044D558 2739D0C4 */  addiu $t9, %lo(func_0044D0C4) # addiu $t9, $t9, -0x2f3c
/* 0044D55C 0320F809 */  jalr  $t9
/* 0044D560 02801025 */   move  $v0, $s4
/* 0044D564 305800FF */  andi  $t8, $v0, 0xff
/* 0044D568 8FBC0034 */  lw    $gp, 0x34($sp)
/* 0044D56C 1700004C */  bnez  $t8, .L0044D6A0
/* 0044D570 304300FF */   andi  $v1, $v0, 0xff
/* 0044D574 92390000 */  lbu   $t9, ($s1)
/* 0044D578 2728FFC0 */  addiu $t0, $t9, -0x40
/* 0044D57C 2D090020 */  sltiu $t1, $t0, 0x20
/* 0044D580 00095023 */  negu  $t2, $t1
/* 0044D584 314B00C0 */  andi  $t3, $t2, 0xc0
/* 0044D588 010B6004 */  sllv  $t4, $t3, $t0
/* 0044D58C 05830045 */  bgezl $t4, .L0044D6A4
/* 0044D590 8E100024 */   lw    $s0, 0x24($s0)
/* 0044D594 8F998028 */  lw    $t9, %got(func_0044CE80)($gp)
/* 0044D598 8E040020 */  lw    $a0, 0x20($s0)
/* 0044D59C 02202825 */  move  $a1, $s1
/* 0044D5A0 2739CE80 */  addiu $t9, %lo(func_0044CE80) # addiu $t9, $t9, -0x3180
/* 0044D5A4 0320F809 */  jalr  $t9
/* 0044D5A8 02801025 */   move  $v0, $s4
/* 0044D5AC 8FBC0034 */  lw    $gp, 0x34($sp)
/* 0044D5B0 1000003B */  b     .L0044D6A0
/* 0044D5B4 304300FF */   andi  $v1, $v0, 0xff
.L0044D5B8:
/* 0044D5B8 5642003A */  bnel  $s2, $v0, .L0044D6A4
/* 0044D5BC 8E100024 */   lw    $s0, 0x24($s0)
/* 0044D5C0 8E020020 */  lw    $v0, 0x20($s0)
/* 0044D5C4 904D0000 */  lbu   $t5, ($v0)
/* 0044D5C8 164D0010 */  bne   $s2, $t5, .L0044D60C
/* 0044D5CC 00000000 */   nop   
/* 0044D5D0 8F99843C */  lw    $t9, %call16(getproc)($gp)
/* 0044D5D4 8C440020 */  lw    $a0, 0x20($v0)
/* 0044D5D8 0320F809 */  jalr  $t9
/* 0044D5DC 00000000 */   nop   
/* 0044D5E0 8FBC0034 */  lw    $gp, 0x34($sp)
/* 0044D5E4 00409825 */  move  $s3, $v0
/* 0044D5E8 90440010 */  lbu   $a0, 0x10($v0)
/* 0044D5EC 8F9983AC */  lw    $t9, %call16(cskilled)($gp)
/* 0044D5F0 00402825 */  move  $a1, $v0
/* 0044D5F4 02203025 */  move  $a2, $s1
/* 0044D5F8 0320F809 */  jalr  $t9
/* 0044D5FC 00000000 */   nop   
/* 0044D600 8FBC0034 */  lw    $gp, 0x34($sp)
/* 0044D604 10000008 */  b     .L0044D628
/* 0044D608 304300FF */   andi  $v1, $v0, 0xff
.L0044D60C:
/* 0044D60C 8F9983AC */  lw    $t9, %call16(cskilled)($gp)
/* 0044D610 8EA40000 */  lw    $a0, ($s5)
/* 0044D614 8EC50000 */  lw    $a1, ($s6)
/* 0044D618 0320F809 */  jalr  $t9
/* 0044D61C 02203025 */   move  $a2, $s1
/* 0044D620 8FBC0034 */  lw    $gp, 0x34($sp)
/* 0044D624 304300FF */  andi  $v1, $v0, 0xff
.L0044D628:
/* 0044D628 304E00FF */  andi  $t6, $v0, 0xff
/* 0044D62C 55C0001D */  bnezl $t6, .L0044D6A4
/* 0044D630 8E100024 */   lw    $s0, 0x24($s0)
/* 0044D634 922F0000 */  lbu   $t7, ($s1)
/* 0044D638 25F8FFC0 */  addiu $t8, $t7, -0x40
/* 0044D63C 2F190020 */  sltiu $t9, $t8, 0x20
/* 0044D640 00194823 */  negu  $t1, $t9
/* 0044D644 312A00C0 */  andi  $t2, $t1, 0xc0
/* 0044D648 030A5804 */  sllv  $t3, $t2, $t8
/* 0044D64C 05630015 */  bgezl $t3, .L0044D6A4
/* 0044D650 8E100024 */   lw    $s0, 0x24($s0)
/* 0044D654 8E080020 */  lw    $t0, 0x20($s0)
/* 0044D658 910C0000 */  lbu   $t4, ($t0)
/* 0044D65C 164C0009 */  bne   $s2, $t4, .L0044D684
/* 0044D660 00000000 */   nop   
/* 0044D664 8F9983A8 */  lw    $t9, %call16(cmkilled)($gp)
/* 0044D668 92640010 */  lbu   $a0, 0x10($s3)
/* 0044D66C 02602825 */  move  $a1, $s3
/* 0044D670 0320F809 */  jalr  $t9
/* 0044D674 02203025 */   move  $a2, $s1
/* 0044D678 8FBC0034 */  lw    $gp, 0x34($sp)
/* 0044D67C 10000008 */  b     .L0044D6A0
/* 0044D680 304300FF */   andi  $v1, $v0, 0xff
.L0044D684:
/* 0044D684 8F9983A8 */  lw    $t9, %call16(cmkilled)($gp)
/* 0044D688 8EA40000 */  lw    $a0, ($s5)
/* 0044D68C 8EC50000 */  lw    $a1, ($s6)
/* 0044D690 0320F809 */  jalr  $t9
/* 0044D694 02203025 */   move  $a2, $s1
/* 0044D698 8FBC0034 */  lw    $gp, 0x34($sp)
/* 0044D69C 304300FF */  andi  $v1, $v0, 0xff
.L0044D6A0:
/* 0044D6A0 8E100024 */  lw    $s0, 0x24($s0)
.L0044D6A4:
/* 0044D6A4 52000005 */  beql  $s0, $zero, .L0044D6BC
/* 0044D6A8 8FBF003C */   lw    $ra, 0x3c($sp)
/* 0044D6AC 5060FF9D */  beql  $v1, $zero, .L0044D524
/* 0044D6B0 92020014 */   lbu   $v0, 0x14($s0)
/* 0044D6B4 AFB30048 */  sw    $s3, 0x48($sp)
.L0044D6B8:
/* 0044D6B8 8FBF003C */  lw    $ra, 0x3c($sp)
.L0044D6BC:
/* 0044D6BC 8FB00014 */  lw    $s0, 0x14($sp)
/* 0044D6C0 8FB10018 */  lw    $s1, 0x18($sp)
/* 0044D6C4 8FB2001C */  lw    $s2, 0x1c($sp)
/* 0044D6C8 8FB30020 */  lw    $s3, 0x20($sp)
/* 0044D6CC 8FB40024 */  lw    $s4, 0x24($sp)
/* 0044D6D0 8FB50028 */  lw    $s5, 0x28($sp)
/* 0044D6D4 8FB6002C */  lw    $s6, 0x2c($sp)
/* 0044D6D8 8FB70030 */  lw    $s7, 0x30($sp)
/* 0044D6DC 8FBE0038 */  lw    $fp, 0x38($sp)
/* 0044D6E0 27BD0058 */  addiu $sp, $sp, 0x58
/* 0044D6E4 03E00008 */  jr    $ra
/* 0044D6E8 00601025 */   move  $v0, $v1
    .type listpskilled, @function
    .size listpskilled, .-listpskilled
    .end listpskilled

glabel parkillprev
    .ent parkillprev
    # 0043CFCC readnxtinst
/* 0044D6EC 3C1C0FBD */  .cpload $t9
/* 0044D6F0 279CCBA4 */  
/* 0044D6F4 0399E021 */  
/* 0044D6F8 27BDFFD0 */  addiu $sp, $sp, -0x30
/* 0044D6FC 8F8E89B4 */  lw     $t6, %got(curgraphnode)($gp)
/* 0044D700 AFBF002C */  sw    $ra, 0x2c($sp)
/* 0044D704 AFBC0028 */  sw    $gp, 0x28($sp)
/* 0044D708 8DCE0000 */  lw    $t6, ($t6)
/* 0044D70C AFB40024 */  sw    $s4, 0x24($sp)
/* 0044D710 AFB30020 */  sw    $s3, 0x20($sp)
/* 0044D714 AFB2001C */  sw    $s2, 0x1c($sp)
/* 0044D718 AFB10018 */  sw    $s1, 0x18($sp)
/* 0044D71C AFB00014 */  sw    $s0, 0x14($sp)
/* 0044D720 8DD10024 */  lw    $s1, 0x24($t6)
/* 0044D724 00809025 */  move  $s2, $a0
/* 0044D728 24130002 */  li    $s3, 2
/* 0044D72C 12200028 */  beqz  $s1, .L0044D7D0
/* 0044D730 24140001 */   li    $s4, 1
/* 0044D734 92220009 */  lbu   $v0, 9($s1)
.L0044D738:
/* 0044D738 1662000E */  bne   $s3, $v0, .L0044D774
/* 0044D73C 00000000 */   nop   
/* 0044D740 8E30000C */  lw    $s0, 0xc($s1)
/* 0044D744 920F0002 */  lbu   $t7, 2($s0)
/* 0044D748 55E0001F */  bnezl $t7, .L0044D7C8
/* 0044D74C 8E310004 */   lw    $s1, 4($s1)
/* 0044D750 8F9983B8 */  lw    $t9, %call16(listplkilled)($gp)
/* 0044D754 02402025 */  move  $a0, $s2
/* 0044D758 02002825 */  move  $a1, $s0
/* 0044D75C 0320F809 */  jalr  $t9
/* 0044D760 92260008 */   lbu   $a2, 8($s1)
/* 0044D764 8E38000C */  lw    $t8, 0xc($s1)
/* 0044D768 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044D76C 10000015 */  b     .L0044D7C4
/* 0044D770 A3020002 */   sb    $v0, 2($t8)
.L0044D774:
/* 0044D774 56820014 */  bnel  $s4, $v0, .L0044D7C8
/* 0044D778 8E310004 */   lw    $s1, 4($s1)
/* 0044D77C 8E30000C */  lw    $s0, 0xc($s1)
/* 0044D780 9203001F */  lbu   $v1, 0x1f($s0)
/* 0044D784 1060000C */  beqz  $v1, .L0044D7B8
/* 0044D788 00000000 */   nop   
/* 0044D78C 8F9983BC */  lw    $t9, %call16(listpskilled)($gp)
/* 0044D790 02402025 */  move  $a0, $s2
/* 0044D794 02002825 */  move  $a1, $s0
/* 0044D798 0320F809 */  jalr  $t9
/* 0044D79C 92260008 */   lbu   $a2, 8($s1)
/* 0044D7A0 8E28000C */  lw    $t0, 0xc($s1)
/* 0044D7A4 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044D7A8 2C590001 */  sltiu $t9, $v0, 1
/* 0044D7AC A119001F */  sb    $t9, 0x1f($t0)
/* 0044D7B0 8E30000C */  lw    $s0, 0xc($s1)
/* 0044D7B4 9203001F */  lbu   $v1, 0x1f($s0)
.L0044D7B8:
/* 0044D7B8 54600003 */  bnezl $v1, .L0044D7C8
/* 0044D7BC 8E310004 */   lw    $s1, 4($s1)
/* 0044D7C0 A200001D */  sb    $zero, 0x1d($s0)
.L0044D7C4:
/* 0044D7C4 8E310004 */  lw    $s1, 4($s1)
.L0044D7C8:
/* 0044D7C8 5620FFDB */  bnezl $s1, .L0044D738
/* 0044D7CC 92220009 */   lbu   $v0, 9($s1)
.L0044D7D0:
/* 0044D7D0 8FBF002C */  lw    $ra, 0x2c($sp)
/* 0044D7D4 8FB00014 */  lw    $s0, 0x14($sp)
/* 0044D7D8 8FB10018 */  lw    $s1, 0x18($sp)
/* 0044D7DC 8FB2001C */  lw    $s2, 0x1c($sp)
/* 0044D7E0 8FB30020 */  lw    $s3, 0x20($sp)
/* 0044D7E4 8FB40024 */  lw    $s4, 0x24($sp)
/* 0044D7E8 03E00008 */  jr    $ra
/* 0044D7EC 27BD0030 */   addiu $sp, $sp, 0x30
    .type parkillprev, @function
    .size parkillprev, .-parkillprev
    .end parkillprev

glabel varkilled
    .ent varkilled
    # 0043CFCC readnxtinst
    # 0045CD78 expaltered
    # 0046D428 oneloopblockexpr
/* 0044D7F0 3C1C0FBD */  .cpload $t9
/* 0044D7F4 279CCAA0 */  
/* 0044D7F8 0399E021 */  
/* 0044D7FC 27BDFFD8 */  addiu $sp, $sp, -0x28
/* 0044D800 AFBF0024 */  sw    $ra, 0x24($sp)
/* 0044D804 AFBC0020 */  sw    $gp, 0x20($sp)
/* 0044D808 AFB2001C */  sw    $s2, 0x1c($sp)
/* 0044D80C AFB10018 */  sw    $s1, 0x18($sp)
/* 0044D810 AFB00014 */  sw    $s0, 0x14($sp)
/* 0044D814 908E0000 */  lbu   $t6, ($a0)
/* 0044D818 24010004 */  li    $at, 4
/* 0044D81C 00809025 */  move  $s2, $a0
/* 0044D820 15C1000B */  bne   $t6, $at, .L0044D850
/* 0044D824 00001825 */   move  $v1, $zero
/* 0044D828 908F0020 */  lbu   $t7, 0x20($a0)
/* 0044D82C 24010033 */  li    $at, 51
/* 0044D830 11E10005 */  beq   $t7, $at, .L0044D848
/* 0044D834 00000000 */   nop   
/* 0044D838 90980001 */  lbu   $t8, 1($a0)
/* 0044D83C 2401000E */  li    $at, 14
/* 0044D840 17010003 */  bne   $t8, $at, .L0044D850
/* 0044D844 00000000 */   nop   
.L0044D848:
/* 0044D848 10000001 */  b     .L0044D850
/* 0044D84C 24030001 */   li    $v1, 1
.L0044D850:
/* 0044D850 10A00015 */  beqz  $a1, .L0044D8A8
/* 0044D854 00A08025 */   move  $s0, $a1
/* 0044D858 14600013 */  bnez  $v1, .L0044D8A8
/* 0044D85C 24110001 */   li    $s1, 1
/* 0044D860 92190009 */  lbu   $t9, 9($s0)
.L0044D864:
/* 0044D864 5639000C */  bnel  $s1, $t9, .L0044D898
/* 0044D868 8E100004 */   lw    $s0, 4($s0)
/* 0044D86C 92080008 */  lbu   $t0, 8($s0)
/* 0044D870 55000009 */  bnezl $t0, .L0044D898
/* 0044D874 8E100004 */   lw    $s0, 4($s0)
/* 0044D878 8F99838C */  lw    $t9, %call16(slkilled)($gp)
/* 0044D87C 8E04000C */  lw    $a0, 0xc($s0)
/* 0044D880 02402825 */  move  $a1, $s2
/* 0044D884 0320F809 */  jalr  $t9
/* 0044D888 00000000 */   nop   
/* 0044D88C 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044D890 304300FF */  andi  $v1, $v0, 0xff
/* 0044D894 8E100004 */  lw    $s0, 4($s0)
.L0044D898:
/* 0044D898 52000004 */  beql  $s0, $zero, .L0044D8AC
/* 0044D89C 8FBF0024 */   lw    $ra, 0x24($sp)
/* 0044D8A0 5060FFF0 */  beql  $v1, $zero, .L0044D864
/* 0044D8A4 92190009 */   lbu   $t9, 9($s0)
.L0044D8A8:
/* 0044D8A8 8FBF0024 */  lw    $ra, 0x24($sp)
.L0044D8AC:
/* 0044D8AC 8FB00014 */  lw    $s0, 0x14($sp)
/* 0044D8B0 8FB10018 */  lw    $s1, 0x18($sp)
/* 0044D8B4 8FB2001C */  lw    $s2, 0x1c($sp)
/* 0044D8B8 27BD0028 */  addiu $sp, $sp, 0x28
/* 0044D8BC 03E00008 */  jr    $ra
/* 0044D8C0 00601025 */   move  $v0, $v1
    .type varkilled, @function
    .size varkilled, .-varkilled
    .end varkilled

glabel strlkilled
    .ent strlkilled
    # 004175BC copypropagate
    # 0043CFCC readnxtinst
    # 0045D208 patchvectors
    # 0046E77C oneloopblockstmt
    # 004737E0 par_to_str
    # 00473F04 pmov_to_mov
/* 0044D8C4 3C1C0FBD */  .cpload $t9
/* 0044D8C8 279CC9CC */  
/* 0044D8CC 0399E021 */  
/* 0044D8D0 27BDFFD0 */  addiu $sp, $sp, -0x30
/* 0044D8D4 AFBF002C */  sw    $ra, 0x2c($sp)
/* 0044D8D8 AFBC0028 */  sw    $gp, 0x28($sp)
/* 0044D8DC AFB30024 */  sw    $s3, 0x24($sp)
/* 0044D8E0 AFB20020 */  sw    $s2, 0x20($sp)
/* 0044D8E4 AFB1001C */  sw    $s1, 0x1c($sp)
/* 0044D8E8 AFB00018 */  sw    $s0, 0x18($sp)
/* 0044D8EC 908E0000 */  lbu   $t6, ($a0)
/* 0044D8F0 24010040 */  li    $at, 64
/* 0044D8F4 00808825 */  move  $s1, $a0
/* 0044D8F8 15C10003 */  bne   $t6, $at, .L0044D908
/* 0044D8FC 00001825 */   move  $v1, $zero
/* 0044D900 10000001 */  b     .L0044D908
/* 0044D904 24030001 */   li    $v1, 1
.L0044D908:
/* 0044D908 10A0002E */  beqz  $a1, .L0044D9C4
/* 0044D90C 00A08025 */   move  $s0, $a1
/* 0044D910 1460002C */  bnez  $v1, .L0044D9C4
/* 0044D914 24130003 */   li    $s3, 3
/* 0044D918 24120002 */  li    $s2, 2
/* 0044D91C 92020009 */  lbu   $v0, 9($s0)
.L0044D920:
/* 0044D920 1642001A */  bne   $s2, $v0, .L0044D98C
/* 0044D924 00000000 */   nop   
/* 0044D928 920F0008 */  lbu   $t7, 8($s0)
/* 0044D92C 15E00017 */  bnez  $t7, .L0044D98C
/* 0044D930 00000000 */   nop   
/* 0044D934 8E05000C */  lw    $a1, 0xc($s0)
/* 0044D938 3C012480 */  lui   $at, 0x2480
/* 0044D93C 90B80000 */  lbu   $t8, ($a1)
/* 0044D940 2F190020 */  sltiu $t9, $t8, 0x20
/* 0044D944 00194023 */  negu  $t0, $t9
/* 0044D948 01014824 */  and   $t1, $t0, $at
/* 0044D94C 03095004 */  sllv  $t2, $t1, $t8
/* 0044D950 05410007 */  bgez  $t2, .L0044D970
/* 0044D954 00000000 */   nop   
/* 0044D958 8F99861C */  lw    $t9, %call16(dbgerror)($gp)
/* 0044D95C 24040274 */  li    $a0, 628
/* 0044D960 0320F809 */  jalr  $t9
/* 0044D964 00000000 */   nop   
/* 0044D968 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044D96C 8E05000C */  lw    $a1, 0xc($s0)
.L0044D970:
/* 0044D970 8F99838C */  lw    $t9, %call16(slkilled)($gp)
/* 0044D974 02202025 */  move  $a0, $s1
/* 0044D978 0320F809 */  jalr  $t9
/* 0044D97C 00000000 */   nop   
/* 0044D980 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044D984 1000000A */  b     .L0044D9B0
/* 0044D988 304300FF */   andi  $v1, $v0, 0xff
.L0044D98C:
/* 0044D98C 56620009 */  bnel  $s3, $v0, .L0044D9B4
/* 0044D990 8E100004 */   lw    $s0, 4($s0)
/* 0044D994 8F998390 */  lw    $t9, %call16(smkilled)($gp)
/* 0044D998 02202025 */  move  $a0, $s1
/* 0044D99C 8E05000C */  lw    $a1, 0xc($s0)
/* 0044D9A0 0320F809 */  jalr  $t9
/* 0044D9A4 00000000 */   nop   
/* 0044D9A8 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044D9AC 304300FF */  andi  $v1, $v0, 0xff
.L0044D9B0:
/* 0044D9B0 8E100004 */  lw    $s0, 4($s0)
.L0044D9B4:
/* 0044D9B4 52000004 */  beql  $s0, $zero, .L0044D9C8
/* 0044D9B8 8FBF002C */   lw    $ra, 0x2c($sp)
/* 0044D9BC 5060FFD8 */  beql  $v1, $zero, .L0044D920
/* 0044D9C0 92020009 */   lbu   $v0, 9($s0)
.L0044D9C4:
/* 0044D9C4 8FBF002C */  lw    $ra, 0x2c($sp)
.L0044D9C8:
/* 0044D9C8 8FB00018 */  lw    $s0, 0x18($sp)
/* 0044D9CC 8FB1001C */  lw    $s1, 0x1c($sp)
/* 0044D9D0 8FB20020 */  lw    $s2, 0x20($sp)
/* 0044D9D4 8FB30024 */  lw    $s3, 0x24($sp)
/* 0044D9D8 27BD0030 */  addiu $sp, $sp, 0x30
/* 0044D9DC 03E00008 */  jr    $ra
/* 0044D9E0 00601025 */   move  $v0, $v1
    .type strlkilled, @function
    .size strlkilled, .-strlkilled
    .end strlkilled

glabel strskilled
    .ent strskilled
    # 004175BC copypropagate
    # 0043CFCC readnxtinst
    # 0045D208 patchvectors
    # 0046E77C oneloopblockstmt
    # 004737E0 par_to_str
    # 00473F04 pmov_to_mov
/* 0044D9E4 3C1C0FBD */  .cpload $t9
/* 0044D9E8 279CC8AC */  
/* 0044D9EC 0399E021 */  
/* 0044D9F0 27BDFFD8 */  addiu $sp, $sp, -0x28
/* 0044D9F4 AFBF0024 */  sw    $ra, 0x24($sp)
/* 0044D9F8 AFBC0020 */  sw    $gp, 0x20($sp)
/* 0044D9FC AFB2001C */  sw    $s2, 0x1c($sp)
/* 0044DA00 AFB10018 */  sw    $s1, 0x18($sp)
/* 0044DA04 AFB00014 */  sw    $s0, 0x14($sp)
/* 0044DA08 908E0000 */  lbu   $t6, ($a0)
/* 0044DA0C 24010040 */  li    $at, 64
/* 0044DA10 00808825 */  move  $s1, $a0
/* 0044DA14 15C10003 */  bne   $t6, $at, .L0044DA24
/* 0044DA18 00001825 */   move  $v1, $zero
/* 0044DA1C 10000001 */  b     .L0044DA24
/* 0044DA20 24030001 */   li    $v1, 1
.L0044DA24:
/* 0044DA24 10A00026 */  beqz  $a1, .L0044DAC0
/* 0044DA28 00A08025 */   move  $s0, $a1
/* 0044DA2C 14600024 */  bnez  $v1, .L0044DAC0
/* 0044DA30 24120001 */   li    $s2, 1
/* 0044DA34 920F0009 */  lbu   $t7, 9($s0)
.L0044DA38:
/* 0044DA38 564F001D */  bnel  $s2, $t7, .L0044DAB0
/* 0044DA3C 8E100004 */   lw    $s0, 4($s0)
/* 0044DA40 92180008 */  lbu   $t8, 8($s0)
/* 0044DA44 5700001A */  bnezl $t8, .L0044DAB0
/* 0044DA48 8E100004 */   lw    $s0, 4($s0)
/* 0044DA4C 8F998394 */  lw    $t9, %call16(sskilled)($gp)
/* 0044DA50 8E04000C */  lw    $a0, 0xc($s0)
/* 0044DA54 02202825 */  move  $a1, $s1
/* 0044DA58 0320F809 */  jalr  $t9
/* 0044DA5C 00000000 */   nop   
/* 0044DA60 92390000 */  lbu   $t9, ($s1)
/* 0044DA64 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044DA68 304300FF */  andi  $v1, $v0, 0xff
/* 0044DA6C 2728FFC0 */  addiu $t0, $t9, -0x40
/* 0044DA70 2D090020 */  sltiu $t1, $t0, 0x20
/* 0044DA74 00095023 */  negu  $t2, $t1
/* 0044DA78 314B00C0 */  andi  $t3, $t2, 0xc0
/* 0044DA7C 010B6004 */  sllv  $t4, $t3, $t0
/* 0044DA80 0581000A */  bgez  $t4, .L0044DAAC
/* 0044DA84 304D00FF */   andi  $t5, $v0, 0xff
/* 0044DA88 55A00009 */  bnezl $t5, .L0044DAB0
/* 0044DA8C 8E100004 */   lw    $s0, 4($s0)
/* 0044DA90 8F998390 */  lw    $t9, %call16(smkilled)($gp)
/* 0044DA94 8E04000C */  lw    $a0, 0xc($s0)
/* 0044DA98 02202825 */  move  $a1, $s1
/* 0044DA9C 0320F809 */  jalr  $t9
/* 0044DAA0 00000000 */   nop   
/* 0044DAA4 8FBC0020 */  lw    $gp, 0x20($sp)
/* 0044DAA8 304300FF */  andi  $v1, $v0, 0xff
.L0044DAAC:
/* 0044DAAC 8E100004 */  lw    $s0, 4($s0)
.L0044DAB0:
/* 0044DAB0 52000004 */  beql  $s0, $zero, .L0044DAC4
/* 0044DAB4 8FBF0024 */   lw    $ra, 0x24($sp)
/* 0044DAB8 5060FFDF */  beql  $v1, $zero, .L0044DA38
/* 0044DABC 920F0009 */   lbu   $t7, 9($s0)
.L0044DAC0:
/* 0044DAC0 8FBF0024 */  lw    $ra, 0x24($sp)
.L0044DAC4:
/* 0044DAC4 8FB00014 */  lw    $s0, 0x14($sp)
/* 0044DAC8 8FB10018 */  lw    $s1, 0x18($sp)
/* 0044DACC 8FB2001C */  lw    $s2, 0x1c($sp)
/* 0044DAD0 27BD0028 */  addiu $sp, $sp, 0x28
/* 0044DAD4 03E00008 */  jr    $ra
/* 0044DAD8 00601025 */   move  $v0, $v1
    .type strskilled, @function
    .size strskilled, .-strskilled
    .end strskilled

glabel strlant
    .ent strlant
    # 0045C8A0 lvalaltered
/* 0044DADC 3C1C0FBD */  .cpload $t9
/* 0044DAE0 279CC7B4 */  
/* 0044DAE4 0399E021 */  
/* 0044DAE8 27BDFFD0 */  addiu $sp, $sp, -0x30
/* 0044DAEC AFBF002C */  sw    $ra, 0x2c($sp)
/* 0044DAF0 AFBC0028 */  sw    $gp, 0x28($sp)
/* 0044DAF4 AFB30024 */  sw    $s3, 0x24($sp)
/* 0044DAF8 AFB20020 */  sw    $s2, 0x20($sp)
/* 0044DAFC AFB1001C */  sw    $s1, 0x1c($sp)
/* 0044DB00 AFB00018 */  sw    $s0, 0x18($sp)
/* 0044DB04 908E0000 */  lbu   $t6, ($a0)
/* 0044DB08 24010040 */  li    $at, 64
/* 0044DB0C 00808825 */  move  $s1, $a0
/* 0044DB10 15C10003 */  bne   $t6, $at, .L0044DB20
/* 0044DB14 00001825 */   move  $v1, $zero
/* 0044DB18 10000001 */  b     .L0044DB20
/* 0044DB1C 24030001 */   li    $v1, 1
.L0044DB20:
/* 0044DB20 10A00020 */  beqz  $a1, .L0044DBA4
/* 0044DB24 00A08025 */   move  $s0, $a1
/* 0044DB28 1460001E */  bnez  $v1, .L0044DBA4
/* 0044DB2C 24130003 */   li    $s3, 3
/* 0044DB30 24120002 */  li    $s2, 2
/* 0044DB34 92020009 */  lbu   $v0, 9($s0)
.L0044DB38:
/* 0044DB38 1642000C */  bne   $s2, $v0, .L0044DB6C
/* 0044DB3C 00000000 */   nop   
/* 0044DB40 920F0008 */  lbu   $t7, 8($s0)
/* 0044DB44 15E00009 */  bnez  $t7, .L0044DB6C
/* 0044DB48 00000000 */   nop   
/* 0044DB4C 8F99838C */  lw    $t9, %call16(slkilled)($gp)
/* 0044DB50 02202025 */  move  $a0, $s1
/* 0044DB54 8E05000C */  lw    $a1, 0xc($s0)
/* 0044DB58 0320F809 */  jalr  $t9
/* 0044DB5C 00000000 */   nop   
/* 0044DB60 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044DB64 1000000A */  b     .L0044DB90
/* 0044DB68 304300FF */   andi  $v1, $v0, 0xff
.L0044DB6C:
/* 0044DB6C 56620009 */  bnel  $s3, $v0, .L0044DB94
/* 0044DB70 8E100000 */   lw    $s0, ($s0)
/* 0044DB74 8F998390 */  lw    $t9, %call16(smkilled)($gp)
/* 0044DB78 02202025 */  move  $a0, $s1
/* 0044DB7C 8E05000C */  lw    $a1, 0xc($s0)
/* 0044DB80 0320F809 */  jalr  $t9
/* 0044DB84 00000000 */   nop   
/* 0044DB88 8FBC0028 */  lw    $gp, 0x28($sp)
/* 0044DB8C 304300FF */  andi  $v1, $v0, 0xff
.L0044DB90:
/* 0044DB90 8E100000 */  lw    $s0, ($s0)
.L0044DB94:
/* 0044DB94 52000004 */  beql  $s0, $zero, .L0044DBA8
/* 0044DB98 8FBF002C */   lw    $ra, 0x2c($sp)
/* 0044DB9C 5060FFE6 */  beql  $v1, $zero, .L0044DB38
/* 0044DBA0 92020009 */   lbu   $v0, 9($s0)
.L0044DBA4:
/* 0044DBA4 8FBF002C */  lw    $ra, 0x2c($sp)
.L0044DBA8:
/* 0044DBA8 8FB00018 */  lw    $s0, 0x18($sp)
/* 0044DBAC 8FB1001C */  lw    $s1, 0x1c($sp)
/* 0044DBB0 8FB20020 */  lw    $s2, 0x20($sp)
/* 0044DBB4 8FB30024 */  lw    $s3, 0x24($sp)
/* 0044DBB8 27BD0030 */  addiu $sp, $sp, 0x30
/* 0044DBBC 03E00008 */  jr    $ra
/* 0044DBC0 2C620001 */   sltiu $v0, $v1, 1
    .type strlant, @function
    .size strlant, .-strlant
    .end strlant

glabel indirectaccessed
    .ent indirectaccessed
    # 004175BC copypropagate
/* 0044DBC4 3C1C0FBD */  .cpload $t9
/* 0044DBC8 279CC6CC */  
/* 0044DBCC 0399E021 */  
/* 0044DBD0 27BDFFC0 */  addiu $sp, $sp, -0x40
/* 0044DBD4 AFB50030 */  sw    $s5, 0x30($sp)
/* 0044DBD8 AFB30028 */  sw    $s3, 0x28($sp)
/* 0044DBDC AFB20024 */  sw    $s2, 0x24($sp)
/* 0044DBE0 00C0A825 */  move  $s5, $a2
/* 0044DBE4 AFBF003C */  sw    $ra, 0x3c($sp)
/* 0044DBE8 AFBC0038 */  sw    $gp, 0x38($sp)
/* 0044DBEC AFB60034 */  sw    $s6, 0x34($sp)
/* 0044DBF0 AFB4002C */  sw    $s4, 0x2c($sp)
/* 0044DBF4 AFB10020 */  sw    $s1, 0x20($sp)
/* 0044DBF8 AFB0001C */  sw    $s0, 0x1c($sp)
/* 0044DBFC AFA40040 */  sw    $a0, 0x40($sp)
/* 0044DC00 AFA50044 */  sw    $a1, 0x44($sp)
/* 0044DC04 00009025 */  move  $s2, $zero
/* 0044DC08 10E000BD */  beqz  $a3, .L0044DF00
/* 0044DC0C 00E09825 */   move  $s3, $a3
/* 0044DC10 24160007 */  li    $s6, 7
/* 0044DC14 27B40040 */  addiu $s4, $sp, 0x40
/* 0044DC18 92620009 */  lbu   $v0, 9($s3)
.L0044DC1C:
/* 0044DC1C 24010001 */  li    $at, 1
/* 0044DC20 54410047 */  bnel  $v0, $at, .L0044DD40
/* 0044DC24 24010002 */   li    $at, 2
/* 0044DC28 8E64000C */  lw    $a0, 0xc($s3)
/* 0044DC2C 908E0000 */  lbu   $t6, ($a0)
/* 0044DC30 00808025 */  move  $s0, $a0
/* 0044DC34 25CFFFE0 */  addiu $t7, $t6, -0x20
/* 0044DC38 2DF80080 */  sltiu $t8, $t7, 0x80
/* 0044DC3C 13000009 */  beqz  $t8, .L0044DC64
/* 0044DC40 00000000 */   nop   
/* 0044DC44 8F898044 */  lw    $t1, %got(D_10010AFC)($gp)
/* 0044DC48 000FC943 */  sra   $t9, $t7, 5
/* 0044DC4C 00194080 */  sll   $t0, $t9, 2
/* 0044DC50 25290AFC */  addiu $t1, %lo(D_10010AFC) # addiu $t1, $t1, 0xafc
/* 0044DC54 01285021 */  addu  $t2, $t1, $t0
/* 0044DC58 8D4B0000 */  lw    $t3, ($t2)
/* 0044DC5C 01EB6004 */  sllv  $t4, $t3, $t7
/* 0044DC60 29980000 */  slti  $t8, $t4, 0
.L0044DC64:
/* 0044DC64 530000A2 */  beql  $t8, $zero, .L0044DEF0
/* 0044DC68 8E730004 */   lw    $s3, 4($s3)
/* 0044DC6C 8E110024 */  lw    $s1, 0x24($s0)
/* 0044DC70 92220000 */  lbu   $v0, ($s1)
/* 0044DC74 56C20008 */  bnel  $s6, $v0, .L0044DC98
/* 0044DC78 2C4E0020 */   sltiu $t6, $v0, 0x20
/* 0044DC7C 8F99862C */  lw    $t9, %call16(fix_sbase)($gp)
/* 0044DC80 0320F809 */  jalr  $t9
/* 0044DC84 00000000 */   nop   
/* 0044DC88 8E110024 */  lw    $s1, 0x24($s0)
/* 0044DC8C 8FBC0038 */  lw    $gp, 0x38($sp)
/* 0044DC90 92220000 */  lbu   $v0, ($s1)
/* 0044DC94 2C4E0020 */  sltiu $t6, $v0, 0x20
.L0044DC98:
/* 0044DC98 000EC823 */  negu  $t9, $t6
/* 0044DC9C 3C014400 */  lui   $at, 0x4400
/* 0044DCA0 03214824 */  and   $t1, $t9, $at
/* 0044DCA4 00494004 */  sllv  $t0, $t1, $v0
/* 0044DCA8 05010011 */  bgez  $t0, .L0044DCF0
/* 0044DCAC 2C580020 */   sltiu $t8, $v0, 0x20
/* 0044DCB0 8E840000 */  lw    $a0, ($s4)
/* 0044DCB4 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044DCB8 8E26002C */  lw    $a2, 0x2c($s1)
/* 0044DCBC AFA40000 */  sw    $a0, ($sp)
/* 0044DCC0 8E850004 */  lw    $a1, 4($s4)
/* 0044DCC4 8E270030 */  lw    $a3, 0x30($s1)
/* 0044DCC8 8E2D0024 */  lw    $t5, 0x24($s1)
/* 0044DCCC AFB50010 */  sw    $s5, 0x10($sp)
/* 0044DCD0 AFA60008 */  sw    $a2, 8($sp)
/* 0044DCD4 AFA50004 */  sw    $a1, 4($sp)
/* 0044DCD8 AFA7000C */  sw    $a3, 0xc($sp)
/* 0044DCDC 0320F809 */  jalr  $t9
/* 0044DCE0 AFAD0014 */   sw    $t5, 0x14($sp)
/* 0044DCE4 8FBC0038 */  lw    $gp, 0x38($sp)
/* 0044DCE8 10000080 */  b     .L0044DEEC
/* 0044DCEC 305200FF */   andi  $s2, $v0, 0xff
.L0044DCF0:
/* 0044DCF0 00187023 */  negu  $t6, $t8
/* 0044DCF4 3C011B00 */  lui   $at, 0x1b00
/* 0044DCF8 01C1C824 */  and   $t9, $t6, $at
/* 0044DCFC 00594804 */  sllv  $t1, $t9, $v0
/* 0044DD00 0523007B */  bgezl $t1, .L0044DEF0
/* 0044DD04 8E730004 */   lw    $s3, 4($s3)
/* 0044DD08 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044DD0C 02202025 */  move  $a0, $s1
/* 0044DD10 0320F809 */  jalr  $t9
/* 0044DD14 00000000 */   nop   
/* 0044DD18 14400074 */  bnez  $v0, .L0044DEEC
/* 0044DD1C 8FBC0038 */   lw    $gp, 0x38($sp)
/* 0044DD20 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044DD24 02802025 */  move  $a0, $s4
/* 0044DD28 0320F809 */  jalr  $t9
/* 0044DD2C 00000000 */   nop   
/* 0044DD30 8FBC0038 */  lw    $gp, 0x38($sp)
/* 0044DD34 1000006D */  b     .L0044DEEC
/* 0044DD38 305200FF */   andi  $s2, $v0, 0xff
/* 0044DD3C 24010002 */  li    $at, 2
.L0044DD40:
/* 0044DD40 5441003C */  bnel  $v0, $at, .L0044DE34
/* 0044DD44 24010003 */   li    $at, 3
/* 0044DD48 8E64000C */  lw    $a0, 0xc($s3)
/* 0044DD4C 24010004 */  li    $at, 4
/* 0044DD50 90880000 */  lbu   $t0, ($a0)
/* 0044DD54 00808025 */  move  $s0, $a0
/* 0044DD58 55010065 */  bnel  $t0, $at, .L0044DEF0
/* 0044DD5C 8E730004 */   lw    $s3, 4($s3)
/* 0044DD60 8C910034 */  lw    $s1, 0x34($a0)
/* 0044DD64 92220000 */  lbu   $v0, ($s1)
/* 0044DD68 56C20008 */  bnel  $s6, $v0, .L0044DD8C
/* 0044DD6C 2C4A0020 */   sltiu $t2, $v0, 0x20
/* 0044DD70 8F998630 */  lw    $t9, %call16(fix_base)($gp)
/* 0044DD74 0320F809 */  jalr  $t9
/* 0044DD78 00000000 */   nop   
/* 0044DD7C 8E110034 */  lw    $s1, 0x34($s0)
/* 0044DD80 8FBC0038 */  lw    $gp, 0x38($sp)
/* 0044DD84 92220000 */  lbu   $v0, ($s1)
/* 0044DD88 2C4A0020 */  sltiu $t2, $v0, 0x20
.L0044DD8C:
/* 0044DD8C 000A5823 */  negu  $t3, $t2
/* 0044DD90 3C014400 */  lui   $at, 0x4400
/* 0044DD94 01617824 */  and   $t7, $t3, $at
/* 0044DD98 004F6004 */  sllv  $t4, $t7, $v0
/* 0044DD9C 05810011 */  bgez  $t4, .L0044DDE4
/* 0044DDA0 2C480020 */   sltiu $t0, $v0, 0x20
/* 0044DDA4 8E840000 */  lw    $a0, ($s4)
/* 0044DDA8 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044DDAC 8E26002C */  lw    $a2, 0x2c($s1)
/* 0044DDB0 AFA40000 */  sw    $a0, ($sp)
/* 0044DDB4 8E850004 */  lw    $a1, 4($s4)
/* 0044DDB8 8E270030 */  lw    $a3, 0x30($s1)
/* 0044DDBC 8E290024 */  lw    $t1, 0x24($s1)
/* 0044DDC0 AFB50010 */  sw    $s5, 0x10($sp)
/* 0044DDC4 AFA60008 */  sw    $a2, 8($sp)
/* 0044DDC8 AFA50004 */  sw    $a1, 4($sp)
/* 0044DDCC AFA7000C */  sw    $a3, 0xc($sp)
/* 0044DDD0 0320F809 */  jalr  $t9
/* 0044DDD4 AFA90014 */   sw    $t1, 0x14($sp)
/* 0044DDD8 8FBC0038 */  lw    $gp, 0x38($sp)
/* 0044DDDC 10000043 */  b     .L0044DEEC
/* 0044DDE0 305200FF */   andi  $s2, $v0, 0xff
.L0044DDE4:
/* 0044DDE4 00085023 */  negu  $t2, $t0
/* 0044DDE8 3C011B00 */  lui   $at, 0x1b00
/* 0044DDEC 01415824 */  and   $t3, $t2, $at
/* 0044DDF0 004B7804 */  sllv  $t7, $t3, $v0
/* 0044DDF4 05E3003E */  bgezl $t7, .L0044DEF0
/* 0044DDF8 8E730004 */   lw    $s3, 4($s3)
/* 0044DDFC 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044DE00 02202025 */  move  $a0, $s1
/* 0044DE04 0320F809 */  jalr  $t9
/* 0044DE08 00000000 */   nop   
/* 0044DE0C 14400037 */  bnez  $v0, .L0044DEEC
/* 0044DE10 8FBC0038 */   lw    $gp, 0x38($sp)
/* 0044DE14 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044DE18 02802025 */  move  $a0, $s4
/* 0044DE1C 0320F809 */  jalr  $t9
/* 0044DE20 00000000 */   nop   
/* 0044DE24 8FBC0038 */  lw    $gp, 0x38($sp)
/* 0044DE28 10000030 */  b     .L0044DEEC
/* 0044DE2C 305200FF */   andi  $s2, $v0, 0xff
/* 0044DE30 24010003 */  li    $at, 3
.L0044DE34:
/* 0044DE34 5441002E */  bnel  $v0, $at, .L0044DEF0
/* 0044DE38 8E730004 */   lw    $s3, 4($s3)
/* 0044DE3C 8E70000C */  lw    $s0, 0xc($s3)
/* 0044DE40 3C014400 */  lui   $at, 0x4400
/* 0044DE44 8E11002C */  lw    $s1, 0x2c($s0)
/* 0044DE48 92220000 */  lbu   $v0, ($s1)
/* 0044DE4C 2C4C0020 */  sltiu $t4, $v0, 0x20
/* 0044DE50 000C6823 */  negu  $t5, $t4
/* 0044DE54 01A1C024 */  and   $t8, $t5, $at
/* 0044DE58 00587004 */  sllv  $t6, $t8, $v0
/* 0044DE5C 05C10011 */  bgez  $t6, .L0044DEA4
/* 0044DE60 2C4F0020 */   sltiu $t7, $v0, 0x20
/* 0044DE64 8E840000 */  lw    $a0, ($s4)
/* 0044DE68 8F998380 */  lw    $t9, %call16(overlapping)($gp)
/* 0044DE6C 8E26002C */  lw    $a2, 0x2c($s1)
/* 0044DE70 AFA40000 */  sw    $a0, ($sp)
/* 0044DE74 8E850004 */  lw    $a1, 4($s4)
/* 0044DE78 8E270030 */  lw    $a3, 0x30($s1)
/* 0044DE7C 8E2B0024 */  lw    $t3, 0x24($s1)
/* 0044DE80 AFB50010 */  sw    $s5, 0x10($sp)
/* 0044DE84 AFA60008 */  sw    $a2, 8($sp)
/* 0044DE88 AFA50004 */  sw    $a1, 4($sp)
/* 0044DE8C AFA7000C */  sw    $a3, 0xc($sp)
/* 0044DE90 0320F809 */  jalr  $t9
/* 0044DE94 AFAB0014 */   sw    $t3, 0x14($sp)
/* 0044DE98 8FBC0038 */  lw    $gp, 0x38($sp)
/* 0044DE9C 10000013 */  b     .L0044DEEC
/* 0044DEA0 305200FF */   andi  $s2, $v0, 0xff
.L0044DEA4:
/* 0044DEA4 000F6023 */  negu  $t4, $t7
/* 0044DEA8 3C011B00 */  lui   $at, 0x1b00
/* 0044DEAC 01816824 */  and   $t5, $t4, $at
/* 0044DEB0 004DC004 */  sllv  $t8, $t5, $v0
/* 0044DEB4 0703000E */  bgezl $t8, .L0044DEF0
/* 0044DEB8 8E730004 */   lw    $s3, 4($s3)
/* 0044DEBC 8F998388 */  lw    $t9, %call16(pointtoheap)($gp)
/* 0044DEC0 02202025 */  move  $a0, $s1
/* 0044DEC4 0320F809 */  jalr  $t9
/* 0044DEC8 00000000 */   nop   
/* 0044DECC 14400007 */  bnez  $v0, .L0044DEEC
/* 0044DED0 8FBC0038 */   lw    $gp, 0x38($sp)
/* 0044DED4 8F998384 */  lw    $t9, %call16(aliaswithptr)($gp)
/* 0044DED8 02802025 */  move  $a0, $s4
/* 0044DEDC 0320F809 */  jalr  $t9
/* 0044DEE0 00000000 */   nop   
/* 0044DEE4 8FBC0038 */  lw    $gp, 0x38($sp)
/* 0044DEE8 305200FF */  andi  $s2, $v0, 0xff
.L0044DEEC:
/* 0044DEEC 8E730004 */  lw    $s3, 4($s3)
.L0044DEF0:
/* 0044DEF0 52600004 */  beql  $s3, $zero, .L0044DF04
/* 0044DEF4 8FBF003C */   lw    $ra, 0x3c($sp)
/* 0044DEF8 5240FF48 */  beql  $s2, $zero, .L0044DC1C
/* 0044DEFC 92620009 */   lbu   $v0, 9($s3)
.L0044DF00:
/* 0044DF00 8FBF003C */  lw    $ra, 0x3c($sp)
.L0044DF04:
/* 0044DF04 02401025 */  move  $v0, $s2
/* 0044DF08 8FB20024 */  lw    $s2, 0x24($sp)
/* 0044DF0C 8FB0001C */  lw    $s0, 0x1c($sp)
/* 0044DF10 8FB10020 */  lw    $s1, 0x20($sp)
/* 0044DF14 8FB30028 */  lw    $s3, 0x28($sp)
/* 0044DF18 8FB4002C */  lw    $s4, 0x2c($sp)
/* 0044DF1C 8FB50030 */  lw    $s5, 0x30($sp)
/* 0044DF20 8FB60034 */  lw    $s6, 0x34($sp)
/* 0044DF24 03E00008 */  jr    $ra
/* 0044DF28 27BD0040 */   addiu $sp, $sp, 0x40
    .type indirectaccessed, @function
    .size indirectaccessed, .-indirectaccessed
    .end indirectaccessed
)"");
