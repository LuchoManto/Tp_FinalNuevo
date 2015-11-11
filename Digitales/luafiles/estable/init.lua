-- init.lua --

-- Global Variables (Modify for your network)
ssid = "perez m"
pass = "xx123pp856luv"

-- Configure Wireless Internet
print('\n Running init.lua\n')
wifi.setmode(wifi.STATION)
cfg={
    ip='192.168.0.248',
    netmask='255.255.255.0',
    gateway='192.168.0.1'
}
wifi.sta.setip(cfg)

print('set mode=STATION (mode='..wifi.getmode()..')\n')
print('MAC Address: ',wifi.sta.getmac())
print('Chip ID: ',node.chipid())
print('Heap Size: ',node.heap(),'\n')
 
wifi.sta.config(ssid,pass)

-- Run the main file
dofile("main.lua")
