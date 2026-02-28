namespace vm.ViewModels;

public interface IDialog
{
    void Show(InputVm inputVm);
    Task<string> Save();
    Task<string> Open();
}
