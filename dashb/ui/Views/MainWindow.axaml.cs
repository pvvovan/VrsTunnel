using System;
using System.Threading.Tasks;
using Avalonia.Controls;
using Avalonia.Input;
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
            FileTypeChoices = [ new FilePickerFileType("json") { Patterns = [ "*.json" ] } ],
        });
        return file?.Path.AbsolutePath ?? "";
    }

    public async Task<string> Open()
    {
        var file = await this.StorageProvider.OpenFilePickerAsync(new FilePickerOpenOptions
        {
            Title = "Open Json File",
            FileTypeFilter = [ new FilePickerFileType("json") { Patterns = [ "*.json" ] } ],
            AllowMultiple = false,
        });

        if (file is not null)
        {
            if (file.Count == 1)
            {
                return file[0].Path.AbsolutePath;
            }
        }
        return "";
    }

    private static readonly DataFormat<NtripClientVm> dragFormat =
        DataFormat.CreateInProcessFormat<NtripClientVm>("myid");

    private Avalonia.Point dragPos;
    private bool dragStared;
    private PointerPressedEventArgs? dragArgs;

    private void Clients_OnPointerPressed(object? sender, PointerPressedEventArgs e)
    {
        dragPos = e.GetCurrentPoint(sender as Control).Position;
        dragArgs = e;
        dragStared = false;
    }

    private async void Clients_OnPointerMoved(object? sender, PointerEventArgs e)
    {
        if (e.Properties.IsLeftButtonPressed)
        {
            if (Math.Abs(dragPos.X - e.GetCurrentPoint(sender as Control).Position.X) > 10)
            {
                if (!dragStared)
                {
                    dragStared = true;
                    DataTransfer dragData = new();
                    DataTransferItem dragItem = new();
                    dragItem.Set(dragFormat, (sender as Control)?.DataContext as NtripClientVm);
                    dragData.Add(dragItem);
                    await DragDrop.DoDragDropAsync(dragArgs!, dragData, DragDropEffects.Copy | DragDropEffects.Move);
                }
            }
        }
    }

    private void Servers_OnDragOver(object? sender, DragEventArgs e)
    {
        e.DragEffects = DragDropEffects.None;
        var client = e.DataTransfer.TryGetValue(dragFormat);
        if (client is not null)
        {
            var server = ((sender as Control)!.DataContext as NtripServerVm)!;
            if (!server.Clients.Contains(client))
            {
                e.DragEffects = DragDropEffects.Copy;
            }
        }
    }

    private void Servers_OnDrop(object? sender, DragEventArgs e)
    {
        var client = e.DataTransfer.TryGetValue(dragFormat);
        if (client is not null)
        {
            var server = ((sender as Control)!.DataContext as NtripServerVm)!;
            if (!server.Clients.Contains(client))
            {
                server.Clients.Add(client);
            }
        }
    }
}
