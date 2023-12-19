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

int size_map_y(void)
{
    int fd;
    char *str = NULL;
    int y;

    y = 0;
    fd = open ("map.cub", O_RDONLY);
    str = get_next_line(fd);
    while (str)
    {
        if (strncmp(str, "0", 1) == 0 || strncmp(str, "1", 1) == 0)
        {
           // printf("str = %s\n", str);
            y++;
        }
        if ((strncmp(str, "0", 1) != 0 && strncmp(str, "1", 1) != 0) && y > 0)
        {
            printf("str = %s\n", str);
            return (-1);
        }
        str = get_next_line(fd);
    }
    return (y);
}

char **Parse_map(int fd, char *line)
{
    int y;
    int i;
    int j;
    char **map;

    y = size_map_y();
    i = 0;
    j = 0;
    printf(" y = %d\n", y);
    if (y <= 0)
    {
        printf("MAP ERROR\n");
        return (NULL);
    }
    map = malloc(sizeof(char *) * (y + 1));
    if (!map)
        return (NULL);
    while (line)
    {
        map[i++] = line;
        line = get_next_line(fd);
    }
    map[i] = NULL;
    i = 0;
    return (map);
}
int check_coord(char **map, int y, int x)
{
    if (y == 0 || x == 0 || (map[y - 1][x - 1] != '0' && map[y - 1][x - 1] != '1'))
        return (1);
    if (y == 0 || (map[y - 1][x] != '0' && map[y - 1][x] != '1'))
        return (1);
    if (y == 0 || (map[y - 1][x + 1] != '0' && map[y - 1][x + 1] != '1'))
        return (1);

    if (x == 0 || (map[y][x - 1] != '0' && map[y][x - 1] != '1'))
        return (1);
    if (map[y][x + 1] != '0' && map[y][x + 1] != '1')
        return (1);
    return (0);

    if (x == 0 || (map[y + 1][x - 1] != '0' && map[y + 1][x - 1] != '1'))
        return (1);
    if ((map[y + 1][x] != '0' && map[y + 1][x] != '1'))
        return (1);
    if ((map[y + 1][x + 1] != '0' && map[y + 1][x + 1] != '1'))
        return (1);
    return (0);
}

int true_map(char **map)
{
    int x;
    int y;

    x = 0;
    y = 0;
    while (map[y] != NULL)
    {
        printf("%s", map[y]);
        while (map[y][x])
        {
            if (map[y][x] == '0')
                if (check_coord(map, y, x) == 1)
                {
                    printf("y = %d x = %d", y, x);
                    return (1);
                }
            x++;
        }
        x = 0;
        y++;
    }
    return (0);
}

int main (void)
{
    int fd = open("map.cub", O_RDONLY);
    char *str;
    char **map;
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
    map = Parse_map(fd, str);
    if (true_map(map) == 1)
    {
        printf("map error\n");
        return (1);
    }
    printf("parsing passé\n");

    return (0);
}