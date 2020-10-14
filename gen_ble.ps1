#!/bin/bash
Write-Output "eneter erpc_idl"
Set-Location erpc_idl
Write-Output "gen erpc_shim ble"
erpcgen rpc_system.erpc
Set-Location ..
Write-Output "auto release"
Remove-Item src/erpc_shim/* 
Copy-Item erpc_idl/erpc_shim/* src/erpc_shim/

Remove-Item src/erpc_shim/rpc_ble_api_client.cpp
Remove-Item src/erpc_shim/rpc_ble_callback_server.cpp
Remove-Item src/erpc_shim/rpc_ble_callback_server.h
Remove-Item src/erpc_shim/rpc_wifi_api_client.cpp

