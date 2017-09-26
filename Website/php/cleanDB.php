<?php 
	include_once 'phpconfig.php'; //server connection info
	
	$mysqli = new mysqli(HOST, USER, PASS, DATABASE);
	
	$sql = "DELETE FROM `Temperature` WHERE `Temperature` = '-61'"; //clean alle waarden die phidgetboard uitleest als tempsensor niet aangesloten is
	//$sql = "DELETE FROM `Temperature` WHERE Timestamp != CURRENT_DATE"; //alleen huidige temperatuur wordt vertoond 
	$result = $mysqli->query($sql);
	
	 echo '<h1><center>DATABASE CLEANED </center></h1>';
?>