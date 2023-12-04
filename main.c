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

int Check_value(char *str)
{
    int i;
    char *num;
    int nb;
    int j;
    int b;

    b = 0;
    i = 0;
    j = 0;

    while (b <= 2)
    {
        while ((str[i] && str[i] < '0') || (str[i] && str[i] > '9'))
            i++;
        if (str[i] >= '0' && str[i] <= '9')
            while (str[i + j] >= '0' && str[i +j] <= '9')
                j++;
        num = malloc(sizeof(char) * j + 1);
        if (!num)
            return (1);
        j = 0;
        while (str[i + j] >= '0' && str[i + j] <= '9')
        {
            num[j] = str[i + j];
            j++;
        }
        num[j] = '\0';
        if (num[0] == '\0')
        {
            printf("nombre manquant\n");
            return (0);
        }
        nb = atoi(num);
        i = i + j;
        j = 0;
        if (nb < 0 || nb > 255)
        {
            printf("Erreur nombre %d\n", nb);
            return (1);
        }
        b++;
        i++;
    }
    return (0); 
}

int size_map_y(int fd)
{
    char *str;
    int y;

    y = 0;
    str = get_next_line(fd);
    printf("|%s|\n", str);
    while (str)
    {
        str = get_next_line(fd);
        if (strncmp(str, "0", 1) == 0 || strncmp(str, "1", 1) == 0)
            y++;
    }
    return (y);
}
int size_map_x(void)
{
    char *str;
    int fd = open("map.cub", O_RDONLY);
    int x;
    while (str && )
    str = get_next_line(fd);
    
    x = strlen(str);
    if (strncmp(str, "0", 1) == 0 || strncmp(str, "1", 1) == 0)
    {
        while ((str && fd > 0 && strncmp(str, "0", 1) == 0) || (str && fd > 0 && strncmp(str, "1", 1) == 0))
        {
            str = get_next_line(fd);
            if (str)
                if (strlen(str) > x)
                    x = strlen(str);
        }
    }
    return (x);
}

int Parse_map(int fd)
{
    int y;
    int x;

   x = size_map_x();
   close(fd);
   // fd = open("map.cub", O_RDONLY);
    y = size_map_y(fd);
    printf(" y = %d\n", y);
    return (0);
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
    while (strncmp(str, "F ", 2) != 0 && fd != 0)
    {
        str = get_next_line(fd);
        if (strncmp(str, "F ", 2) == 0)
        {
            if (Check_value(str) == 1)
                return (1);
        }
    }
    printf("ok\n");
    while (strncmp(str, "C ", 2) != 0 && fd != 0)
    {
        str = get_next_line(fd);
        if (strncmp(str, "C ", 2) == 0)
        {
            if (Check_value(str) == 1)
                return (1);
        } 
    }
    printf("ok\nParsing Map \n");
    while ((fd > 0 && strncmp(str, "0", 1) != 0) && (fd > 0 && strncmp(str, "1", 1) != 0))
        str = get_next_line(fd);
    Parse_map(fd);
    printf("parsing passé\n");

    return (0);
}