import java.sql.DriverManager;

public class DropTables {
    private static final String USERNAME = "coms363";
    private static final String PASSWORD = "password";
    private static final String SERVER = "jdbc:mysql://localhost:3306";

    private static final String DROP_TABLES = "DROP TABLE IF EXISTS students, departments, degrees, courses, register, major, minor;";

    public static void main(String... args) throws Exception {
        DriverManager.getConnection(SERVER, USERNAME, PASSWORD).createStatement().executeUpdate(DROP_TABLES);
    }
}
