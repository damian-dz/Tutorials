using System.Data;
using MySqlConnector;

public class DapperContext
{
    private readonly IConfiguration _configuration;
    private readonly string _connectionString;

    public DapperContext(IConfiguration configuration)
    {
        _configuration = configuration;
        _connectionString = _configuration["ConnectionStrings:Storage"]!;
    }

    public IDbConnection CreateConnection()
        => new MySqlConnection(_connectionString);

}