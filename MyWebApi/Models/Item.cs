namespace MyWebApi.Models;

public record Item
{
    public int Id {get; set;}
    public required string Name { get; set; }
    public int Quantity { get; set; }
}