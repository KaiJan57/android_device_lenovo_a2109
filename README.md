android_device_lenovo_kai
=========================

Lenovo Ideatab A2109A (kai)

This is Android Marshmallow for Lenovo Ideatab A2109A (kai)
  Major thanks to Ziyann and Sheffzor for their amazing work bringing kernel 3.4 to tegra3
  Thanks to alfsamsung for his work on another old, unsupported tegra3 device, the lge p880
  Thanks to decatf for further inspirations.

Special thanks to the tf700t and tf300t developers for Cyanogenmod:
  dasunsrule32
  xplodwild
  nycbjr

Thanks to the A2109A pioneers:
  superhansi
  gmarkall
  kahlo007

Other tegra3 devices and their repos:
grouper (Nexus 7 2012)
https://github.com/CyanogenMod/android_device_asus_grouper
devs: Lloir, jrizzoli, thoemy, dhacker29
https://android.googlesource.com/device/asus/grouper/

tf300t (Asus Transformer Pad TF300T)
https://github.com/CyanogenMod/android_device_asus_tf300t
devs: chrmhoffmann, jruesga, dasunsrule32

tf700t (Asus Transformer Pad Infinity TF700T)
https://github.com/CyanogenMod/android_device_asus_tf700t
devs: jruesga, dasunsrule32, tombriden, rmcc

a500 (Acer Iconia A500 Picasso)
https://github.com/CyanogenMod/android_device_acer_a500

p880 (LG Optimus 4X HD)
https://github.com/CyanogenMod/android_device_lge_p880
devs: Ntemis, rmcc
https://github.com/P880-dev/proprietary_vendor_lge
devs: laufersteppenwolf

endeavoru (HTC One X)
https://github.com/CyanogenMod/android_device_htc_endeavoru
enrc2b (HTC One X+ (international))
https://github.com/cyanogenmod/android_device_htc_enrc2b
https://github.com/CyanogenMod/android_device_htc_tegra3-common
devs: Lloir, thoemy


Some basic hardware info
-Bluetooth: BCM4330
-GPS: BCM47511
-WiFi: BCMDHD
-Audio: tegra-aic326x
-Touchscreen: ft5x0x_ts
-Accelerometer: lis3dh
-Ambient light sensor: Solteam JSA1127	
-Back camera: s5k5cag
-Front camera: mt9m114

Be sure to have this patched before building LineageOS: https://github.com/PJBrs/platform_frameworks_base/commit/97c0f56efd0c23c399439f76a2733d56491e2af1
