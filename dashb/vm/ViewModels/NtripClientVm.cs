using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace vm.ViewModels;

public partial class NtripClientVm : UserVm
{
    public NtripClientVm(
        RelayCommand<NtripClientVm> removeCmd,
        RelayCommand<NtripClientVm> assignCmd,
        Models.NtripClient? model)
    {
        RemoveCmd = removeCmd;
        AssignCmd = assignCmd;
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

    [ObservableProperty]
    public partial RelayCommand<NtripClientVm> AssignCmd { get; set; }

    [RelayCommand]
    private void Unassign(NtripServerVm? server)
    {
        server?.Clients.Remove(this);
    }
}
