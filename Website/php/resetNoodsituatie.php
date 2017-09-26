<?php 
	include_once 'phpconfig.php'; //server connection info
	
	$mysqli = new mysqli(HOST, USER, PASS, DATABASE);
	
	$sql = "UPDATE `Setting` SET `Noodsituatie`= 0";
	$result = $mysqli->query($sql);
?>