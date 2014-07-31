/* File: node.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef NODE_H
#define NODE_H

#include <x86os/types.h>

/* Each file system node represented such way */
struct fs_node {
	dev_t dev;		// device where node stored

	void *data;		// Some fs-dependend data
	struct fs_node *mounted;	// If != NULL another fs mounted here
	struct node_operations *op;
};

struct node_operations {
	/* Files only */
	int (*open) (struct fs_node * node, int flags, mode_t mode);
	 ssize_t(*read) (struct fs_node * node, char *buf, size_t count);
	 ssize_t(*write) (struct fs_node * node, char *buf, size_t count);
	int (*close) (struct fs_node * node);

	/* Directories only */
	struct fs_node *(*lookup) (struct fs_node * node, const char *entry);
	int (*create) (struct fs_node * node, const char *entry,
		       struct fs_node * new_node);
	int (*remove) (struct fs_node * node, const char *entry);
};

#endif				/* NODE_H */
