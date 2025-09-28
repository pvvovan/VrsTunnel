using dashb.DAL;
using dashb.Ntrip;

namespace dashb.Test
{
    public class UnitTest1
    {
        [Fact]
        public void Test1()
        {
            using UoW_stub uow = new();
            Assert.Equal("Client 1", uow.Repo<Client>().Items.First().Name);
        }
    }
}