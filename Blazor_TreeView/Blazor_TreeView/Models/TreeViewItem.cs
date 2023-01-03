namespace Blazor_TreeView.Models;

public class TreeViewItem
{
    public string? Text { get; set; }
    public List<TreeViewItem>? Children { get; set; }
    public bool IsCollapsed { get; set; }
    public bool IsSelected { get; set; }

    public bool HasChildren => Children != null && Children.Any();

    public TreeViewItem(string? text, List<TreeViewItem>? children = null, bool isCollapsed = false, bool isSelected = false)
    {
        Text = text;
        Children = children;
        IsCollapsed = isCollapsed;
        IsSelected = isSelected;
    }
}
