using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Data.Core.Plugins;
using System.Linq;
using Avalonia.Markup.Xaml;
using vm.DAL;
using vm.ViewModels;
using ui.Views;
using System.Threading.Tasks;

namespace ui;

public partial class App : Application
{
    public override void Initialize()
    {
        AvaloniaXamlLoader.Load(this);
    }

    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            desktop.MainWindow = new MainWindow();
            var mainVm = new MainWindowVm((IDialog)desktop.MainWindow, new JsonConfig());
            desktop.MainWindow.DataContext = mainVm;
            Task storeTask = new(() => mainVm.StoreConfig().Wait());
            desktop.MainWindow.Closing += (_, _) =>
            {
                storeTask.Start();
            };
            desktop.MainWindow.Closed += (_, _) =>
            {
                storeTask.Wait();
            };
        }

        base.OnFrameworkInitializationCompleted();
    }
}
