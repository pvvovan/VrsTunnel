namespace vm.Models;

public interface IConfig
{
    Task<(IQueryable<NtripClient> clients, IQueryable<NtripServer> servers)> LoadAsync();
    Task StoreAsync(IQueryable<NtripClient> clients, IQueryable<NtripServer> servers);
}
