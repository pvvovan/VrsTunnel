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

    public void ShowAddServer(InputVm inputVm)
    {
        InputView inputView = new()
        {
            DataContext = inputVm
        };
        inputVm.Wnd = inputView;
        inputView.ShowDialog(this);
    }

    public void ShowAddClient()
    {
        throw new System.NotImplementedException();
    }
}
