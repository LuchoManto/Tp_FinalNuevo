local client = {}
local response = {}
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
client.get_response = get_response
return client
