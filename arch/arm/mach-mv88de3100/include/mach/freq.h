#ifndef _FREQ_H_
#define _FREQ_H_

typedef struct SOC_FREQ_VECTOR_T_LINUX{
    unsigned long cpu0;
    unsigned long cfg;
} SOC_FREQ_VECTOR_Linux;

extern SOC_FREQ_VECTOR_Linux socFreqVec;

#ifdef CONFIG_MV88DE3100_DYNAMIC_FREQ
extern void query_board_freq(SOC_FREQ_VECTOR_Linux *freq);
#else
#define query_board_freq(freq) do {} while(0)
#endif

#endif
