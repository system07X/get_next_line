#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFERSIZE 5

typedef struct s_list t_list;

struct s_list
{
	void *content;
	t_list *next;
};

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

int found_newline(char *stash)
{
	int i;

	i = 0;
	while (stash[i])
	{
		if (stash[i] == '\n')
			return (1);
		i++;	
	}
	return (0);
}

static char *ft_strjoin(char *s1, char *s2)
{
	static char *s;
	size_t len;

	len = (ft_strlen(s1) + ft_strlen(s2));
	s = malloc(sizeof(char) * len);
	if (s == NULL)
		return (NULL);
	while (*s1)
	{
		*s++ = *s1++;
	}
	while (*s2)
		*s++ = *s2++;
	*s = 0;
	return (s - len);
}

int gnl_strlen(char *stash)
{
	int i;

	i = 0;
	while (stash[i] != '\n')
		i++;
	return(i);
}

char *gnl_extractline(char *stash)
{
	char *ret;
	int i;

	i = 0;
	ret = malloc(sizeof(char) * (gnl_strlen(stash) + 1));
	while (i <= gnl_strlen(stash))
	{
		ret[i] = stash[i];
		i++;
	}
	return (ret);
}

static char *gnl_clear(char *stash)
{
	static char *ret;
	int i;
	int j;

	i = 0;
	j = 0;
	ret = malloc(sizeof(char) * (ft_strlen(stash) - gnl_strlen(stash) + 1));
	if (!ret)
		return NULL;
	while (stash[j] != '\n')
		j++;
	j++;
	while(stash[j + i])
	{
		ret[i] = stash[j + i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}


char *get_next_line(int fd)
{
	char *buf;
	static char *stash;
	char *line;
	int r;

	buf = malloc(sizeof(char) * (BUFFERSIZE + 1));
	buf[BUFFERSIZE] = '\0';
	line = "";
	if (stash == NULL)
		stash = "";
	while (r = read(fd, buf, BUFFERSIZE))
	{
		if (r != BUFFERSIZE)
			buf[r] = '\0';
		stash = ft_strjoin(stash, buf);
		if (found_newline(stash))
		{
			line = ft_strjoin(line, gnl_extractline(stash));
			stash = gnl_clear(stash);
			return (line);
		}
	}
	if (*line == '\0' && *stash == '\0')
		return NULL;
	line = ft_strjoin(line, stash);
	stash = NULL;
	return (line);
}


int main()
{
	int fd;
	int bool;
	char *line;

	bool = 1;

	fd = open("test_gnl.txt", O_RDONLY);
	while (bool)
	{
		line = get_next_line(fd);
		printf("%s", line);
		free(line);
		if (line == NULL)
			bool = 0;
	}
}
