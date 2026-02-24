/***************************************************************************
 * File Name        : Smart_Room_Control_config.h
 *
 * Description      : Configuration header for WWD and NLU.
 *
 * NOTE: This is a PLACEHOLDER file. Replace with the auto-generated file
 * from the DEEPCRAFT Voice Assistant cloud tool when your model is ready.
 *******************************************************************************/
#ifndef PROJ_CM55_INC_SMART_ROOM_CONTROL_CONFIG_H_
#define PROJ_CM55_INC_SMART_ROOM_CONTROL_CONFIG_H_

#include "mtb_wwd_nlu_common.h"

#define SMART_ROOM_CONTROL_NO_MAX_WAKE_WORD 8
#define SMART_ROOM_CONTROL_NO_OF_WAKE_WORD 1

#define SMART_ROOM_CONTROL_WAKE_WORD_1 0x01

#define SMART_ROOM_CONTROL_ALL_WAKE_WORD 0x01
void Smart_Room_Control_wake_word_callback(mtb_wwd_nlu_events_t event);
extern char *Smart_Room_Control_ww_str[SMART_ROOM_CONTROL_NO_OF_WAKE_WORD];

// configuration object
extern mtb_wwd_nlu_config_t
    *Smart_Room_Control_ww_nlu_configs[SMART_ROOM_CONTROL_NO_OF_WAKE_WORD];

#endif /* PROJ_CM55_INC_SMART_ROOM_CONTROL_CONFIG_H_ */
