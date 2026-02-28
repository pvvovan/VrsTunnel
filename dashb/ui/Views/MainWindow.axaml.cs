using System.IO;
using System.Threading.Tasks;
using Avalonia.Controls;
using Avalonia.Interactivity;
using Avalonia.Platform.Storage;
using vm.ViewModels;

namespace ui.Views;

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

    public async Task<string> Save()
    {
        var file = await this.StorageProvider.SaveFilePickerAsync(new FilePickerSaveOptions
        {
            Title = "Save Json File",
            FileTypeChoices = [ new FilePickerFileType("json") { Patterns = [ "*.json" ] } ]
        });
        return file?.Path.AbsolutePath ?? "";
    }
}
