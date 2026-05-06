#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"

float __not_in_flash_func(readtemp)(int option){
    uint32_t read=0;
    const int samples = 64;

    for(int i=0;i<samples;i++){
        read+=adc_read();
        busy_wait_us(5);
    }

    float raw=(float)read/samples;
    float vol=raw*(3.3f/4095.0f);
    float temp=27.0f-((vol-0.706f)/0.001721f);

    if(option){
        return temp;
    }else if(!option){
        return raw;
    }
}

bool __not_in_flash_func(loopTimer)(struct repeating_timer *t){
    float tempIn=readtemp(1);
    float raw=readtemp(0);
    uint32_t clock=clock_get_hz(clk_sys)/1000000;

    printf("Bruto: %f | Temp: %.2f C | Clock: %lu Mhz\n",raw,tempIn,(unsigned long)clock);

    return true;
}

int main() {
    vreg_set_voltage(VREG_VOLTAGE_1_25);
    sleep_ms(10);
    set_sys_clock_khz(288000,true);

    stdio_init_all();
    sleep_ms(2000);
    printf("%lu Mhz \n",clock_get_hz(clk_sys)/1000000);

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    struct repeating_timer timer;

    add_repeating_timer_ms(1000,loopTimer,NULL,&timer);

    sleep_ms(10); 
    
    while(true) {
        tight_loop_contents();
    }
}