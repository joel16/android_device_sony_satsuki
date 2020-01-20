LOCAL_PATH := $(call my-dir)

# Configuration scripts
include $(CLEAR_VARS)
LOCAL_MODULE		:= init.qcom.power.sh
LOCAL_MODULE_TAGS	:= optional eng
LOCAL_MODULE_CLASS	:= EXECUTABLES
LOCAL_SRC_FILES		:= etc/init.qcom.power.sh
LOCAL_MODULE_PATH	:= $(TARGET_OUT_VENDOR_EXECUTABLES)
include $(BUILD_PREBUILT)
