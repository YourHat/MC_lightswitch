#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

//define
//-------------------------------------------------------------------------
#define DF_PWMSIG   2
//servo motor SG90 setting
#define DF_MOT_PERIOD_CYCLE     25000   //value of period cycles
#define DF_MOT_CYCLE_TIME      20.00F   //time of one cycle[ms]
#define DF_MOT_DUTY_N90_DEG     0.50F   //-90deg time of high level[ms]
#define DF_MOT_DUTY_N70_DEG     0.71F   //-70deg time of high level[ms]
#define DF_MOT_DUTY_N65_DEG     0.76F   //-65deg time of high level[ms]
#define DF_MOT_DUTY_N60_DEG     0.82F   //-60deg time of high level[ms]
#define DF_MOT_DUTY_N30_DEG     0.8F   //-30deg time of high level[ms]
#define DF_MOT_DUTY_0_DEG       1.45F   //  0deg time of high level[ms]
#define DF_MOT_DUTY_P30_DEG     1.80F   //+30deg time of high level[ms]
#define DF_MOT_DUTY_P60_DEG     2.10F   //+60deg time of high level[ms]
#define DF_MOT_DUTY_P90_DEG     2.40F   //+90deg time of high level[ms]
//-------------------------------------------------------------------------
uint servomotor_sg90_init(uint8_t port);
static bool set_pwm_50Hz(uint port_num);
static uint16_t set_pwm_duty(uint16_t period_cycle,float cycletime,float hightime);
//button define
#define  BUTTON_PIN 16
//-------------------------------------------------------------------------
//function     : main
//return       : ---
//-------------------------------------------------------------------------
int main() {
stdio_init_all();
gpio_init(BUTTON_PIN);
gpio_set_dir(BUTTON_PIN, GPIO_IN);
gpio_pull_up(BUTTON_PIN);
uint portnum = DF_PWMSIG;
uint slice_num;
uint16_t count;
bool swi;
//motor init position(0[deg] open position)
slice_num = servomotor_sg90_init(portnum);
//wait 1[s]
busy_wait_ms(1000);
// Set the PWM stop
pwm_set_enabled(slice_num, false);
swi = false;
while(true){
    //set pwm duty(0[deg])
    if(!gpio_get(BUTTON_PIN)){
        printf("button pressed\n");
        if(swi){
        count = set_pwm_duty(DF_MOT_PERIOD_CYCLE,DF_MOT_CYCLE_TIME,DF_MOT_DUTY_N30_DEG);
        // Find out which PWM slice is connected to GPIO port number (it's slice 0)
        //slice = pwm_gpio_to_slice_num(port);
        // Set channel A or B output high for one cycle before dropping
        if(portnum % 2)
            {   //odd number
        pwm_set_chan_level(slice_num, PWM_CHAN_B, count);
            }
        else
            {   //even number
        pwm_set_chan_level(slice_num, PWM_CHAN_A, count);
            }
        // Set the PWM running
        pwm_set_enabled(slice_num, true);
        //wait
        busy_wait_ms(180);
        // Set the PWM stop
        pwm_set_enabled(slice_num, false);
        swi = false;
        }else{
        //set pwm duty(0[deg])
        count = set_pwm_duty(DF_MOT_PERIOD_CYCLE,DF_MOT_CYCLE_TIME,DF_MOT_DUTY_P30_DEG);
        // Find out which PWM slice is connected to GPIO port number (it's slice 0)
        //slice = pwm_gpio_to_slice_num(port);
        // Set channel A or B output high for one cycle before dropping
        if(portnum % 2)
            {   //odd number
        pwm_set_chan_level(slice_num, PWM_CHAN_B, count);
            }
        else
            {   //even number
        pwm_set_chan_level(slice_num, PWM_CHAN_A, count);
            }
        // Set the PWM running
        pwm_set_enabled(slice_num, true);
        //wait
        busy_wait_ms(180);
        // Set the PWM stop
        pwm_set_enabled(slice_num, false);
        swi = true;
        }
    }
    printf("sleep\n");
    sleep_ms(200);
}




}


//-------------------------------------------------------------------------
//function: servo motor SG90 initial
//          0[deg]position set
//port    : servo motor port number
//return  : slice number
//-------------------------------------------------------------------------
uint servomotor_sg90_init(uint8_t port)
{
uint snum;
uint16_t count;
//motor init position
if( set_pwm_50Hz(port) )
    {
//set pwm duty(0[deg])
count = set_pwm_duty(DF_MOT_PERIOD_CYCLE,DF_MOT_CYCLE_TIME,DF_MOT_DUTY_0_DEG);
// Find out which PWM slice is connected to GPIO port number (it's slice 0)
snum = pwm_gpio_to_slice_num(port);
// Set channel A output high for one cycle before dropping
if( port % 2 )
        {   //odd number
pwm_set_chan_level(snum, PWM_CHAN_B, count);
        }
else
        {   //even number
pwm_set_chan_level(snum, PWM_CHAN_A, count);
        }
// Set the PWM running
pwm_set_enabled(snum, true);
//wait
busy_wait_ms(180);
// Set the PWM stop
pwm_set_enabled(snum, false);
    }
return(snum);
}
//-------------------------------------------------------------------------
//function: pwm free count(50Hz cycle)setting
//port_num: using port number
//return  : true=success/false=fault
//-------------------------------------------------------------------------
static bool set_pwm_50Hz(uint port_num)
{
if(port_num>=30)
    {
return(false);
    }
// Tell GPIO port number this is allocated to the PWM
gpio_set_function(port_num,GPIO_FUNC_PWM);
// Find out which PWM slice is connected to GPIO port number (it's slice 0)
uint slice_num = pwm_gpio_to_slice_num(port_num);
// get default pwm confg
pwm_config cfg = pwm_get_default_config();
// set pwm config modified div mode and div int value
pwm_config_set_clkdiv_mode(&cfg,PWM_DIV_FREE_RUNNING);
pwm_config_set_clkdiv_int(&cfg,100);
pwm_init(slice_num,&cfg,false);
// Set period of DF_MOT_PERIOD_CYCLE (0 to DF_MOT_PERIOD_CYCLE-1 inclusive)
pwm_set_wrap(slice_num, (DF_MOT_PERIOD_CYCLE-1));
// Set channel A or B output high for one cycle before dropping
if(port_num % 2)
    {   //odd number
pwm_set_chan_level(slice_num, PWM_CHAN_B, 0);
    }
else
    {   //even numberf
pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);
    }
return(true);
}
//-------------------------------------------------------------------------
//function     : pwm duty value
//period_cycle : value of period cycles
//cycletime    : time of one cycle [ms]
//hightime     : time of high level[ms]
//return       : level for the selected output
//-------------------------------------------------------------------------
static uint16_t set_pwm_duty(uint16_t period_cycle,float cycletime,float hightime)
{
float count_pms = (float)period_cycle / cycletime * hightime;
return((uint16_t)count_pms);
}
