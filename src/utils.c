#include "../include/cub3d.h"

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

char	*gc_strdup(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = gc_malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
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