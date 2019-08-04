#include <gtest/gtest.h>

#include "../cli.hpp"

TEST(test_cli, test_cli_addr)
{
    const char* argv[] = {"appname", "-a", "8.8.8.8", "--address", "rtk.ua", "-pw", "myvrs.com.ua"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("a");
    EXPECT_TRUE(arg);
    EXPECT_EQ("8.8.8.8", std::get<std::string>(*arg));
}

TEST(test_cli, test_cli_address)
{
    const char* argv[] = {"appname", "--address", "192.168.1.12"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("-address");
    EXPECT_TRUE(arg);
    if (auto arg = cli.find("-address"); arg) {
        EXPECT_EQ("192.168.1.12", std::get<std::string>(*arg));
    }
    else {
        EXPECT_TRUE(false);
    }
}

TEST(test_cli, test_cli_a)
{
    const char* argv[] = {"appname", "--address", "rtk.ua", "-a", "myvrs.com.ua"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("a");
    EXPECT_TRUE(arg);
    EXPECT_EQ("myvrs.com.ua", std::get<std::string>(*arg));
}

TEST(test_cli, test_cli_port)
{
    const char* argv[] = {"appname", "--address", "rtk.ua", "-a", "myvrs.com.ua", "--port", "2222"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("-port");
    EXPECT_TRUE(arg);
    EXPECT_EQ(2222, std::get<int>(*arg));
}

TEST(test_cli, test_cli_p)
{
    const char* argv[] = {"appname", "--address", "rtk.ua", "-p", "1235"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("p");
    EXPECT_TRUE(arg);
    EXPECT_EQ(1235, std::get<int>(*arg));
}

TEST(test_cli, test_cli_mount)
{
    const char* argv[] = {"appname", "--mount", "rtk.ua", "-a", "myvrs.com.ua", "--port", "2222"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("-mount");
    EXPECT_TRUE(arg);
    EXPECT_EQ("rtk.ua", std::get<std::string>(*arg));
}

TEST(test_cli, test_cli_m)
{
    const char* argv[] = {"appname", "-m", "myrtk.ua", "-p", "1235"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("m");
    EXPECT_TRUE(arg);
    EXPECT_EQ("myrtk.ua", std::get<std::string>(*arg));
}

TEST(test_cli, test_cli_user)
{
    const char* argv[] = {"appname", "--mount", "rtk.ua", "-a", "myvrs.com.ua", "--user", "c22x22"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("-user");
    EXPECT_TRUE(arg);
    EXPECT_EQ("c22x22", std::get<std::string>(*arg));
}

TEST(test_cli, test_cli_u)
{
    const char* argv[] = {"appname", "-m", "myrtk.ua", "-u", "q1235y"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("u");
    EXPECT_TRUE(arg);
    EXPECT_EQ("q1235y", std::get<std::string>(*arg));
}

TEST(test_cli, test_cli_password)
{
    const char* argv[] = {"appname", "--mount", "rtk.ua", "-a", "myvrs.com.ua", "--password", "p2222"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("-password");
    EXPECT_TRUE(arg);
    EXPECT_EQ("p2222", std::get<std::string>(*arg));
}

TEST(test_cli, test_cli_pw)
{
    const char* argv[] = {"appname", "-m", "myrtk.ua", "-pw", "w1235"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("pw");
    EXPECT_TRUE(arg);
    EXPECT_EQ("w1235", std::get<std::string>(*arg));
}

TEST(test_cli, test_cli_latitude)
{
    const char* argv[] = {"appname", "--address", "rtk.ua", "-a", "myvrs.com.ua", "--port", "2222", "--latitude", "49"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("-latitude");
    EXPECT_TRUE(arg);
    EXPECT_EQ(49, std::get<int>(*arg));
}

TEST(test_cli, test_cli_la)
{
    const char* argv[] = {"appname", "--address", "rtk.ua", "-a", "myvrs.com.ua", "--port", "2222", "-la", "49.8"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("la");
    EXPECT_TRUE(arg);
    EXPECT_DOUBLE_EQ(49.8, std::get<double>(*arg));
}

TEST(test_cli, test_cli_longitude)
{
    const char* argv[] = {"appname", "--longitude", "28", "-a", "myvrs.com.ua", "--port", "2222", "--latitude", "49"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("-longitude");
    EXPECT_TRUE(arg);
    EXPECT_EQ(28, std::get<int>(*arg));
}

TEST(test_cli, test_cli_lo)
{
    const char* argv[] = {"appname", "--longitude", "28", "-lo", "32.324", "-a", "myvrs.com.ua", "--port", "2222", "--latitude", "49"};
    const int argc = sizeof(argv)/sizeof(argv[0]);
    VrsTunnel::cli cli(argc, argv);
    auto arg = cli.find("lo");
    EXPECT_TRUE(arg);
    EXPECT_DOUBLE_EQ(32.324, std::get<double>(*arg));
}