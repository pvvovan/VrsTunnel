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

    private bool dialogOpen = false;

    private async void Button_Clicked(object sender, RoutedEventArgs args)
    {
        if (dialogOpen)
        {
            return;
        }
        dialogOpen = true;

        // Get top level from the current control. Alternatively, you can use Window reference instead.
        var topLevel = TopLevel.GetTopLevel(this);

        // Start async operation to open the dialog.
        var file = await topLevel!.StorageProvider.SaveFilePickerAsync(new FilePickerSaveOptions
        {
            Title = "Save Text File"
        });

        if (file is not null)
        {
            // Open writing stream from the file.
            await using var stream = await file.OpenWriteAsync();
            using var streamWriter = new StreamWriter(stream);
            // Write some content to the file.
            await streamWriter.WriteLineAsync("Hello World!");
        }
        dialogOpen = false;
    }
}
