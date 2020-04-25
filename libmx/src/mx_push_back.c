#include "libmx.h"

void mx_push_back(t_list **list, void *data)
{
   t_list *p;
    p = *list;
    if (p) {
        while (p->next) p = p->next;
        p->next = mx_create_node(mx_strdup(data));
    }
    else *list = mx_create_node(mx_strdup(data));
}
