namespace vm.Models;

public record class NtripServer(string Name, string Password, Guid Id, List<Guid>? Clients);
