/* Alexander Harms
   ajharms@iastate */

/* 1) Change the name of the student with ssn = 746897816 to Scott */
UPDATE students SET name = 'Scott' WHERE ssn = 746897816;
/* 2) Change the major of the student with ssn = 746897816 to Computer Science, Master */
UPDATE major SET name = 'Computer Science', level = 'MS' WHERE snum IN (SELECT snum FROM students WHERE ssn = 746897816);
/* 3) Delete all registration records that were in "Spring2015" */
DELETE FROM register WHERE regtime = 'Spring2015';