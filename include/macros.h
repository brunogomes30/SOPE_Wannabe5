#ifndef MACROS_H
#define MACROS_H

#define VERB_FLAG   0b100
#define CHANG_FLAG  0b010
#define REC_FLAG    0b001

#define READ_MASK 4
#define WRITE_MASK 2
#define EXE_MASK 1

#define SYMB_PERM_START 2

#define ALL_PERMS 0777

#define USER_MASK 077 // 63
#define GROUP_MASK 0707 // 7 + 511 - 63
#define OTHER_MASK 0770 // 511 - 7
#define ALL_MASK 0

#define USER_POS  0100 // 64
#define GROUP_POS 010 //8
#define OTHER_POS 1

#define FILE_MASK 0100000 // ler mini-rel 



#endif