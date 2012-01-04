@echo off
CLS
start "Serwerek" "srv.exe"


start "Prisoner1" "client.exe"  "127.0.0.1" 

start "Prisoner2" "prison.exe"  "127.0.0.1"
