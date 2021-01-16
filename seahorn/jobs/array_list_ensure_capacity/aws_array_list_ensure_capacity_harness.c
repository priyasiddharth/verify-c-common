/*
 *
 */

#include <seahorn/seahorn.h>
#include <aws/common/array_list.h>
#include <array_list_helper.h>
#include <utils.h>

/**
 * Runtime: 9s
 */
int main() {
    /* data structure */
    struct aws_array_list list;
    initialize_bounded_array_list(&list);

    /* assumptions */
    assume(aws_array_list_is_valid(&list));

    /* save current state of the data structure */
    struct aws_array_list old = list;
    struct store_byte_from_buffer old_byte;
    save_byte_from_array((uint8_t *)list.data, list.current_size, &old_byte);

    /* perform operation under verification */
    size_t index = nd_size_t();
    KLEE_ASSUME(index <= KLEE_MAX_SIZE);
    if (!aws_array_list_ensure_capacity(&list, index)) {
        /* assertions */
        sassert(aws_array_list_is_valid(&list));
        sassert(list.item_size == old.item_size);
        sassert(list.alloc == old.alloc);
        sassert(list.length == old.length);
        sassert(list.current_size >= old.current_size);
    } else {
        /* In the case aws_array_list_ensure_capacity is not successful, the list must not change */
        assert_array_list_equivalence(&list, &old, &old_byte);
    }

    return 0;
}
