#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"

int main() {
    vreg_set_voltage(VREG_VOLTAGE_1_20);
    sleep_ms(10);
    set_sys_clock_khz(250000,true);

    stdio_init_all();
    sleep_ms(2000);
    printf("%lu Mhz \n",clock_get_hz(clk_sys)/1000000);

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    sleep_ms(10); 
    
    while(true) {
        uint16_t raw=adc_read();
        float vol=raw*(3.3f/4095.0f);
        float temp=27-((vol-0.706f)/0.001721f);
        uint32_t clock=clock_get_hz(clk_sys)/1000000;

        printf("Bruto: %u | Temp: %.2f C | Clock: %lu Mhz\n",raw,temp,(unsigned long)clock);

        sleep_ms(1000);
    }
}