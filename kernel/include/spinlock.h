/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Spin Lock Definition
 * Explorer/include/spinlock.h
 * version:Alpha
 * 12/5/2014 11:47 PM
 */

#ifndef SPIN_LOCK_H_

	typedef unsigned long spinlock_t;

	#define SPIN_LOCK_LOCKED	0
	#define SPIN_LOCK_UNLOCKED	1
	
	void spin_lock_init(spinlock_t *lock);
	
	void spin_lock(spinlock_t *lock);
	void spin_lock_irqsave(spinlock_t *lock, unsigned long flags);
	void spin_lock_irq(spinlock_t *lock);
	void spin_lock_bh(spinlock_t *lock);

	void spin_unlock(spinlock_t *lock);
	void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags);
	void spin_unlock_irq(spinlock_t *lock);
	void spin_unlock_bh(spinlock_t *lock);

#define SPIN_LOCK_H_
#endif