#include <gtest/gtest.h>

#include "../cli.hpp"

TEST(test_cli, test_cli_addr)
{
    const char* argv[] = {"appname", "-a", "8.8.8.8", "--address", "rtk.ua", "-pw", "myvrs.com.ua"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    std::string address;
    bool exists = cli.retrieve({"a"}, address);
    EXPECT_TRUE(exists);
    EXPECT_EQ("8.8.8.8", address);
}

TEST(test_cli, test_cli_address)
{
    const char* argv[] = {"appname", "--address", "192.168.1.12"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    std::string address;
    bool exists = cli.retrieve({"-address"}, address);
    EXPECT_TRUE(exists);
    EXPECT_EQ("192.168.1.12", address);
}

TEST(test_cli, test_cli_a)
{
    const char* argv[] = {"appname", "--address", "rtk.ua", "-a", "myvrs.com.ua"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    std::string address;
    auto arg = cli.retrieve({"a"}, address);
    EXPECT_TRUE(arg);
    EXPECT_EQ("myvrs.com.ua", address);
}

TEST(test_cli, test_cli_port)
{
    const char* argv[] = {"appname", "--address", "rtk.ua", "-a", "myvrs.com.ua", "--port", "2222"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    int port;
    auto arg = cli.retrieve({"-port"}, port);
    EXPECT_TRUE(arg);
    EXPECT_EQ(2222, port);
}

TEST(test_cli, test_cli_p)
{
    const char* argv[] = {"appname", "--address", "rtk.ua", "-p", "1235"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    int port;
    auto arg = cli.retrieve({"p"}, port);
    EXPECT_TRUE(arg);
    EXPECT_EQ(1235, port);
}

TEST(test_cli, test_cli_mount)
{
    const char* argv[] = {"appname", "--mount", "rtkua", "-a", "myvrs.com.ua", "--port", "2222"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    std::string mount;
    auto arg = cli.retrieve({"-mount"}, mount);
    EXPECT_TRUE(arg);
    EXPECT_EQ("rtkua", mount);
}

TEST(test_cli, test_cli_m)
{
    const char* argv[] = {"appname", "-m", "myrtk.ua", "-p", "1235"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    std::string mount;
    auto arg = cli.retrieve({"m"}, mount);
    EXPECT_TRUE(arg);
    EXPECT_EQ("myrtk.ua", mount);
}

TEST(test_cli, test_cli_user)
{
    const char* argv[] = {"appname", "--mount", "rtk.ua", "-a", "myvrs.com.ua", "--user", "c22x22"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    std::string user;
    auto arg = cli.retrieve({"-user"}, user);
    EXPECT_TRUE(arg);
    EXPECT_EQ("c22x22", user);
}

TEST(test_cli, test_cli_uint)
{
    const char* argv[] = {"appname", "-m", "myrtk.ua", "-u", "1235"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    std::string user;
    auto arg = cli.retrieve({"u"}, user);
    EXPECT_TRUE(arg) << "retrieve failed";
    EXPECT_EQ("1235", user) << "int as string failure";
}

TEST(test_cli, test_cli_udouble)
{
    const char* argv[] = {"appname", "-m", "myrtk.ua", "-u", "12.35"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    std::string user;
    auto arg = cli.retrieve({"u"}, user);
    EXPECT_TRUE(arg);
    EXPECT_EQ("12.35", user) << "double as string failure";
}

TEST(test_cli, test_cli_password)
{
    const char* argv[] = {"appname", "--mount", "rtk.ua", "-a", "myvrs.com.ua", "--password", "p2222"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    std::string password;
    auto arg = cli.retrieve({"-password"}, password);
    EXPECT_TRUE(arg);
    EXPECT_EQ("p2222", password);
}

TEST(test_cli, test_cli_pw)
{
    const char* argv[] = {"appname", "-m", "myrtk.ua", "-pw", "w1235"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    std::string password;
    auto arg = cli.retrieve({"pw"}, password);
    EXPECT_TRUE(arg);
    EXPECT_EQ("w1235", password);
}

TEST(test_cli, test_cli_latitude)
{
    const char* argv[] = {"appname", "--address", "rtk.ua", "-a", "myvrs.com.ua", "--port", "2222", "--latitude", "49"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    double latitude;
    auto arg = cli.retrieve({"-latitude"}, latitude);
    EXPECT_TRUE(arg);
    EXPECT_EQ(49, latitude);
}

TEST(test_cli, test_cli_la)
{
    const char* argv[] = {"appname", "--address", "rtk.ua", "-a", "myvrs.com.ua", "--port", "2222", "-la", "49.8"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    double latitude;
    auto arg = cli.retrieve({"la"}, latitude);
    EXPECT_TRUE(arg);
    EXPECT_DOUBLE_EQ(49.8, latitude);
}

TEST(test_cli, test_cli_longitude)
{
    const char* argv[] = {"appname", "--longitude", "28", "-a", "myvrs.com.ua", "--port", "2222", "--latitude", "49"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    double longitude;
    auto arg = cli.retrieve({"-longitude"}, longitude);
    EXPECT_TRUE(arg);
    EXPECT_EQ(28, longitude);
}

TEST(test_cli, test_cli_lo)
{
    const char* argv[] = {"appname", "--longitude", "28", "-lo", "32.324", "-a", "myvrs.com.ua", "--port", "2222", "--latitude", "49"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    double longitude;
    auto arg = cli.retrieve({"lo"}, longitude);
    EXPECT_TRUE(arg);
    EXPECT_DOUBLE_EQ(32.324, longitude);
}

TEST(test_cli, test_cli_notoken)
{
    const char* argv[] = {"appname", "--longitude", "28", "-lo", "32.324", "-a", "myvrs.com.ua"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    double longitude;
    auto arg = cli.retrieve({"notoken"}, longitude);
    EXPECT_FALSE(arg);
}

TEST(test_cli, test_cli_error)
{
    const char* argv[] = {"appname", "--longitude", "28", "-lo", "32.324", "a", "myvrs.com.ua"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    std::string err{};
    try {
        VrsTunnel::cli cli(argc, argv);
    }
    catch (const std::runtime_error& ex) {
        err = ex.what();
    }
    ASSERT_STREQ(err.data(), "wrong command name");
}
