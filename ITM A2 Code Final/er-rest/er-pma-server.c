/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      Erbium (Er) REST Engine example.
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "rest-engine.h"

//#include "ioc.h"

#include "ti-lib.h"
#include "driverlib/aux_adc.h"
#include "driverlib/aux_wuc.h"


#if PLATFORM_HAS_BUTTON
#include "dev/button-sensor.h"
#endif

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF("[%02x:%02x:%02x:%02x:%02x:%02x]", (lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3], (lladdr)->addr[4], (lladdr)->addr[5])
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif

/*
 * Resources to be activated need to be imported through the extern keyword.
 * The build system automatically compiles the resources in the corresponding sub-directory.
 */
extern resource_t
  res_hello,
  res_adc,
  res_mirror,
  res_chunks,
  res_separate,
  res_push,
  res_event,
  res_sub,
  res_b1_sep_b2;
#if PLATFORM_HAS_LEDS
extern resource_t res_leds, res_toggle;
#endif

extern int32_t temp_setpoint;

int16_t getTemperatureSetpoint(void);


/*DP Added */
PROCESS(adc_process, "ADC process");


PROCESS(er_example_server, "Erbium Example Server");
AUTOSTART_PROCESSES(&er_example_server);

PROCESS_THREAD(er_example_server, ev, data)
{
  PROCESS_BEGIN();

  PROCESS_PAUSE();

  PRINTF("Starting Erbium Example Server\n");

#ifdef RF_CHANNEL
  PRINTF("RF channel: %u\n", RF_CHANNEL);
#endif
#ifdef IEEE802154_PANID
  PRINTF("PAN ID: 0x%04X\n", IEEE802154_PANID);
#endif

  PRINTF("uIP buffer: %u\n", UIP_BUFSIZE);
  PRINTF("LL header: %u\n", UIP_LLH_LEN);
  PRINTF("IP+UDP header: %u\n", UIP_IPUDPH_LEN);
  PRINTF("REST max chunk: %u\n", REST_MAX_CHUNK_SIZE);

/* Initialize the REST engine. */
/*  rest_activate_resource(&res_chunks, "test/chunks"); */
/*  rest_activate_resource(&res_separate, "test/separate"); */
/*  rest_activate_resource(&res_push, "test/push");*/
 
/*  rest_activate_resource(&res_sub, "test/sub"); */

  rest_init_engine();

  rest_activate_resource(&res_hello, "Device/Manufacturer");
  rest_activate_resource(&res_toggle, "Device/Identify");
  
  rest_activate_resource(&res_event, "Temperature/Set Point"); 
  rest_activate_resource(&res_adc, "Temperature/Current");

  rest_activate_resource(&res_leds, "Heating/On or Off");
  
  
  // Configure GPIO pins DIO18-22 for User Interface LEDs
  // Set initial state to Off
  for(int pinNumber = 18; pinNumber < 23; pinNumber++){
	  ti_lib_ioc_pin_type_gpio_output(pinNumber);
	  ti_lib_gpio_clear_dio(pinNumber);
	  }
  
  
  process_start(&adc_process, NULL);


  while(1) {
    PROCESS_WAIT_EVENT();
#if PLATFORM_HAS_BUTTON
    if(ev == sensors_event && data == &button_sensor) {
      printf("*******BUTTON*******\n");

      /* Call the event_handler for this application-specific event. */
      res_event.trigger();

      
      printf("Set Point Temp %d", getTemperatureSetpoint());
	  
	  
	  // User Interface LEDs for heating demand
      if(getTemperatureSetpoint()>=17 && getTemperatureSetpoint() < 19){
		  ti_lib_gpio_set_dio(18);
	  }
      else if(getTemperatureSetpoint()>=19 && getTemperatureSetpoint() < 21){
		  ti_lib_gpio_set_dio(19);
	  }
      else if(getTemperatureSetpoint()>=21 && getTemperatureSetpoint() < 23){
		  ti_lib_gpio_set_dio(20);
	  }
      else if(getTemperatureSetpoint()>=23 && getTemperatureSetpoint() < 25){
		  ti_lib_gpio_set_dio(21);
	  }
	  else if(getTemperatureSetpoint()== 25){
		  ti_lib_gpio_set_dio(22);
	  }
	  else{
		  for(int pinNumber = 18; pinNumber < 23; pinNumber++){
			  ti_lib_gpio_clear_dio(pinNumber);
		  }
	  }


      /* Also call the separate response example handler. */
      res_separate.resume();
    }
#endif /* PLATFORM_HAS_BUTTON */
  }                             /* while (1) */

  PROCESS_END();
}


uint16_t singleSample;
uint16_t temperatureC;

PROCESS_THREAD(adc_process, ev, data)
{
  PROCESS_BEGIN();
  static struct etimer et_adc;
  while(1)
  {
         etimer_set(&et_adc, CLOCK_SECOND*1);
         PROCESS_WAIT_EVENT();
         if(etimer_expired(&et_adc)) {
               //intialisation of ADC
               ti_lib_aon_wuc_aux_wakeup_event(AONWUC_AUX_WAKEUP);
               while(!(ti_lib_aon_wuc_power_status_get() & AONWUC_AUX_POWER_ON))
               { }

               // Enable clock for ADC digital and analog interface (not currently enabled in driver)
               // Enable clocks
               ti_lib_aux_wuc_clock_enable(AUX_WUC_ADI_CLOCK | AUX_WUC_ANAIF_CLOCK | AUX_WUC_SMPH_CLOCK);
               while(ti_lib_aux_wuc_clock_status(AUX_WUC_ADI_CLOCK | AUX_WUC_ANAIF_CLOCK | AUX_WUC_SMPH_CLOCK) != AUX_WUC_CLOCK_READY)
               { }
               //printf("clock selected\r\n");

               // Connect AUX IO7 (DIO23, but also DP2 on XDS110) as analog input.
               AUXADCSelectInput(ADC_COMPB_IN_AUXIO7);
               //printf("input selected\r\n");

               // Set up ADC range
               // AUXADC_REF_FIXED = nominally 4.3 V
               AUXADCEnableSync(AUXADC_REF_FIXED,  AUXADC_SAMPLE_TIME_2P7_US, AUXADC_TRIGGER_MANUAL);
               //printf("init adc --- OK\r\n");

               //Trigger ADC converting
               AUXADCGenManualTrigger();
               //printf("trigger --- OK\r\n");

               //reading adc value
               singleSample = AUXADCReadFifo();

               printf("%d mV on ADC\r\n",singleSample);
			   
			   //Convert ADC value to temperature
			   temperatureC = (singleSample - 500) / 10;
			   printf("%d Current Temp C\r\n",temperatureC);

               //shut the adc down
               AUXADCDisable();
               //printf("disable --- OK\r\n");
               //get_batmon_reading(NULL);
			   
			   //dp if set up as obsever, we need to notify!
			   //res_adc.trigger();
			   
			   /* Call the event_handler for this application-specific event. */
			   //res_adc.trigger();
			   
			   //printf("Desired temp %d", getTemperatureC());
			   
			   /* Also call the separate response example handler. */
			   //res_separate.resume();

               etimer_reset(&et_adc);
               }
  }
  PROCESS_END();
}
