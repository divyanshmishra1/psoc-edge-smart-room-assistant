/****************************************************************************
 * File Name        : Smart_Room_Control_config.c
 *
 * Description      : Configuration object for WWD and NLU.
 *
 * NOTE: This is a PLACEHOLDER file. Replace with the auto-generated file
 * from the DEEPCRAFT Voice Assistant cloud tool when your model is ready.
 * The intent names, command list, and model pointers MUST match your
 * actual DeepCraft model output.
 *****************************************************************************/

#include "Smart_Room_Control_config.h"

#include "AM_LSTM_tflm_model_int16x8.h"
#include "mtb_ml.h"
#include "mtb_ml_model_16x8.h"

#include "ifx_sp_common_priv.h"
#include "ifx_va_prms.h"

#include "Smart_Room_Control_U55_CMDmodel.h"
#include "Smart_Room_Control_U55_WWmodel.h"
#include "U55_NMBmodel.h"

#include "Smart_Room_Control.h"
#include "Smart_Room_Control_ifx_va_config_prms.h"

/* Following am_tensor_arena has been counted as part of persistent memory total
 * size */
/* Tensor_arena buffer must be in SOCMEM and aligned by 16 which are required by
 * U55 */
static uint8_t am_tensor_arena[AM_LSTM_ARENA_SIZE] __attribute__((aligned(16)))
__attribute__((section(".cy_socmem_data")));

static int16_t data_feed_int[N_SEQ * FEATURE_BUF_SZ]
    __attribute__((aligned(16)));
static float mtb_ml_input_buffer[N_SEQ * FEATURE_BUF_SZ];

static float xIn[FRAME_SIZE_16K] __attribute__((section(".wwd_nlu_data3")));
static float features[FEATURE_BUF_SZ]
    __attribute__((section(".wwd_nlu_data4")));
static float output_scores[(N_PHONEMES + 1) * (1 + AM_LOOKBACK)]
    __attribute__((section(".wwd_nlu_data5")));

// common buffers
static mtb_wwd_nlu_buff_t wwd_nlu_buff = {
    .am_model_bin = {MTB_ML_MODEL_BIN_DATA(AM_LSTM)},
    .am_model_buffer = {.tensor_arena = am_tensor_arena,
                        .tensor_arena_size = AM_LSTM_ARENA_SIZE},
    .data_feed_int = data_feed_int,
    .mtb_ml_input_buffer = mtb_ml_input_buffer,
    .output_scores = output_scores,
    .xIn = xIn,
    .features = features};

// NLU setup array
static mtb_nlu_setup_array_t nlu_setup_array = {
    .intent_name_list = Smart_Room_Control_intent_name_list,
    .variable_name_list = Smart_Room_Control_variable_name_list,
    .variable_phrase_list = Smart_Room_Control_variable_phrase_list,
    .unit_phrase_list = Smart_Room_Control_unit_phrase_list,
    .intent_map_array = Smart_Room_Control_intent_map_array,
    .intent_map_array_sizes = Smart_Room_Control_intent_map_array_sizes,
    .variable_phrase_sizes = Smart_Room_Control_variable_phrase_sizes,
    .unit_phrase_map_array = Smart_Room_Control_unit_phrase_map_array,
    .unit_phrase_map_array_sizes =
        Smart_Room_Control_unit_phrase_map_array_sizes,
    .NUM_UNIT_PHRASES = sizeof(Smart_Room_Control_unit_phrase_list),
};

// WW config
static mtb_wwd_conf_t ww_conf = {.ww_params = Smart_Room_Control_dfww_prms,
                                 .callback.cb_for_event = CY_EVENT_SOD,
                                 .callback.cb_function =
                                     Smart_Room_Control_wake_word_callback};

// NLU config
static mtb_nlu_config_t nlu_conf = {
    .nlu_params = Smart_Room_Control_dfcmd_prms,
    .nlu_command_timeout = 5000,
};

static mtb_wwd_nlu_config_t ww_1_conf = {
    .ww_model_ptr = Smart_Room_Control_WWmodeldata,
    .cmd_model_ptr = Smart_Room_Control_CMDmodeldata,
    .nmb_model_ptr = NMBmodeldata,
    .wwd_nlu_buff_data = &wwd_nlu_buff,
    .sod_params = Smart_Room_Control_sod_prms,
    .hpf_params = Smart_Room_Control_pre_proc_hpf_prms,
    .denoise_params = Smart_Room_Control_denoise_prms,
    .ww_conf = &ww_conf,
    .nlu_conf.nlu_config = &nlu_conf,
    .nlu_conf.nlu_variable_data = &nlu_setup_array,
};

mtb_wwd_nlu_config_t
    *Smart_Room_Control_ww_nlu_configs[SMART_ROOM_CONTROL_NO_OF_WAKE_WORD] = {
        &ww_1_conf};

