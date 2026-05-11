using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System.Collections.ObjectModel;
using System.Diagnostics.CodeAnalysis;
using vm.Models;

namespace vm.ViewModels;

public partial class NtripClientVm : UserVm
{
    [ObservableProperty]
    [NotifyCanExecuteChangedFor(nameof(AssignCommand))]
    public partial bool CanAssign { get; set; } = false;

    [SetsRequiredMembers]
    public NtripClientVm() : base()
    {
        Model = new NtripClient() { Id = Guid.NewGuid() };
    }

    [SetsRequiredMembers]
    public NtripClientVm(NtripClient model) : base()
    {
        Model = model;
    }

    [RelayCommand(CanExecute = nameof(CanAssign))]
    private void Assign(object? item)
    {
        if (item is NtripServerVm server && !server.Clients.Any(c => c.Model.Id == this.Model.Id))
        {
            server.Clients.Add(new(new NtripClient()
            {
                Name = this.Name,
                PasswordHash = this.PasswordHash,
                Id = Model.Id
            }));
        }
    }

    [RelayCommand]
    private void Unassign(NtripServerVm? server)
    {
        server?.Clients.Remove(this);
    }

    [RelayCommand]
    private void Remove(ReadOnlyCollection<object> objs)
    {
        var servers = (ObservableCollection<NtripServerVm>)objs[0];
        var clients = (ObservableCollection<NtripClientVm>)objs[1];
        clients.Remove(this);
        foreach (var srv in servers)
        {
            NtripClientVm? cl = srv.Clients.FirstOrDefault(c => c.Model.Id == this.Model.Id);
            if (cl is not null)
            {
                srv.Clients.Remove(cl);
            }
        }
    }
}
