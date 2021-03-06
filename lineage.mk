# Copyright (C) 2014 The CyanogenMod Project
# Copyright (C) 2017 The LineageOS Project
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

# Release name
PRODUCT_RELEASE_NAME := A2109

# Boot animation
TARGET_SCREEN_HEIGHT := 800
TARGET_SCREEN_WIDTH := 1280

# Inherit from those products. Most specific first.
#$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit some common lineage stuff.
$(call inherit-product, vendor/cm/config/common_full_tablet_wifionly.mk)

# Inherit from a2109 device
$(call inherit-product, device/lenovo/a2109/full_a2109.mk)

PRODUCT_NAME := lineage_a2109
PRODUCT_DEVICE := a2109
PRODUCT_BRAND := IdeaTab
PRODUCT_MANUFACTURER := Lenovo
PRODUCT_MODEL := A2109A

TARGET_RESTRICT_VENDOR_FILES := false

TARGET_CONTINUOUS_SPLASH_ENABLED := true
