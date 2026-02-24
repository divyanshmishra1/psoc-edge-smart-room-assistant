/*****************************************************************************
 * \file Smart_Room_Control_ifx_va_config_prms.c
 *****************************************************************************
 * NOTE: This is a PLACEHOLDER file. Replace with the auto-generated file
 * from the DEEPCRAFT Voice Assistant cloud tool when your model is ready.
 * \copyright
 * Copyright 2025, Infineon Technologies.
 * All rights reserved.
 *****************************************************************************/

#include "Smart_Room_Control_ifx_va_config_prms.h"

int32_t Smart_Room_Control_sod_prms[] = {
    0, 16000, 160, IFX_PRE_PROCESS_IP_COMPONENT_SOD, 2, 400, 16384};

int32_t Smart_Room_Control_pre_proc_hpf_prms[] = {
    0, 16000, 160, IFX_PRE_PROCESS_IP_COMPONENT_HPF, 0};

int32_t Smart_Room_Control_denoise_prms[] = {
    0, 16000, 160, IFX_PRE_PROCESS_IP_COMPONENT_DENOISE, 1, 38};

int32_t Smart_Room_Control_dfww_prms[] = {
    0, 16000, 160, IFX_POST_PROCESS_IP_COMPONENT_DFWWD, 1, 500};

int32_t Smart_Room_Control_dfcmd_prms[] = {
    0, 16000, 160, IFX_POST_PROCESS_IP_COMPONENT_DFCMD, 2, 2000, 0};
