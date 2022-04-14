<%@ page language="java" contentType="text/html; charset=ISO-8859-1" pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="ISO-8859-1">
<title>Queries</title>
</head>
<body>
<%
if ("Q1".equals(request.getParameter("q1"))) response.sendRedirect("QueryResult1.jsp");
if ("Q2".equals(request.getParameter("q2"))) response.sendRedirect("QueryResult2.jsp");
if ("Q3".equals(request.getParameter("q3"))) response.sendRedirect("QueryResult3.jsp");
%>
<form method="post">
<b>Click "Q1" to see result of query 1</b>
<p><input type="submit" name="q1" value="Q1"></input></p>
<b>Click "Q2" to see result of query 2</b>
<p><input type="submit" name="q2" value="Q2"></input></p>
<b>Click "Q3" to see result of query 3</b>
<p><input type="submit" name="q3" value="Q3"></input></p>
</form>
</body>
</html>