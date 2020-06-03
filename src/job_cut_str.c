#include "../inc/ush.h"

char *mx_cut_str_forjob(char *args) {
    char *res = NULL;

    if (args == NULL || args[0] == '\0') //%
        return res;
    else if (args[0] != '%' || (args[0] == '/' && args[0] == '\0')) // fg alpha
        res = mx_strjoin(res, args);
    else { 
        for (int i = 1; args[i]; i++)
            res = mx_strjoin(res, &args[i]);
    }
    return res;
}
