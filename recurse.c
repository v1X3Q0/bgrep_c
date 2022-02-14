#include <stdio.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <linux/limits.h>
#include <dirent.h>
#include <sys/stat.h>

#include "bgrep_c_util.h"

int recurse_op(int (*routine_on_file)(const char*, int, void**), const char* path_dir, int count, void** vargs)
{
    int result = -1;
    struct dirent *de; // Pointer for directory entry
    char path_tmp[PATH_MAX] = { 0 };
    DIR *dr = 0;
    struct stat fileStat = {0};

    // printf("%s\n", path_dir);

    SAFE_BAIL(stat(path_dir, &fileStat) < 0)
    if((fileStat.st_mode & __S_IFMT) == __S_IFREG)
    {
        result = routine_on_file(path_dir, count, vargs);
        goto finish;
    }

    // opendir() returns a pointer of DIR type.
    dr = opendir(path_dir);
    SAFE_BAIL(dr == NULL);

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL)
    {
        if ((strcmp(de->d_name, ".") == 0) || (strcmp(de->d_name, "..") == 0))
        {
            continue;
        }

        memset(path_tmp, 0, sizeof(path_tmp));
        strcpy(path_tmp, path_dir);
        strcat(path_tmp, "/");
        strcat(path_tmp, de->d_name);

        if (de->d_type == DT_REG)
        {
            routine_on_file(path_tmp, count, vargs);
        }
        else if (de->d_type == DT_DIR)
        {
            recurse_op(routine_on_file, path_tmp, count, vargs);
        }
    }


finish:
    SAFE_CLOSEDIR(dr);
    result = 0;
fail:
    return result;
}
