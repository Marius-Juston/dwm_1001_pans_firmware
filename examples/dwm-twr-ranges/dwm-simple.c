/**
 * LEAPS - Low Energy Accurate Positioning System.
 *
 * Simple user application.
 *
 * Copyright (c) 2016-2019, LEAPS. All rights reserved.
 *
 */

#include "dwm.h"
#include <stdio.h>

/* Thread priority */
#ifndef THREAD_APP_PRIO
#define THREAD_APP_PRIO	20
#endif /* THREAD_APP_PRIO */

/* Thread stack size */
#ifndef THREAD_APP_STACK_SIZE
#define THREAD_APP_STACK_SIZE	(3 * 1024)
#endif /* THREAD_APP_STACK_SIZE */

#define APP_ERR_CHECK(err_code)	\
do {							\
	if ((err_code) != DWM_OK)	\
		printf("err: line(%u) code(%u)", __LINE__, (err_code));\
} while (0)						\

/**
 * Event callback
 *
 * @param[in] p_evt  Pointer to event structure
 */
void on_dwm_evt(dwm_evt_t *p_evt)
{
	int len;
	int i;
        int n;

	switch (p_evt->header.id) {
	/* New location data */
	case DWM_EVT_LOC_READY:
          n = p_evt->loc.anchors.dist.cnt; 

          printf("CNT,%u", n);

		for (i = 0; i < n; ++i) {
                       if(i > 0)
                       printf(",");

			printf("%04X,", (unsigned int)(p_evt->loc.anchors.dist.addr[i] & 0xffff));
                      

			if (i < p_evt->loc.anchors.an_pos.cnt) {
				printf("%ld,%ld,%ld,",
						p_evt->loc.anchors.an_pos.pos[i].x,
						p_evt->loc.anchors.an_pos.pos[i].y,
						p_evt->loc.anchors.an_pos.pos[i].z);
			}

			printf("%lu", p_evt->loc.anchors.dist.dist[i]);
		}
		printf("\n");
		break;
	default:
		break;
	}
}


void config_tag(void){
	int rv;
	dwm_cfg_t cfg;
	dwm_cfg_tag_t  cfg_tag;

	/* Get node configuration */
	APP_ERR_CHECK(dwm_cfg_get(&cfg));

	/* Configure device as TAG */
	cfg_tag.stnry_en = false;
	cfg_tag.loc_engine_en = false;
	cfg_tag.low_power_en = false;
	cfg_tag.meas_mode = DWM_MEAS_MODE_TWR;
	cfg_tag.common.fw_update_en = false;
	cfg_tag.common.uwb_mode = DWM_UWB_MODE_ACTIVE;
	cfg_tag.common.ble_en = false;
	cfg_tag.common.led_en = true;
	cfg_tag.common.enc_en = false;

	if ((cfg.mode != DWM_MODE_TAG) ||
	(cfg.stnry_en != cfg_tag.stnry_en) ||
	(cfg.loc_engine_en != cfg_tag.loc_engine_en) ||
	(cfg.low_power_en != cfg_tag.low_power_en) ||
	(cfg.meas_mode != cfg_tag.meas_mode) ||
	(cfg.common.fw_update_en != cfg_tag.common.fw_update_en) ||
	(cfg.common.uwb_mode != cfg_tag.common.uwb_mode) ||
	(cfg.common.ble_en != cfg_tag.common.ble_en) ||
	(cfg.common.led_en != cfg_tag.common.led_en)) {

		if(cfg.mode 			!= DWM_MODE_TAG) 		printf("mode: get = %d, set = %d\n", cfg.mode, 		DWM_MODE_ANCHOR);
		if(cfg.stnry_en     		!= cfg_tag.stnry_en)  		printf("acce: get = %d, set = %d\n", cfg.stnry_en, 	cfg_tag.stnry_en);
		if(cfg.loc_engine_en 		!= cfg_tag.loc_engine_en) 	printf("le  : get = %d, set = %d\n", cfg.loc_engine_en, cfg_tag.loc_engine_en);
		if(cfg.low_power_en		!= cfg_tag.low_power_en)	printf("lp  : get = %d, set = %d\n", cfg.low_power_en, 	cfg_tag.low_power_en);
		if(cfg.meas_mode 		!= cfg_tag.meas_mode) 		printf("meas: get = %d, set = %d\n", cfg.meas_mode, 	cfg_tag.meas_mode);
		if(cfg.common.fw_update_en 	!= cfg_tag.common.fw_update_en)	
										printf("fwup: get = %d, set = %d\n", cfg.common.fw_update_en, cfg_tag.common.fw_update_en);
		if(cfg.common.uwb_mode		!= cfg_tag.common.uwb_mode)	printf("uwb : get = %d, set = %d\n", cfg.common.uwb_mode, cfg_tag.common.uwb_mode);
		if(cfg.common.ble_en 		!= cfg_tag.common.ble_en)	printf("ble : get = %d, set = %d\n", cfg.common.ble_en, cfg_tag.common.ble_en);
		if(cfg.common.enc_en 		!= cfg_tag.common.enc_en)	printf("enc : get = %d, set = %d\n", cfg.common.enc_en, cfg_tag.common.enc_en);
		if(cfg.common.led_en 		!= cfg_tag.common.led_en)	printf("led : get = %d, set = %d\n", cfg.common.led_en, cfg_tag.common.led_en);

		APP_ERR_CHECK(rv = dwm_cfg_tag_set(&cfg_tag));

		dwm_reset();
	}  
}

