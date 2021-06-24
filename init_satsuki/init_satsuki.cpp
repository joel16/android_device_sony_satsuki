/*
 * Copyright (C) 2008 The Android Open Source Project
 * Copyright (C) 2014 The CyanogenMod Project
 * Copyright (C) 2017 The LineageOS Project
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

#include <stdlib.h>

#include <android-base/logging.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/types.h>

#include "vendor_init.h"
#include "property_service.h"
#include "util.h"

using android::init::import_kernel_cmdline;
using android::init::property_set;

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void property_override_dual(char const system_prop[], char const vendor_prop[], char const value[])
{
    property_override(system_prop, value);
    property_override(vendor_prop, value);
}

void property_override_triple(char const product_prop[], char const system_prop[], char const vendor_prop[], char const value[])
{
    property_override(product_prop, value);
    property_override(system_prop, value);
    property_override(vendor_prop, value);
}

static void import_kernel_nv(const std::string& key,
        const std::string& value, bool for_emulator __attribute__((unused)))
{
    if (key.empty()) return;

    if (key == "oemandroidboot.phoneid") {
        // Dual Sim variant contains two IMEIs separated by comma.
        if ((count(value.begin(), value.end(),',')) > 0) {
            property_set("persist.multisim.config", "dsds");
            property_set("persist.radio.multisim.config", "dsds");
            property_set("ro.telephony.default_network", "9,1");
            property_set("ro.semc.product.model", "E6883");
            property_set("ro.semc.product.name", "Xperia Z5 Premium Dual");
            property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "E6883");
            property_override_dual("ro.product.name", "ro.vendor.product.name", "satsuki_dsds");
            property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "satsuki_dsds");
            property_override("ro.build.description", "E6883-user 7.1.1 32.4.A.1.54 3761073091 release-keys");
            property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "Sony/E6883/E6883:7.1.1/32.4.A.1.54/3761073091:user/release-keys");
        } else {
            property_set("ro.telephony.default_network", "9");
            property_set("ro.semc.product.model", "E6853");
            property_set("ro.semc.product.name", "Xperia Z5 Premium");
            property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "E6853");
            property_override_dual("ro.product.name", "ro.vendor.product.name", "satsuki");
            property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "satsuki");
            property_override("ro.build.description", "E6853-user 7.1.1 32.4.A.1.54 3761073091 release-keys");
            property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "Sony/E6853/E6853:7.1.1/32.4.A.1.54/3761073091:user/release-keys");
        }
    }
}

void vendor_load_properties()
{
    import_kernel_cmdline(0, import_kernel_nv);
}
