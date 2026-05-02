using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.Input;

namespace vm.ViewModels;

public partial class NtripClientVm : UserVm
{
    public NtripClientVm(Models.NtripClient? model)
    {
        Model = model;
        if (Model is not null)
        {
            Name = Model.Name;
            PasswordHash = Model.PasswordHash;
        }
    }

    public readonly Models.NtripClient? Model;

    [RelayCommand]
    private void Assign(NtripServerVm? server)
    {
        if (server is not null && !server.Clients.Contains(this))
        {
            server.Clients.Add(this);
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
            while (srv.Clients.Contains(this))
            {
                srv.Clients.Remove(this);
            }
        }
    }
}
