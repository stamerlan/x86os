/* File: super.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

asmlinkage int sys_mount(char *dev_name, char *dir_name, char *type)
{
	struct file_system_type *fstype;
	struct inode *inode;
	dev_t dev;
	int r
}

