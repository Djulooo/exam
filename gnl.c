#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 5
#endif

static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	char				*res;
	unsigned long int	i;
	size_t				len1;
	size_t				len2;

	if (s1 == NULL)
		return ((char *)s2);
	len1 = ft_strlen((char *)s1);
	len2 = len1 + ft_strlen((char *)s2);
	i = -1;
	res = malloc(sizeof(char) * (len2 + 1));
	if (!res)
		return (free((char *)s1), free((char *)s2), NULL);
	while (++i < len1)
		res[i] = s1[i];
	while (i < len2)
	{
		res[i] = s2[i - len1];
		i++;
	}
	res[i] = '\0';
	return (free((char *)s1), free((char *)s2), res);
}

static char	*ft_strdup(char *str)
{
	char	*dst;
	int		len;
	int		i;

	len = ft_strlen(str);
	dst = malloc(sizeof(char) * len + 1);
	if (!dst)
		return (NULL);
	i = 0;
	while (str[i])
	{
		dst[i] = str[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

static int	check_newline(char	*temp)
{
	int	i;

	i = 0;
	if (temp == NULL)
		return (1);
	while (temp[i] && temp[i] != '\n')
		i++;
	if (temp[i] == '\n')
		return (1);
	return (0);
}

static char	*extract_buffer(char **temp, char *buffer)
{
	char	*str;
	char	*holder;

	str = *temp;
	holder = ft_strdup(buffer);
	*temp = ft_strjoin(str, holder);
	if (!*temp)
		return(NULL);
	return (*temp);
}

static char	*ft_strchar(char *str, char needle)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == needle && str[i + 1])
			return (&str[i + 1]);
		i++;
	}
	return (NULL);
}

static char	*copy_buffer(char *oldtemp, char *newtemp)
{
	int	i;

	i = 0;
	while (newtemp && newtemp[i])
	{
		oldtemp[i] = newtemp[i];
		i++;
	}
	oldtemp[i] = '\0';
	return (oldtemp);
}

static char *extract_line(char **line, char **temp)
{
	int		i;
	int		j;
	int		newl;
	char	*holder;
	char	*str;

	i = 0;
	newl = 0;
	while ((*temp)[i] && (*temp)[i] != '\n')
		i++;
	if ((*temp)[i] == '\n')
		newl = 1;
	holder = malloc(sizeof(char) * (i + newl + 1));
	if (!holder)
		return (NULL);
	j = 0;
	while (j < i + newl)
	{
		holder[j] = (*temp)[j];
		j++;
	}
	holder[j] = '\0';
	str = *temp;
	*line = ft_strjoin(*line, holder);
	*temp = ft_strdup(copy_buffer(str, ft_strchar(str, '\n')));
	if (!*temp || !*line)
		return (NULL);
	return(*line);
}

char	*get_next_line(int fd)
{
	char	 	*line;
	char		buffer[BUFFER_SIZE + 1];
	static char	*temp;
	int			treated;

	if (read(fd, NULL, 0) == -1)
		return (free(temp), NULL);
	line = ft_strdup("");
	if (fd < 0 || BUFFER_SIZE <= 0 || !line)
		return (free(temp), NULL);
	treated = 0;
	if (temp)
		treated = check_newline(temp);
    while (treated == 0)
	{
		treated = read(fd, buffer, BUFFER_SIZE);
		if (treated == 0 || treated < 0)
			break ;
		buffer[treated] = '\0';
		temp = extract_buffer(&temp, buffer);
		if (!temp)
			break ;
		treated = check_newline(temp);
	}
	if (temp && line && treated >= 0)
		line = extract_line(&line, &temp);
	if (!temp || !line || line[0] == '\0' || treated < 0)
		return (free(temp), buffer[0] = '\0', temp = NULL, free(line), NULL);
	return (line);
}
