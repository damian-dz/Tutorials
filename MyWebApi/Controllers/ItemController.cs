using Microsoft.AspNetCore.Mvc;
using MyWebApi.Models;
using MyWebApi.Repositories;

namespace MyWebApi.Controllers;

[ApiController]
[Route("api/items")]
public class ItemController : ControllerBase
{
    private readonly ItemRepository _repo;
    private readonly ILogger<ItemController> _logger;

    public ItemController(ItemRepository repo, ILogger<ItemController> logger)
    {
        _repo = repo;
        _logger = logger;
    }

    [HttpGet]
    public async Task<ActionResult<IEnumerable<Item>>> GetItems()
    {
        IEnumerable<Item> items = await _repo.GetItemsAsync();
        return Ok(items);
    }

    [HttpGet("{id:int}")]
    public async Task<ActionResult<Item>> GetItem(int id)
    {
        Item? item = await _repo.GetItemAsync(id);
        if (item is null)
            return NotFound();
        return Ok(item);
    }

    [HttpPost]
    public async Task<IActionResult> SaveItem([FromBody] Item item)
    {
        int rowsAffected = await _repo.SaveItemAsync(item);
        if (rowsAffected > 0)
            return NoContent();
        return BadRequest("Failed to save the item.");
    }

    [HttpDelete("{id:int}")]
    public async Task<IActionResult> DeleteItem(int id)
    {
        int rowsAffected = await _repo.DeleteItemAsync(id);
        if (rowsAffected == 0)
            return BadRequest("Failed to delete the item.");
        return NoContent();
    }

}
