/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Page Definition
 * Explorer/include/page.h
 * version:Alpha
 * 12/21/2014 9:43 PM
 */

#ifndef PAGE_H_
#define PAGE_H_

#define PAGE_SHIFT 12
#define HEXAGON_L1_PTE_SIZE __HVM_PDE_S_4KB

#define PAGE_SIZE  (1UL << PAGE_SHIFT)
#define PAGE_MASK  (~((1 << PAGE_SHIFT) - 1))

#endif
