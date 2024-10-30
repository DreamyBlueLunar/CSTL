#ifndef DCL_LIST_H_
#define DCL_LIST_H_

#include <stddef.h>
#include <stdlib.h>

/*
 * 双向链表指针部分结构体声明
 * 其中 name 是链表对象的名字，type是节点的类型
 */
#define LIST_HEAD(name, type)                        \
    struct name {                                    \
        struct type *first_;                         \
    }

/*
 * 这是一个很有意思的定义方式
 * prev_直接指向上一个节点的next_指针地址，这样可以直接用*prev_表示上一节点的next_
 */
#define LIST_ENTRY(type)                             \
    struct {                                         \
        struct type *next_;                          \
        struct type **prev_;                         \
    }

/*
 *  函数，实现对双向循环链表的各项操作
 */

/* checked
 * 获取头节点
 * @param: list 是链表名称
 */
#define LIST_FIRST(list)                         \
    (list).first_

/* checked
 * 获取节点 elm 的下一个节点
 * @param: elm 是当前节点
 */
#define LIST_NEXT(elm, field)                    \
    (elm)->field.next_

/* checked
 * 初始化链表
 * @param: list 是链表名称
 * @param: type 是节点类型
 * @param: field 是节点中链式结构对应的成员名称
 */
#define LIST_INIT(list, type, field) do {            \
    list.first_ = malloc(sizeof(type));              \
    if (NULL == list.first_) {                       \
        break;                                       \
    }                                                \
    type *head = LIST_FIRST(list);                   \
    head->field.next_ = head;                        \
    head->field.prev_ = &head->field.next_;          \
} while (0)

/* checked
 * 销毁链表
 * @param: list 是链表名称
 * @param: type 是节点类型
 * @param: field 是节点中链式结构对应的成员名称
 */
#define LIST_DESTROY(list, type, field) do {         \
    type *head = list.first_;                        \
    type *cur = LIST_NEXT((head), field);            \
    type *del = NULL;                                \
    while (cur != head) {                            \
        del = cur;                                   \
        cur = LIST_NEXT((cur), field);               \
        free(del);                                   \
    }                                                \
    free(head);                                      \
    head = NULL;                                     \
    del = NULL;                                      \
    cur = NULL;                                      \
} while (0)

/* checked
 * 封装的 for 循环
 * @param: var 是遍历链表的节点
 * @param: list 是链表名称
 * @param: field 是节点中链式结构对应的成员名称
 */
#define LIST_FOREACH(var, list, field)                     \
    for ((var) = LIST_NEXT((LIST_FIRST(list)), field);     \
            (var) != LIST_FIRST(list);                     \
            (var) = LIST_NEXT((var), field))

/* checked
 * 在head节点后面插入节点elm
 * @param: list 是链表名称
 * @param: type 是节点类型
 * @param: elm 是要插入的节点
 * @param: field 是节点中链式结构对应的成员名称
 */
#define LIST_INSERT_HEAD(list, type, elm, field) do {                   \
    type *head = list.first_;                                           \
    LIST_NEXT((elm), field) = LIST_NEXT((head), field);                 \
    (LIST_NEXT((head), field))->field.prev_ = &LIST_NEXT((elm), field); \
    LIST_NEXT((head), field) = (elm);                                   \
    (elm)->field.prev_ = &LIST_NEXT((head), field);                     \
} while (0)

/* checked
 * 在listelm之后插入节点elm
 * @param: type 是节点类型
 * @param: elm 是要插入的节点
 * @param: field 是节点中链式结构对应的成员名称
 */
#define	LIST_INSERT_AFTER(listelm, elm, field) do {				         \
	LIST_NEXT((elm), field) = LIST_NEXT((listelm), field);               \
	LIST_NEXT((listelm), field)->field.prev_ = &LIST_NEXT((elm), field); \
	LIST_NEXT((listelm), field) = (elm);						         \
	(elm)->field.prev_ = &LIST_NEXT((listelm), field);		             \
} while (0)

/* checked
 * 在listelm之前插入节点elm
 * @param: type 是节点类型
 * @param: elm 是要插入的节点
 * @param: field 是节点中链式结构对应的成员名称
 */
