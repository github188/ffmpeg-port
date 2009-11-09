
#pragma once

static inline int is_dos_path(const char *path)
{
#if HAVE_DOS_PATHS
	if (path[0] && path[1] == ':')
		return 1;
#endif
	return 0;
}