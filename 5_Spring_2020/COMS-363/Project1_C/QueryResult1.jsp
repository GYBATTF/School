<%@ page language="java" contentType="text/html; charset=ISO-8859-1" pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<%@ page import="java.io.*,java.util.*,java.sql.*" %>
<%@ page import="javax.servlet.http.*,javax.servlet.*" %>
<html>
<head>
<meta charset="ISO-8859-1">
<title>Query 1 Results</title>
<%
// Change back to com363 & password before submitting & remove /project_1
String USERNAME = "root";
String PASSWORD = "fghgf355";
String SERVER = "jdbc:mysql://localhost:3306/project_1";
String QUERY = "SELECT c.name FROM courses c, departments d WHERE c.department_code = d.code and d.name = 'Computer Science' ORDER BY c.name;";
String[] COLUMNS = {"snum", "ssn"};
%>
</head>
<body>
<% ResultSet rs = DriverManager.getConnection(SERVER, USERNAME, PASSWORD).createStatement().executeQuery(QUERY); %>       
<b>This is the result of query 1</b>
<table border = "1">
<tr bgcolor = "#949494">
<tr>
<% for (String s : COLUMNS) { %>
<th><%= s %></th>
<% } %>
</tr>
<% while (rs.next()) { %>
<tr>
<td><%= rs.getInt(COLUMNS[0]) %></td>
<td><%= rs.getInt(COLUMNS[1]) %></td>
</tr>
<% } %>
</table>
</body>
</html>