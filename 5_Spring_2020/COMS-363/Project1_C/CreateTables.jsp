<%@ page language="java" contentType="text/html; charset=ISO-8859-1" pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<%@ page import="java.io.*,java.util.*,java.sql.*" %>
<%@ page import="javax.servlet.http.*,javax.servlet.*" %>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Create Tables</title>
<%
// Change back to com363 & password before submitting & remove /project_1
String USERNAME = "root";
String PASSWORD = "fghgf355";
String SERVER = "jdbc:mysql://localhost:3306/project_1";

String STUDENTS = "CREATE TABLE students (snum INTEGER, ssn INTEGER, name VARCHAR(10), gender VARCHAR(1), dob DATETIME, c_addr VARCHAR(20), c_phone VARCHAR(10), p_addr VARCHAR(20), p_phone VARCHAR(10), PRIMARY KEY (ssn), UNIQUE (snum));";
String DEPARTMENTS = "CREATE TABLE departments (code INTEGER, name VARCHAR(50), phone VARCHAR(10), college VARCHAR(20), PRIMARY KEY (code), UNIQUE (name));";
String DEGREES = "CREATE TABLE degrees (name VARCHAR(50), level VARCHAR(5), department_code INTEGER, PRIMARY KEY (name, level), FOREIGN KEY (department_code) REFERENCES departments(code));";
String REGISTER = "CREATE TABLE register (snum INTEGER, course_number INTEGER, regtime VARCHAR(20), grade INTEGER, PRIMARY KEY (snum, course_number), FOREIGN KEY (snum) REFERENCES students(snum), FOREIGN KEY (course_number) REFERENCES courses(number));";
String COURSES = "CREATE TABLE courses (number INTEGER, name VARCHAR(50), description VARCHAR(50), credithours INTEGER, level VARCHAR(20), department_code INTEGER, PRIMARY KEY (number), UNIQUE (name), FOREIGN KEY (department_code) REFERENCES departments(code));";
String MAJOR = "CREATE TABLE major (snum INTEGER, name VARCHAR(50), level VARCHAR(5), PRIMARY KEY (snum, name, level), FOREIGN KEY (snum) REFERENCES students(snum), FOREIGN KEY (name, level) REFERENCES degrees(name, level));";
String MINOR = "CREATE TABLE minor (snum INTEGER, name VARCHAR(50), level VARCHAR(5), PRIMARY KEY (snum, name, level), FOREIGN KEY (snum) REFERENCES students(snum), FOREIGN KEY (name, level) REFERENCES degrees(name, level))";
String[] QUERIES = {STUDENTS, DEPARTMENTS, DEGREES, COURSES, REGISTER, MAJOR, MINOR};
%>

</head>
<body>
	<%
if ("Submit".equals(request.getParameter("submit"))) {
	boolean success = false;
	
	try {
		Connection sql = DriverManager.getConnection(SERVER, USERNAME, PASSWORD);
	    for (String s : QUERIES) {
	        sql.createStatement().executeUpdate(s);
	    }
		
		success = true;
	} catch (Exception ignored) {
		out.println(ignored.toString());
	}
	
	response.sendRedirect("CreateTablesResult.jsp?success=" + success);
}
%>

	<b>Click "Submit" to create tables.</b>
	<form method="post"><input type="submit" name="submit" value="Submit"></input></form>
</body>
</html>