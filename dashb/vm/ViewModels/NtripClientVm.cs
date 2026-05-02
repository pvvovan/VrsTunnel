using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace vm.ViewModels;

public partial class NtripClientVm : UserVm
{
    public NtripClientVm(
        RelayCommand<NtripClientVm> removeCmd,
        RelayCommand<NtripClientVm> assignCmd,
        RelayCommand<NtripClientVm> unassignCmd,
        Models.NtripClient? model)
    {
        RemoveCmd = removeCmd;
        AssignCmd = assignCmd;
        UnassignCmd = unassignCmd;
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

    [ObservableProperty]
    public partial RelayCommand<NtripClientVm> UnassignCmd { get; set; }
}
