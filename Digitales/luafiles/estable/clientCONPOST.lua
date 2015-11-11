local client = {}
local HOST = "192.168.1.118"
local URI = "/module_post/"
local function sprintf(fmt, ...)
    return string.format(fmt, ...)
end
local function get_response(body)
    if(body == nil) then
        body=" "
    end
    response={}
    table.insert(response, "HTTP/1.1 200 OK\r\n")
    table.insert(response, "Host: gaston\r\n")
    table.insert(response, "Content-Type: text/plain\r\n")
    local value = tostring(#body)
    table.insert(response, sprintf("Content-Length: %s\r\n", value))
    table.insert(response, "\r\n")
    table.insert(response, body)
    return table.concat(response)
end
local function build_post_request(host, uri, data_table)
     local data = ""
     for param,value in pairs(data_table) do
          data = data .. param.."="..value.."&"
     end
     request = "POST "..uri.." HTTP/1.1\r\n"..
     "Host: "..host.."\r\n"..
     "Connection: close\r\n"..
     "Content-Type: application/x-www-form-urlencoded\r\n"..
     "Content-Length: "..string.len(data).."\r\n"..
     "\r\n"..
     data
     return request
end
local function send_to_server(received)
     local data = {
         received = received
     }
     socket = net.createConnection(net.TCP,0)
     socket:connect(50505,HOST)
     socket:on("connection",function(sck)
          local post_request = build_post_request(HOST,URI,data)
          sck:send(post_request)
     end)
end
client.send_to_server = send_to_server
client.get_response = get_response
client.sprintf = sprintf
return client
