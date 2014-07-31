/* File: nname.c
 * Author: Vlad Vovchneko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/config.h>
#include <x86os/string.h>
#include <x86os/types.h>
#include <x86os/proc.h>
#include <x86os/printk.h>
#include <x86os/fs/node.h>

// TODO: may be move it to vfs.c?
extern struct fs_node *root;

struct fs_node *
get_node(const char *filename)
{
	struct fs_node *base;
	size_t i, p, namelen;
	char path[MAX_PATH + 1];

	if (filename[0] == '/') {
		base = root;
		filename++;
	} else {
		base = current->pwd;
	}

	namelen = strlen(filename);
	if (namelen > MAX_PATH)
		// error: filename too long
		return NULL;
	memmove(path, filename, namelen);

	i = 0;
	p = 0;
	while (i < namelen) {
		if (path[i] != '/') {
			i++;
			continue;
		}

		path[i] = '\0';
		base = base->op->lookup(base, &path[p]);
		if (!base)
			return NULL;
		p = ++i;
	}
	// last component maybe
	if (i != p) {
		path[i] = '\0';
		printk("debug: get_node: path = %s, lookup addr = 0x%x, "
		       "base = %d\n", &path[p], &base->op->lookup,
		       (int) base->data);
		base = base->op->lookup(base, &path[p]);
		if (!base)
			return NULL;
	}

	return base;
}
