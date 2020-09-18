#!/bin/bash
echo "eneter erpc_idl"
cd erpc_idl
echo "gen erpc_shim"
erpcgen rpc_ble.erpc
cd ..
echo "auto release"
rm src/erpc_shim/* -rf
cp erpc_idl/erpc_shim/ble/* src/erpc_shim/
rm src/erpc_shim/ble/rpc_ble_api_client.cpp
rm src/erpc_shim/ble/rpc_ble_callback_server.cpp
rm src/erpc_shim/ble/rpc_ble_callback_server.h