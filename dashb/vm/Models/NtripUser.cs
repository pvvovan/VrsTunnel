namespace vm.Models;

public abstract class NtripUser
{
    public Guid Id { get; init; }
    public string Name { get; set; } = string.Empty;
    public string PasswordHash { get; set; } = string.Empty;
}
