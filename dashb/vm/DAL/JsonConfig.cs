using vm.Models;
using System.Text.Json;

namespace vm.DAL;

public class JsonConfig : IConfig
{
    public JsonConfig()
    {
        _filename = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "NtripConfig.json");
    }

    private string _filename;

    public async Task<(IQueryable<NtripClient> clients, IQueryable<NtripServer> servers)> LoadAsync(string file = "")
    {
        if (!string.IsNullOrEmpty(file))
        {
            _filename = file;
        }
        if (!File.Exists(_filename))
        {
            await StoreAsync(new List<NtripClient>().AsQueryable(),
                             new List<NtripServer>().AsQueryable());
        }
        await using FileStream fileStream = File.OpenRead(_filename);
        var cfg = await JsonSerializer.DeserializeAsync<JsonDal>(fileStream);
        return (cfg!.Clients.AsQueryable(), cfg!.Servers.AsQueryable());
    }

    public async Task StoreAsync(IQueryable<NtripClient> clients, IQueryable<NtripServer> servers, string file = "")
    {
        if (!string.IsNullOrEmpty(file))
        {
            _filename = file;
        }
        JsonDal cfg = new()
        {
            Clients = [.. clients],
            Servers = [.. servers]
        };
        await using FileStream fileStream = File.Create(_filename);
        var opt = new JsonSerializerOptions { WriteIndented = true };
        await JsonSerializer.SerializeAsync(fileStream, cfg, opt);
    }
}
