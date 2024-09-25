#!/usr/bin/expect
spawn bluetoothctl
expect "Agent Registred"
sleep 3

expect "Request confirmation"
sleep 1
send  "yes\r"
	
#while true
#do
#echo "waiting measurements"
sleep 10000000
#done
expect eof