/**
 * Application thread
 *
 * @param[in] data  Pointer to user data
 */
void app_thread_entry(uint32_t data)
{
	dwm_evt_t evt;
	int rv;
        config_tag();


	/* Update rate set to 1 second, stationary update rate set to 5 seconds */
	APP_ERR_CHECK(dwm_upd_rate_set(1, 1));

	/* Register event callback */
	dwm_evt_listener_register(
			DWM_EVT_LOC_READY, NULL);


        
        dwm_loc_data_t loc;
        int i;

        volatile uint8_t* const _meas_cnt = (volatile uint8_t*) 0x200070E0;
        

        while(1){
        *_meas_cnt = 5;
        printf("%d", *_meas_cnt);
          //*_meas_cnt = 5;
          /* if pos_available is false, position data are not read and function returns without error */
          rv = dwm_loc_get(&loc);

          if (0 == rv) {
          if (loc.pos_available) {
          printf("[%ld,%ld,%ld,%u] ", loc.pos.x, loc.pos.y, loc.pos.z,
          loc.pos.qf);
          }
          for (i = 0; i < loc.anchors.dist.cnt; ++i) {
          printf("%u)", i);
          printf("0x%04x", loc.anchors.dist.addr[i]);
          if (i < loc.anchors.an_pos.cnt) {
          printf("[%ld,%ld,%ld,%u]", loc.anchors.an_pos.pos[i].x,
          loc.anchors.an_pos.pos[i].y,
          loc.anchors.an_pos.pos[i].z,
          loc.anchors.an_pos.pos[i].qf);
          }
          printf("=%lu,%u ", loc.anchors.dist.dist[i], loc.anchors.dist.qf[i]);
          }
          printf("\n");
          } else {
          printf("err code: %d\n", rv);
          }

        
        }


	//while (1) {
	//	/* Thread loop */
 //           rv = dwm_evt_wait(&evt);

 //           if (rv == DWM_OK) {
 //                   on_dwm_evt(&evt);
 //           }
	//}
}

/**
 * Application entry point. Initialize application thread.
 *
 * @warning ONLY ENABLING OF LOCATION ENGINE OR BLE AND CREATION AND STARTING OF
 * USER THREADS CAN BE DONE IN THIS FUNCTION
 */
void dwm_user_start(void)
{
	uint8_t hndl;
	int rv;

	dwm_shell_compile();
	//Disabling ble by default as softdevice prevents debugging with breakpoints (due to priority)
	//dwm_ble_compile();
	dwm_le_compile();
	dwm_serial_spi_compile();

	/* Create thread */
	rv = dwm_thread_create(THREAD_APP_PRIO, app_thread_entry, (void*)NULL,
			"app", THREAD_APP_STACK_SIZE, &hndl);
	APP_ERR_CHECK(rv);

	/* Start the thread */
	dwm_thread_resume(hndl);
}