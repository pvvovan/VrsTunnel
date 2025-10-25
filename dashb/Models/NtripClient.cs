using System;

namespace dashb.Models;

public class NtripClient
{
    public string Name { get; set; } = string.Empty;
    public string Password { get; set; } = string.Empty;
    public Guid Id { get; set; } = Guid.Empty;
}
