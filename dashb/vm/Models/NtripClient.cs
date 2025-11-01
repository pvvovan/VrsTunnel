using System;

namespace dashb.Models;

public record class NtripClient(string Name, string Password, Guid Id);
