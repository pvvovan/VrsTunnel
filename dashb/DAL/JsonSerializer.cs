using System;
using System.Collections.Generic;
using System.Linq;
using dashb.Models;
using System.Text.Json;
using System.IO;
using System.Threading.Tasks;

namespace dashb.DAL;

public class JsonDal
{
    public List<NtripClient> Clients { get; set; } = [];
    public List<NtripServer> Servers { get; set; } = [];

}

public class JsonConfig : IConfig
{
    public JsonConfig()
    {
        _filename = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "NtripConfig.json");
    }

    private readonly string _filename;

    public async Task<(IQueryable<NtripClient> clients, IQueryable<NtripServer> servers)> LoadAsync()
    {
        if (!File.Exists(_filename))
        {
            await StoreAsync(new List<NtripClient>().AsQueryable(),
                             new List<NtripServer>().AsQueryable());
        }
        await using FileStream fileStream = File.OpenRead(_filename);
        var cfg = await JsonSerializer.DeserializeAsync<JsonDal>(fileStream);
        return (cfg!.Clients.AsQueryable(), cfg!.Servers.AsQueryable());
    }

    public async Task StoreAsync(IQueryable<NtripClient> clients, IQueryable<NtripServer> servers)
    {
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
