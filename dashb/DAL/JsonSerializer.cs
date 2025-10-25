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

    public (IQueryable<NtripClient> Clients, IQueryable<NtripServer> Servers) Load()
    {
        throw new NotImplementedException();
    }

    public async Task Store(IQueryable<NtripClient> Clients, IQueryable<NtripServer> Servers)
    {
        JsonDal cfg = new()
        {
            Clients = [..Clients]
        };
        await using FileStream createStream = File.Create(_filename);
        var opt = new JsonSerializerOptions { WriteIndented = true };
        await JsonSerializer.SerializeAsync(createStream, cfg, opt);
    }
}
