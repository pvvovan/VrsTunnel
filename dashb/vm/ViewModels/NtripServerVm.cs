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
        _removeCmd = removeCmd;
        Model = model;
        if (Model is not null)
        {
            Name = Model.Name;
            PasswordHash = Model.PasswordHash;
        }
    }

    public readonly Models.NtripServer? Model;

    [ObservableProperty]
    private ObservableCollection<NtripClientVm> _clients = [];

    [ObservableProperty]
    private RelayCommand<NtripServerVm> _removeCmd;
}
