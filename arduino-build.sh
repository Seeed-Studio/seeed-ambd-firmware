#!/bin/bash
PROJECT=seeed-ambd-firmware
BOARD=realtek:AmebaD:ameba_rtl8721d
COMMAND=$1
PORT=$2
CRTDIR=$(pwd)

#easylogger
INCLUDE+=" -I$CRTDIR/src/easylogger"
INCLUDE+=" -I$CRTDIR/src/easylogger/inc"

#ble
INCLUDE+=" -I$CRTDIR/src/ble"

#wifi
INCLUDE+=" -I$CRTDIR/src/wifi"

#esp_lib
INCLUDE+=" -I$CRTDIR/src/esp_lib"

#erpc
INCLUDE+=" -I$CRTDIR/src/erpc"

#erpc_shim
INCLUDE+=" -I$CRTDIR/src/erpc_shim"

#mdns
INCLUDE+=" -I$CRTDIR/src/mDNS"

FLAGS+=""


if [ "$COMMAND" = "--build" ];
then
	echo "Building $PROJECT"
	arduino-cli compile --fqbn  $BOARD --verbose --build-property build.extra_flags="$INCLUDE $FLAGS" $PROJECT &
	pid=$! # Process Id of the previous running command
	while kill -0 $pid 2>/dev/null
	do
		echo "Still building..."
		sleep 2
	done
	wait $pid
	ret=$?
	if [ $ret -eq 0 ]; then
		echo "Building $PROJECT done"
	else
		exit "Building $PROJECT failed"
	fi
elif [ "$COMMAND" = "--flash" ];
then
	arduino-cli upload  --fqbn $BOARD -p $PORT
elif [ "$COMMAND" = "--all" ];
then
	arduino-cli compile --fqbn  $BOARD --verbose --build-property build.extra_flags="$INCLUDE $FLAGS" $PROJECT
	status=$?
	[ $status -eq 0 ] && arduino-cli upload  --fqbn $BOARD --port $PORT
else
	echo "Nothing to do for target"
fi
