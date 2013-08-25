
/**
 * TODO: add logger
 * TODO: add options
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include <ext2fs/ext2fs.h>


struct DirIterate
{
    ext2_filsys fs;
    ext2_inode_scan scanner;

    char prefix[PATH_MAX];
    char name[PATH_MAX];
};


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

    return 0;
}

void readDirs(struct DirIterate *it)
{
    ext2_ino_t ino;
    struct ext2_inode inode;
    while (!ext2fs_get_next_inode(it->scanner, &ino, &inode)) {
        if (ext2fs_check_directory(it->fs, ino)) {
            continue;
        }

        char buffer[PATH_MAX];
        assert(!ext2fs_dir_iterate2(it->fs, ino, 0, buffer, dirIterator, it));
    }
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s block_device\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *dev = argv[1];

    struct DirIterate it = {
        .prefix = "/",
        .name = ""
    };
    assert(!ext2fs_open(dev, BLOCK_FLAG_READ_ONLY, 0, 0, unix_io_manager, &it.fs));
    assert(!ext2fs_open_inode_scan(it.fs, 0 /* TODO: adjust */, &it.scanner));

    readDirs(&it);

    ext2fs_close_inode_scan(it.scanner);
    assert(!ext2fs_close(it.fs));

    return EXIT_SUCCESS;
}
