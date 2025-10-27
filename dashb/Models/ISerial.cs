using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace dashb.Models;

public interface IConfig
{
    (IQueryable<NtripClient> clients, IQueryable<NtripServer> servers) Load();
    Task StoreAsync(IQueryable<NtripClient> clients, IQueryable<NtripServer> servers);
}
