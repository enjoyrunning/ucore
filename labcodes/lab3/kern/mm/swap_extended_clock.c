#include <defs.h>
#include <x86.h>
#include <stdio.h>
#include <string.h>
#include <swap.h>
#include <swap_extended_clock.h>
#include <list.h>


// #define GET_LIST_ENTRY_PTE(pgdir, le) (get_pte(pgdir, le2page((le), pra_page_link)->pra_vaddr, 0))
// #define GET_DIRTY_FLAG(pgdir, le) (*GET_LIST_ENTRY_PTE((pgdir), (le)) & PTE_D)
// #define GET_ACCESS_FLAG(pgdir, le) (*GET_LIST_ENTRY_PTE((pgdir), (le)) & PTE_A)



static *pte_t _get_list_entry_pte(pte_t *pgdir, list_entry_t *le) {
    struct Page *p = le2page(le, pra_page_link);
    uintptr_t pra_vaddr = p->pra_vaddr;
    pte_t *ptep = get_pte(pgdir, pra_vaddr, 0);
    return ptep;
}

static int _get_dirty_flag(pte_t *pgdir, list_entry_t *le) {
    pte_t *ptep = _get_list_entry_pte(pgdir, le);
    return (*ptep) & PTE_D;
}

static int _get_access_flag(pte_t *pgdir, list_entry_t *le) {
    pte_t *ptep = _get_list_entry_pte(pgdir, le);
    return (*ptep) & PTE_A;
}

static int
_extended_clock_init_mm(struct mm_struct *mm)
{   
    mm->sm_priv = NULL;
    return 0;
}
/*
 * (3)_extended_clock_map_swappable: According FIFO PRA, we should link the most recent arrival page at the back of pra_list_head qeueue
 */
static int
_extended_clock_map_swappable(struct mm_struct *mm, uintptr_t addr, struct Page *page, int swap_in)
{
    list_entry_t *head = (list_entry_t *)mm->sm_priv;
    list_entry_t *entry = &(page->pra_page_link);

    assert(entry != NULL);

    if (head == NULL) {
        list_init(entry);
        mm->sm_priv = entry;
    } else {
        list_add_before(head, entry);
    }

    return 0;
}
/*
 *  (4)_extended_clock_swap_out_victim: According FIFO PRA, we should unlink the  earliest arrival page in front of pra_list_head qeueue,
 *                            then set the addr of addr of this page to ptr_page.
 */
static int
_extended_clock_swap_out_victim(struct mm_struct *mm, struct Page ** ptr_page, int in_tick)
{

    
    return 0;
}

static int
_extended_clock_check_swap(void) {
   cprintf("write Virt Page c in fifo_check_swap\n");
    *(unsigned char *)0x3000 = 0x0c;
    assert(pgfault_num==4);
    cprintf("write Virt Page a in fifo_check_swap\n");
    *(unsigned char *)0x1000 = 0x0a;
    assert(pgfault_num==4);
    cprintf("write Virt Page d in fifo_check_swap\n");
    *(unsigned char *)0x4000 = 0x0d;
    assert(pgfault_num==4);
    cprintf("write Virt Page b in fifo_check_swap\n");
    *(unsigned char *)0x2000 = 0x0b;
    assert(pgfault_num==4);
    cprintf("write Virt Page e in fifo_check_swap\n");
    *(unsigned char *)0x5000 = 0x0e;
    assert(pgfault_num==5);
    cprintf("write Virt Page b in fifo_check_swap\n");
    *(unsigned char *)0x2000 = 0x0b;
    assert(pgfault_num==5);
    cprintf("write Virt Page a in fifo_check_swap\n");
    *(unsigned char *)0x1000 = 0x0a;
    assert(pgfault_num==6);
    cprintf("write Virt Page b in fifo_check_swap\n");
    *(unsigned char *)0x2000 = 0x0b;
    assert(pgfault_num==7);
    cprintf("write Virt Page c in fifo_check_swap\n");
    *(unsigned char *)0x3000 = 0x0c;
    assert(pgfault_num==8);
    cprintf("write Virt Page d in fifo_check_swap\n");
    *(unsigned char *)0x4000 = 0x0d;
    assert(pgfault_num==9);
    cprintf("write Virt Page e in fifo_check_swap\n");
    *(unsigned char *)0x5000 = 0x0e;
    assert(pgfault_num==10);
    cprintf("write Virt Page a in fifo_check_swap\n");
    assert(*(unsigned char *)0x1000 == 0x0a);
    *(unsigned char *)0x1000 = 0x0a;
    assert(pgfault_num==11);
    return 0;
}


static int
_extended_clock_init(void)
{
    return 0;
}

static int
_extended_clock_set_unswappable(struct mm_struct *mm, uintptr_t addr)
{
    return 0;
}

static int
_extended_clock_tick_event(struct mm_struct *mm)
{ return 0; }


struct swap_manager swap_manager_extended_clock =
{
     .name            = "extended clock swap manager",
     .init            = &_extended_clock_init,
     .init_mm         = &_extended_clock_init_mm,
     .tick_event      = &_extended_clock_tick_event,
     .map_swappable   = &_extended_clock_map_swappable,
     .set_unswappable = &_extended_clock_set_unswappable,
     .swap_out_victim = &_extended_clock_swap_out_victim,
     .check_swap      = &_extended_clock_check_swap,
};
