using System;
using System.Collections.Generic;

namespace dashb.Models;

public record class NtripServer(string Name, string Password, Guid Id, List<Guid>? Clients);
