using vm.Models;

namespace vm.DAL;

public class JsonDal
{
    public List<NtripClient> Clients { get; set; } = [];
    public List<NtripServer> Servers { get; set; } = [];

}
