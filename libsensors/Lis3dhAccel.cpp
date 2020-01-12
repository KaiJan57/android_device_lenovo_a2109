/*
 * Copyright (C) 2012 Freescale Semiconductor Inc.
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define LOG_TAG "AccelSensor"
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/select.h>
#include <dlfcn.h>
#include <cutils/log.h>
#include <cutils/properties.h>
#include "Lis3dhAccel.h"

#include "InputEventReader.h"
#include "SensorBase.h"

#define ACC_DATA_NAME    gsensorInfo.sensorName
#define ACC_EVENT_X ABS_X
#define ACC_EVENT_Y ABS_Y
#define ACC_EVENT_Z ABS_Z

Lis3dhAccel::Lis3dhAccel()
        : SensorBase(NULL, ACC_DATA_NAME),
        mEnabled(0),
        mPendingMask(0),
        convert(0.0),
        direct_x(0),
        direct_y(0),
        direct_z(0),
        direct_xy(0),
        mInputReader(16),
        mDelay(0)
{
#ifdef DEBUG_SENSOR
        ALOGD("sensorName: %s, classPath: %s, lsg: %f\n",
                gsensorInfo.sensorName, gsensorInfo.classPath, gsensorInfo.priData);
#endif
        memset(&mPendingEvent, 0, sizeof(mPendingEvent));
        memset(&mAccData, 0, sizeof(mAccData));

        mPendingEvent.version = sizeof(sensors_event_t);
        mPendingEvent.sensor = ID_A;
        mPendingEvent.type = SENSOR_TYPE_ACCELEROMETER;
        mPendingEvent.acceleration.status = SENSOR_STATUS_ACCURACY_HIGH;

	mUser = 0;

	direct_x = (GRAVITY_EARTH/1000);
	direct_y = (GRAVITY_EARTH/1000);
	direct_z = (GRAVITY_EARTH/1000);

        char property[PROPERTY_VALUE_MAX];
        property_get("ro.sf.hwrotation", property, 0);
        switch (atoi(property)) {
            case 90:
                direct_y = (-1) * direct_y;
                direct_xy = (direct_xy == 1) ? 0 : 1;
                break;
            case 180:
                direct_x = (-1) * direct_x;
                direct_y = (-1) * direct_y;
                break;
            case 270:
                direct_x = (-1) * direct_x;
                direct_xy = (direct_xy == 1) ? 0 : 1;
                break;
        }

#ifdef DEBUG_SENSOR
	ALOGD("%s: data_fd: %d\n", __func__, data_fd);
#endif
}

Lis3dhAccel::~Lis3dhAccel() {

}

int Lis3dhAccel::setEnable(int32_t handle, int en) {
	int err = 0;

	//ALOGD("enable:  handle:  %ld, en: %d", handle, en);
	if(handle != ID_A && handle != ID_O && handle != ID_M)
		return -1;

	if(en)
		mUser++;
	else{
		mUser--;
		if(mUser < 0)
			mUser = 0;
	}

	if(mUser > 0)
		err = enable_sensor();
	else
		err = disable_sensor();

	if(handle == ID_A ) {
		if(en)
         	        mEnabled++;
		else
			mEnabled--;
		if(mEnabled < 0)
			mEnabled = 0;
        }

	//update_delay();
#ifdef DEBUG_SENSOR
	ALOGD("Lis3dhAccel enable %d ,usercount %d, handle %d ,mEnabled %d, err %d",
	        en, mUser, handle, mEnabled, err);
#endif

        return 0;
}

int Lis3dhAccel::setDelay(int32_t handle, int64_t ns) {
	(void)handle;
        if (ns < 0)
                return -EINVAL;

#ifdef DEBUG_SENSOR
        ALOGD("%s: ns = %lld", __func__, ns);
#endif
        mDelay = ns;

        return update_delay();
}

int Lis3dhAccel::update_delay() {
        return set_delay(mDelay);
}

int Lis3dhAccel::readEvents(sensors_event_t* data, int count) {
        if (count < 1)
                return -EINVAL;

        ssize_t n = mInputReader.fill(data_fd);
        if (n < 0)
                return n;

        int numEventReceived = 0;
        input_event const* event;

        while (count && mInputReader.readEvent(&event)) {
                int type = event->type;

                if ((type == EV_ABS) || (type == EV_REL) || (type == EV_KEY)) {
                        processEvent(event->code, event->value);
                        mInputReader.next();
                } else if (type == EV_SYN) {
                        int64_t time = getTimestamp();

			if (mPendingMask) {
				mPendingMask = 0;
				mPendingEvent.timestamp = time;

				if (mEnabled) {
					*data++ = mPendingEvent;
					mAccData = mPendingEvent;
					count--;
					numEventReceived++;
				}
			}

                        if (!mPendingMask) {
                                mInputReader.next();
                        }

                } else {
                        ALOGE("Lis3dhAccel: unknown event (type=%d, code=%d)",
                                type, event->code);
                        mInputReader.next();
                }
        }

        return numEventReceived;
}

void Lis3dhAccel::processEvent(int code, int value) {
        switch (code) {
                case ACC_EVENT_X :
                        mPendingMask = 1;

                        if(direct_xy) {
                                mPendingEvent.acceleration.y= value * direct_y;
                        }else {
                                mPendingEvent.acceleration.x = value * direct_x;
                        }

                        break;

                case ACC_EVENT_Y :
                        mPendingMask = 1;

                        if(direct_xy) {
                                mPendingEvent.acceleration.x = value * direct_x;
                        }else {
                                mPendingEvent.acceleration.y = value * direct_y;
                        }

                        break;

                case ACC_EVENT_Z :
                        mPendingMask = 1;
                        mPendingEvent.acceleration.z = value * direct_z ;
                        break;
        }

#ifdef DEBUG_SENSOR
        ALOGD("Sensor data:  x,y,z:  %f, %f, %f\n", mPendingEvent.acceleration.x,
						    mPendingEvent.acceleration.y,
						    mPendingEvent.acceleration.z);
#endif

}

int Lis3dhAccel::writeEnable(int isEnable) {

        int err = -1 ;

	if(gsensorInfo.classPath[0] == ICHAR)
		return -1;

        err = set_sysfs_input_int(gsensorInfo.classPath, "device/enable", isEnable);
	return err;
}

int Lis3dhAccel::writeDelay(int64_t ns) {
	if(gsensorInfo.classPath[0] == ICHAR)
		return -1;

	if (ns > 10240000000LL) {
		ns = 10240000000LL; /* maximum delay in nano second. */
	}
	if (ns < 312500LL) {
		ns = 312500LL; /* minimum delay in nano second. */
	}

	int64_t ms = (ns / 1000000);

        int err = set_sysfs_input_int(gsensorInfo.classPath, "device/pollrate_ms", ms);

        return 0;

}

int Lis3dhAccel::enable_sensor() {
	return writeEnable(1);
}

int Lis3dhAccel::disable_sensor() {
	return writeEnable(0);
}

int Lis3dhAccel::set_delay(int64_t ns) {
	return writeDelay(ns);
}

int Lis3dhAccel::getEnable(int32_t handle) {
	return (handle == ID_A) ? mEnabled : 0;
}

/*****************************************************************************/

