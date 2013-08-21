
/**
 * TODO: add logger
 * TODO: add options
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include <ext2fs/ext2fs.h>


int dirIterator(struct ext2_dir_entry *dirent,
                int offset,
                int blocksize,
                char *buf,
                void *priv_data)
{
    printf("%s\n", dirent->name);
    return 0;
}

void readDirs(ext2_filsys fs)
{
    ext2_inode_scan scanner;

    assert(!ext2fs_open_inode_scan(fs, 0 /* TODO: adjust */, &scanner));

    ext2_ino_t ino;
    struct ext2_inode inode;
    while (!ext2fs_get_next_inode(scanner, &ino, &inode)) {
        if (!ext2fs_check_directory(fs, ino)) {
            break;
        }
    }

    char buffer[PATH_MAX];
    assert(!ext2fs_dir_iterate(fs, ino, 0, buffer, dirIterator, NULL));

    ext2fs_close_inode_scan(scanner);
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s block_device\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *dev = argv[1];

    ext2_filsys fs;
    assert(!ext2fs_open(dev, BLOCK_FLAG_READ_ONLY, 0, 0, unix_io_manager, &fs));

    readDirs(fs);

    assert(!ext2fs_close(fs));

    return EXIT_SUCCESS;
}
