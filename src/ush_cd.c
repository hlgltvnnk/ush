#include "../inc/ush.h"

static int false_cd(char *name, t_cd *num) {
    if (num->error == 1)
        write(2, "cd: no such file or directory: ", 31);
    else if (num->error  == 2)
        write(2, "cd: not a directory: ", 21);
    else if (num->error == 4)
        write(2, "cd: permission denied: ", 23);
    write(2, name, mx_strlen(name));
    write(2, "\n", 1);
    free(num->newpwd);
    free(num);
    errno = 0;
    return 1;
}

static int env_work(t_ush *ush, char *pwd, char *args, t_cd *in) {
    if (!mx_opencheck(in->newpwd, &in->error, in->flag_s)) {
        free(pwd);
        return false_cd(in->newpwd, in);
    }
    if (chdir(in->newpwd) == -1) {
        free(pwd);
        in->error = 1;
        return false_cd(args, in);
    }
    if (in->flag_P == 1) {
        free(in->newpwd);
        in->newpwd = NULL;
        in->newpwd = getcwd(NULL, 0);
        setenv("PWD", in->newpwd, 1);
    }
    else 
        setenv("PWD", in->newpwd, 1);
    setenv("OLDPWD", pwd, 1);
    free(pwd);
    free(ush->pwd);
    ush->pwd = mx_strdup(in->newpwd);
    free(in->newpwd);
    free(in);
    return 2;
}

static char *proverka(char **args, int f, char *pwd) {
    char *newpwd = NULL;

    if (args[f][0] != '/')
        newpwd = mx_strdup(pwd); 
    return newpwd;
}

static int tak_syak(char **args, char *pwd, t_cd *in, t_ush *ush) {
    if (args[1] == NULL) {
        in->newpwd = mx_strdup(ush->home);
        return env_work(ush, pwd, args[0], in);
    }
    if (!strcmp(args[in->f], "-") || !strcmp(args[in->f], "~-")) {
        in->newpwd  = mx_strdup(getenv("OLDPWD"));
        return env_work(ush, pwd, args[in->f], in);
    }
    if (!strcmp(args[in->f], "~+")) {
        in->newpwd = mx_strdup(getenv("PWD"));
        return env_work(ush, pwd, args[in->f], in);
    }
    if (strcmp(args[in->f], "/") == 0) {
        in->newpwd = mx_strdup("/");
        return env_work(ush, pwd, args[in->f], in);
    }
    return 0;
}

int mx_ush_cd(char **args, t_ush *ush) {
    char *pwd = mx_strdup(ush->pwd);
    char **m = NULL;
    t_cd *in = (t_cd *)malloc(sizeof(t_cd) * 5);
    
    in->f = 1;
    if (args[1] && (!strcmp(args[1], "-s") || !strcmp(args[1], "-P"))){
        in->f = 2;
        !strcmp(args[1], "-s") ? in->flag_s = 1 : 0;
        !strcmp(args[1], "-P") ? in->flag_P = 1 : 0;
    }
    if (tak_syak(args, pwd, in, ush) > 0)
        return 0;
    in->newpwd = proverka(args, in->f, pwd);
    m = mx_strsplit(args[in->f], '/');
    in->newpwd = gogo(in->newpwd, m, in);
    if (in->newpwd == NULL) {
        free(pwd);
        return false_cd(args[in->f], in);
    }
    env_work(ush ,pwd, args[in->f], in);
    return 0;
}
