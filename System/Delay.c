#include "ti_msp_dl_config.h"

void Delay_us(uint32_t us)
{
    delay_cycles(80*us);
}

void Delay_ms(uint32_t ms)
{
	delay_cycles(80000*ms);
}
