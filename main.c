#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
# include "get_next_line/get_next_line.h"

char *Parse_link(char *str)
{
    int i;
    int j;
    char *ret;

    i = 0;
    j = 0;
    while (str[i] && str[i] != '.')
        i++;
    while (str[i + j] && str[i + j] != '\n')
        j++;
    ret = malloc(sizeof(char) * (j + 1));
    if (!ret)
        return (NULL);
    j = 0;
    while (str[i + j] && str[(i + j)] != '\n')
    {
        ret[j] = str[i + j];
        j++;
    }
    ret[j] = '\0';
    return (ret);
}
int Check_texture(char *str, int index)
{

    if((index == 0 && strncmp(str, "NO ", 3) == 0) || (index == 1 && strncmp(str, "SO ", 3) == 0) || (index == 2 && strncmp(str, "WE ", 3) == 0) || (index == 3 && strncmp(str, "EA ", 3) == 0))
        if (access(Parse_link(str), R_OK) == 0)
        {
            printf("ok\n");
            return (0);
        }
    printf("faux\n");
    return (1);
}
int main (void)
{
    int fd = open("map.cub", O_RDONLY);
    char *str;
    int i;

    i = 0;
    while (i < 4)
    {
        str = get_next_line(fd);
        if ((strncmp(str, "NO ", 3) == 0) || (strncmp(str, "SO ", 3) == 0) || (strncmp(str, "WE ", 3) == 0) || (strncmp(str, "EA ", 3) == 0))
        {
            if (Check_texture(str, i) == 1)
                return (0);
            else 
                 i++;
        }
    }

    return (0);
}