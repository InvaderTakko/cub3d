#include "../include/cub3d.h"

int32_t	exit_with_error(const char *message, t_app *app)
{
	if (app)
	{
		if (app->mlx)
			mlx_terminate(app->mlx);
		if (app->image)
			mlx_delete_image(app->mlx, app->image);
		gc_free_all();
	}
	ft_printf("Error: %s\n", message);
	return (1);
}

char	*ft_strstr(const char *haystack, const char *needle)
{
	const char	*h = {0};
	const char	*n = {0};

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