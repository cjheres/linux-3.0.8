/*
 *  Copyright (C) 2002 ARM Ltd.
 *  Copyright (C) 2010 Marvell International Ltd.
 *
 *  This file is based on arm vexpress platform
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/clockchips.h>

#include <asm/smp_twd.h>
#include <asm/localtimer.h>
#include <mach/irqs.h>

/*
 * Setup the local clock events for a CPU.
 */
int __cpuinit local_timer_setup(struct clock_event_device *evt)
{
	evt->irq = IRQ_LOCALTIMER;
	twd_timer_setup(evt);
}
