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
    private readonly string _filename = "config.json";

    public (IQueryable<NtripClient> clients, IQueryable<NtripServer> servers) Load()
    {
        JsonDal cfg = new()
        {
            Clients = [],
            Servers = []
        };
        if (File.Exists(_filename))
        {
            using FileStream fileStream = File.OpenRead(_filename);
            cfg = JsonSerializer.Deserialize<JsonDal>(fileStream)!;
        }
        return (cfg.Clients.AsQueryable(), cfg.Servers.AsQueryable());
    }

    public async Task Store(IQueryable<NtripClient> clients, IQueryable<NtripServer> servers)
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
