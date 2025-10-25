using System;
using System.Linq;

namespace dashb.Models;

public class NtripServer
{
    public string Name = string.Empty;
    public string Password = string.Empty;
    public Guid Id = Guid.Empty;
    public IQueryable<Guid>? Clients;
}
