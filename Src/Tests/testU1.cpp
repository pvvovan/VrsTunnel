
#include <future>
#include <CppUTest/TestHarness.h>
#include <string>
#include <thread>


TEST_GROUP(FirstTestGroup)
{

};

TEST(FirstTestGroup, FirstTest)
{
    double d = 1;
    CHECK_EQUAL(1, d);
}


TEST_GROUP(ListTestGroup)
{
    void setup()
    {

    }
    void teardown()
    {

    }
};
