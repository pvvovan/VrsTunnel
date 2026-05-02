using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace vm.ViewModels;

public partial class NtripClientVm : UserVm
{
    public NtripClientVm(
        RelayCommand<NtripClientVm> removeCmd,
        Models.NtripClient? model)
    {
        RemoveCmd = removeCmd;
        Model = model;
        if (Model is not null)
        {
            Name = Model.Name;
            PasswordHash = Model.PasswordHash;
        }
    }

    public readonly Models.NtripClient? Model;

    [ObservableProperty]
    public partial RelayCommand<NtripClientVm> RemoveCmd { get; set; }


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
}
