using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace dashb.Models;

public interface IConfig
{
    (IQueryable<NtripClient> Clients, IQueryable<NtripServer> Servers) Load();
    Task Store(IQueryable<NtripClient> Clients, IQueryable<NtripServer> Servers);
}
