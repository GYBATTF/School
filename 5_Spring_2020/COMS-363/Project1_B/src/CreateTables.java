import java.sql.*;

public class CreateTables {
    private static final String USERNAME = "coms363";
    private static final String PASSWORD = "password";
    private static final String SERVER = "jdbc:mysql://localhost:3306";

    private static final String STUDENTS = "CREATE TABLE students (snum INTEGER, ssn INTEGER, name VARCHAR(10), gender VARCHAR(1), dob DATETIME, c_addr VARCHAR(20), c_phone VARCHAR(10), p_addr VARCHAR(20), p_phone VARCHAR(10), PRIMARY KEY (ssn), UNIQUE (snum));";
    private static final String DEPARTMENTS = "CREATE TABLE departments (code INTEGER, name VARCHAR(50), phone VARCHAR(10), college VARCHAR(20), PRIMARY KEY (code), UNIQUE (name));";
    private static final String DEGREES = "CREATE TABLE degrees (name VARCHAR(50), level VARCHAR(5), department_code INTEGER, PRIMARY KEY (name, level), FOREIGN KEY (department_code) REFERENCES departments(code));";
    private static final String COURSES = "CREATE TABLE courses (number INTEGER, name VARCHAR(50), description VARCHAR(50), credithours INTEGER, level VARCHAR(20), department_code INTEGER, PRIMARY KEY (number), UNIQUE (name), FOREIGN KEY (department_code) REFERENCES departments(code));";
    private static final String REGISTER = "CREATE TABLE register (snum INTEGER, course_number INTEGER, regtime VARCHAR(20), grade INTEGER, PRIMARY KEY (snum, course_number), FOREIGN KEY (snum) REFERENCES students(snum), FOREIGN KEY (course_number) REFERENCES courses(number));";
    private static final String MAJOR = "CREATE TABLE major (snum INTEGER, name VARCHAR(50), level VARCHAR(5), PRIMARY KEY (snum, name, level), FOREIGN KEY (snum) REFERENCES students(snum), FOREIGN KEY (name, level) REFERENCES degrees(name, level));";
    private static final String MINOR = "CREATE TABLE minor (snum INTEGER, name VARCHAR(50), level VARCHAR(5), PRIMARY KEY (snum, name, level), FOREIGN KEY (snum) REFERENCES students(snum), FOREIGN KEY (name, level) REFERENCES degrees(name, level))";
    private static final String[] QUERIES = {STUDENTS, DEPARTMENTS, DEGREES, COURSES, REGISTER, MAJOR, MINOR};

    public static void main(String... args) throws Exception {
        Connection sql = DriverManager.getConnection(SERVER, USERNAME, PASSWORD);
        for (String s : QUERIES) {
            sql.createStatement().executeUpdate(s);
        }
    }
}
