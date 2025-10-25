using Avalonia.Controls;

namespace dashb.Views;

public partial class InputView : Window
{
    public InputView()
    {
        InitializeComponent();
    }

    private void TextBlock_ActualThemeVariantChanged(object? sender, System.EventArgs e)
    {
    }

    private void Button_Click(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
    {
        Close();
    }
}