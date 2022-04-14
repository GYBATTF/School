import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;

public class Query {
    private static final String USERNAME = "coms363";
    private static final String PASSWORD = "password";
    private static final String SERVER = "jdbc:mysql://localhost:3306";

    private static final String QUERY1_SQL = "SELECT snum, ssn FROM students WHERE name = 'Becky' LIMIT 1;";
    private static final String[] QUERY1_COLUMNS = {"snum", "ssn"};
    private static final String[] QUERY1_TYPES = {"int", "int"};

    private static final String QUERY2_SQL = "SELECT m.name, m.level FROM major m, students s WHERE m.snum = s.snum and s.ssn = 123097834 LIMIT 1;";
    private static final String[] QUERY2_COLUMNS = {"name", "level"};
    private static final String[] QUERY2_TYPES = {"String", "String"};

    private static final String QUERY3_SQL = "SELECT c.name FROM courses c, departments d WHERE c.department_code = d.code and d.name = 'Computer Science' ORDER BY c.name;";
    private static final String[] QUERY3_COLUMNS = {"name"};
    private static final String[] QUERY3_TYPES = {"String"};

    private static final String QUERY4_SQL = "SELECT d.name, d.level FROM degrees d, departments deps WHERE d.department_code = deps.code and deps.name = 'Computer Science' ORDER BY d.name;";
    private static final String[] QUERY4_COLUMNS = {"name", "level"};
    private static final String[] QUERY4_TYPES = {"String", "String"};

    private static final String QUERY5_SQL = "SELECT DISTINCT s.name FROM students s, minor m WHERE s.snum = m.snum ORDER BY s.name;";
    private static final String[] QUERY5_COLUMNS = {"name"};
    private static final String[] QUERY5_TYPES = {"String"};

    private static final Query363 QUERY1 = new Query363(QUERY1_SQL, QUERY1_COLUMNS, QUERY1_TYPES);
    private static final Query363 QUERY2 = new Query363(QUERY2_SQL, QUERY2_COLUMNS, QUERY2_TYPES);
    private static final Query363 QUERY3 = new Query363(QUERY3_SQL, QUERY3_COLUMNS, QUERY3_TYPES);
    private static final Query363 QUERY4 = new Query363(QUERY4_SQL, QUERY4_COLUMNS, QUERY4_TYPES);
    private static final Query363 QUERY5 = new Query363(QUERY5_SQL, QUERY5_COLUMNS, QUERY5_TYPES);
    
    private static final Query363[] QUERIES = {QUERY1, QUERY2, QUERY3, QUERY4, QUERY5};
    
    public static void main(String... args) throws Exception {
        Connection sql = DriverManager.getConnection(SERVER, USERNAME, PASSWORD);
        for (Query363 q : QUERIES) {
            query(q, sql);
        }
    }
    
    private static void query(Query363 q, Connection sql) throws Exception {
        ResultSet rs = sql.createStatement().executeQuery(q.sql);
        while (rs.next()) {
            System.out.print("{ ");
            for (int i = 0; i < q.columns.length; i++) {
                System.out.print(q.columns[i] + " : ");
                switch (q.types[i]) {
                    case "String":
                        System.out.print(rs.getString(q.columns[i]) + ", ");
                        continue;
                    case "int":
                        System.out.print(rs.getInt(q.columns[i]) + ", ");
                }
            }
            System.out.println("\b\b }");
        }
    }
    
    private static class Query363 {
        String sql;
        String[] columns;
        String[] types;
        
        Query363(String s, String[] c, String[] t) {
            sql = s;
            columns = c;
            types = t;
        }
    }
}
