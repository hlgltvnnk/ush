#include "../inc/ush.h"

int ush_exit(char **args, t_ush *ush){
    exit(0);
    write(1, "exit\n", 5);
    if (args[1] == NULL)
        ush->exit = 0;
    else if (args[2]) {
        write(1, "exit: too many arguments\n", 25);
        return 1;
    }
    else {
        ush->exit = mx_atoi(args[1]);
        if (ush->exit >= 256) {
            while (ush->exit > 256)
                ush->exit = ush->exit - 256;
        }
    }
    return 0;
}
