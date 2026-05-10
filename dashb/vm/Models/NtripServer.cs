namespace vm.Models;

public class NtripServer : NtripUser
{
    public List<Guid> Clients { get; set; } = [];
}
