using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace vm.ViewModels;

public partial class UserVm : ObservableObject
{
    private void UpdatePasswordHash()
    {
        string data = _name + ":" + _password;
        byte[] buffer = System.Text.Encoding.ASCII.GetBytes(data);
        _passwordHash = Convert.ToBase64String(buffer, 0, buffer.Length);
    }

    private string _passwordHash = string.Empty;
    public string PasswordHash
    {
        get => _passwordHash;
        set => _passwordHash = value;
    }

    private string _name = string.Empty;
    public string Name
    {
        get => _name;
        set
        {
            SetProperty(ref _name, value);
            UpdatePasswordHash();
        }
    }

    private string _password = string.Empty;
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
    private RelayCommand<UserVm>? _editCmd;
}
