using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System.Collections.ObjectModel;

namespace vm.ViewModels;

public partial class NtripServerVm : UserVm
{
    public NtripServerVm(
        RelayCommand<NtripServerVm> removeCmd,
        Models.NtripServer? model)
    {
        Clients = [];
        RemoveCmd = removeCmd;
        Model = model;
        if (Model is not null)
        {
            Name = Model.Name;
            PasswordHash = Model.PasswordHash;
        }
    }

    public readonly Models.NtripServer? Model;

    [ObservableProperty]
    public partial ObservableCollection<NtripClientVm> Clients { get; set; }

    [ObservableProperty]
    public partial RelayCommand<NtripServerVm> RemoveCmd { get; set; }
}
