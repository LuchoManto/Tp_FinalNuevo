-- main.lua --

local client = require "client"

local serial_data = ""
local response = ""
local conexion = nil

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
        serial_data = serial_data..data
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

local function enviar()
    tmr.alarm(1, 800, 0, function()
        if conexion ~= nil then
            response = client.get_response(serial_data)
            serial_data = ""
            conexion:send(response)
            conexion:on("sent",function() conexion:close() conexion=nil end)
        end
        local intentos = 1
        while conexion ~= nil and intentos < 5 do
            conexion:send(response)
            tmr.delay(1000)
            intentos = intentos+1
        end
        if conexion ~= nil then
            conexion:close()
        end
        conexion=nil
    end)
end

 -- Start a simple http server
srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
  conn:on("receive",function(conn,request)
    GET = parse_get(request)
    if(GET.uartstate == 'disconnected')
    then
        serial_data = "Se deja de escuchar a travez del UART"
        stop_uart()
    end
    if(GET.uartstate == 'connected')
    then
        serial_data = "Se escuchara a travez del UART"
        listen_uart()
    end
    if(GET.tosend)
    then
        uart.write(0, GET.tosend .. '\n')
    end
    conexion = conn
    tmr.delay(50000)
    enviar()
  end)
end)
