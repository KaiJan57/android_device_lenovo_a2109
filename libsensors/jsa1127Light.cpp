/*
 * Copyright (C) 2011 Freescale Semiconductor Inc.
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
#define LOG_TAG "LightSensor"
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/select.h>
#include <cutils/log.h>
#include <cutils/properties.h>

#include "jsa1127Light.h"

#include "InputEventReader.h"
#include "SensorBase.h"

#define LIGSENSOR_DATA_NAME    ligSensorInfo.sensorName

/*****************************************************************************/
jsa1127Light::jsa1127Light()
        : SensorBase(NULL, LIGSENSOR_DATA_NAME),
        mEnabled(0),
        mInputReader(4),
        mPendingMask(0)
{
        memset(&mPendingEvent, 0, sizeof(mPendingEvent));

        mPendingEvent.version = sizeof(sensors_event_t);
        mPendingEvent.sensor = ID_L;
        mPendingEvent.type = SENSOR_TYPE_LIGHT;
        memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

#ifdef SENSOR_DEBUG
        ALOGD("%s: sensorName: %s, classPath: %s, lsg: %f, data_fd: %d\n", __func__,
                ligSensorInfo.sensorName, ligSensorInfo.classPath, ligSensorInfo.priData, data_fd);
#endif
}

jsa1127Light::~jsa1127Light() {
        if (mEnabled) {
                setEnable(ID_L, 0);
        }
}

int jsa1127Light::setDelay(int32_t handle, int64_t ns)
{
	(void)handle;
	int64_t ms = (ns / 1000000);

        if(ligSensorInfo.classPath[0] == ICHAR)
		return -1;


	if (ms > 2000LL) {
		ms = 2000; /* maximum delay in millisecond. */
	}
	if (ms < 200LL) {
		ms = 200; /* minimum delay in millisecond. */
	}

	int err = set_sysfs_input_int(ligSensorInfo.classPath, "delay", ms);

        return 0;
}

int jsa1127Light::setEnable(int32_t handle, int en)
{
	(void)handle;
        char buf[2];
        int err = -1;

	if(ligSensorInfo.classPath[0] == ICHAR)
		return -1;

        int flags = en ? 1 : 0;

        if (flags != mEnabled) {
	        err = set_sysfs_input_int(ligSensorInfo.classPath, "enable", en);
	        mEnabled = flags;
	}

	return 0;
}

int jsa1127Light::readEvents(sensors_event_t* data, int count)
{
        if (count < 1)
                return -EINVAL;

	if (mPendingMask) {
		mPendingMask = false;
		mPendingEvent.timestamp = getTimestamp();
		*data = mPendingEvent;
		return mEnabled ? 1 : 0;
	}

        ssize_t n = mInputReader.fill(data_fd);
        if (n < 0)
                return n;

        int numEventReceived = 0;
        input_event const* event;

        while (count && mInputReader.readEvent(&event)) {
                int type = event->type;

                if ((type == EV_ABS) || (type == EV_REL) || (type == EV_KEY)) {
                        processEvent(event->code, event->value);
                } else if (type == EV_SYN) {
			mPendingEvent.timestamp = getTimestamp();

			if (mEnabled) {
				*data++ = mPendingEvent;
				count--;
				numEventReceived++;
			}

                } else {
                        ALOGE("unknown event (type=%d, code=%d)",
                                type, event->code);
                }

        mInputReader.next();
        }

        return numEventReceived;
}

void jsa1127Light::processEvent(int code, int value) {

#ifdef SENSOR_DEBUG
	ALOGD("Received event with code=%d, value=%d)",
                                code, value);
#else
	(void)code;
#endif

	// Sensor reports values up to 60000, but the original HAL limited those
	// to 20000. We'll do the same here.
	int max = 20000;
	if ( value < max ) {
		mPendingEvent.light = (float)value;
	} else {
		mPendingEvent.light = (float)max;
	}
}
