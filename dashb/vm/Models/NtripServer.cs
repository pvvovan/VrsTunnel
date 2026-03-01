namespace vm.Models;

public record class NtripServer(string Name, string PasswordHash, Guid Id, List<Guid>? Clients);
