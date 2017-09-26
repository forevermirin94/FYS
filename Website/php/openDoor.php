<?php 
	//php code die een update query uitvoert
	include_once 'phpconfig.php'; //server connection info
	
	$mysqli = new mysqli(HOST, USER, PASS, DATABASE);
	
	$sql = "UPDATE `Setting` SET Door = 1";
	$mysqli->query($sql);
?>