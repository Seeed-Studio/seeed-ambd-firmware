#!/bin/bash
echo "eneter erpc_idl"
cd erpc_idl
echo "gen erpc_shim"
erpcgen rpc_system.erpc
cd ..
echo "auto release"
rm src/erpc_shim/* -rf

cp erpc_idl/erpc_shim/*  src/erpc_shim/
rm src/erpc_shim/rpc_ble_api_client.cpp
rm src/erpc_shim/rpc_ble_callback_server.cpp
rm src/erpc_shim/rpc_ble_callback_server.h
rm src/erpc_shim/rpc_wifi_api_client.cpp
rm src/erpc_shim/rpc_wifi_callback_server.cpp
rm src/erpc_shim/rpc_wifi_callback_server.h
