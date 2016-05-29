--
-- Created by IntelliJ IDEA.
-- User: Michal Tomaszewski
-- Date: 17.04.16
-- Time: 02:48
-- To change this template use File | Settings | File Templates.
--
tin_proto = Proto ("tin_protocol","TIN Protocol")

function tin_proto.dissector (buf, pkt, root)
    if buf:len() == 0 then return end
    pkt.cols.protocol = tin_proto.name
    local length = buf:len()
    local reportType = buf(8,4):le_uint()

    subtree = root:add(tin_proto, buf(0))
    subtree:add(buf(0,8), "ID: " .. buf(0,8):le_int64())
    if (reportType == 0) then
        subtree:add(buf(8,4), "REPORT. Message type: " .. buf(8,4):le_uint())
    else
        if (reportType == 1) then
            subtree:add(buf(8,4), "ALARM. Message type: " .. buf(8,4):le_uint())
        else
            if (reportType == 2) then
                subtree:add(buf(8,4), "INIT. Message type: " .. buf(8,4):le_uint())
            else
                if (reportType == 3) then
                    subtree:add(buf(8,4), "CALL ALARM. Message type: " .. buf(8,4):le_uint())
                else
                    if (reportType == 4) then
                        subtree:add(buf(8,4), "GET CURRENT VALUE. Message type: " .. buf(8,4):le_uint())
                    else
                        if (reportType == 4) then
                            subtree:add(buf(8,4), "SET ALARM VALUE. Message type: " .. buf(8,4):le_uint())
                        end
                    end
                end
            end
        end
    end
    subtree:add(buf(12,4), "Water level: " .. buf(12,4):le_uint())
    subtree:add(buf(16,4), "Safe level: " .. buf(16,4):le_uint())

    subtree:append_text(", Packet details below")
end

function tin_proto.init()
end

local tcp_dissector_table = DissectorTable.get("tcp.port")
dissector = tcp_dissector_table:get_dissector(17183)
tcp_dissector_table:add(17183, tin_proto)