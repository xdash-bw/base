#ifndef SKYWIND3000_LIST_H
#define SKYWIND3000_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

struct list_head {
    struct list_head *prev, *next;
};




#ifdef __cplusplus
}
#endif

#endif
