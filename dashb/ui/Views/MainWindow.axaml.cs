using Avalonia.Controls;
using dashb.ViewModels;

namespace dashb.Views;

public partial class MainWindow : Window, IDialog
{
    public MainWindow()
    {
        InitializeComponent();
    }

    public void Show(InputVm inputVm)
    {
        InputView inputView = new()
        {
            DataContext = inputVm
        };
        inputVm.Wnd = inputView;
        inputView.ShowDialog(this);
    }
}
