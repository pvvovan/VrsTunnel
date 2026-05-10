using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System.ComponentModel;
using System.Reflection;

namespace vm.ViewModels;

public abstract partial class UserVm : ObservableObject
{
    private string _password = string.Empty;

    private void UpdatePasswordHash()
    {
        string data = Model.Name + ":" + _password;
        byte[] buffer = System.Text.Encoding.ASCII.GetBytes(data);
        Model.PasswordHash = Convert.ToBase64String(buffer, 0, buffer.Length);
    }

    public required Models.NtripUser Model { get; init; }

    public string PasswordHash
    {
        get => Model.PasswordHash;
        set => Model.PasswordHash = value;
    }

    public string Name
    {
        get => Model.Name;
        set
        {
            Model.Name = value;
            OnPropertyChanged();
            UpdatePasswordHash();
        }
    }

    public string Password
    {
        get => "";
        set
        {
            SetProperty(ref _password, value);
            UpdatePasswordHash();
        }
    }

    [ObservableProperty]
    public partial RelayCommand<UserVm>? EditCmd { get; set; }
}
