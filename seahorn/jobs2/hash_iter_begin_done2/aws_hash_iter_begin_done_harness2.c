/**
 */

#include <aws/common/hash_table.h>
#include <aws/common/private/hash_table_impl.h>

#include <seahorn/seahorn.h>

#include <config.h>
#include <hash_table_helper.h>
#include <proof_allocators.h>
#include <utils.h>

int main(void) {
  /* parameters */
  struct aws_hash_table map;
  initialize_bounded_aws_hash_table(&map, MAX_TABLE_SIZE);

  /* assumptions */
  assume(aws_hash_table_is_valid(&map));
  assume((map.p_impl->entry_count == 0) == aws_hash_table_deep_is_empty(&map));

  /* save current state */
  sea_tracking_on();
  sea_reset_modified((char *)&map);
  sea_reset_modified((char *)map.p_impl);

  /* operation under verification */
  struct aws_hash_iter iter = aws_hash_iter_begin(&map);

  /* assertions */
  sassert(aws_hash_iter_is_valid(&iter));
  sassert(iter.status == AWS_HASH_ITER_STATUS_DONE ||
          iter.status == AWS_HASH_ITER_STATUS_READY_FOR_USE);
    /* quote header file comment:
    "...This function cannot fail, but if there are no elements in the table,
    the returned iterator will return true for aws_hash_iter_done(&iter)."
    */
  if (map.p_impl->entry_count == 0) {
    sassert(aws_hash_iter_done(&iter));
  }
  sassert(aws_hash_table_is_valid(&map));
  sassert(!sea_is_modified((char *)&map));
  sassert(!sea_is_modified((char *)map.p_impl));
  return 0;
}
