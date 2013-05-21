#include <linux/module.h>
#include <mach/freq.h>

#include "Galois_memmap.h"
#include "global.h"
#include "galois_io.h"
#include "galois_speed.h"

SOC_FREQ_VECTOR_Linux socFreqVec;

void query_board_freq(SOC_FREQ_VECTOR_Linux *freq)
{
	freq->cpu0 = GaloisGetFrequency(SOC_FREQ_CPU0) * 1000000;
	freq->cfg = GaloisGetFrequency(SOC_FREQ_CFG) * 1000000;
}
