param($1,$2)
$PROJECT="seeed-ambd-firmware"
$BOARD="realtek:AmebaD:ameba_rtl8721d"
$COMMAND=$1 
$PORT=$2
$COMMAND
$CRTDIR=Split-Path -Parent $MyInvocation.MyCommand.Definition

#easylogger
$INCLUDE+=" -I$CRTDIR/src/easylogger"
$INCLUDE+=" -I$CRTDIR/src/easylogger/inc"

#ble
$INCLUDE+=" -I$CRTDIR/src/ble"

#wifi
$INCLUDE+=" -I$CRTDIR/src/wifi"


#esp_lib
$INCLUDE+=" -I$CRTDIR/src/esp_lib"

#erpc
$INCLUDE+=" -I$CRTDIR/src/erpc"

#erpc_shim 
$INCLUDE+=" -I$CRTDIR/src/erpc_shim"

$FLAGS+=""


if ("$COMMAND" -eq "--build"){
	Write-Output "Building $PROJECT"
	arduino-cli compile --fqbn  $BOARD --verbose --build-properties build.extra_flags="$INCLUDE $FLAGS" $PROJECT 
}
elseif ( "$COMMAND" -eq "--flash" ){
	if(!$PORT)
	{
	 Write-Output "port not allow null"
	 exit
	}
	arduino-cli upload  --fqbn $BOARD -p $PORT
}
elseif ("$COMMAND" -eq "--all" )
{
	if(!$PORT)
	{
	 Write-Output "port not allow null"
	 exit
	}
	arduino-cli compile --fqbn  $BOARD --verbose --build-properties build.extra_flags="$INCLUDE $FLAGS" $PROJECT
	arduino-cli upload  --fqbn $BOARD -p $PORT
}
else 
{
	Write-Output "Nothing to do for target"
}