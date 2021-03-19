#ifndef INCLUDE_MACROS_H_
#define INCLUDE_MACROS_H_

#define VERB_FLAG           0b100
#define CHANG_FLAG          0b010
#define REC_FLAG            0b001

#define READ_MASK               4
#define WRITE_MASK              2
#define EXE_MASK                1

#define SYMB_PERM_START         2

#define ALL_PERMS            0777

#define USER_MASK            0077 // 63
#define GROUP_MASK           0707 // 7 + 511 - 63
#define OTHER_MASK           0770 // 511 - 7
#define ALL_MASK             0000

#define USER_POS             0100 // 64
#define GROUP_POS            0010 // 8
#define OTHER_POS            0001

#define FILE_MASK          0100000 // regular file's mask

#define DEFAULT_ERROR -1
#define TYPE_ERROR -2
#define MODE_ERROR -3
#define OPERATOR_ERROR -4
#define PERMISSION_ERROR -5



#endif //INCLUDE_MACROS_H_