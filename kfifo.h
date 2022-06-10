#ifndef __KFIFO_H
#define __KFIFO_H

struct __kfifo {
    unsigned int in;
    unsigned int out;
    unsigned int mask;
    unsigned int esize;
    void *data;
};

#define __STRUCT_KFIFO_COMMON(datatype, recsize, ptrtype)   \
    union {                                                 \
        struct __kfifo fifo;                                \
        datatype *type;                                     \
        const datatype *const_type;                         \
        char (*rectype) [recsize];                          \
        ptrtype *ptr;                                       \
        ptrtype const *ptr_const;                           \
    }

#define __STRUCT_KFIFO(type, size, ptrtype)                         \
    {                                                               \
        __STRUCT_KFIFO_COMMON(type, size, ptrtype);                 \
        type buf[((size < 2) || (size & (size - 1))) ? -1 : size];  \
    }

#define STRUCT_KFIFO(type, size)                \
    __STRUCT_KFIFO(type, size, type);           \


#endif // __KFIFO_H
