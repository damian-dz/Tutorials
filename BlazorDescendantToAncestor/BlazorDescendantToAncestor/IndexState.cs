namespace BlazorDescendantToAncestor;

public class IndexState
{
    public event Action<string>? OnGrandchildClick;

    public void GrandchildClicked(string text)
    {
        OnGrandchildClick?.Invoke(text);
    }
}
