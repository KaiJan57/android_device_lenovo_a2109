# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#
# This file sets variables that control the way modules are built
# thorughout the system. It should not be used to conditionally
# disable makefiles (the proper mechanism to control what gets
# included in a build is to use PRODUCT_PACKAGES in a product
# definition file).
#

USE_CAMERA_STUB := false
# TARGET_RECOVERY_UI_LIB := librecovery_ui_a2109a
RECOVERY_FSTAB_VERSION := 2

# inherit from the proprietary version
-include vendor/lenovo/a2109/BoardConfigVendor.mk

include hardware/nvidia/tegra3/BoardConfigCommon.mk

TARGET_NO_RADIOIMAGE := true

TARGET_USERIMAGES_USE_EXT4 := true
TARGET_BOOTLOADER_BOARD_NAME := a2109a

BOARD_KERNEL_CMDLINE := androidboot.selinux=enforcing androidboot.hardware=kai
BOARD_KERNEL_BASE := 0x10000000
BOARD_KERNEL_PAGESIZE := 2048
TARGET_KERNEL_CONFIG := cl2n_defconfig
TARGET_KERNEL_SOURCE := kernel/nvidia/tegra3
KERNEL_TOOLCHAIN := $(ANDROID_BUILD_TOP)/prebuilts/gcc/$(HOST_OS)-x86/arm/arm-eabi-4.8/bin
KERNEL_TOOLCHAIN_PREFIX := arm-eabi-

BOARD_SYSTEMIMAGE_PARTITION_SIZE := 671088640
# System partition might be too small, if so, disable journaling on system.img to save space.
BOARD_SYSTEMIMAGE_JOURNAL_SIZE := 0
BOARD_CACHEIMAGE_PARTITION_SIZE := 805306368
BOARD_USERDATAIMAGE_PARTITION_SIZE := 14088667136

BOARD_FLASH_BLOCK_SIZE := 4096

# Only pre-optimize the boot image
WITH_DEXPREOPT_BOOT_IMG_ONLY := true

# Configure jemalloc for low-memory
MALLOC_SVELTE := true

# Use clang platform builds
USE_CLANG_PLATFORM_BUILD := true

# Set HAL1 to fix loading the camera, due to old libnvodm_{query,imager}.so
TARGET_HAS_LEGACY_CAMERA_HAL1 := true

BOARD_HAVE_WIFI := true
# Wifi related defines
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
WPA_SUPPLICANT_VERSION      := VER_0_8_X
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_bcmdhd
BOARD_HOSTAPD_DRIVER        := NL80211
BOARD_HOSTAPD_PRIVATE_LIB   := lib_driver_cmd_bcmdhd
BOARD_WLAN_DEVICE           := bcmdhd
WIFI_DRIVER_FW_PATH_PARAM   := "/sys/module/bcmdhd/parameters/firmware_path"
WIFI_DRIVER_FW_PATH_STA     := "/vendor/firmware/fw_bcmdhd.bin"
WIFI_DRIVER_FW_PATH_AP      := "/vendor/firmware/fw_bcmdhd_apsta.bin"

TARGET_BOOTLOADER_BOARD_NAME := kai
#TARGET_BOARD_INFO_FILE := device/lenovo/a2109/board-info.txt

# assert
TARGET_OTA_ASSERT_DEVICE := kai,a2109

TARGET_NO_BOOTLOADER := true

BOARD_USES_GENERIC_INVENSENSE := false

# Audio Options
BOARD_USES_GENERIC_AUDIO := false

TARGET_NEEDS_PLATFORM_TEXT_RELOCATIONS := true

# Bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true
BOARD_BLUEDROID_VENDOR_CONF := device/lenovo/a2109/bluetooth/vnd_a2109.txt
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR ?= device/lenovo/a2109/bluetooth

# Use Cortex A9 optimizations for A7
USE_ALL_OPTIMIZED_STRING_FUNCS := true

# Turn on Cortex A9 Optimizations for A7
TARGET_EXTRA_CFLAGS += $(call cc-option,-mtune=cortex-a9) $(call cc-option,-mcpu=cortex-a9)

ifneq ($(HAVE_NVIDIA_PROP_SRC),false)
# needed for source compilation of nvidia libraries
-include vendor/nvidia/proprietary_src/build/definitions.mk
-include vendor/nvidia/build/definitions.mk
endif

# Security
BOARD_SEPOLICY_DIRS += \
        device/lenovo/a2109/sepolicy

TARGET_RECOVERY_FSTAB = device/lenovo/a2109/rootdir/fstab.kai
