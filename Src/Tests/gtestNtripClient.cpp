

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

    VrsTunnel::Ntrip::NtripClient nc{};
    EXPECT_TRUE(nc.hasTableEnding(tbl));

    auto table = nc.parseTable(tbl);
    EXPECT_EQ(5, table.size());

    EXPECT_EQ(table[0].Raw, "STR;RTCM3_HUST0;RTCM3_HUST0;RTCM 3;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZAKPOS;UKR;48.18;23.29;0;0;Trimble GPSNet;None;B;Y;19200;ZAKPOS, Khust;");
    EXPECT_EQ(table[1].Raw, "STR;RTCM3_RAHI0;RTCM3_RAHI0;RTCM 3;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZAKPOS;UKR;48.05;24.2;0;0;Trimble GPSNet;None;B;Y;19200;ZAKPOS, Rakhiv;");
    EXPECT_EQ(table[2].Raw, "STR;RTCM3_MUKA;RTCM3_MUKA;RTCM 3;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZAKPOS;UKR;48.45;22.72;0;0;Trimble NetR5;None;B;Y;19200;ZAKPOS, Mukachevo;");
    EXPECT_EQ(table[3].Raw, "STR;RAW_CRNI;RAW_CRNI;RAW;1004(1),1005/1007(5),PBS(10);2;GPS;ZAKPOS;UKR;0;0;0;0;Trimble 5700;None;B;Y;19200;ZAKPOS, Chernivtsi;");
    EXPECT_EQ(table[4].Raw, "STR;RAW_VOLO;RAW_VOLO;RAW;1004(1),1005/1007(5),PBS(10);2;GPS+GLONASS;ZakPOS;UKR;0;0;0;0;Trimble GPSNet;None;B;Y;0;;");
    
    EXPECT_EQ("RTCM3_HUST0", table[0].Name);
    EXPECT_EQ("RTCM3_RAHI0", table[1].Name);
    EXPECT_EQ( "RTCM3_MUKA", table[2].Name);
    EXPECT_EQ(   "RAW_CRNI", table[3].Name);
    EXPECT_EQ(   "RAW_VOLO", table[4].Name);

    EXPECT_DOUBLE_EQ(48.18, table[0].Reference.Latitude);
    EXPECT_DOUBLE_EQ(23.29, table[0].Reference.Longitude);
    EXPECT_DOUBLE_EQ(48.05, table[1].Reference.Latitude);
    EXPECT_DOUBLE_EQ(24.20, table[1].Reference.Longitude);
    EXPECT_DOUBLE_EQ(48.45, table[2].Reference.Latitude);
    EXPECT_DOUBLE_EQ(22.72, table[2].Reference.Longitude);
    EXPECT_DOUBLE_EQ( 0.00, table[3].Reference.Latitude);
    EXPECT_DOUBLE_EQ( 0.00, table[3].Reference.Longitude);
    EXPECT_DOUBLE_EQ( 0.00, table[4].Reference.Latitude);
    EXPECT_DOUBLE_EQ( 0.00, table[4].Reference.Longitude);
}

TEST(testNtripClient, getMountPointsTest1)
{
    VrsTunnel::Ntrip::NtripClient nc{};
    auto resp = nc.getMountPoints("195.16.76.194", 2101);
    auto mounts = std::get<std::vector<VrsTunnel::Ntrip::MountPoint>>(resp);
    EXPECT_EQ("RTCM3_HUST0", mounts[0].Name);
}

TEST(testNtripClient, getMountPointsTest2)
{
    VrsTunnel::Ntrip::NtripClient nc{};
    auto resp = nc.getMountPoints("195.16.76.194", 2101, "test", "test");
    auto mounts = std::get<std::vector<VrsTunnel::Ntrip::MountPoint>>(resp);
    EXPECT_EQ("RTCM3_HUST0", mounts[0].Name);
}

TEST(testNtripClient, getMountPointsTest3)
{
    VrsTunnel::Ntrip::NtripClient nc{};
    auto resp = nc.getMountPoints("titanmachinery.ua", 8021, "test", "test");
    auto mounts = std::get<std::vector<VrsTunnel::Ntrip::MountPoint>>(resp);
    EXPECT_EQ("DynRTK", mounts[0].Name);
}