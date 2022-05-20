#include "../freebsd_list.h"

#include <stdio.h>

struct student
{
  LIST_ENTRY(student) list_next;
  int id;
};

#define LIST_ENTRY_INIT(entry) \
  do                           \
  {                            \
    (entry)->le_next = NULL;   \
    (entry)->le_prev = NULL;   \
  } while (0)

int main()
{
  struct student stu0;
  LIST_ENTRY_INIT(&stu0.list_next);
  stu0.id = 0;

  struct student stu1;
  LIST_ENTRY_INIT(&stu1.list_next);
  stu1.id = 1;

  struct student stu2;
  LIST_ENTRY_INIT(&stu2.list_next);
  stu2.id = 2;

  struct student stu3;
  LIST_ENTRY_INIT(&stu3.list_next);
  stu3.id = 3;

  LIST_HEAD(, student) stu_head;
  LIST_INIT(&stu_head);

  LIST_INSERT_HEAD(&stu_head, &stu0, list_next);
  LIST_INSERT_HEAD(&stu_head, &stu1, list_next);
  LIST_INSERT_HEAD(&stu_head, &stu3, list_next);
  LIST_INSERT_HEAD(&stu_head, &stu2, list_next);

  LIST_REMOVE(&stu1, list_next);
  LIST_REMOVE(&stu2, list_next);
  LIST_REMOVE(&stu0, list_next);

  struct student *stu_iter;
  LIST_FOREACH(stu_iter, &stu_head, list_next) { printf("%d ", stu_iter->id); }
}