#define	LIST_INSERT_BEFORE(listelm, elm, field) do {   \
	(elm)->field.prev_ = (listelm)->field.prev_;       \
	LIST_NEXT((elm), field) = (listelm);               \
	*(listelm)->field.prev_ = (elm);                   \
	(listelm)->field.prev_ = &LIST_NEXT((elm), field); \
} while (0)

/* checked
 * 移除elm节点，但不释放对应堆空间
 * @param: elm 是要插入的节点
 * @param: field 是节点中链式结构对应的成员名称
 */
#define	LIST_REMOVE(elm, field) do {                           \
	LIST_NEXT((elm), field)->field.prev_ = (elm)->field.prev_; \
	*(elm)->field.prev_ = LIST_NEXT((elm), field);             \
} while (0)

/* checked
 * 删除elm节点，并且释放对应堆空间
 * @param: elm 是要插入的节点
 * @param: field 是节点中链式结构对应的成员名称
 */
#define	LIST_DELETE(elm, field) do {                           \
	LIST_REMOVE(elm, field);                                   \
    free((elm));                                               \
} while (0)

// 交换两个链表的头节点，没测，这个实验好像用不到
#define LIST_SWAP(list_1, list_2, type, field) do {     \
    type *head_1 = LIST_FIRST(list_1);                  \
    type *head_2 = LIST_FIRST(list_2);                  \
	struct type *swap_tmp = LIST_FIRST((head_1));       \
	LIST_FIRST((head_1)) = LIST_FIRST((head_2));        \
	LIST_FIRST((head_2)) = swap_tmp;                    \
	if ((swap_tmp = LIST_FIRST((head_1))) != NULL)		\
		swap_tmp->field.prev_ = &LIST_FIRST((head_1));	\
	if ((swap_tmp = LIST_FIRST((head_2))) != NULL)		\
		swap_tmp->field.prev_ = &LIST_FIRST((head_2));	\
} while (0)

/*
 * 好像是对的吧，实现快速排序的关键宏定义————交换 elm_1 和 elm_2 
 * 代码比较丑陋，但性能应该是可以保证的。后面再来看能不能做优化
 * @param: elm_1、elm_2 是要交换的节点
 * @param: type 是节点类型
 * @param: field 是节点中链式结构对应的成员名称
 */
#define LIST_SWAP_ENTRIES(elm_1, elm_2, type, field) do {     \
    type **elm_1_prev = (elm_1)->field.prev_;                 \
    type **elm_2_prev = (elm_2)->field.prev_;                 \
    type *elm_1_next = LIST_NEXT((elm_1), field);             \
    type *elm_2_next = LIST_NEXT((elm_2), field);             \
    if ((elm_1)->field.next_ == (elm_2)) {                    \
        *elm_1_prev = (elm_2);                                \
        (elm_2)->field.prev_ = elm_1_prev;                    \
        LIST_NEXT((elm_2), field) = (elm_1);                  \
        (elm_1)->field.prev_ = &LIST_NEXT((elm_2), field);    \
        LIST_NEXT((elm_1), field) = elm_2_next;               \
        elm_2_next->field.prev_ = &LIST_NEXT((elm_1), field); \
    } else if ((elm_2)->field.next_ == (elm_1)) {             \
        *elm_2_prev = (elm_1);                                \
        (elm_1)->field.prev_ = elm_2_prev;                    \
        LIST_NEXT((elm_1), field) = (elm_2);                  \
        (elm_2)->field.prev_ = &LIST_NEXT((elm_1), field);    \
        LIST_NEXT((elm_2), field) = elm_1_next;               \
        elm_1_next->field.prev_ = &LIST_NEXT((elm_2), field); \
    } else {                                                  \
        *elm_1_prev = (elm_2);                                \
        (elm_2)->field.prev_ = elm_1_prev;                    \
        LIST_NEXT((elm_2), field) = elm_1_next;               \
        elm_1_next->field.prev_ = &LIST_NEXT((elm_2), field); \
        *elm_2_prev = (elm_1);                                \
        (elm_1)->field.prev_ = elm_2_prev;                    \
        LIST_NEXT((elm_1), field) = elm_2_next;               \
        elm_2_next->field.prev_ = &LIST_NEXT((elm_1), field); \
    }                                                         \
} while (0)



