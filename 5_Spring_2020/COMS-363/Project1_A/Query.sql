/* Alexander Harms
   ajharms@iastate */

/* 1)	The student number and ssn of the student whose name is "Becky" */
SELECT snum, ssn FROM students WHERE name = 'Becky' LIMIT 1;
/* 2)	The major name and major level of the student whose ssn is 123097834 */
SELECT m.name, m.level FROM major m, students s WHERE m.snum = s.snum and s.ssn = 123097834 LIMIT 1;
/* 3)	The names of all courses offered by the department of Computer Science */
SELECT c.name FROM courses c, departments d WHERE c.department_code = d.code and d.name = 'Computer Science' ORDER BY c.name;
/* 4)	All degree names and levels offered by the department Computer Science */
SELECT d.name, d.level FROM degrees d, departments deps WHERE d.department_code = deps.code and deps.name = 'Computer Science' ORDER BY d.name;
/* 5)	The names of all students who have a minor */
SELECT DISTINCT s.name FROM students s, minor m WHERE s.snum = m.snum ORDER BY s.name;
/* 6)	The number of students who have a minor */
SELECT DISTINCT COUNT(snum) FROM minor;
/* 7)	The names and numbers of all students enrolled in course “Algorithm” */
SELECT DISTINCT s.name, s.snum FROM students s, register r, courses c WHERE r.snum = s.snum and r.course_number = c.number and c.name = 'Algorithm' ORDER BY s.name;
/* 8)	The name and snum of the oldest student */
SELECT name, snum FROM students WHERE dob IN (SELECT MIN(dob) FROM students) LIMIT 1;
/* 9)	The name and snum of the youngest student */
SELECT name, snum FROM students WHERE dob IN (SELECT MAX(dob) FROM students) LIMIT 1;
/* 10)	The name, snum and SSN of the students whose name contains letter “n” or “N” */
SELECT DISTINCT name, snum, ssn FROM students WHERE name LIKE '%n%' ORDER BY name;
/* 11)	The name, snum and SSN of the students whose name does not contain letter “n” or “N” */
SELECT DISTINCT name, snum, ssn FROM students WHERE name NOT LIKE '%n%' ORDER BY name;
/* 12)	The course number, name and the number of students registered for each course */
SELECT DISTINCT c.number, s.name, s.snum FROM courses c, students s, register r WHERE r.snum = s.snum and r.course_number = c.number ORDER BY c.number, s.name;
/* 13)	The name of the students enrolled in Fall2015 semester. */
SELECT DISTINCT s.name FROM students s, register r WHERE r.regtime = 'Fall2015' and s.snum = r.snum ORDER BY s.name;
/* 14)	The course numbers and names of all courses offered by Department of Computer Science */
SELECT c.number, c.name FROM courses c, departments d WHERE c.department_code = d.code and d.name = 'Computer Science' ORDER BY c.number, c.name;
/* 15)	The course numbers and names of all courses offered by either Department of Computer Science or Department of Landscape Architect. */
SELECT c.number, c.name FROM courses c, departments d WHERE c.department_code = d.code and (d.name = 'Computer Science' or d.name = 'Landscape Architect') ORDER BY d.code, c.number, c.name;