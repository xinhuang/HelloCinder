#include <gtest/gtest.h>

#include "../LifeGame/Universe.h"

TEST(AvxUniverse, cell_with_no_neighbor_should_die) {
	CpuAvxUniverse sut(8, 8);
	uint8_t src[] = { 0, 0, 0,
		0, 1, 0,
		0, 0, 0, };
	uint8_t dst[sizeof(src)] = { 0 };
	uint8_t expect[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, };

	sut.next(3, 3, src, 3, 1, dst, 3, 1);

	ASSERT_EQ(0, memcmp(expect, dst, sizeof(dst)));
}

TEST(AvxUniverse, cell_with_2_neighbors_should_survive_and_3_should_give_birth) {
	CpuAvxUniverse sut(8, 8);
	uint8_t src[] = { 0, 0, 0, 0, 0,
		              0, 0, 0, 0, 0,
				      0, 0xFF, 0xFF, 0xFF, 0,
					  0, 0, 0, 0, 0,
					  0, 0, 0, 0, 0, };
	uint8_t dst[sizeof(src)] = { 0 };
	uint8_t expect[] = { 0, 0, 0, 0, 0,
		                 0, 0, 0xFF, 0, 0,
		                 0, 0, 0xFF, 0, 0,
						 0, 0, 0xFF, 0, 0,
						 0, 0, 0, 0, 0,
	};

	sut.next(5, 5, src, 5, 1, dst, 5, 1);

	ASSERT_EQ(0, memcmp(expect, dst, sizeof(dst)));
}

TEST(AvxUniverse, cell_with_4_neighbors_should_die) {
	CpuAvxUniverse sut(8, 8);
	uint8_t src[] = { 0, 0, 0, 0, 0,
		              0, 0, 0xFF, 0, 0,
				      0, 0xFF, 0xFF, 0xFF, 0,
					  0, 0, 0xFF, 0, 0,
					  0, 0, 0, 0, 0, };
	uint8_t dst[sizeof(src)] = { 0 };
	uint8_t expect[] = { 0, 0, 0, 0, 0,
		                 0, 0xFF, 0xFF, 0xFF, 0,
		                 0, 0xFF, 0, 0xFF, 0,
						 0, 0xFF, 0xFF, 0xFF, 0,
						 0, 0, 0, 0, 0,
	};

	sut.next(5, 5, src, 5, 1, dst, 5, 1);

	ASSERT_EQ(0, memcmp(expect, dst, sizeof(dst)));
}
