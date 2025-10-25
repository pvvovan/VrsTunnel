using Avalonia.Controls;
using CommunityToolkit.Mvvm.Input;
using dashb.ViewModels;
using System;

namespace dashb.Views;

public partial class MainWindow : Window, IShowDialog
{
    public MainWindow()
    {
        InitializeComponent();
    }

    public InputVm ShowAddServer(NtripServerVm server, RelayCommand addServerCmd)
    {
        InputView inputView = new();
        var inputVm = new InputVm(inputView)
        {
            User = server,
            OkCmd = addServerCmd
        };
        inputView.DataContext = inputVm;
        inputView.ShowDialog(this);
        return inputVm;
    }

    public void ShowAddClient()
    {
        throw new System.NotImplementedException();
    }
}
