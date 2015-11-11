-- main.lua --

local client = require "client"

local gps_data = ""
local response = ""

uart.setup(0,9600,8,0,1)

-- Connect
print('\nRunning main.lua\n')
tmr.alarm(0, 1000, 1, function()
   if wifi.sta.getip() == nil then
      print("Connecting to AP...\n")
   else
      ip, nm, gw=wifi.sta.getip()
      print("IP Info: \nIP Address: ",ip)
      print("Netmask: ",nm)
      print("Gateway Addr: ",gw,'\n')
      tmr.stop(0)
   end
end)

local function listen_uart()
    return uart.on("data", 0,
      function(data)
        gps_data = gps_data..data
    end, 0)
end

local function stop_uart()
    return uart.on("data")
end

local function parse_get(request)
    local _, _, method, path, vars = string.find(request, "([A-Z]+) (.+)?(.+) HTTP");
    if(method == nil)then
        _, _, method, path = string.find(request, "([A-Z]+) (.+) HTTP");
    end
    local GET = {}
    if (vars ~= nil)then
        for k, v in string.gmatch(vars, "(%w+)=(%w+)&*") do
            GET[k] = v
        end
    end
    return GET
end

 -- Start a simple http server
srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
  conn:on("receive",function(conn,request)
    GET = parse_get(request)
    response = client.get_response('Waiting response')
    if(GET.uartstate == 'disconnected')
    then
        gps_data = "Se deja de escuchar a travez del UART"
        stop_uart()
    end
    if(GET.uartstate == 'connected')
    then
        gps_data = "Se escuchara a travez del UART"
        listen_uart()
    end
    if(GET.tosend)
    then
        uart.write(0, GET.tosend .. '\n')
    end
    if(GET.hextosend)
    then
        for i=1, string.len(GET.hextosend), 2 do
            byte =("0x"..string.sub(GET.hextosend,0+i,1+i))
            uart.write(0,tonumber(byte))
        end
    end
    if(GET.getresponse)
    then
        response = client.get_response(gps_data)
        gps_data = ""
    end
    conn:send(response)
  end)
  conn:on("sent",function(conn) conn:close() end)
end)
