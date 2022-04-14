	<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
    
<%@ page import="java.io.*,java.util.*,java.sql.*" %>
<%@ page import="javax.servlet.http.*,javax.servlet.*" %>

<html>
<head>
<meta charset="ISO-8859-1">
<title>Modify Records</title>

	<%
	//Change back to com363 & password before submitting & remove /project_1
	String USERNAME = "root";
	String PASSWORD = "fghgf355";
	String SERVER = "jdbc:mysql://localhost:3306/project_1";
	%>

</head>
<body>
	<%
		if ("Submit".equals(request.getParameter("submit"))) {
			boolean success = false;
			
			try {
				Connection sql = DriverManager.getConnection(SERVER, USERNAME, PASSWORD);
				sql.createStatement().executeUpdate("UPDATE students SET name = 'Scott' WHERE ssn = 746897816;");
				
				success = true;
			} catch (Exception ignored) {}
			
			response.sendRedirect("ModifyRecordsResult.jsp?success=" + success);
		}
	%>

	<b>Click "Submit" to modify records.</b>
	<form method="post"><input type="submit" name="submit" value="Submit"></input></form>
</body>
</html>