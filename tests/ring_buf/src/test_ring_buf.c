/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	int ret;
	/* TODO(l8-task1): Push a value, call rb_init(4) again, then
	 * verify the buffer is empty and count is 0.
	 * See TEST_SPEC.md "Suite ring_buf_init" #2.
	 */
	ret = rb_push(99);
	zassert_equal(ret, 0, "push should be successful");
	zassert_equal(rb_init(4), 0, "reset should succeed");
	zassert_true(rb_is_empty(), "buffer must be empty post reinit");
	zassert_equal(rb_count(), 0, " buffer count must be 0 reinit");

}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	int v;
	/* TODO(l8-task1): rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	 */
	zassert_equal(rb_push(42), 0, "push should succeed");
	zassert_equal(rb_pop(&v), 0, "pop should succeed");
	zexpect_equal(v, 42, "popped value should be pushed value");
	zassert_true(rb_is_empty(), "buffer must be empty after popping only element");
	zexpect_equal(rb_count(), 0, " buffer count must be 0 after popping only element");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */
	const int check_count = 3;
	int v;
	for (int i = 1; i <= check_count; i++) {
		zassert_equal(rb_push(i), 0, "push must succeed");
	}
	for (int i = 1; i <= check_count; i++) {
		zassert_equal(rb_pop(&v), 0, "pop must succeed");
		zassert_equal(v, i, "order should match push (FIFO)");
	}
	zassert_true(rb_is_empty(), "buffer must be empty after popping all three");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* TODO(l8-task1): Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */
	const int capacity = 4;
	for (int i = 1; i <= capacity; i++) {
		zassert_equal(rb_push(i), 0, "push must succeed");
	}
	zassert_true(rb_is_full(), "buffer full after pushing 4");

	zassert_equal(rb_push(99), -ENOSPC, "push over capacity should fail");

	zexpect_equal(rb_count(), 4, "failed push should not modify count");

}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	/* TODO(l8-task1): rb_push(7); rb_peek(&v) -> v == 7; rb_peek(&v) again
	 * -> v == 7; rb_count() still == 1.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #1.
	 */
	int v;
	zassert_equal(rb_push(7), 0, "push must succeed");
	zassert_equal(rb_peek(&v), 0, "peek must succeed");
	zexpect_equal(v, 7, "peeked value should be what was pushed");
	zassert_equal(rb_peek(&v), 0, "2nd peek must succeed");
	zexpect_equal(v, 7, "peeked value should be what was pushed after second peek");
	zexpect_equal(rb_count(), 1, "peeking should not remove or add elements");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* TODO(l8-task1): rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */
	zassert_equal(rb_pop(NULL), -EINVAL, "popping to bad pointer should not succeed");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* TODO(l8-task1): push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */
	const int capacity = 4;
	for (int i = 1; i <= capacity; i++) {
		zassert_equal(rb_push(i), 0, "push must succeed");
	}
	zassert_true(rb_is_full(), "buffer should be full after pushing 4");
	zexpect_equal(rb_count(), capacity, "count should equal number pushed");
}
