
/**
 * TODO: add logger
 * TODO: add options
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include <ext2fs/ext2fs.h>


void readDirs(ext2_filsys fs)
{
    ext2_inode_scan scanner;

    assert(!ext2fs_open_inode_scan(fs, 0 /* TODO: adjust */, &scanner));

    ext2_ino_t ino;
    struct ext2_inode inode;
    char *buffer;
    while (!ext2fs_get_next_inode(scanner, &ino, &inode)) {
        /**
         * TODO: by some reason this check is not correct
         */
        if (ext2fs_check_directory(fs, ino) == ENOTDIR) {
            continue;
        }

        ext2fs_get_pathname(fs, ino, 0, &buffer);
        /**
         * Skip non-dirs here
         */
        if (buffer[0] != '/') {
            continue;
        }

        printf("%s\n", buffer);
    }

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
