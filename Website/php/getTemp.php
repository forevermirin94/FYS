<?php
	//een code die de laatste temperatuur haalt
	//met select query // waarbij limit 1 betekent dat je laatste record van het tabel uitleest
	include_once 'phpconfig.php';
	$mysqli = new mysqli(HOST, USER, PASS, DATABASE);
	$sql = "SELECT `Temperature` FROM `Temperature` ORDER BY `Timestamp` DESC LIMIT 1";
	$result = $mysqli->query($sql);
	$current_temperature = $result->fetch_assoc();
	
	print($current_temperature['Temperature'] . " °C");
?>