/*
 * 上面的都可以不看捏
 * 双向循环链表指针部分结构体声明
 * 其中 name 是链表对象的名字，type是节点的类型
 */
#define DCL_LIST_HEAD(name, type)                    \
    struct name {                                    \
        struct type *first_;                         \
    }

/*
 * 链式结构部分成员的定义
 */
#define DCL_LIST_ENTRY(type)                         \
    struct {                                         \
        struct type *next_;                          \
        struct type *prev_;                          \
    }

/*
 *  函数，实现对双向循环链表的各项操作
 */

/* checked
 * 获取头节点
 * @param: list 是链表名称
 */
#define DCL_LIST_FIRST(list)                         \
    (list).first_

/* checked
 * 获取节点 elm 的下一个节点
 * @param: elm 是当前节点
 */
#define DCL_LIST_NEXT(elm, field)                    \
    (elm)->field.next_

/* checked
 * 获取节点 elm 的下一个节点
 * @param: elm 是当前节点
 */
#define DCL_LIST_PREV(elm, field)                    \
    (elm)->field.prev_

/* checked
 * 初始化链表
 * @param: list 是链表名称
 * @param: type 是节点类型
 * @param: field 是节点中链式结构对应的成员名称
 */
#define DCL_LIST_INIT(list, type, field) do {        \
    list.first_ = malloc(sizeof(type));              \
    if (NULL == list.first_) {                       \
        break;                                       \
    }                                                \
    type *head = DCL_LIST_FIRST(list);               \
    head->field.next_ = head;                        \
    head->field.prev_ = head;                        \
} while (0)

/* checked
 * 销毁链表
 * @param: list 是链表名称
 * @param: type 是节点类型
 * @param: field 是节点中链式结构对应的成员名称
 */
#define DCL_LIST_DESTROY(list, type, field) do {     \
    type *head = list.first_;                        \
    type *cur = DCL_LIST_NEXT((head), field);        \
    type *del = NULL;                                \
    while (cur != head) {                            \
        del = cur;                                   \
        cur = DCL_LIST_NEXT((cur), field);           \
        free(del);                                   \
    }                                                \
    free(head);                                      \
    head = NULL;                                     \
    del = NULL;                                      \
    cur = NULL;                                      \
} while (0)

/* checked
 * 封装的 for 循环
 * @param: var 是遍历链表的节点
 * @param: list 是链表名称
 * @param: field 是节点中链式结构对应的成员名称
 */
#define DCL_LIST_FOREACH(var, list, field)                     \
    for ((var) = DCL_LIST_NEXT((DCL_LIST_FIRST(list)), field); \
            (var) != DCL_LIST_FIRST(list);                     \
            (var) = DCL_LIST_NEXT((var), field))

/* checked
 * 在head节点后面插入节点elm
 * @param: list 是链表名称
 * @param: type 是节点类型
 * @param: elm 是要插入的节点
 * @param: field 是节点中链式结构对应的成员名称
 */
#define DCL_LIST_INSERT_HEAD(list, type, elm, field) do {         \
    type *head = list.first_;                                     \
    DCL_LIST_NEXT((elm), field) = DCL_LIST_NEXT((head), field);   \
    DCL_LIST_PREV((DCL_LIST_NEXT((head), field)), field) = (elm); \
    DCL_LIST_NEXT((head), field) = (elm);                         \
    DCL_LIST_PREV((elm), field) = (head);                         \
} while (0)

/* checked
 * 在listelm之后插入节点elm
 * @param: type 是节点类型
 * @param: elm 是要插入的节点
 * @param: field 是节点中链式结构对应的成员名称
 */
#define	DCL_LIST_INSERT_AFTER(listelm, elm, field) do {				\
	DCL_LIST_NEXT((elm), field) = DCL_LIST_NEXT((listelm), field);  \
	DCL_LIST_PREV(DCL_LIST_NEXT((listelm), field), field) = (elm);  \
	DCL_LIST_NEXT((listelm), field) = (elm);						\
	DCL_LIST_PREV((elm), field) = (listelm);	             	    \
} while (0)

