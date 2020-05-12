#include "../inc/ush.h"

static char *hist_but(char *line, char *data, int *x) {
    for (; *x > 0; (*x)--) {
        printf("%c[1C", 27);
        fflush(stdout);
    }
    for (int i = 0; line != NULL && i < mx_strlen(line); i++)
        write(1, "\b \b", 3);
    mx_strdel(&line);
    write(1, data, mx_strlen(data));
    line = mx_strdup(data);
    return line;
}

static char *button(t_hst **hs, char *line, int buf, int *x) {
    t_hst *h = *hs;

    if (buf == 4283163) {
        h->next ? h = h->next : 0;
        line = hist_but(line, h->data, x);
    }
    if (buf == 4348699) {
        h->prev ? h = h->prev : 0;
        line = hist_but(line, h->data, x);
    }
    if (buf == 4414235 && *x > 0) {
        printf("%c[1C", 27);
        (*x)--;
    }
    if (buf == 4479771 && line != NULL && mx_strlen(line) > *x) { 
        printf("%c[1D", 27);
        (*x)++;
    }
    *hs = h;
    fflush(stdout);
    return line;
}

char *read_stream(t_hst *h) {
    unsigned int buf = 0;
    char *line = NULL;
    int len = 0;
    int x = 0;
    t_hst *head = NULL;
    
    push_f(&h, "\0"); 
    head = h; 
    while ((len = read(0, &buf, 4)) > 0) {
        if (len == 1) {
            if (buf == 10 || buf == 12 || (buf == 4 && 
                (line == NULL || !strlen(line)))) {
                buf == 4 ? line = mx_strdup("exit") : 0;
                buf == 12 ? line = mx_strdup("clear") : 0;
                break ;
            }
            line = stream(buf, line, &x);
        }
        if (len > 1) 
            line = button(&h, line, buf, &x);
        buf = 0;
    }
    free_node(head);
    return line;
}
