import java.sql.Connection;
import java.sql.DriverManager;

public class ModifyRecords {
    private static final String USERNAME = "coms363";
    private static final String PASSWORD = "password";
    private static final String SERVER = "jdbc:mysql://localhost:3306";

    private static final String NAME_CHANGE = "UPDATE students SET name = 'Scott' WHERE ssn = 746897816;";
    private static final String MAJOR_CHANGE = "UPDATE major SET name = 'Computer Science', level = 'MS' WHERE snum IN (SELECT snum FROM students WHERE ssn = 746897816);";
    private static final String DELETE_SPRING2015 = "DELETE FROM register WHERE regtime = 'Spring2015';";
    private static final String[] QUERIES = {NAME_CHANGE, MAJOR_CHANGE, DELETE_SPRING2015};

    public static void main(String... args) throws Exception {
        Connection sql = DriverManager.getConnection(SERVER, USERNAME, PASSWORD);
        for (String s : QUERIES) {
            sql.createStatement().executeUpdate(s);
        }
    }
}