/* checked
 * 在listelm之前插入节点elm
 * @param: type 是节点类型
 * @param: elm 是要插入的节点
 * @param: field 是节点中链式结构对应的成员名称
 */
#define	DCL_LIST_INSERT_BEFORE(listelm, elm, field) do {           \
	DCL_LIST_NEXT(DCL_LIST_PREV((listelm), field), field) = (elm); \
    DCL_LIST_PREV((elm), field) = DCL_LIST_PREV((listelm), field); \
    DCL_LIST_NEXT((elm), field) = (listelm);                       \
    DCL_LIST_PREV((listelm), field) = (elm);                       \
} while (0)

/* checked
 * 移除elm节点，但不释放对应堆空间
 * @param: elm 是要插入的节点
 * @param: field 是节点中链式结构对应的成员名称
 */
#define	DCL_LIST_REMOVE(elm, field) do {                                             \
	DCL_LIST_PREV(DCL_LIST_NEXT((elm), field), field) = DCL_LIST_PREV((elm), field); \
	DCL_LIST_NEXT(DCL_LIST_PREV((elm), field), field) = DCL_LIST_NEXT((elm), field); \
} while (0)

/* checked
 * 删除elm节点，并且释放对应堆空间
 * @param: elm 是要插入的节点
 * @param: field 是节点中链式结构对应的成员名称
 */
#define	DCL_LIST_DELETE(elm, field) do {                           \
	DCL_LIST_REMOVE(elm, field);                                   \
    free((elm));                                                   \
} while (0)

/* checked
 * 实现快速排序的关键宏定义————交换 elm_1 和 elm_2 
 * @param: elm_1、elm_2 是要交换的节点
 * @param: type 是节点类型
 * @param: field 是节点中链式结构对应的成员名称
 */
#define DCL_LIST_SWAP_ENTRIES(elm_1, elm_2, type, field) do {     \
    type *elm_1_prev = DCL_LIST_PREV((elm_1), field);             \
    type *elm_2_prev = DCL_LIST_PREV((elm_2), field);             \
    type *elm_1_next = DCL_LIST_NEXT((elm_1), field);             \
    type *elm_2_next = DCL_LIST_NEXT((elm_2), field);             \
    if (DCL_LIST_NEXT((elm_1), field) == (elm_2)) {               \
        DCL_LIST_NEXT((elm_1_prev), field) = (elm_2);             \
        DCL_LIST_PREV((elm_2), field) = (elm_1_prev);             \
        DCL_LIST_NEXT((elm_2), field) = (elm_1);                  \
        DCL_LIST_PREV((elm_1), field) = (elm_2);                  \
        DCL_LIST_NEXT((elm_1), field) = (elm_2_next);             \
        DCL_LIST_PREV((elm_2_next), field) = (elm_1);             \
    } else if (DCL_LIST_NEXT((elm_2), field) == (elm_1)) {        \
        DCL_LIST_NEXT((elm_2_prev), field) = (elm_1);             \
        DCL_LIST_PREV((elm_1), field) = (elm_2_prev);             \
        DCL_LIST_NEXT((elm_1), field) = (elm_2);                  \
        DCL_LIST_PREV((elm_2), field) = (elm_1);                  \
        DCL_LIST_NEXT((elm_2), field) = (elm_1_next);             \
        DCL_LIST_PREV((elm_1_next), field) = (elm_2);             \
    } else {                                                      \
        DCL_LIST_NEXT((elm_1_prev), field) = (elm_2);             \
        DCL_LIST_PREV((elm_2), field) = (elm_1_prev);             \
        DCL_LIST_NEXT((elm_2), field) = (elm_1_next);             \
        DCL_LIST_PREV((elm_1_next), field) = (elm_2);             \
        DCL_LIST_NEXT((elm_2_prev), field) = (elm_1);             \
        DCL_LIST_PREV((elm_1), field) = (elm_2_prev);             \
        DCL_LIST_NEXT((elm_1), field) = (elm_2_next);             \
        DCL_LIST_PREV((elm_2_next), field) = (elm_1);             \
    }                                                             \
} while (0)

#endif