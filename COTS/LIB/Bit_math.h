#ifndef BIT_MATH_H
#define BIT_MATH_H

#define SET_BIT(REG,BIT)                (REG |= (1 << BIT))
#define CLR_BIT(REG,BIT)                (REG &= ~(1 << BIT))
#define TGL_BIT(REG,BIT)                (REG ^= (1 << BIT))
#define GET_BIT(REG,BIT)                ((REG & (1 << BIT)) >> BIT)
#define SET_HIG_NIB(REG)                (REG |= (-1 << (sizeof(REG) * 4)))
#define CLR_HIG_NIB(REG)                (REG &= ~(-1 << (sizeof(REG) * 4)))
#define GET_HIG_NIB(REG)                (REG &  (-1 << (sizeof(REG) * 4)))
#define TGL_HIG_NIB(REG)                (REG ^= (-1 << (sizeof(REG) * 4)))
#define SET_LOW_NIB(REG)                (REG |= ~(-1 << (sizeof(REG) * 4)))
#define CLR_LOW_NIB(REG)                (REG &= (-1 << (sizeof(REG) * 4)))
#define GET_LOW_NIB(REG)                (REG &  (~(-1 << (sizeof(REG) * 4))))
#define TGL_LOW_NIB(REG)                (REG ^= ~(-1 << (sizeof(REG) * 4)))
#define ASSIGN_REG(REG,VALUE)           (REG = VALUE)
#define ASSIGN_HIG_NIB(REG,VALUE)       (REG |= ((VALUE << sizeof(REG) * 4)))
#define ASSIGN_LOW_NIB(REG,VALUE)       (REG |= (VALUE & ~(-1 << (sizeof(REG) * 4))))
#define SHIFT_RIGHT(REG,VALUE)          (REG =  (REG >> VALUE))
#define SHIFT_SHIFT(REG,VALUE)          (REG =  (REG << VALUE))
#define CLR_RIGHT_SHIFT(REG,VALUE)      (REG =  (REG >> VALUE)|(REG << (sizeof(REG)*8 - VALUE)))
#define CLR_LEFT_SHIFT(REG,VALUE)       (REG =  (REG << VALUE)|(REG >> (sizeof(REG)*8 - VALUE)))

#endif