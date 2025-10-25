using System;
using System.Collections.Generic;

namespace dashb.Models;

public class NtripServer
{
    public string Name { get; set; } = string.Empty;
    public string Password { get; set; } = string.Empty;
    public Guid Id { get; set; } = Guid.Empty;
    public List<Guid>? Clients { get; set; }
}
