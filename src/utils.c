#include "../include/cub3d.h"

char	**str_array_dup(char **src, int32_t height)
{
	char	**dest;
	int32_t	i;

	if (!src)
		return (NULL);
	dest = gc_malloc(sizeof(char *) * (height + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (i < height)
	{
		dest[i] = gc_strdup(src[i]);
		if (!dest[i])
		{
			// If gc_strdup fails, gc will handle cleanup.
			// No need for free_str_array(dest, i); here.
			return (NULL);
		}
		i++;
	}
	dest[i] = NULL;
	return (dest);
}

int32_t	validate_texture_file(const char *path)
{
	int32_t	fd;

	if (!path)
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

int32_t	ft_isspace(int32_t c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

char	*ft_strpbrk(const char *s, const char *charset)
{
	while (*s)
	{
		if (ft_strchr(charset, *s))
			return ((char *)s);
		s++;
	}
	return (NULL);
}

int32_t	exit_with_error(const char *message, t_app *app)
{
	ft_printf("Error: %s\n", message);
	if (app)
	{
		if (app->image)
			mlx_delete_image(app->mlx, app->image);
		if (app->mlx)
			mlx_terminate(app->mlx);
	}
	gc_free_all();
	exit(1);
	return (1);
}

char	*ft_strstr(const char *haystack, const char *needle)
{
	const char *h = {0};
	const char *n = {0};

	if (!*needle)
		return ((char *)haystack);
	while (*haystack)
	{
		h = haystack;
		n = needle;
		while (*h && *n && *h == *n)
		{
			h++;
			n++;
		}
		if (!*n)
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}
