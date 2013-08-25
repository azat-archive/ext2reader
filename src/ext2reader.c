
/**
 * Only directory reader
 *
 * TODO: add logger
 * TODO: add options
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#include <ext2fs/ext2fs.h>


struct DirIterate
{
    ext2_filsys fs;
    ext2_inode_scan scanner;

    ext2_ino_t ino;
    struct ext2_inode inode;

    char prefix[PATH_MAX];
    char name[PATH_MAX];
    char dirIterateBuffer[PATH_MAX];
};
void dirIterateAddroot(struct DirIterate *it)
{
    char *buffer;

    ext2fs_get_pathname(it->fs, it->ino, 0, &buffer);

    strcat(it->prefix, buffer);
    strcat(it->prefix, "/");
}
void dirIterateRemoveRoot(struct DirIterate *it)
{
    void *lastSlash = memrchr(it->prefix, '/', strlen(it->prefix) - 1);
    assert(lastSlash);
    lastSlash = '\0';
}


int dirIterator(ext2_ino_t dir,
                int entry,
                struct ext2_dir_entry *dirent,
                int offset,
                int blockSize,
                char *buf,
                void *privData)
{
    struct DirIterate *it = (struct DirIterate *)privData;

    int length = (dirent->name_len & 0xFF);
    strncpy(it->name, dirent->name, length);
    // TODO: do we need this?
    it->name[length] = '\0';

    printf("%s%s\n", it->prefix, it->name);

    assert(!ext2fs_read_inode(it->fs, dirent->inode, &it->inode));
    if (LINUX_S_ISDIR(it->inode.i_mode) && dirent->name[0] != '.' /* TODO: fix this hack */) {
        it->ino = dirent->inode;
        dirIterateAddroot(it);
        assert(!ext2fs_dir_iterate2(it->fs, it->ino, 0, it->dirIterateBuffer, dirIterator, it));
        dirIterateRemoveRoot(it);
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s block_device\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *dev = argv[1];

    struct DirIterate it;
    memset(&it, 0, sizeof(struct DirIterate));
    strcpy(it.prefix, "/");
    it.ino = EXT2_ROOT_INO;

    assert(!ext2fs_open(dev, BLOCK_FLAG_READ_ONLY, 0, 0, unix_io_manager, &it.fs));
    assert(!ext2fs_open_inode_scan(it.fs, 0 /* TODO: adjust */, &it.scanner));

    assert(!ext2fs_dir_iterate2(it.fs, it.ino, 0, it.dirIterateBuffer, dirIterator, &it));

    ext2fs_close_inode_scan(it.scanner);
    assert(!ext2fs_close(it.fs));

    return EXIT_SUCCESS;
}
