#include <swap.h>
#include <swapfs.h>
#include <mmu.h>
#include <fs.h>
#include <ide.h>
#include <pmm.h>
#include <assert.h>

void
swapfs_init(void) {
    static_assert((PGSIZE % SECTSIZE) == 0);
    if (!ide_device_valid(SWAP_DEV_NO)) {
        panic("swap fs isn't available.\n");
    }
    max_swap_offset = ide_device_size(SWAP_DEV_NO) / (PGSIZE / SECTSIZE);
}

/*
 * According to the PTE, read a page from swap space.
 * typedef pte_t swap_entry_t;
 */
int
swapfs_read(swap_entry_t entry, struct Page *page) {
    if ((entry & PTE_P != 0x0) || ((entry >> 0x8) == 0x0)) {
        panic("swapfs_read error: first argument \"entry\" is invalid\n");
        return -1;
    }
    return ide_read_secs(SWAP_DEV_NO, swap_offset(entry) * PAGE_NSECT, page2kva(page), PAGE_NSECT);
}

// write to swap space
int
swapfs_write(swap_entry_t entry, struct Page *page) {
    if ((entry & PTE_P != 0x0) || ((entry >> 0x8) == 0x0)) {
        panic("swapfs_write error: first argument \"entry\" is invalid\n");
        return (-1);
    }
    return ide_write_secs(SWAP_DEV_NO, swap_offset(entry) * PAGE_NSECT, page2kva(page), PAGE_NSECT);
}

