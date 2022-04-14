<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="ISO-8859-1">
<title>Records Inserted</title>
</head>
<body>
	<%
		String success = request.getParameter("success");
		
		String s = String.format("Your data records have%s been inserted.", "true".equals(success) ? "" : " NOT");
	%>
	
	<b><%= s %></b>
</body>
</html>