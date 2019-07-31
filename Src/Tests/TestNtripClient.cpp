

#include <gtest/gtest.h>
#include <string>

#include "NtripClient.hpp"

TEST(testNtripClient, hasTableTest1)
{
    std::string tbl { "SOURCETABLE 200 OK\r\n"
"Server: NTRIP Trimble NTRIP Caster\r\n"
"Content-Type: text/plain\r\n"
"Content-Length: 716\r\n"
"Date: 31/Jul/2019:17:31:03 UTC\r\n"
"\r\n"
"STR;RTCM3_HUST0;RTCM3_HUST0;RTCM 3;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZAKPOS;UKR;48.18;23.29;0;0;Trimble GPSNet;None;B;Y;19200;ZAKPOS, Khust;\r\n"
"STR;RTCM3_RAHI0;RTCM3_RAHI0;RTCM 3;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZAKPOS;UKR;48.05;24.2;0;0;Trimble GPSNet;None;B;Y;19200;ZAKPOS, Rakhiv;\r\n"
"STR;RTCM3_MUKA;RTCM3_MUKA;RTCM 3;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZAKPOS;UKR;48.45;22.72;0;0;Trimble NetR5;None;B;Y;19200;ZAKPOS, Mukachevo;\r\n"
"STR;RAW_CRNI;RAW_CRNI;RAW;1004(1),1005/1007(5),PBS(10);2;GPS;ZAKPOS;UKR;0;0;0;0;Trimble 5700;None;B;Y;19200;ZAKPOS, Chernivtsi;\r\n"
"STR;RAW_VOLO;RAW_VOLO;RAW;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZakPOS;UKR;0;0;0;0;Trimble GPSNet;None;B;Y;0;;\r\n"
"ENDSOURCETABLE\r\n" };

    VrsTunnel::Ntrip::NtripClient nc{};
    EXPECT_TRUE(nc.hasTableEnding(tbl));
}

TEST(testNtripClient, hasTableTest2)
{
    std::string tbl { "SOURCETABLE 200 OK\r\n"
"Server: NTRIP Trimble NTRIP Caster\r\n"
"Content-Type: text/plain\r\n"
"Content-Length: 716\r\n"
"Date: 31/Jul/2019:17:31:03 UTC\r\n"
"\r\n"
"STR;RTCM3_HUST0;RTCM3_HUST0;RTCM 3;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZAKPOS;UKR;48.18;23.29;0;0;Trimble GPSNet;None;B;Y;19200;ZAKPOS, Khust;\r\n"
"STR;RTCM3_RAHI0;RTCM3_RAHI0;RTCM 3;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZAKPOS;UKR;48.05;24.2;0;0;Trimble GPSNet;None;B;Y;19200;ZAKPOS, Rakhiv;\r\n"
"STR;RTCM3_MUKA;RTCM3_MUKA;RTCM 3;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZAKPOS;UKR;48.45;22.72;0;0;Trimble NetR5;None;B;Y;19200;ZAKPOS, Mukachevo;\r\n"
"STR;RAW_CRNI;RAW_CRNI;RAW;1004(1),1005/1007(5),PBS(10);2;GPS;ZAKPOS;UKR;0;0;0;0;Trimble 5700;None;B;Y;19200;ZAKPOS, Chernivtsi;\r\n"
"STR;RAW_VOLO;RAW_VOLO;RAW;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZakPOS;UKR;0;0;0;0;Trimble GPSNet;None;B;Y;0;;\r\n"
"ENDSOURCETABLE" };

    VrsTunnel::Ntrip::NtripClient nc{};
    EXPECT_FALSE(nc.hasTableEnding(tbl));
}