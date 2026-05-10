using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System.Collections.ObjectModel;
using System.Diagnostics.CodeAnalysis;

namespace vm.ViewModels;

public partial class NtripServerVm : UserVm
{
    [SetsRequiredMembers]
    public NtripServerVm() : base()
    {
        Model = new Models.NtripServer() { Id = Guid.NewGuid() };
    }

    [SetsRequiredMembers]
    public NtripServerVm(Models.NtripServer model) : base()
    {
        Model = model;
    }

    [ObservableProperty]
    public partial ObservableCollection<NtripClientVm> Clients { get; set; } = [];

    [RelayCommand]
    private void Remove(ObservableCollection<NtripServerVm> servers)
    {
        servers.Remove(this);
    }
}
