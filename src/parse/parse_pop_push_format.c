#include "../../inc/ush.h"

void mx_pop_format(t_frmt_lst **del) {
    free((*del)->data);
    mx_pop_front((t_list **)del);
}

void mx_push_format(t_frmt_lst **add, int start, int end, t_frmt_lst **del) {
    mx_push_front((t_list **)add, malloc(sizeof(t_qts_params)));
    (*add)->data->start = start;
    (*add)->data->end = end;
    if (del)
        mx_pop_format(del);
}
