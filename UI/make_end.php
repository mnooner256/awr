
<!DOCTYPE html>
<html>
<body>

<?php

// The function to connect to MySQL is called mysql_connect.

$username = "your_name";
$password = "your_password";
$hostname = "localhost"; 

//connection to the database
$dbhandle = mysql_connect($hostname, $username, $password) 
  or die("Unable to connect to MySQL");
echo "Connected to MySQL<br>";

//select a database to work with
$selected = mysql_select_db("examples",$dbhandle) 
  or die("Could not select examples");
  


// Strings

// The PHP Concatenation Operator. The concatenation operator (.)  is used to join two string values together.
$txt1="Hello world!";
$txt2="What a nice day!";
echo $txt1 . " " . $txt2;

// The PHP string length function
echo strlen("Hello world!");

/* The PHP string position function. Used to search for a character or a specific text within a string.
If a match is found, it will return the character position of the first match. */
echo strpos("Hello world!","world");



// Write a string to a file
echo file_put_contents("test.txt","Hello World. Testing!");


  
// Select Data From a Database. 


/* The SQL SELECT Statement. The SELECT statement is used to select data from a database.
The result is stored in a result table, called the result-set.
*/ 

//SQL SELECT Syntax

$results = mysql.request($dbhandle, "select * from table_name");

SELECT column_name,column_name
FROM table_name;

//and

SELECT * FROM table_name;


// Close the connection  
mysql_close(host,username,password,dbname);
?>

</body>
</html>