

#include <gtest/gtest.h>
#include <string>

#include "ntrip_client.hpp"
#include "mount_point.hpp"

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

    VrsTunnel::Ntrip::ntrip_client nc{};
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

    VrsTunnel::Ntrip::ntrip_client nc{};
    EXPECT_FALSE(nc.hasTableEnding(tbl));
}

TEST(testNtripClient, pasreTableTest1)
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

    VrsTunnel::Ntrip::ntrip_client nc{};
    EXPECT_TRUE(nc.hasTableEnding(tbl));

    auto table = VrsTunnel::Ntrip::mount_point::parse_table(tbl);
    EXPECT_EQ(5UL, table.size());

    EXPECT_EQ(table[0].raw_entry, "STR;RTCM3_HUST0;RTCM3_HUST0;RTCM 3;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZAKPOS;UKR;48.18;23.29;0;0;Trimble GPSNet;None;B;Y;19200;ZAKPOS, Khust;");
    EXPECT_EQ(table[1].raw_entry, "STR;RTCM3_RAHI0;RTCM3_RAHI0;RTCM 3;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZAKPOS;UKR;48.05;24.2;0;0;Trimble GPSNet;None;B;Y;19200;ZAKPOS, Rakhiv;");
    EXPECT_EQ(table[2].raw_entry, "STR;RTCM3_MUKA;RTCM3_MUKA;RTCM 3;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZAKPOS;UKR;48.45;22.72;0;0;Trimble NetR5;None;B;Y;19200;ZAKPOS, Mukachevo;");
    EXPECT_EQ(table[3].raw_entry, "STR;RAW_CRNI;RAW_CRNI;RAW;1004(1),1005/1007(5),PBS(10);2;GPS;ZAKPOS;UKR;0;0;0;0;Trimble 5700;None;B;Y;19200;ZAKPOS, Chernivtsi;");
    EXPECT_EQ(table[4].raw_entry, "STR;RAW_VOLO;RAW_VOLO;RAW;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZakPOS;UKR;0;0;0;0;Trimble GPSNet;None;B;Y;0;;");
    
    EXPECT_EQ("RTCM3_HUST0", table[0].name);
    EXPECT_EQ("RTCM3_RAHI0", table[1].name);
    EXPECT_EQ( "RTCM3_MUKA", table[2].name);
    EXPECT_EQ(   "RAW_CRNI", table[3].name);
    EXPECT_EQ(   "RAW_VOLO", table[4].name);

    EXPECT_EQ("RTCM 3", table[0].type);
    EXPECT_EQ("RTCM 3", table[1].type);
    EXPECT_EQ("RTCM 3", table[2].type);
    EXPECT_EQ(   "RAW", table[3].type);
    EXPECT_EQ(   "RAW", table[4].type);

    EXPECT_DOUBLE_EQ(48.18, table[0].reference.Latitude);
    EXPECT_DOUBLE_EQ(23.29, table[0].reference.Longitude);
    EXPECT_DOUBLE_EQ(48.05, table[1].reference.Latitude);
    EXPECT_DOUBLE_EQ(24.20, table[1].reference.Longitude);
    EXPECT_DOUBLE_EQ(48.45, table[2].reference.Latitude);
    EXPECT_DOUBLE_EQ(22.72, table[2].reference.Longitude);
    EXPECT_DOUBLE_EQ( 0.00, table[3].reference.Latitude);
    EXPECT_DOUBLE_EQ( 0.00, table[3].reference.Longitude);
    EXPECT_DOUBLE_EQ( 0.00, table[4].reference.Latitude);
    EXPECT_DOUBLE_EQ( 0.00, table[4].reference.Longitude);
}

TEST(testNtripClient, getMountPointsTest1)
{
    VrsTunnel::Ntrip::ntrip_client nc{};
    auto resp = nc.getMountPoints("195.16.76.194", 2101);
    auto mounts = std::get<std::vector<VrsTunnel::Ntrip::mount_point>>(resp);
    EXPECT_EQ("RTCM3_HUST0", mounts[0].name);
}

TEST(testNtripClient, getMountPointsTest2)
{
    VrsTunnel::Ntrip::ntrip_client nc{};
    auto resp = nc.getMountPoints("195.16.76.194", 2101, "test", "test");
    auto mounts = std::get<std::vector<VrsTunnel::Ntrip::mount_point>>(resp);
    EXPECT_EQ("RTCM3_HUST0", mounts[0].name);
}
