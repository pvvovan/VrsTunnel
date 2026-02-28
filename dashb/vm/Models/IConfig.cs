namespace vm.Models;

public interface IConfig
{
    Task<(IQueryable<NtripClient> clients, IQueryable<NtripServer> servers)> LoadAsync(string file = "");
    Task StoreAsync(IQueryable<NtripClient> clients, IQueryable<NtripServer> servers, string file = "");
}
