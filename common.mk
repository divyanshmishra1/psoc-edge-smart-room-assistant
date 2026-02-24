################################################################################
# \file common.mk
# \version 1.0
#
# \brief
# Settings shared across all projects.
#
################################################################################
# \copyright
# (c) 2023-2026, Infineon Technologies AG, or an affiliate of Infineon
# Technologies AG.  SPDX-License-Identifier: Apache-2.0
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################

MTB_TYPE=PROJECT

# Target board/hardware (BSP).
# To change the target, it is recommended to use the Library manager
# ('make library-manager' from command line), which will also update 
# Eclipse IDE launch configurations.
TARGET=KIT_PSE84_EVAL_EPC2

# Name of toolchain to use. Options include:
#
# ARM     	-- ARM Compiler (must be installed separately)
# LLVM_ARM	-- LLVM Embedded Toolchain (must be installed separately)
#
# See also: CY_COMPILER_PATH below
TOOLCHAIN=LLVM_ARM

# Default build configuration. Options include:
#
# Debug -- build with minimal optimizations, focus on debugging.
# Release -- build with full optimizations
# Custom -- build with custom configuration, set the optimization flag in CFLAGS
# 
# If CONFIG is manually edited, ensure to update or regenerate 
# launch configurations for your IDE.
CONFIG=Debug

MTB_SUPPORTED_TOOLCHAINS?=LLVM_ARM ARM

# Config file for postbuild sign and merge operations.
# NOTE: Check the JSON file for the command parameters
COMBINE_SIGN_JSON?=configs/boot_with_extended_boot.json

# Option to enable the DEEPCRAFT Audio Enhancement.
#
# ENABLED   - use the audio-enhancement to filter the audio input stream before
#             executing the voice-assistant process function.
# DISABLED  - use raw audio input stream from microphone to the feed the 
#             voice assistant process function.
USE_AUDIO_ENHANCEMENT=ENABLED

# Option to use FULL or LIMITED version of the Audio Voice Core library
#
# LIMITED - Limited time functionality (Default)
# FULL    - Full functionality, no time limit
#
CONFIG_VOICE_CORE_MODE=LIMITED

# Set the name of the project created in DEEPCRAFT Voice Assistant cloud tool 
# and placed in the va_models/ folder.
#
# IMPORTANT: Set this to the exact name of your DeepCraft model folder.
# The folder must be placed at: proj_cm55/va_models/<YourProjectName>/
#
# Available options:
# Smart_Room_Control -- custom smart room control model (YOUR MODEL)
# Smart_Lights_Demo  -- built-in smart lights demo model
# LED_Demo           -- built-in LED demo model
# Cooktop_Demo       -- built-in cooktop demo model
DEEPCRAFT_PROJECT_NAME=Smart_Room_Control

include ../common_app.mk
