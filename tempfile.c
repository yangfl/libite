/* A secure tmpfile() replacement.
 *
 * Copyright (c) 2015  Joachim Nilsson <troglobit@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <paths.h>
#include <fcntl.h>		/* O_TMPFILE requires -D_GNU_SOURCE */
#include <stdlib.h>		/* mkstemp() */
#include <stdio.h>		/* fdopen() */
#include <sys/stat.h>		/* umask() */

/**
 * tempfile - A secure tmpfile() replacement
 *
 * This is the secure replacement for tmpfile() that does not exist in
 * GLIBC.  The function uses the Linux specific %O_TMPFILE and %O_EXCL
 * for security.  When the %FILE is fclose()'ed the file contents is
 * lost.  The file is hidden in the %_PATH_TMP directory on the system.
 *
 * Returns:
 * An open %FILE pointer, or %NULL on error.
 */
FILE *tempfile(void)
{
	int fd;
	mode_t oldmask;

	oldmask = umask(0077);
	fd = open(_PATH_TMP, O_TMPFILE | O_RDWR | O_EXCL | O_CLOEXEC, S_IRUSR | S_IWUSR);
	umask(oldmask);
	if (-1 == fd)
		return NULL;

	return fdopen(fd, "rw");
}

#ifdef UNITTEST
int main(void)
{
	FILE *fp = tempfile(); system("ls -lrt "
	_PATH_TMP " | tail -10"); return fclose(fp);
}
#endif

/**
 * Local Variables:
 *  compile-command: "gcc -D_GNU_SOURCE -DUNITTEST -o tempy tempfile.c && ./tempy"
 *  version-control: t
 *  indent-tabs-mode: t
 *  c-file-style: "linux"
 * End:
 */