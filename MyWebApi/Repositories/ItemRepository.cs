
using System.Data;
using Dapper;
using MyWebApi.Models;

namespace MyWebApi.Repositories;

public class ItemRepository
{
    private readonly DapperContext _context;
    private readonly ILogger<ItemRepository> _logger;

    public ItemRepository(DapperContext context, ILogger<ItemRepository> logger) 
    {
        _context = context;
        _logger = logger;
    }

    public async Task<IEnumerable<Item>> GetItemsAsync()
    {
        const string sql = "sp_GetItems";
        using IDbConnection connection = _context.CreateConnection();
        IEnumerable<Item> items = await connection.QueryAsync<Item>(sql, commandType: CommandType.StoredProcedure);
        return items;
    }

    public async Task<Item?> GetItemAsync(int id)
    {
        const string sql = "sp_GetItem";
        using IDbConnection connection = _context.CreateConnection();
        Item? item = await connection.QuerySingleOrDefaultAsync<Item>(
            sql, new { id }, commandType: CommandType.StoredProcedure);
        return item;
    }

    public async Task<int> SaveItemAsync(Item item)
    {
        const string sql = "sp_SaveItem";
        using IDbConnection connection = _context.CreateConnection();
        int rowsAffected = await connection.ExecuteAsync(sql, item, commandType: CommandType.StoredProcedure);
        return rowsAffected;
    }

    public async Task<int> DeleteItemAsync(int id)
    {
        const string sql = "sp_DeleteItem";
        using IDbConnection connection = _context.CreateConnection();
        int rowsAffected = await connection.ExecuteAsync(sql, new { id }, commandType: CommandType.StoredProcedure);
        return rowsAffected;
    }
}