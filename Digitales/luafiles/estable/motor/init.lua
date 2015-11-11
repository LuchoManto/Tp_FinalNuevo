ssid = "LAC"
pass = "HErmes1611$"
wifi.setmode(wifi.STATION)
cfg={
    ip='192.168.1.248',
    netmask='255.255.255.0',
    gateway='192.168.1.1'
}
wifi.sta.setip(cfg)
wifi.sta.config(ssid,pass)
dofile("main.lua")