char *Smart_Room_Control_ww_str[SMART_ROOM_CONTROL_NO_OF_WAKE_WORD] = {
    "OK Infineon"};

/*******************************************************************************
 * Intent Name List — maps intent index to human-readable name
 * These MUST match the order of intents in the DeepCraft project.
 ******************************************************************************/
const char
    *Smart_Room_Control_intent_name_list[SMART_ROOM_CONTROL_NUM_INTENTS] = {
        "TurnOnLight",      /* 0x0 */
        "TurnOffLight",     /* 0x1 */
        "TurnOnProjector",  /* 0x2 */
        "TurnOffProjector", /* 0x3 */
        "TurnOnAC",         /* 0x4 */
        "TurnOffAC",        /* 0x5 */
        "StartMeeting",     /* 0x6 */
        "StopMeeting",      /* 0x7 */
        "SetTemperature",   /* 0x8 */
        "BookRoom",         /* 0x9 */
};

const char
    *Smart_Room_Control_variable_name_list[SMART_ROOM_CONTROL_NUM_VARIABLES] = {
        "Temperature",
        "Duration",
};

const char *Smart_Room_Control_variable_phrase_list
    [SMART_ROOM_CONTROL_NUM_VARIABLE_PHRASES] = {
        "", // Temperature
        "", // Duration
};

const char *
    Smart_Room_Control_unit_phrase_list[SMART_ROOM_CONTROL_NUM_UNIT_PHRASES] = {
        "degree", "degrees", "percent", "level",  "levels",  "hour",
        "hours",  "minute",  "minutes", "second", "seconds", "day",
        "days",   "",        "AM",      "PM",
};

/*******************************************************************************
 * Intent Map Array
 * Each entry: intent_index, num_variables, [variable_index, variable_value]...
 * Format: {intent_idx, 0} for commands without variables
 *         {intent_idx, num_vars, var_idx, var_val, ...} for commands with
 * variables
 ******************************************************************************/
const int Smart_Room_Control_intent_map_array
    [SMART_ROOM_CONTROL_INTENT_MAP_ARRAY_TOTAL_SIZE] = {
        0, 0,        // turn on light
        0, 0,        // turn on the light
        0, 0,        // switch on light
        0, 0,        // switch on the light
        1, 0,        // turn off light
        1, 0,        // turn off the light
        1, 0,        // switch off light
        1, 0,        // switch off the light
        2, 0,        // turn on projector
        2, 0,        // turn on the projector
        2, 0,        // switch on projector
        2, 0,        // switch on the projector
        3, 0,        // turn off projector
        3, 0,        // turn off the projector
        3, 0,        // switch off projector
        3, 0,        // switch off the projector
        4, 0,        // turn on ac
        4, 0,        // turn on the ac
        4, 0,        // switch on ac
        4, 0,        // switch on the ac
        5, 0,        // turn off ac
        5, 0,        // turn off the ac
        5, 0,        // switch off ac
        5, 0,        // switch off the ac
        6, 0,        // start the meeting
        7, 0,        // stop the meeting
        8, 1, 0, -1, // set temperature <numbers30> {degree,degrees}
        9, 1, 1, -1, // book room <numbers9> {hour,hours}
        9, 1, 1, -1, // book room <numbers0to100by5> {minute,minutes}
};

const int
    Smart_Room_Control_intent_map_array_sizes[SMART_ROOM_CONTROL_NUM_COMMANDS] =
        {
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4,
};

const int
    Smart_Room_Control_variable_phrase_sizes[SMART_ROOM_CONTROL_NUM_VARIABLES] =
        {
            0, // Temperature: None (numeric)
            0, // Duration: None (numeric)
};

const int Smart_Room_Control_unit_phrase_map_array
    [SMART_ROOM_CONTROL_UNIT_PHRASE_MAP_ARRAY_TOTAL_SIZE] = {
        0,          // None (turn on light)
        0,          // None
        0,          // None
        0,          // None
        0,          // None (turn off light)
        0,          // None
        0,          // None
        0,          // None
        0,          // None (turn on projector)
        0,          // None
        0,          // None
        0,          // None
        0,          // None (turn off projector)
        0,          // None
        0,          // None
        0,          // None
        0,          // None (turn on ac)
        0,          // None
        0,          // None
        0,          // None
        0,          // None (turn off ac)
        0,          // None
        0,          // None
        0,          // None
        0,          // None (start meeting)
        0,          // None (stop meeting)
        1, 2, 0, 1, // set temperature: {degree, degrees}
        1, 2, 5, 6, // book room hours: {hour, hours}
        1, 2, 7, 8, // book room minutes: {minute, minutes}
};

const int Smart_Room_Control_unit_phrase_map_array_sizes
    [SMART_ROOM_CONTROL_NUM_COMMANDS] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4,
};

__attribute__((weak)) void
Smart_Room_Control_wake_word_callback(mtb_wwd_nlu_events_t event) {}
